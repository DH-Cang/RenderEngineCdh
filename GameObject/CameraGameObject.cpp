#include "CameraGameObject.h"

Vector3 CameraGameObject::GetPosition() const
{
    return m_root_component->GetWorldLocation();
}

void CameraGameObject::SetPosition(float x, float y, float z)
{
    m_root_component->SetWorldLocation(Vector3(x, y, z));
    m_is_dirty = true;
}

void CameraGameObject::SetPosition(const Vector3& position)
{
    m_root_component->SetWorldLocation(position);
    m_is_dirty = true;
}
	
Vector3 CameraGameObject::GetUp() const
{
    return m_up;
}

Vector3 CameraGameObject::GetLook() const
{
    return m_look;
}

Vector3 CameraGameObject::GetRight() const
{
    return m_right;
}

float CameraGameObject::GetNearZ()const
{
    return m_nearz;
}

float CameraGameObject::GetFarZ()const
{
    return m_farz;
}

float CameraGameObject::GetAspectRatio()const
{
    return m_aspect_ratio;
}

float CameraGameObject::GetFovY()const
{
    return m_fov_vertical;
}

float CameraGameObject::GetFovX()const
{
    float halfWidth = 0.5f * GetNearWindowWidth();
	return float(2.0f * atan(halfWidth / m_nearz));
}

float CameraGameObject::GetNearWindowWidth()const
{
    return m_aspect_ratio * m_near_window_height;
}

float CameraGameObject::GetNearWindowHeight()const
{
    return m_near_window_height;
}

float CameraGameObject::GetFarWindowWidth()const
{
    return m_aspect_ratio * m_far_window_height;
}

float CameraGameObject::GetFarWindowHeight()const
{
    return m_far_window_height;
}

// Set frustum.
void CameraGameObject::SetLens(float fov_vertical, float aspect, float znear, float zfar)
{
    m_fov_vertical = fov_vertical;
	m_aspect_ratio = aspect;
	m_nearz = znear;
	m_farz = zfar;

	m_near_window_height = 2.0f * m_nearz * tanf( 0.5f * m_fov_vertical );
	m_far_window_height  = 2.0f * m_farz * tanf( 0.5f * m_fov_vertical );
}

// Define camera space via LookAt parameters.
void CameraGameObject::LookAt(const Vector3& camera_pos, const Vector3& target_pos, const Vector3& up_dir)
{
    SetPosition(camera_pos);

	m_look = (target_pos - m_root_component->GetWorldLocation());
    m_look.Normalize();
    m_right = up_dir.Cross(m_look);
    m_right.Normalize();
    m_up = m_look.Cross(m_right);

    m_is_dirty = true;
}

void CameraGameObject::UpdateViewMatrix()
{
    if (m_is_dirty)
	{
		// Keep camera's axes orthogonal to each other and of unit length.
		m_look.Normalize();
		m_up = m_look.Cross(m_right);
		m_up.Normalize();

		// Up, Look already ortho-normal, so no need to normalize cross product.
		m_right = m_up.Cross(m_look);

		// Fill in the view matrix entries.
		float x = - m_root_component->GetWorldLocation().Dot(m_right);
		float y = - m_root_component->GetWorldLocation().Dot(m_up);
		float z = - m_root_component->GetWorldLocation().Dot(m_look);

		m_view(0, 0) = m_right.x;
		m_view(1, 0) = m_right.y;
		m_view(2, 0) = m_right.z;
		m_view(3, 0) = x;

		m_view(0, 1) = m_up.x;
		m_view(1, 1) = m_up.y;
		m_view(2, 1) = m_up.z;
		m_view(3, 1) = y;

		m_view(0, 2) = m_look.x;
		m_view(1, 2) = m_look.y;
		m_view(2, 2) = m_look.z;
		m_view(3, 2) = z;

		m_view(0, 3) = 0.0f;
		m_view(1, 3) = 0.0f;
		m_view(2, 3) = 0.0f;
		m_view(3, 3) = 1.0f;

		m_is_dirty = false;
	}
}

// Get View/Proj matrices.
Matrix CameraGameObject::GetViewMatrix()const
{
	assert(m_is_dirty == false);
    return m_view;
}

Matrix CameraGameObject::GetProjMatrix()const
{
    return Matrix::CreatePerspectiveFieldOfView(m_fov_vertical, m_aspect_ratio, m_nearz, m_farz);
}

