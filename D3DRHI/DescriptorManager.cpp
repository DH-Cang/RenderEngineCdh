#include "DescriptorManager.h"


DescriptorManager::DescriptorManager(ID3D12Device* device, uint32_t num_of_desciptors_per_heap, D3D12_DESCRIPTOR_HEAP_TYPE type):
    m_device(device),
    m_heap_desc(CreateHeapDescription(type, num_of_desciptors_per_heap)),
    m_descriptor_size(m_device->GetDescriptorHandleIncrementSize(m_heap_desc.Type))
{}

D3D12_DESCRIPTOR_HEAP_DESC DescriptorManager::CreateHeapDescription(D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t num_of_desciptors_per_heap)
{
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = type;
	desc.NumDescriptors = num_of_desciptors_per_heap;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // This heap will not be bound to the shader
	desc.NodeMask = 0;

	return desc;
}

void DescriptorManager::AllocateNewHeap()
{
    // create descriptor heap
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap = nullptr;
    ThrowIfFailed(m_device->CreateDescriptorHeap(&m_heap_desc, IID_PPV_ARGS(&heap)));
    SetDebugName(heap.Get(), L"Descriptor Manager: descriptor heap");

    // create heap entry
    DescriptorHandle heap_base = heap->GetCPUDescriptorHandleForHeapStart();
    assert(heap_base.ptr != 0);
    HeapEntry entry;
    entry.heap = heap;
    entry.free_list.push_back({heap_base.ptr, heap_base.ptr + (SIZE_T)m_heap_desc.NumDescriptors * m_descriptor_size});

    m_heaps.push_back(std::move(entry));
}

DescriptorManager::DescriptorSlot DescriptorManager::AllocateDesriptorSlot()
{
    DescriptorSlot ret;

    // find a heap with empty space
    int heap_index = -1;
    for(int i=0; i<m_heaps.size(); i++)
    {
        if(m_heaps[i].free_list.size() > 0)
        {
            heap_index = i;
            break;
        }
    }

    // if not found, allocate a new heap
    if(heap_index == -1)
    {
        AllocateNewHeap();
        heap_index = m_heaps.size() - 1;
    }

    // allocate a slot in this heap
    HeapEntry& entry = m_heaps[heap_index];
    assert(entry.free_list.size() > 0);
    FreeRange& range = entry.free_list.front();
    ret.heap_id = heap_index;
    ret.cpu_handle = DescriptorHandle({range.start});

    // remove free range if it is fulfilled
    range.start += m_descriptor_size;
    if(range.start == range.end)
    {
        entry.free_list.pop_front();
    }

    return ret;
}

void DescriptorManager::FreeDescriptorSlot(const DescriptorSlot &slot)
{
    assert(slot.heap_id < m_heaps.size());
    HeapEntry& entry = m_heaps[slot.heap_id];

    FreeRange new_range = {
        slot.cpu_handle.ptr,
        slot.cpu_handle.ptr + m_descriptor_size
    };

    bool is_found = false;
    for(auto node = entry.free_list.begin(); node != entry.free_list.end() && is_found == false; node++)
    {
        FreeRange& current_range = *node;
        assert(current_range.start < current_range.end);

        if(current_range.start == new_range.end) // merge new_range, current_range
        {
            current_range.start = new_range.start;
            is_found = true;
        }
        else if(current_range.end == new_range.start) // merge current_range, new_range
        {
            current_range.end = new_range.end;
            is_found = true;
        }
        else
        {
            assert(current_range.end < new_range.start || current_range.start > new_range.start);
            if(current_range.start > new_range.start) // insert new_range before current_range
            {
                entry.free_list.insert(node, new_range);
                is_found = true;
            }
        } 
    }

    if(is_found == false)
    {
        // add new range to tail
        entry.free_list.push_back(new_range);
    }
}






