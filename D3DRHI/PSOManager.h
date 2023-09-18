#pragma once
#include <string>
#include <unordered_map>
#include "Common/d3dUtil.h"

using Microsoft::WRL::ComPtr;

class PSOManager
{
public:
    PSOManager() = default;
    ~PSOManager() = default;

    void CreatePSO(const std::string& name, D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc, ID3D12Device* device);
    ID3D12PipelineState* GetPSO(const std::string& name);

private:
    
    std::unordered_map<std::string, ComPtr<ID3D12PipelineState>> m_pso_map;
};