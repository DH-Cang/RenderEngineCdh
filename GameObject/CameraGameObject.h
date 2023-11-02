#pragma once
#include "GameObject.h"

// left hand coord
// cross(right, up) = -look
class CameraGameObject : public GameObject
{
public:
	CameraGameObject() = delete;
    CameraGameObject(const std::string& Name): GameObject(Name) { UpdateViewMatrix(); }
    virtual ~CameraGameObject() = default;

	// Get/Set world camera position.
	Vector3 GetPosition() const;
	void SetPosition(float x, float y, float z);
	void SetPosition(const Vector3& position);
	
	// Get camera basis vectors.
	Vector3 GetUp() const;
	Vector3 GetLook() const;
	Vector3 GetRight() const;

	// Get frustum properties.
	float GetNearZ()const;
	float GetFarZ()const;
	float GetAspectRatio()const;
	float GetFovY()const;
	float GetFovX()const;

	// Get near and far plane dimensions in view space coordinates.
	float GetNearWindowWidth()const;
	float GetNearWindowHeight()const;
	float GetFarWindowWidth()const;
	float GetFarWindowHeight()const;
	
	// Set frustum.
	void SetLens(float fovY, float aspect, float znear, float zfar); // fovY in radians

	// Define camera space via LookAt parameters.
	void LookAt(const Vector3& camera_pos, const Vector3& target_pos, const Vector3& up_dir);

	// Get View/Proj matrices.
	void UpdateViewMatrix();
	Matrix GetViewMatrix()const;
	Matrix GetProjMatrix()const;

	//TODO
	// Strafe/Walk the camera a distance d.
	void Strafe(float d);
	void Walk(float d);

	// TODO
	// Rotate the camera.
	void Pitch(float angle);
	void RotateY(float angle);

private:

	// Camera left hand coord system
	Vector3 m_right = { 1.0f, 0.0f, 0.0f };
	Vector3 m_up = { 0.0f, 1.0f, 0.0f };
	Vector3 m_look = { 0.0f, 0.0f, 1.0f };

	// Cache frustum properties.
	float m_nearz = 0.0f;
	float m_farz = 0.0f;
	float m_aspect_ratio = 0.0f; // width/height
	float m_fov_vertical = 0.0f;
	float m_near_window_height = 0.0f;
	float m_far_window_height = 0.0f;

	// Cache View/Proj matrices.
	Matrix m_view = Matrix::Identity;
	bool m_is_dirty = true;
};