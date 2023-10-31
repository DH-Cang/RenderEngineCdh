#include "Material.h"

void Material::SetShader(Shader *shader)
{
    m_shader = shader;
}

D3D12ConstantBuffer *Material::GetCbPerObject()
{
    return m_cb_per_object.get();
}

void Material::CreateCb(ID3D12Device *device)
{
    // reset cb
    m_cb_size = 0;
    m_cb_per_object.release();
    m_mapped_data.resize(0);

    // traverse all the variables and store them
    auto cb_structure = m_shader->GetCbStructure("cbPerObject");
    for(int i=0; i<cb_structure.size(); i++)
    {
        ConstantBufferAttribute& cb_var = cb_structure[i];
        VariableAttribute material_var;
        material_var.type = cb_var.type;
        material_var.offset = cb_var.offset;
        material_var.size = cb_var.size;
        m_material_vars[cb_var.name] = material_var;

        m_cb_size += cb_var.size;
    }

    // create cb
    m_cb_per_object = std::make_unique<D3D12ConstantBuffer>(device, m_cb_size);
    m_mapped_data.resize(m_cb_size);
}

void Material::UpdateCb()
{
    m_cb_per_object->CopyData(m_mapped_data.data(), m_cb_size);
    bool result = m_shader->SetParameter("cbPerObject", m_cb_per_object.get());
    assert(result == true);
}

void Material::PassParametersToShader(ID3D12GraphicsCommandList *cmd_list, DescriptorCacheGPU *descriptor_cache)
{
    UpdateCb();
    m_shader->BindParameters(cmd_list, descriptor_cache);
}

void Material::SetParameter(const std::string &name, DirectX::XMFLOAT4X4 data)
{
    VariableAttribute var_attr = m_material_vars[name];
    assert(var_attr.type == "float4x4");
    memcpy(m_mapped_data.data() + var_attr.offset,
        &data,
        sizeof(data));
}

void Material::SetParameter(const std::string &name, ShaderResourceView *srv)
{
    bool result = m_shader->SetParameter(name, srv);
    assert(result == true);
}
