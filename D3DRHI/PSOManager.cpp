#include "PSOManager.h"

void PSOManager::CreatePSO(const std::string& name, D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc, ID3D12Device* device)
{
    assert(m_pso_map.find(name) == m_pso_map.end());

    std::pair<std::string, ComPtr<ID3D12PipelineState>> element;
    element.first = name;
    ThrowIfFailed(device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&element.second)));

    m_pso_map.insert(element);
}

ID3D12PipelineState *PSOManager::GetPSO(const std::string &name)
{
    return m_pso_map[name].Get();
}
