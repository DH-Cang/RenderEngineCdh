#pragma once
#include <windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <string>
#include "./D3DRHI/ResourceView.h"

class Texture
{
public:
	Texture() = default;
	~Texture() = default;

public:
	// Unique material name for lookup.
	std::string Name;

	std::wstring Filename;

	Microsoft::WRL::ComPtr<ID3D12Resource> Resource = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> UploadHeap = nullptr;

	std::unique_ptr<ShaderResourceView> m_srv = nullptr;
};