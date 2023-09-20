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

D3D12VertexBuffer::D3D12VertexBuffer(ID3D12Device *device, UINT size)
{
    // Create the actual default buffer resource.
    ThrowIfFailed(device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(size),
		D3D12_RESOURCE_STATE_COMMON,
        nullptr,
        IID_PPV_ARGS(m_d3d_resource.GetAddressOf())));
}

void D3D12VertexBuffer::UploadData(ID3D12Device* device, ID3D12GraphicsCommandList* cmd_list, UINT size, void* data)
{
    if(m_d3d_resource == nullptr)
    {
        // Create the actual default buffer resource.
        ThrowIfFailed(device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
            D3D12_HEAP_FLAG_NONE,
            &CD3DX12_RESOURCE_DESC::Buffer(size),
            D3D12_RESOURCE_STATE_COMMON,
            nullptr,
            IID_PPV_ARGS(m_d3d_resource.GetAddressOf())));
    }

    // In order to copy CPU memory data into our default buffer, we need to create
    // an intermediate upload heap. 
    ThrowIfFailed(device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(size),
		D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(m_upload_buffer.GetAddressOf())));

    // Describe the data we want to copy into the default buffer.
    D3D12_SUBRESOURCE_DATA subResourceData = {};
    subResourceData.pData = data;
    subResourceData.RowPitch = size;
    subResourceData.SlicePitch = subResourceData.RowPitch;

    // Schedule to copy the data to the default buffer resource.  At a high level, the helper function UpdateSubresources
    // will copy the CPU memory into the intermediate upload heap.  Then, using ID3D12CommandList::CopySubresourceRegion,
    // the intermediate upload heap data will be copied to mBuffer.
	cmd_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_d3d_resource.Get(), 
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
    UpdateSubresources<1>(cmd_list, m_d3d_resource.Get(), m_upload_buffer.Get(), 0, 0, 1, &subResourceData);
	cmd_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_d3d_resource.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));
}

void D3D12VertexBuffer::ReleaseUploadBuffer()
{
    if(m_upload_buffer)
    {
        m_upload_buffer.Reset();
    }
}

