#include "DescriptorCacheGPU.h"


DescriptorCacheGPU::DescriptorCacheGPU(ID3D12Device *device):
    m_device(device)
{
    CreateCbvSrvUavHeap();
    CreateRtvHeap();
}

ID3D12DescriptorHeap* DescriptorCacheGPU::GetCachedRtvDescriptorHeap()
{
    return m_rtv_heap.Get();
}

ID3D12DescriptorHeap* DescriptorCacheGPU::GetCachedCbvSrvUavDescriptorHeap()
{
    return m_cbv_srv_uav_heap.Get();
}

void DescriptorCacheGPU::AppendRtvDescriptorsToHeap(const std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> &rtv_descriptors, CD3DX12_GPU_DESCRIPTOR_HANDLE &out_gpu_handle, CD3DX12_CPU_DESCRIPTOR_HANDLE &out_cpu_handle)
{
    // get cpu descriptor num
    uint32_t descriptor_num = rtv_descriptors.size();
    assert(m_rtv_offset + descriptor_num < max_rtv_descriptor_count);

    // copy cpu descriptor to gpu descriptor
    auto dest_cpu_handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_rtv_heap->GetCPUDescriptorHandleForHeapStart(), m_rtv_offset, m_rtv_descriptor_size);
    m_device->CopyDescriptors(1, &dest_cpu_handle, &descriptor_num, descriptor_num, rtv_descriptors.data(), nullptr, D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    // get output
    out_gpu_handle = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_rtv_heap->GetGPUDescriptorHandleForHeapStart(), m_rtv_offset, m_rtv_descriptor_size);
    out_cpu_handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_rtv_heap->GetCPUDescriptorHandleForHeapStart(), m_rtv_offset, m_rtv_descriptor_size);

    // increase gpu descriptor number
    m_rtv_offset += descriptor_num;

    return;
}

CD3DX12_GPU_DESCRIPTOR_HANDLE DescriptorCacheGPU::AppendCbvSrvUavDescriptorsToHeap(const std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>& srv_descriptors)
{
    // get cpu descriptor number
    uint32_t descriptor_num = srv_descriptors.size();
    assert(m_cbv_srv_uav_offset + descriptor_num < max_cbv_srv_uav_descriptor_count);

    // copy cpu descriptor to gpu descriptor
    auto dest_cpu_handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_cbv_srv_uav_heap->GetCPUDescriptorHandleForHeapStart(), m_cbv_srv_uav_offset, m_cbv_srv_uav_descriptor_size);
    m_device->CopyDescriptors(1, &dest_cpu_handle, &descriptor_num, descriptor_num, srv_descriptors.data(), nullptr, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    // get gpu handle
    auto gpu_handle = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_cbv_srv_uav_heap->GetGPUDescriptorHandleForHeapStart(), m_cbv_srv_uav_offset, m_cbv_srv_uav_descriptor_size);

    // increase gpu descriptor number
    m_cbv_srv_uav_offset += descriptor_num;

    return gpu_handle;
}

void DescriptorCacheGPU::ResetCachedHeap()
{
    ResetCbvSrvUavHeap();
    ResetRtvHeap();
}

void DescriptorCacheGPU::CreateCbvSrvUavHeap()
{
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.NumDescriptors = max_cbv_srv_uav_descriptor_count;
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

    ThrowIfFailed(m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_cbv_srv_uav_heap)));
    SetDebugName(m_cbv_srv_uav_heap.Get(), L"GPUCachedCbvSrvUavHeap");

    m_cbv_srv_uav_descriptor_size = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void DescriptorCacheGPU::CreateRtvHeap()
{
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.NumDescriptors = max_rtv_descriptor_count;
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    ThrowIfFailed(m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_rtv_heap)));
    SetDebugName(m_rtv_heap.Get(), L"GPUCachedRtvHeap");

    m_rtv_descriptor_size = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
}

void DescriptorCacheGPU::ResetCbvSrvUavHeap()
{
    m_cbv_srv_uav_offset = 0;
}

void DescriptorCacheGPU::ResetRtvHeap()
{
    m_rtv_offset = 0;
}
