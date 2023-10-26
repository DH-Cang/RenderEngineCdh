// Modified version of DirectXTK12's source file

//-------------------------------------------------------------------------------------
// SimpleMath.h -- Simplified C++ Math wrapper for DirectXMath
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkId=248929
// http://go.microsoft.com/fwlink/?LinkID=615561
//-------------------------------------------------------------------------------------

#pragma once

#include <DirectXMath.h>

// 3D vector
struct Vector3 : public DirectX::XMFLOAT3
{
	Vector3() noexcept : XMFLOAT3(0.f, 0.f, 0.f) {}
	constexpr explicit Vector3(float ix) noexcept : XMFLOAT3(ix, ix, ix) {}
	constexpr Vector3(float ix, float iy, float iz) noexcept : XMFLOAT3(ix, iy, iz) {}
	explicit Vector3(_In_reads_(3) const float* pArray) noexcept : XMFLOAT3(pArray) {}
	Vector3(DirectX::FXMVECTOR V) noexcept { XMStoreFloat3(this, V); }
	Vector3(const XMFLOAT3& V) noexcept { this->x = V.x; this->y = V.y; this->z = V.z; }
	explicit Vector3(const DirectX::XMVECTORF32& F) noexcept { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; }

	Vector3(const Vector3&) = default;
	Vector3& operator=(const Vector3&) = default;

	Vector3(Vector3&&) = default;
	Vector3& operator=(Vector3&&) = default;

	operator DirectX::XMVECTOR() const noexcept { return XMLoadFloat3(this); }

	// Access operators
	float operator [](int i) const noexcept
	{
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}

	float& operator [](int i) noexcept
	{
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}

	// Comparison operators
	bool operator == (const Vector3& V) const noexcept;
	bool operator != (const Vector3& V) const noexcept;

	// Assignment operators
	Vector3& operator= (const DirectX::XMVECTORF32& F) noexcept { x = F.f[0]; y = F.f[1]; z = F.f[2]; return *this; }
	Vector3& operator+= (const Vector3& V) noexcept;
	Vector3& operator-= (const Vector3& V) noexcept;
	Vector3& operator*= (const Vector3& V) noexcept;
	Vector3& operator*= (float S) noexcept;
	Vector3& operator/= (float S) noexcept;

	// Unary operators
	Vector3 operator+ () const noexcept { return *this; }
	Vector3 operator- () const noexcept;

	// Vector operations
	bool InBounds(const Vector3& Bounds) const noexcept;

	float Length() const noexcept;
	float LengthSquared() const noexcept;

	float Dot(const Vector3& V) const noexcept;
	void Cross(const Vector3& V, Vector3& result) const noexcept;
	Vector3 Cross(const Vector3& V) const noexcept;

	void Normalize() noexcept;
	void Normalize(Vector3& result) const noexcept;

	void Clamp(const Vector3& vmin, const Vector3& vmax) noexcept;
	void Clamp(const Vector3& vmin, const Vector3& vmax, Vector3& result) const noexcept;

	// Static functions
	static float Distance(const Vector3& v1, const Vector3& v2) noexcept;
	static float DistanceSquared(const Vector3& v1, const Vector3& v2) noexcept;

	static void Min(const Vector3& v1, const Vector3& v2, Vector3& result) noexcept;
	static Vector3 Min(const Vector3& v1, const Vector3& v2) noexcept;

	static void Max(const Vector3& v1, const Vector3& v2, Vector3& result) noexcept;
	static Vector3 Max(const Vector3& v1, const Vector3& v2) noexcept;

	static void Lerp(const Vector3& v1, const Vector3& v2, float t, Vector3& result) noexcept;
	static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) noexcept;

	static void SmoothStep(const Vector3& v1, const Vector3& v2, float t, Vector3& result) noexcept;
	static Vector3 SmoothStep(const Vector3& v1, const Vector3& v2, float t) noexcept;

	static void Barycentric(const Vector3& v1, const Vector3& v2, const Vector3& v3, float f, float g, Vector3& result) noexcept;
	static Vector3 Barycentric(const Vector3& v1, const Vector3& v2, const Vector3& v3, float f, float g) noexcept;

	static void CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float t, Vector3& result) noexcept;
	static Vector3 CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float t) noexcept;

	static void Hermite(const Vector3& v1, const Vector3& t1, const Vector3& v2, const Vector3& t2, float t, Vector3& result) noexcept;
	static Vector3 Hermite(const Vector3& v1, const Vector3& t1, const Vector3& v2, const Vector3& t2, float t) noexcept;

	static void Reflect(const Vector3& ivec, const Vector3& nvec, Vector3& result) noexcept;
	static Vector3 Reflect(const Vector3& ivec, const Vector3& nvec) noexcept;

	static void Refract(const Vector3& ivec, const Vector3& nvec, float refractionIndex, Vector3& result) noexcept;
	static Vector3 Refract(const Vector3& ivec, const Vector3& nvec, float refractionIndex) noexcept;

	// Constants
	static const Vector3 Zero;
	static const Vector3 One;
	static const Vector3 UnitX;
	static const Vector3 UnitY;
	static const Vector3 UnitZ;
	static const Vector3 Up;
	static const Vector3 Down;
	static const Vector3 Right;
	static const Vector3 Left;
	static const Vector3 Forward;
	static const Vector3 Backward;
};

//------------------------------------------------------------------------------
// Comparision operators
//------------------------------------------------------------------------------

inline bool Vector3::operator == (const Vector3& V) const noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR v2 = XMLoadFloat3(&V);
	return XMVector3Equal(v1, v2);
}

inline bool Vector3::operator != (const Vector3& V) const noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR v2 = XMLoadFloat3(&V);
	return XMVector3NotEqual(v1, v2);
}

//------------------------------------------------------------------------------
// Assignment operators
//------------------------------------------------------------------------------

inline Vector3& Vector3::operator+= (const Vector3& V) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR v2 = XMLoadFloat3(&V);
	XMVECTOR X = XMVectorAdd(v1, v2);
	XMStoreFloat3(this, X);
	return *this;
}

inline Vector3& Vector3::operator-= (const Vector3& V) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR v2 = XMLoadFloat3(&V);
	XMVECTOR X = XMVectorSubtract(v1, v2);
	XMStoreFloat3(this, X);
	return *this;
}

inline Vector3& Vector3::operator*= (const Vector3& V) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR v2 = XMLoadFloat3(&V);
	XMVECTOR X = XMVectorMultiply(v1, v2);
	XMStoreFloat3(this, X);
	return *this;
}

inline Vector3& Vector3::operator*= (float S) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR X = XMVectorScale(v1, S);
	XMStoreFloat3(this, X);
	return *this;
}

inline Vector3& Vector3::operator/= (float S) noexcept
{
	using namespace DirectX;
	assert(S != 0.0f);
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR X = XMVectorScale(v1, 1.f / S);
	XMStoreFloat3(this, X);
	return *this;
}

//------------------------------------------------------------------------------
// Urnary operators
//------------------------------------------------------------------------------

inline Vector3 Vector3::operator- () const noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR X = XMVectorNegate(v1);
	Vector3 R;
	XMStoreFloat3(&R, X);
	return R;
}

//------------------------------------------------------------------------------
// Binary operators
//------------------------------------------------------------------------------

inline Vector3 operator+ (const Vector3& V1, const Vector3& V2) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(&V1);
	XMVECTOR v2 = XMLoadFloat3(&V2);
	XMVECTOR X = XMVectorAdd(v1, v2);
	Vector3 R;
	XMStoreFloat3(&R, X);
	return R;
}

inline Vector3 operator- (const Vector3& V1, const Vector3& V2) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(&V1);
	XMVECTOR v2 = XMLoadFloat3(&V2);
	XMVECTOR X = XMVectorSubtract(v1, v2);
	Vector3 R;
	XMStoreFloat3(&R, X);
	return R;
}

inline Vector3 operator* (const Vector3& V1, const Vector3& V2) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(&V1);
	XMVECTOR v2 = XMLoadFloat3(&V2);
	XMVECTOR X = XMVectorMultiply(v1, v2);
	Vector3 R;
	XMStoreFloat3(&R, X);
	return R;
}

inline Vector3 operator* (const Vector3& V, float S) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(&V);
	XMVECTOR X = XMVectorScale(v1, S);
	Vector3 R;
	XMStoreFloat3(&R, X);
	return R;
}

inline Vector3 operator/ (const Vector3& V1, const Vector3& V2) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(&V1);
	XMVECTOR v2 = XMLoadFloat3(&V2);
	XMVECTOR X = XMVectorDivide(v1, v2);
	Vector3 R;
	XMStoreFloat3(&R, X);
	return R;
}

inline Vector3 operator/ (const Vector3& V, float S) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(&V);
	XMVECTOR X = XMVectorScale(v1, 1.f / S);
	Vector3 R;
	XMStoreFloat3(&R, X);
	return R;
}

inline Vector3 operator* (float S, const Vector3& V) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(&V);
	XMVECTOR X = XMVectorScale(v1, S);
	Vector3 R;
	XMStoreFloat3(&R, X);
	return R;
}

//------------------------------------------------------------------------------
// Vector operations
//------------------------------------------------------------------------------

inline bool Vector3::InBounds(const Vector3& Bounds) const noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR v2 = XMLoadFloat3(&Bounds);
	return XMVector3InBounds(v1, v2);
}

inline float Vector3::Length() const noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR X = XMVector3Length(v1);
	return XMVectorGetX(X);
}

inline float Vector3::LengthSquared() const noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR X = XMVector3LengthSq(v1);
	return XMVectorGetX(X);
}

inline float Vector3::Dot(const Vector3& V) const noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR v2 = XMLoadFloat3(&V);
	XMVECTOR X = XMVector3Dot(v1, v2);
	return XMVectorGetX(X);
}

inline void Vector3::Cross(const Vector3& V, Vector3& result) const noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR v2 = XMLoadFloat3(&V);
	XMVECTOR R = XMVector3Cross(v1, v2);
	XMStoreFloat3(&result, R);
}

inline Vector3 Vector3::Cross(const Vector3& V) const noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR v2 = XMLoadFloat3(&V);
	XMVECTOR R = XMVector3Cross(v1, v2);

	Vector3 result;
	XMStoreFloat3(&result, R);
	return result;
}

inline void Vector3::Normalize() noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR X = XMVector3Normalize(v1);
	XMStoreFloat3(this, X);
}

inline void Vector3::Normalize(Vector3& result) const noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR X = XMVector3Normalize(v1);
	XMStoreFloat3(&result, X);
}

inline void Vector3::Clamp(const Vector3& vmin, const Vector3& vmax) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR v2 = XMLoadFloat3(&vmin);
	XMVECTOR v3 = XMLoadFloat3(&vmax);
	XMVECTOR X = XMVectorClamp(v1, v2, v3);
	XMStoreFloat3(this, X);
}

inline void Vector3::Clamp(const Vector3& vmin, const Vector3& vmax, Vector3& result) const noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(this);
	XMVECTOR v2 = XMLoadFloat3(&vmin);
	XMVECTOR v3 = XMLoadFloat3(&vmax);
	XMVECTOR X = XMVectorClamp(v1, v2, v3);
	XMStoreFloat3(&result, X);
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

inline float Vector3::Distance(const Vector3& v1, const Vector3& v2) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat3(&v1);
	XMVECTOR x2 = XMLoadFloat3(&v2);
	XMVECTOR V = XMVectorSubtract(x2, x1);
	XMVECTOR X = XMVector3Length(V);
	return XMVectorGetX(X);
}

inline float Vector3::DistanceSquared(const Vector3& v1, const Vector3& v2) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat3(&v1);
	XMVECTOR x2 = XMLoadFloat3(&v2);
	XMVECTOR V = XMVectorSubtract(x2, x1);
	XMVECTOR X = XMVector3LengthSq(V);
	return XMVectorGetX(X);
}

inline void Vector3::Min(const Vector3& v1, const Vector3& v2, Vector3& result) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat3(&v1);
	XMVECTOR x2 = XMLoadFloat3(&v2);
	XMVECTOR X = XMVectorMin(x1, x2);
	XMStoreFloat3(&result, X);
}

inline Vector3 Vector3::Min(const Vector3& v1, const Vector3& v2) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat3(&v1);
	XMVECTOR x2 = XMLoadFloat3(&v2);
	XMVECTOR X = XMVectorMin(x1, x2);

	Vector3 result;
	XMStoreFloat3(&result, X);
	return result;
}

inline void Vector3::Max(const Vector3& v1, const Vector3& v2, Vector3& result) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat3(&v1);
	XMVECTOR x2 = XMLoadFloat3(&v2);
	XMVECTOR X = XMVectorMax(x1, x2);
	XMStoreFloat3(&result, X);
}

inline Vector3 Vector3::Max(const Vector3& v1, const Vector3& v2) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat3(&v1);
	XMVECTOR x2 = XMLoadFloat3(&v2);
	XMVECTOR X = XMVectorMax(x1, x2);

	Vector3 result;
	XMStoreFloat3(&result, X);
	return result;
}

inline void Vector3::Lerp(const Vector3& v1, const Vector3& v2, float t, Vector3& result) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat3(&v1);
	XMVECTOR x2 = XMLoadFloat3(&v2);
	XMVECTOR X = XMVectorLerp(x1, x2, t);
	XMStoreFloat3(&result, X);
}

inline Vector3 Vector3::Lerp(const Vector3& v1, const Vector3& v2, float t) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat3(&v1);
	XMVECTOR x2 = XMLoadFloat3(&v2);
	XMVECTOR X = XMVectorLerp(x1, x2, t);

	Vector3 result;
	XMStoreFloat3(&result, X);
	return result;
}

inline void Vector3::SmoothStep(const Vector3& v1, const Vector3& v2, float t, Vector3& result) noexcept
{
	using namespace DirectX;
	t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
	t = t * t * (3.f - 2.f * t);
	XMVECTOR x1 = XMLoadFloat3(&v1);
	XMVECTOR x2 = XMLoadFloat3(&v2);
	XMVECTOR X = XMVectorLerp(x1, x2, t);
	XMStoreFloat3(&result, X);
}

inline Vector3 Vector3::SmoothStep(const Vector3& v1, const Vector3& v2, float t) noexcept
{
	using namespace DirectX;
	t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
	t = t * t * (3.f - 2.f * t);
	XMVECTOR x1 = XMLoadFloat3(&v1);
	XMVECTOR x2 = XMLoadFloat3(&v2);
	XMVECTOR X = XMVectorLerp(x1, x2, t);

	Vector3 result;
	XMStoreFloat3(&result, X);
	return result;
}

inline void Vector3::Barycentric(const Vector3& v1, const Vector3& v2, const Vector3& v3, float f, float g, Vector3& result) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat3(&v1);
	XMVECTOR x2 = XMLoadFloat3(&v2);
	XMVECTOR x3 = XMLoadFloat3(&v3);
	XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);
	XMStoreFloat3(&result, X);
}

inline Vector3 Vector3::Barycentric(const Vector3& v1, const Vector3& v2, const Vector3& v3, float f, float g) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat3(&v1);
	XMVECTOR x2 = XMLoadFloat3(&v2);
	XMVECTOR x3 = XMLoadFloat3(&v3);
	XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);

	Vector3 result;
	XMStoreFloat3(&result, X);
	return result;
}

inline void Vector3::CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float t, Vector3& result) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat3(&v1);
	XMVECTOR x2 = XMLoadFloat3(&v2);
	XMVECTOR x3 = XMLoadFloat3(&v3);
	XMVECTOR x4 = XMLoadFloat3(&v4);
	XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);
	XMStoreFloat3(&result, X);
}

inline Vector3 Vector3::CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float t) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat3(&v1);
	XMVECTOR x2 = XMLoadFloat3(&v2);
	XMVECTOR x3 = XMLoadFloat3(&v3);
	XMVECTOR x4 = XMLoadFloat3(&v4);
	XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);

	Vector3 result;
	XMStoreFloat3(&result, X);
	return result;
}

inline void Vector3::Hermite(const Vector3& v1, const Vector3& t1, const Vector3& v2, const Vector3& t2, float t, Vector3& result) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat3(&v1);
	XMVECTOR x2 = XMLoadFloat3(&t1);
	XMVECTOR x3 = XMLoadFloat3(&v2);
	XMVECTOR x4 = XMLoadFloat3(&t2);
	XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);
	XMStoreFloat3(&result, X);
}

inline Vector3 Vector3::Hermite(const Vector3& v1, const Vector3& t1, const Vector3& v2, const Vector3& t2, float t) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat3(&v1);
	XMVECTOR x2 = XMLoadFloat3(&t1);
	XMVECTOR x3 = XMLoadFloat3(&v2);
	XMVECTOR x4 = XMLoadFloat3(&t2);
	XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);

	Vector3 result;
	XMStoreFloat3(&result, X);
	return result;
}

inline void Vector3::Reflect(const Vector3& ivec, const Vector3& nvec, Vector3& result) noexcept
{
	using namespace DirectX;
	XMVECTOR i = XMLoadFloat3(&ivec);
	XMVECTOR n = XMLoadFloat3(&nvec);
	XMVECTOR X = XMVector3Reflect(i, n);
	XMStoreFloat3(&result, X);
}

inline Vector3 Vector3::Reflect(const Vector3& ivec, const Vector3& nvec) noexcept
{
	using namespace DirectX;
	XMVECTOR i = XMLoadFloat3(&ivec);
	XMVECTOR n = XMLoadFloat3(&nvec);
	XMVECTOR X = XMVector3Reflect(i, n);

	Vector3 result;
	XMStoreFloat3(&result, X);
	return result;
}

inline void Vector3::Refract(const Vector3& ivec, const Vector3& nvec, float refractionIndex, Vector3& result) noexcept
{
	using namespace DirectX;
	XMVECTOR i = XMLoadFloat3(&ivec);
	XMVECTOR n = XMLoadFloat3(&nvec);
	XMVECTOR X = XMVector3Refract(i, n, refractionIndex);
	XMStoreFloat3(&result, X);
}

inline Vector3 Vector3::Refract(const Vector3& ivec, const Vector3& nvec, float refractionIndex) noexcept
{
	using namespace DirectX;
	XMVECTOR i = XMLoadFloat3(&ivec);
	XMVECTOR n = XMLoadFloat3(&nvec);
	XMVECTOR X = XMVector3Refract(i, n, refractionIndex);

	Vector3 result;
	XMStoreFloat3(&result, X);
	return result;
}