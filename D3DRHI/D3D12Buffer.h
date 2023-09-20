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
    D3D12ConstantBuffer(ID3D12Device* device, UINT size); // create upload heap type
    ~D3D12ConstantBuffer();

    void CopyData(void* data, int size);

private:
    BYTE* m_mapped_data = nullptr;
};

class D3D12VertexBuffer : public D3D12Buffer
{
public:
    D3D12VertexBuffer() = default;
    ~D3D12VertexBuffer()  = default;
    D3D12VertexBuffer(ID3D12Device* device, UINT size); // create default heap type

    void UploadData(ID3D12Device* device, ID3D12GraphicsCommandList* cmd_list, UINT size, void* data);
    void ReleaseUploadBuffer();

private:
    Microsoft::WRL::ComPtr<ID3D12Resource> m_upload_buffer;
};

typedef D3D12VertexBuffer D3D12IndexBuffer;