#pragma once


#include "Common/d3dApp.h"
#include "Math/MathHelper.h"
#include "Common/UploadBuffer.h"
#include "D3DRHI/DescriptorCacheGPU.h"
#include "D3DRHI/DescriptorManager.h"
#include "D3DRHI/D3D12Buffer.h"
#include "D3DRHI/PSOManager.h"
#include "Texture/TextureManager.h"
#include "Mesh/MeshManager.h"
#include "Material/Material.h"
#include "Component/Component.h"
#include "GameObject/ModelGameObject.h"
#include "GameObject/CameraGameObject.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

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
	void BuildMaterials();
    void BuildShadersAndInputLayout();
    void BuildBoxGeometry();
    void BuildPSO();
    void LoadTexture();

    void SetMaterial();
    void SetGameObject();

private:
    std::unique_ptr<DescriptorCacheGPU> m_descriptor_cache = nullptr; // used to bind texture to shader
    std::unique_ptr<DescriptorManager> m_descriptor_manager = nullptr; // used to create texture srv ...

	std::unique_ptr<Material> m_material = nullptr;

    TextureManager m_texture_manager;
	MeshManager m_mesh_manager;

    std::unique_ptr<Shader> m_shader = nullptr;

    std::unique_ptr<ModelGameObject> m_chest_go;
    std::unique_ptr<CameraGameObject> m_camera;

    //std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

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
