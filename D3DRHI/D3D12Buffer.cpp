#include "D3D12Buffer.h"

D3D12ConstantBuffer::D3D12ConstantBuffer(ID3D12Device *device, UINT byte_size)
{
    // Constant buffer elements need to be multiples of 256 bytes.
    // This is because the hardware can only view constant data 
    // at m*256 byte offsets and of n*256 byte lengths. 
    // typedef struct D3D12_CONSTANT_BUFFER_VIEW_DESC {
    // UINT64 OffsetInBytes; // multiple of 256
    // UINT   SizeInBytes;   // multiple of 256
    // } D3D12_CONSTANT_BUFFER_VIEW_DESC;
    UINT aligned_byte_size = d3dUtil::CalcConstantBufferByteSize(byte_size); // align, TODO: delete?

    auto heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    auto desc = CD3DX12_RESOURCE_DESC::Buffer(aligned_byte_size);
    ThrowIfFailed(device->CreateCommittedResource(
        &heap_properties,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_d3d_resource)));

    ThrowIfFailed(m_d3d_resource->Map(0, nullptr, reinterpret_cast<void**>(&m_mapped_data)));
}

D3D12ConstantBuffer::~D3D12ConstantBuffer()
{
    if(m_d3d_resource != nullptr)
        m_d3d_resource->Unmap(0, nullptr);

    m_mapped_data = nullptr;
}

void D3D12ConstantBuffer::CopyData(void *data, int size)
{
    memcpy(m_mapped_data, data, size);
}
