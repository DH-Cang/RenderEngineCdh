#include "Shader.h"
#include "Utility/FormatConvert.h"

void ShaderDefines::GetD3DShaderMacro(std::vector<D3D_SHADER_MACRO> &out_macro) const
{
    for(const auto& pair : m_defines_map)
    {
        D3D_SHADER_MACRO macro;
        macro.Name = pair.first.c_str();
        macro.Definition = pair.second.c_str();
        out_macro.push_back(macro);
    }

    // end with null macro(why?)
    D3D_SHADER_MACRO macro;
	macro.Name = NULL;
	macro.Definition = NULL;
	out_macro.push_back(macro);
}

bool ShaderDefines::operator==(const ShaderDefines &other) const
{
    if(m_defines_map.size() != other.m_defines_map.size())
    {
        return false;
    }

    for(const auto& pair : m_defines_map)
    {
        const std::string& key = pair.first;
        const std::string& value = pair.second;

        auto iter = other.m_defines_map.find(key);
        if(iter == other.m_defines_map.end() || iter->second != value)
        {
            return false;
        }
    }

    return true;
}

void ShaderDefines::SetDefine(const std::string &name, const std::string &definition)
{
    m_defines_map.insert_or_assign(name, definition);
}

ComPtr<ID3DBlob> Shader::CompileShader(
	const std::wstring& filename,
	const D3D_SHADER_MACRO* defines,
	const std::string& entrypoint,
	const std::string& target)
{
	UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT hr = S_OK;

	ComPtr<ID3DBlob> byteCode = nullptr;
	ComPtr<ID3DBlob> errors;
	hr = D3DCompileFromFile(filename.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entrypoint.c_str(), target.c_str(), compileFlags, 0, &byteCode, &errors);

	if(errors != nullptr)
		OutputDebugStringA((char*)errors->GetBufferPointer());

	ThrowIfFailed(hr);

	return byteCode;
}

Shader::Shader(const ShaderInfo &shader_info, ID3D12Device* device):
    m_shader_info(shader_info)
{
    Initialize(device);
    assert((shader_info.b_create_VS | shader_info.b_create_PS) ^ shader_info.b_create_CS);
}

bool Shader::SetParameter(std::string param_name, D3D12ConstantBuffer *constant_buffer)
{
    bool ret = false;

    for(ShaderCBVParameter& param : m_cbv_params)
    {
        if(param.name == param_name)
        {
            param.constant_buffer = constant_buffer;
            ret = true;
        }
    }

    return ret;
}

bool Shader::SetParameter(std::string param_name, ShaderResourceView *srv)
{
    std::vector<ShaderResourceView*> srv_list{srv};
    return SetParameter(param_name, srv_list);
}

bool Shader::SetParameter(std::string param_name, const std::vector<ShaderResourceView *> &srv_list)
{
    bool ret = false;

    for(ShaderSRVParameter& param : m_srv_params)
    {
        if(param.name == param_name)
        {
            assert(param.bind_count == srv_list.size());
            param.srv_list = srv_list;
            ret = true;
        }
    }

    return ret;
}

bool Shader::SetParameter(std::string param_name, UnorderedAccessView *uav)
{
    std::vector<UnorderedAccessView*> uav_list{uav};
    return SetParameter(param_name, uav_list);
}

bool Shader::SetParameter(std::string param_name, const std::vector<UnorderedAccessView*>& uav_list)
{
    bool ret = false;

    for(ShaderUAVParameter& param : m_uav_params)
    {
        if(param.name == param_name)
        {
            assert(uav_list.size() == param.bind_count);
            param.uav_list = uav_list;
            ret = true;
        }
    }

    return ret;
}

void Shader::Initialize(ID3D12Device* device)
{
    std::wstring file_path = FormatConvert::StrToWStr(m_shader_info.file_name);

    std::vector<D3D_SHADER_MACRO> shader_macros;
    m_shader_info.shader_defines.GetD3DShaderMacro(shader_macros);

    // Compile Shader
    if(m_shader_info.b_create_VS)
    {
        auto VS_blob = CompileShader(file_path, shader_macros.data(), m_shader_info.VS_entry_point, "vs_5_0");
        m_shader_stage["VS"] = VS_blob;

        GetShaderParameters(VS_blob, ShaderType::k_vertex_shader);
    }

    if(m_shader_info.b_create_PS)
    {
        auto PS_blob = CompileShader(file_path, shader_macros.data(), m_shader_info.PS_entry_point, "ps_5_0");
        m_shader_stage["PS"] = PS_blob;

        GetShaderParameters(PS_blob, ShaderType::k_pixel_shader);
    }

    if(m_shader_info.b_create_CS)
    {
        auto CS_blob = CompileShader(file_path, shader_macros.data(), m_shader_info.CS_entry_point, "cs_5_0");
        m_shader_stage["CS"] = CS_blob;

        GetShaderParameters(CS_blob, ShaderType::k_compute_shader);
    }

    CreateRootSignature(device);
}

void Shader::GetShaderParameters(ComPtr<ID3DBlob> blob, ShaderType shader_type)
{
    ID3D12ShaderReflection* reflection = nullptr;
	D3DReflect(blob->GetBufferPointer(), blob->GetBufferSize(), IID_ID3D12ShaderReflection, (void**)&reflection);

	D3D12_SHADER_DESC shader_desc;
	reflection->GetDesc(&shader_desc);

	for(int i=0; i<shader_desc.BoundResources; i++)
	{
		D3D12_SHADER_INPUT_BIND_DESC resource_desc;
		reflection->GetResourceBindingDesc(i, &resource_desc);

		auto shader_var_name = resource_desc.Name;
		auto resource_type = resource_desc.Type;
		auto register_space = resource_desc.Space;	
		auto bind_point = resource_desc.BindPoint;
		auto bind_count = resource_desc.BindCount;

        if (resource_type == D3D_SHADER_INPUT_TYPE::D3D_SIT_CBUFFER)
		{
            // create cbv parameter
			ShaderCBVParameter param;
			param.name = shader_var_name;
			param.shader_type = shader_type;
			param.bind_point = bind_point;
			param.register_space = register_space;
			m_cbv_params.push_back(param);

            // store constant buffer structure
            auto cb = reflection->GetConstantBufferByName(shader_var_name);
            D3D12_SHADER_BUFFER_DESC cb_desc;
            cb->GetDesc(&cb_desc);
            auto num_vars = cb_desc.Variables;
            std::vector<ConstantBufferAttribute> attributes(num_vars);
            for(int j=0; j<num_vars; j++)
            {
                auto var = cb->GetVariableByIndex(j);
                D3D12_SHADER_VARIABLE_DESC var_desc;
                D3D12_SHADER_TYPE_DESC var_type_desc;
                var->GetDesc(&var_desc);
                var->GetType()->GetDesc(&var_type_desc);

                attributes[j].name = var_desc.Name;
                attributes[j].offset = var_desc.StartOffset;
                attributes[j].size = var_desc.Size;
                attributes[j].type = var_type_desc.Name;
            }
            m_cb_structure_map[param.name] = attributes;
		}
		else if (resource_type == D3D_SHADER_INPUT_TYPE::D3D_SIT_STRUCTURED
			  || resource_type == D3D_SHADER_INPUT_TYPE::D3D_SIT_TEXTURE)
		{
			ShaderSRVParameter param;
			param.name = shader_var_name;
			param.shader_type = shader_type;
			param.bind_point = bind_point;
			param.bind_count = bind_count;
			param.register_space = register_space;

			m_srv_params.push_back(param);
		}
		else if (resource_type == D3D_SHADER_INPUT_TYPE::D3D_SIT_UAV_RWSTRUCTURED
			  || resource_type == D3D_SHADER_INPUT_TYPE::D3D_SIT_UAV_RWTYPED)
		{
			assert(shader_type == ShaderType::k_compute_shader);

			ShaderUAVParameter param;
			param.name = shader_var_name;
			param.shader_type = shader_type;
			param.bind_point = bind_point;
			param.bind_count = bind_count;
			param.register_space = register_space;

			m_uav_params.push_back(param);
		}
		else if (resource_type == D3D_SHADER_INPUT_TYPE::D3D_SIT_SAMPLER)
		{
			assert(shader_type == ShaderType::k_pixel_shader);

			ShaderSamplerParameter param;
			param.name = shader_var_name;
			param.shader_type = shader_type;
			param.bind_point = bind_point;
			param.register_space = register_space;

			m_sampler_params.push_back(param);
		}
        else
        {
            assert(false);
        }
	}
}

D3D12_SHADER_VISIBILITY Shader::GetShaderVisibility(ShaderType shader_type)
{
    D3D12_SHADER_VISIBILITY visibility;
	if (shader_type == ShaderType::k_vertex_shader)
	{
		visibility = D3D12_SHADER_VISIBILITY_VERTEX;
	}
	else if (shader_type == ShaderType::k_pixel_shader)
	{
		visibility = D3D12_SHADER_VISIBILITY_PIXEL;
	}
	else if(shader_type == ShaderType::k_compute_shader)
	{
		visibility = D3D12_SHADER_VISIBILITY_ALL;
	}
	else
	{
		assert(false);
	}

	return visibility;
}

std::vector<CD3DX12_STATIC_SAMPLER_DESC> Shader::CreateStaticSamplers()
{
    // Applications usually only need a handful of samplers.  So just define them all up front
	// and keep them available as part of the root signature.  

	const CD3DX12_STATIC_SAMPLER_DESC pointWrap(
		0, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC pointClamp(
		1, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearWrap(
		2, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearClamp(
		3, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicWrap(
		4, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressW
		0.0f,                             // mipLODBias
		8);                               // maxAnisotropy

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicClamp(
		5, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressW
		0.0f,                              // mipLODBias
		8);                                // maxAnisotropy

    return std::vector<CD3DX12_STATIC_SAMPLER_DESC>
    {
        pointWrap, pointClamp,
		linearWrap, linearClamp, 
		anisotropicWrap, anisotropicClamp
    };
}

void Shader::CreateRootSignature(ID3D12Device* device)
{
    //------------------------ set root parameters -------------------------
    std::vector<CD3DX12_ROOT_PARAMETER> root_params;

    // CBV: create root descriptor
    for(const ShaderCBVParameter& param : m_cbv_params)
    {
        if(m_cbv_signature_base_bind_slot == -1)
        {
            m_cbv_signature_base_bind_slot = root_params.size();
        }

        CD3DX12_ROOT_PARAMETER root_param;
        root_param.InitAsConstantBufferView(param.bind_point, param.register_space, GetShaderVisibility(param.shader_type));
        root_params.push_back(root_param);
    }

    // SRV: create root descriptor table
    for(const ShaderSRVParameter& param : m_srv_params)
    {
        m_srv_count += param.bind_count; 
    }
    CD3DX12_DESCRIPTOR_RANGE srv_table; // do not set this var inside the if{} below, because it must be addressed when creating root signature
    if(m_srv_count > 0)
    {
        m_srv_signature_bind_slot = root_params.size();

        srv_table.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, m_srv_count, 0, 0, 0);

        CD3DX12_ROOT_PARAMETER root_param;
        D3D12_SHADER_VISIBILITY shader_visibility = m_shader_info.b_create_CS ? 
            D3D12_SHADER_VISIBILITY_ALL : D3D12_SHADER_VISIBILITY_PIXEL; // TODO
        root_param.InitAsDescriptorTable(1, &srv_table, shader_visibility);
        root_params.push_back(root_param);
    }

    // UAV: create root descriptor table
    for(const ShaderUAVParameter& param : m_uav_params)
    {
        m_uav_count += param.bind_count;
    }
    CD3DX12_DESCRIPTOR_RANGE uav_table; // do not set this var inside the if{} below, because it must be addressed when creating root signature
    if(m_uav_count > 0)
    {
        m_uav_signature_bind_slot = root_params.size();

        uav_table.Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, m_uav_count, 0, 0);

        CD3DX12_ROOT_PARAMETER root_param;
        D3D12_SHADER_VISIBILITY shader_visibility = D3D12_SHADER_VISIBILITY_ALL; // TODO
        root_param.InitAsDescriptorTable(1, &uav_table, shader_visibility);
        root_params.push_back(root_param);
    }

    // Sampler
    //TODO
    auto static_samplers = CreateStaticSamplers();

    //----------------------------- create root signature ---------------------------------
    CD3DX12_ROOT_SIGNATURE_DESC root_sig_desc(
        (UINT)root_params.size(), root_params.data(),
        (UINT)static_samplers.size(), static_samplers.data(),
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
    );

    ComPtr<ID3DBlob> serialized_root_sig = nullptr;
    ComPtr<ID3DBlob> error_blob = nullptr;
    HRESULT hr = D3D12SerializeRootSignature(
        &root_sig_desc, D3D_ROOT_SIGNATURE_VERSION_1,
        serialized_root_sig.GetAddressOf(), error_blob.GetAddressOf()
    );

    if(error_blob != nullptr)
    {
        ::OutputDebugStringA((char*)error_blob->GetBufferPointer());
    }
    ThrowIfFailed(hr);

    ThrowIfFailed(device->CreateRootSignature(
        0,
        serialized_root_sig->GetBufferPointer(),
        serialized_root_sig->GetBufferSize(),
        IID_PPV_ARGS(&m_root_signature)
    ));
}


void Shader::BindParameters(ID3D12GraphicsCommandList* cmd_list, DescriptorCacheGPU* descriptor_cache)
{
    CheckBindings();

    bool b_create_CS = m_shader_info.b_create_CS;

    // CBV binding
    for(int i=0; i<m_cbv_params.size(); i++)
    {
        int root_param_index = m_cbv_signature_base_bind_slot + i;
        D3D12_GPU_VIRTUAL_ADDRESS gpu_virtual_address = m_cbv_params[i].constant_buffer->GetResource()->GetGPUVirtualAddress();

        if(b_create_CS)
        {
            cmd_list->SetComputeRootConstantBufferView(root_param_index, gpu_virtual_address);
        }
        else
        {
            cmd_list->SetGraphicsRootConstantBufferView(root_param_index, gpu_virtual_address);
        }
    }

    // SRV binding
    if(m_srv_count > 0)
    {
        std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> src_descriptors;
        src_descriptors.resize(m_srv_count);

        for(const ShaderSRVParameter& param : m_srv_params)
        {
            for(int i=0; i<param.srv_list.size(); i++)
            {
                int index = param.bind_point + i;
                src_descriptors[index] = param.srv_list[i]->GetDescriptorHandle();
            }
        }

        int root_param_index = m_srv_signature_bind_slot;
        auto gpu_descriptor_handle = descriptor_cache->AppendCbvSrvUavDescriptorsToHeap(src_descriptors);

        if(b_create_CS)
        {
            cmd_list->SetComputeRootDescriptorTable(root_param_index, gpu_descriptor_handle);
        }
        else
        {
            cmd_list->SetGraphicsRootDescriptorTable(root_param_index, gpu_descriptor_handle);
        }
    }

    // UAV binding
    if(m_uav_count > 0)
    {
        std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> src_descriptors;
        src_descriptors.resize(m_uav_count);

        for(const ShaderUAVParameter& param : m_uav_params)
        {
            for(int i=0; i<param.uav_list.size(); i++)
            {
                int index = param.bind_point + i;
                src_descriptors[index] = param.uav_list[i]->GetDescriptorHandle();
            }
        }

        int root_param_index = m_uav_signature_bind_slot;
        auto gpu_descriptor_handle = descriptor_cache->AppendCbvSrvUavDescriptorsToHeap(src_descriptors);

        if(b_create_CS)
        {
            cmd_list->SetComputeRootDescriptorTable(root_param_index, gpu_descriptor_handle);
        }
        else
        {
            assert(false);
        }
    }
    
    ClearBindings();
}

const std::vector<ConstantBufferAttribute> &Shader::GetCbStructure(const std::string &cb_name)
{
    const auto& iter = m_cb_structure_map.find(cb_name);
    assert(iter != m_cb_structure_map.end());
    return iter->second;
}

void Shader::CheckBindings()
{
    for(ShaderCBVParameter& param : m_cbv_params)
    {
        assert(param.constant_buffer != nullptr);
    }
    for(ShaderSRVParameter& param : m_srv_params)
    {
        assert(param.srv_list.size() > 0);
    }
    for(ShaderUAVParameter& param : m_uav_params)
    {
        assert(param.uav_list.size() > 0);
    }
}

void Shader::ClearBindings()
{
    for(ShaderCBVParameter& param : m_cbv_params)
    {
        param.constant_buffer = nullptr;
    }
    for(ShaderSRVParameter& param : m_srv_params)
    {
        param.srv_list.clear();
    }
    for(ShaderUAVParameter& param : m_uav_params)
    {
        param.uav_list.clear();
    }
}
