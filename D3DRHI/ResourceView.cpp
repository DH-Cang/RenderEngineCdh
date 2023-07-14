#include "ResourceView.h"

ResourceView::ResourceView(D3D12_DESCRIPTOR_HEAP_TYPE type, ID3D12Resource* resource, DescriptorManager* descriptor_manager):
    m_resource(resource),
    m_descriptor_manager(descriptor_manager),
    m_type(type)
{
    if(descriptor_manager)
    {
        m_slot = descriptor_manager->AllocateDesriptorSlot();
        assert(m_slot.cpu_handle.ptr != 0);
    }
}

ResourceView::~ResourceView()
{
    if(m_descriptor_manager)
    {
        m_descriptor_manager->FreeDescriptorSlot(m_slot);
    }
}

ShaderResourceView::ShaderResourceView(const D3D12_SHADER_RESOURCE_VIEW_DESC &desc, ID3D12Resource *resource, ID3D12Device *device, DescriptorManager *descriptor_manager):
    ResourceView(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, resource, descriptor_manager)
{
    device->CreateShaderResourceView(resource, &desc, m_slot.cpu_handle);
}

RenderTargetView::RenderTargetView(const D3D12_RENDER_TARGET_VIEW_DESC &desc, ID3D12Resource *resource, ID3D12Device *device, DescriptorManager *descriptor_manager):
    ResourceView(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, resource, descriptor_manager)
{
    device->CreateRenderTargetView(resource, &desc, m_slot.cpu_handle);
}

DepthStencilView::DepthStencilView(const D3D12_DEPTH_STENCIL_VIEW_DESC &desc, ID3D12Resource *resource, ID3D12Device *device, DescriptorManager *descriptor_manager):
    ResourceView(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, resource, descriptor_manager)
{
    device->CreateDepthStencilView(resource, &desc, m_slot.cpu_handle);
}

UnorderedAccessView::UnorderedAccessView(const D3D12_UNORDERED_ACCESS_VIEW_DESC &desc, ID3D12Resource *resource, ID3D12Device *device, DescriptorManager *descriptor_manager):
    ResourceView(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, resource, descriptor_manager)
{
    device->CreateUnorderedAccessView(resource, nullptr, &desc, m_slot.cpu_handle);
}
