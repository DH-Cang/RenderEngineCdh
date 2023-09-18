#pragma once
#include "Common/d3dUtil.h"
#include "D3DRHI/ResourceView.h"
#include "D3DRHI/D3D12Buffer.h"
#include "D3DRHI/DescriptorCacheGPU.h"
#include <string>

using Microsoft::WRL::ComPtr;


enum class ShaderType
{
    k_vertex_shader,
	k_pixel_shader,
	k_compute_shader,
};

struct ShaderParameter
{
	std::string name;
	ShaderType shader_type;
	UINT bind_point;
	UINT register_space;
};

struct ShaderCBVParameter : ShaderParameter
{
	D3D12ConstantBuffer* constant_buffer;
};

struct ShaderSRVParameter : ShaderParameter 
{
	UINT bind_count;

	std::vector<ShaderResourceView*> srv_list;
};

struct ShaderUAVParameter : ShaderParameter
{
	UINT bind_count;

	std::vector<UnorderedAccessView*> uav_list;
};

struct ShaderSamplerParameter : ShaderParameter
{

};


class ShaderDefines
{
public:
	ShaderDefines() = default;
	~ShaderDefines() = default;

	void GetD3DShaderMacro(std::vector<D3D_SHADER_MACRO>& out_macro) const;
	bool operator == (const ShaderDefines& other) const;
	void SetDefine(const std::string& name, const std::string& definition);

public:
	std::unordered_map<std::string, std::string> m_defines_map;
};

// declare hash<ShaderDefines>, for unordered_map<ShaderDefines>
namespace std
{
	template <>
	struct hash<ShaderDefines>
	{
		std::size_t operator()(const ShaderDefines& defines) const
		{
			using std::size_t;
			using std::hash;
			using std::string;
			// Compute individual hash values for each string 
			// and combine them using XOR
			// and bit shifting:

			size_t hash_value = 0;
			for (const auto& pair : defines.m_defines_map)
			{
				hash_value ^= (hash<string>()(pair.first) << 1);
				hash_value ^= (hash<string>()(pair.second) << 1);
			}

			return hash_value;
		}
	};
}

// input description for shader building
struct ShaderInfo
{
	std::string shader_name;
	std::string file_name;
	ShaderDefines shader_defines;

	bool b_create_VS = false;
	std::string VS_entry_point = "VS";

	bool b_create_PS = false;
	std::string PS_entry_point = "PS";

	bool b_create_CS = false;
	std::string CS_entry_point = "CS";
};



class Shader
{
public:
	Shader() = delete;
	Shader(const ShaderInfo& shader_info, ID3D12Device* device);
	~Shader() = default;

	bool SetParameter(std::string param_name, D3D12ConstantBuffer* constant_buffer);
	bool SetParameter(std::string param_name, ShaderResourceView* srv);
	bool SetParameter(std::string param_name, const std::vector<ShaderResourceView*>& srv_list);
	bool SetParameter(std::string param_name, UnorderedAccessView* uav);
	bool SetParameter(std::string param_name, const std::vector<UnorderedAccessView*>& uav_list);
	void BindParameters(ID3D12GraphicsCommandList* cmd_list, DescriptorCacheGPU* descriptor_cache);

private:
	static Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(const std::wstring& Filename, const D3D_SHADER_MACRO* Defines, const std::string& Entrypoint, const std::string& Target);
	void Initialize(ID3D12Device* device);
	void GetShaderParameters(ComPtr<ID3DBlob> blob, ShaderType shader_type);
	D3D12_SHADER_VISIBILITY GetShaderVisibility(ShaderType shader_type);
	std::vector<CD3DX12_STATIC_SAMPLER_DESC> CreateStaticSamplers();
	void CreateRootSignature(ID3D12Device* device);
	void CheckBindings();
	void ClearBindings();

public:
	ShaderInfo m_shader_info;
	std::unordered_map<std::string, ComPtr<ID3DBlob>> m_shader_stage;	// storing vs, ps or cs shader blob
	ComPtr<ID3D12RootSignature> m_root_signature;

private:
	std::vector<ShaderCBVParameter> m_cbv_params;

	std::vector<ShaderSRVParameter> m_srv_params;

	std::vector<ShaderUAVParameter> m_uav_params;

	std::vector<ShaderSamplerParameter> m_sampler_params;

	int m_cbv_signature_base_bind_slot = -1;

	int m_srv_signature_bind_slot = -1;

	UINT m_srv_count = 0;

	int m_uav_signature_bind_slot = -1;

	UINT m_uav_count = 0;

	int m_sampler_signature_bind_slot = -1;

	
};