#pragma once
#include "Common/d3dUtil.h"

class DescriptorCacheGPU
{
public:
    DescriptorCacheGPU() = delete;
    DescriptorCacheGPU(ID3D12Device* device);
    ~DescriptorCacheGPU() = default;

    ID3D12DescriptorHeap* GetCachedRtvDescriptorHeap();
    ID3D12DescriptorHeap* GetCachedCbvSrvUavDescriptorHeap();
    void AppendRtvDescriptorsToHeap(const std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>& rtv_descriptors, CD3DX12_GPU_DESCRIPTOR_HANDLE& out_gpu_handle, CD3DX12_CPU_DESCRIPTOR_HANDLE& out_cpu_handle);
    CD3DX12_GPU_DESCRIPTOR_HANDLE AppendCbvSrvUavDescriptorsToHeap(const std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>& srv_descriptors);
    void ResetCachedHeap();

private:
    ID3D12Device* m_device;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_cbv_srv_uav_heap;
    UINT m_cbv_srv_uav_descriptor_size;
    uint32_t m_cbv_srv_uav_offset = 0;  // current descriptor numbers
    static const int max_cbv_srv_uav_descriptor_count = 1024;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtv_heap;
    UINT m_rtv_descriptor_size;
    uint32_t m_rtv_offset = 0;
    static const int max_rtv_descriptor_count = 1024;
    
private:
    void CreateCbvSrvUavHeap();
    void CreateRtvHeap();
    void ResetCbvSrvUavHeap();
    void ResetRtvHeap();
};

