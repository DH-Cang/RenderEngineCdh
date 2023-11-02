#pragma once
#include <cstdint>
#include <DirectXMath.h>
#include <vector>
#include <d3d12.h>

class Vertex
{
public:
    Vertex() = default;

    Vertex(
        const DirectX::XMFLOAT3& p, 
        const DirectX::XMFLOAT3& n, 
        const DirectX::XMFLOAT3& t, 
        const DirectX::XMFLOAT2& uv) :
        Position(p), 
        Normal(n), 
        TangentU(t), 
        TexC(uv)
        {}

    Vertex(
        float px, float py, float pz, 
        float nx, float ny, float nz,
        float tx, float ty, float tz,
        float u, float v) : 
        Position(px,py,pz), 
        Normal(nx,ny,nz),
        TangentU(tx, ty, tz), 
        TexC(u,v)
        {}

    DirectX::XMFLOAT3 Position;
    DirectX::XMFLOAT3 Normal;
    DirectX::XMFLOAT3 TangentU;
    DirectX::XMFLOAT2 TexC;

    static std::vector<D3D12_INPUT_ELEMENT_DESC> GetVSInputLayout()
    {
        std::vector<D3D12_INPUT_ELEMENT_DESC> ret =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
        };
        return ret;
    }
};
