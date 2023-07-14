#pragma once

#include "./Shader.h"
#include "./Common/d3dApp.h"
#include "./Common/MathHelper.h"
#include "./Common/UploadBuffer.h"
#include "./D3DRHI/DescriptorCacheGPU.h"
#include "./D3DRHI/DescriptorManager.h"
#include "./D3DRHI/D3D12Buffer.h"
#include "./D3DRHI/PSOManager.h"


using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

struct Vertex
{
    XMFLOAT3 Pos;
    XMFLOAT4 Color;
};

struct ObjectConstants
{
    XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();
};

class BoxApp : public D3DApp
{
public:
	BoxApp(HINSTANCE hInstance);
    BoxApp(const BoxApp& rhs) = delete;
    BoxApp& operator=(const BoxApp& rhs) = delete;
	~BoxApp();

	virtual bool Initialize()override;

private:
    virtual void OnResize()override;
    virtual void Update(const GameTimer& gt)override;
    virtual void Draw(const GameTimer& gt)override;

    virtual void OnMouseDown(WPARAM btnState, int x, int y)override;
    virtual void OnMouseUp(WPARAM btnState, int x, int y)override;
    virtual void OnMouseMove(WPARAM btnState, int x, int y)override;

    void BuildDescriptorHeaps();
	void BuildConstantBuffers();
    void BuildShadersAndInputLayout();
    void BuildBoxGeometry();
    void BuildPSO();

private:
    std::unique_ptr<DescriptorCacheGPU> m_descriptor_cache = nullptr; // used to bind texture to shader
    std::unique_ptr<DescriptorManager> m_descriptor_manager = nullptr; // used to create texture srv ...

    std::unique_ptr<D3D12ConstantBuffer> m_object_cb = nullptr;

	std::unique_ptr<MeshGeometry> mBoxGeo = nullptr;


    std::unique_ptr<Shader> m_shader = nullptr;

    std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

    //ComPtr<ID3D12PipelineState> mPSO = nullptr;
    PSOManager m_PSO_manager;

    XMFLOAT4X4 mWorld = MathHelper::Identity4x4();
    XMFLOAT4X4 mView = MathHelper::Identity4x4();
    XMFLOAT4X4 mProj = MathHelper::Identity4x4();

    float mTheta = 1.5f*XM_PI;
    float mPhi = XM_PIDIV4;
    float mRadius = 5.0f;

    POINT mLastMousePos;
};
