#include "TextureManager.h"

Texture *TextureManager::GetTexture(const std::string &name)
{
    Texture* ret = m_textures[name].get();
    assert(ret);
    return ret;
}

void TextureManager::LoadTextureFromFile(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList)
{
    std::string name = "woodCrateTex";
	std::wstring filepath = L"../../../Textures/WoodCrate01.dds";
    auto woodCrateTex = std::make_unique<Texture>();
    m_upload_buffers.resize(1);

    ThrowIfFailed(DirectX::CreateDDSTextureFromFile12(device,
		cmdList, filepath.c_str(),
		woodCrateTex->Resource, m_upload_buffers[0]));

    m_textures[name] = std::move(woodCrateTex);
}

void TextureManager::ReleaseUploadBuffer()
{
    m_upload_buffers.clear();
}
