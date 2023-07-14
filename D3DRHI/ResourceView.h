#pragma once
#include "../Common/d3dUtil.h"
#include "DescriptorManager.h"

class ResourceView
{
public:
    ResourceView() = delete;
    ResourceView(D3D12_DESCRIPTOR_HEAP_TYPE type, ID3D12Resource* resource, DescriptorManager* descriptor_manager);
    virtual ~ResourceView();

    D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptorHandle() const { return m_slot.cpu_handle; }

protected:
    DescriptorManager* m_descriptor_manager = nullptr;
    ID3D12Resource* m_resource = nullptr;
    DescriptorManager::DescriptorSlot m_slot;
    D3D12_DESCRIPTOR_HEAP_TYPE m_type;
};


class ShaderResourceView : public ResourceView
{
public:
    ShaderResourceView() = delete;
    ShaderResourceView(const D3D12_SHADER_RESOURCE_VIEW_DESC& desc, ID3D12Resource* resource, ID3D12Device* device, DescriptorManager* descriptor_manager);
    virtual ~ShaderResourceView() = default;
};


class RenderTargetView : public ResourceView
{
public:
    RenderTargetView() = delete;
    RenderTargetView(const D3D12_RENDER_TARGET_VIEW_DESC& desc, ID3D12Resource* resource, ID3D12Device* device, DescriptorManager* descriptor_manager);
    virtual ~RenderTargetView() = default;
};


class DepthStencilView : public ResourceView
{
public:
    DepthStencilView() = delete;
    DepthStencilView(const D3D12_DEPTH_STENCIL_VIEW_DESC& desc, ID3D12Resource* resource, ID3D12Device* device, DescriptorManager* descriptor_manager);
    virtual ~DepthStencilView() = default;
};


class UnorderedAccessView : public ResourceView
{
public:
    UnorderedAccessView() = delete;
    UnorderedAccessView(const D3D12_UNORDERED_ACCESS_VIEW_DESC& desc, ID3D12Resource* resource, ID3D12Device* device, DescriptorManager* descriptor_manager);
    virtual ~UnorderedAccessView() = default;
};