#include "Mesh.h"

void Mesh::SetVerticesCPU(const std::vector<Vertex> &vertices)
{
    m_vertices.assign(vertices.begin(), vertices.end());
}

void Mesh::SetIndicesCPU(const std::vector<std::uint16_t> &indices)
{
    m_indices16.assign(indices.begin(), indices.end());
}

void Mesh::UploadDataToGPU(ID3D12Device *device, ID3D12GraphicsCommandList *cmd_list)
{
    // upload vertex buffer
    m_vertex_buf.UploadData(device, cmd_list, m_vertices.size() * sizeof(Vertex), m_vertices.data());
    // create vertex buffer view
    m_vbv.BufferLocation = m_vertex_buf.GetResource()->GetGPUVirtualAddress();
    m_vbv.StrideInBytes = sizeof(Vertex);
    m_vbv.SizeInBytes = sizeof(Vertex) * m_vertices.size();

    // upload index buffer
    m_index_buf.UploadData(device, cmd_list, m_indices16.size() * sizeof(std::uint16_t), m_indices16.data());
    // create index buffer view
    m_ibv.BufferLocation = m_index_buf.GetResource()->GetGPUVirtualAddress();
    m_ibv.Format = DXGI_FORMAT_R16_UINT;
    m_ibv.SizeInBytes = sizeof(std::uint16_t) * m_indices16.size();
}

void Mesh::ReleaseUploadBuffer()
{
    m_vertex_buf.ReleaseUploadBuffer();
    m_index_buf.ReleaseUploadBuffer();
}
