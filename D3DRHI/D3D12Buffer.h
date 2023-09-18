#pragma once
#include "Common/d3dUtil.h"

class D3D12Buffer
{
public:
    D3D12Buffer() = default; // create upload buffer and copy
    virtual ~D3D12Buffer() = default;
    ID3D12Resource* GetResource() const { return m_d3d_resource.Get(); }

protected:
    Microsoft::WRL::ComPtr<ID3D12Resource> m_d3d_resource = nullptr;
};


class D3D12ConstantBuffer : public D3D12Buffer
{
public:
    D3D12ConstantBuffer() = delete;
    D3D12ConstantBuffer(ID3D12Device* device, UINT size);
    ~D3D12ConstantBuffer();

    void CopyData(void* data, int size);

private:
    BYTE* m_mapped_data = nullptr;
};
