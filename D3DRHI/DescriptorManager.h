#pragma once
#include "Common/d3dUtil.h"


// this is a non-shader-visible descriptor heap manager
// used for SRV, DSV, RTV descriptor allocation
class DescriptorManager
{
public:
    typedef D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHandle;
	typedef decltype(DescriptorHandle::ptr) DescriptorHandlePtr;

    struct DescriptorSlot
    {
        uint32_t heap_id; // used to delete descriptor
        D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle; // used to identify the address of the descriptor
    };

public:
    DescriptorManager() = delete;
    DescriptorManager(ID3D12Device* device, uint32_t num_of_desciptors_per_heap, D3D12_DESCRIPTOR_HEAP_TYPE type);
    ~DescriptorManager() = default;
    DescriptorSlot AllocateDesriptorSlot();
    void FreeDescriptorSlot(const DescriptorSlot& slot);

private:
    struct FreeRange
    {
        DescriptorHandlePtr start;
		DescriptorHandlePtr end;
    };

    struct HeapEntry
    {
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap = nullptr;
		std::list<DescriptorManager::FreeRange> free_list;
    };

private:
    D3D12_DESCRIPTOR_HEAP_DESC CreateHeapDescription(D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t num_of_desciptors_per_heap);
    void AllocateNewHeap();

private:
    ID3D12Device* m_device;

    const D3D12_DESCRIPTOR_HEAP_DESC m_heap_desc;
	const uint32_t m_descriptor_size;
	std::vector<HeapEntry> m_heaps;
};