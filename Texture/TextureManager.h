#pragma once
#include <unordered_map>
#include <string>
#include <windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <memory>
#include "Texture.h"
#include <vector>

class TextureManager
{
public:
    TextureManager() = default;
    ~TextureManager() = default;

    Texture* GetTexture(const std::string& name);
    void LoadTextureFromFile(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList); // called when init
    void ReleaseUploadBuffer(); // called between init and runtime


private:
    std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures;
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> m_upload_buffers;
    
};