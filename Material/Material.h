#pragma once
#include <unordered_map>
#include <string>
#include <variant>
#include <vector>
#include <d3d12.h>
#include "Common\d3dx12.h"
#include "Common\d3dApp.h"
#include "Common\d3dUtil.h"
#include "Shader.h"
#include "Texture\TextureManager.h"
#include "D3DRHI\D3D12Buffer.h"

class Material
{
public:
    Material() = default;
    ~Material() = default;

    void SetShader(Shader* shader);
    void CreateCb(ID3D12Device* device);

    D3D12ConstantBuffer* GetCbPerObject();
    ID3D12RootSignature* GetRootSignature() {return m_shader->m_root_signature.Get();}

    void SetParameter(const std::string& name, DirectX::XMFLOAT4X4 data);
    void SetParameter(const std::string& name, ShaderResourceView *srv);
    void PassParametersToShader(ID3D12GraphicsCommandList* cmd_list, DescriptorCacheGPU* descriptor_cache);

private:
    void UpdateCb();

private:
    struct VariableAttribute
    {
        std::string type;
        unsigned int offset;
        unsigned int size;
    };
    
    //std::unordered_map<std::string, VariableAttribute> m_material_vars;
    std::unique_ptr<D3D12ConstantBuffer> m_cb_per_object = nullptr;
    Shader* m_shader = nullptr;
    unsigned int m_cb_size = 0;
    std::vector<char> m_mapped_data; // data (size in bytes)
};
