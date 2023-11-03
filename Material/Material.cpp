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

    // get cb reflection size
    auto& cb_per_object_reflection = m_shader->GetCbReflection("cbPerObject");
    m_cb_size = cb_per_object_reflection.GetSize();

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
    auto& cb_per_object_reflection = m_shader->GetCbReflection("cbPerObject");
    auto metadata = cb_per_object_reflection.GetVarMetaData(name);
    assert(metadata.type == "float4x4");
    memcpy(m_mapped_data.data() + metadata.offset,
        &data,
        sizeof(data));
}

void Material::SetParameter(const std::string &name, ShaderResourceView *srv)
{
    bool result = m_shader->SetParameter(name, srv);
    assert(result == true);
}
