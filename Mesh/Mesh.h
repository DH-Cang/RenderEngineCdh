#pragma once
#include <vector>
#include "D3DRHI/D3D12Buffer.h"
#include "Vertex.h"

class Mesh
{
public:
    Mesh() = default;
    ~Mesh() = default;

    void SetVerticesCPU(const std::vector<Vertex>& vertices);
    void SetIndicesCPU(const std::vector<std::uint16_t>& indices);
    size_t GetIndicesCount() const { return m_indices16.size(); }
    void UploadDataToGPU(ID3D12Device* device, ID3D12GraphicsCommandList* cmd_list);
    void ReleaseUploadBuffer();
    D3D12_VERTEX_BUFFER_VIEW* GetVertexBufferView(){ return &m_vbv; }
    D3D12_INDEX_BUFFER_VIEW* GetIndexBufferView(){ return &m_ibv; }

private:
    std::vector<std::uint16_t> m_indices16;
    std::vector<Vertex> m_vertices;

    D3D12VertexBuffer m_vertex_buf;
    D3D12IndexBuffer m_index_buf;

    D3D12_VERTEX_BUFFER_VIEW m_vbv;
    D3D12_INDEX_BUFFER_VIEW m_ibv;
};