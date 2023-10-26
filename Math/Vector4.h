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

// 4D vector
struct Vector4 : public DirectX::XMFLOAT4
{
	Vector4() noexcept : XMFLOAT4(0.f, 0.f, 0.f, 0.f) {}
	constexpr explicit Vector4(float ix) noexcept : XMFLOAT4(ix, ix, ix, ix) {}
	constexpr Vector4(float ix, float iy, float iz, float iw) noexcept : XMFLOAT4(ix, iy, iz, iw) {}
	explicit Vector4(_In_reads_(4) const float* pArray) noexcept : XMFLOAT4(pArray) {}
	Vector4(DirectX::FXMVECTOR V) noexcept { XMStoreFloat4(this, V); }
	Vector4(const XMFLOAT4& V) noexcept { this->x = V.x; this->y = V.y; this->z = V.z; this->w = V.w; }
	explicit Vector4(const DirectX::XMVECTORF32& F) noexcept { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; this->w = F.f[3]; }

	Vector4(const Vector4&) = default;
	Vector4& operator=(const Vector4&) = default;

	Vector4(Vector4&&) = default;
	Vector4& operator=(Vector4&&) = default;

	operator DirectX::XMVECTOR() const  noexcept { return XMLoadFloat4(this); }

	// Comparison operators
	bool operator == (const Vector4& V) const noexcept;
	bool operator != (const Vector4& V) const noexcept;

	// Assignment operators
	Vector4& operator= (const DirectX::XMVECTORF32& F) noexcept { x = F.f[0]; y = F.f[1]; z = F.f[2]; w = F.f[3]; return *this; }
	Vector4& operator+= (const Vector4& V) noexcept;
	Vector4& operator-= (const Vector4& V) noexcept;
	Vector4& operator*= (const Vector4& V) noexcept;
	Vector4& operator*= (float S) noexcept;
	Vector4& operator/= (float S) noexcept;

	// Unary operators
	Vector4 operator+ () const noexcept { return *this; }
	Vector4 operator- () const noexcept;

	// Vector operations
	bool InBounds(const Vector4& Bounds) const noexcept;

	float Length() const noexcept;
	float LengthSquared() const noexcept;

	float Dot(const Vector4& V) const noexcept;
	void Cross(const Vector4& v1, const Vector4& v2, Vector4& result) const noexcept;
	Vector4 Cross(const Vector4& v1, const Vector4& v2) const noexcept;

	void Normalize() noexcept;
	void Normalize(Vector4& result) const noexcept;

	void Clamp(const Vector4& vmin, const Vector4& vmax) noexcept;
	void Clamp(const Vector4& vmin, const Vector4& vmax, Vector4& result) const noexcept;

	// Static functions
	static float Distance(const Vector4& v1, const Vector4& v2) noexcept;
	static float DistanceSquared(const Vector4& v1, const Vector4& v2) noexcept;

	static void Min(const Vector4& v1, const Vector4& v2, Vector4& result) noexcept;
	static Vector4 Min(const Vector4& v1, const Vector4& v2) noexcept;

	static void Max(const Vector4& v1, const Vector4& v2, Vector4& result) noexcept;
	static Vector4 Max(const Vector4& v1, const Vector4& v2) noexcept;

	static void Lerp(const Vector4& v1, const Vector4& v2, float t, Vector4& result) noexcept;
	static Vector4 Lerp(const Vector4& v1, const Vector4& v2, float t) noexcept;

	static void SmoothStep(const Vector4& v1, const Vector4& v2, float t, Vector4& result) noexcept;
	static Vector4 SmoothStep(const Vector4& v1, const Vector4& v2, float t) noexcept;

	static void Barycentric(const Vector4& v1, const Vector4& v2, const Vector4& v3, float f, float g, Vector4& result) noexcept;
	static Vector4 Barycentric(const Vector4& v1, const Vector4& v2, const Vector4& v3, float f, float g) noexcept;

	static void CatmullRom(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4, float t, Vector4& result) noexcept;
	static Vector4 CatmullRom(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4, float t) noexcept;

	static void Hermite(const Vector4& v1, const Vector4& t1, const Vector4& v2, const Vector4& t2, float t, Vector4& result) noexcept;
	static Vector4 Hermite(const Vector4& v1, const Vector4& t1, const Vector4& v2, const Vector4& t2, float t) noexcept;

	static void Reflect(const Vector4& ivec, const Vector4& nvec, Vector4& result) noexcept;
	static Vector4 Reflect(const Vector4& ivec, const Vector4& nvec) noexcept;

	static void Refract(const Vector4& ivec, const Vector4& nvec, float refractionIndex, Vector4& result) noexcept;
	static Vector4 Refract(const Vector4& ivec, const Vector4& nvec, float refractionIndex) noexcept;

	// Constants
	static const Vector4 Zero;
	static const Vector4 One;
	static const Vector4 UnitX;
	static const Vector4 UnitY;
	static const Vector4 UnitZ;
	static const Vector4 UnitW;
};


//------------------------------------------------------------------------------
// Comparision operators
//------------------------------------------------------------------------------

inline bool Vector4::operator == (const Vector4& V) const noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(this);
	XMVECTOR v2 = XMLoadFloat4(&V);
	return XMVector4Equal(v1, v2);
}

inline bool Vector4::operator != (const Vector4& V) const noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(this);
	XMVECTOR v2 = XMLoadFloat4(&V);
	return XMVector4NotEqual(v1, v2);
}

//------------------------------------------------------------------------------
// Assignment operators
//------------------------------------------------------------------------------

inline Vector4& Vector4::operator+= (const Vector4& V) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(this);
	XMVECTOR v2 = XMLoadFloat4(&V);
	XMVECTOR X = XMVectorAdd(v1, v2);
	XMStoreFloat4(this, X);
	return *this;
}

inline Vector4& Vector4::operator-= (const Vector4& V) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(this);
	XMVECTOR v2 = XMLoadFloat4(&V);
	XMVECTOR X = XMVectorSubtract(v1, v2);
	XMStoreFloat4(this, X);
	return *this;
}

inline Vector4& Vector4::operator*= (const Vector4& V) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(this);
	XMVECTOR v2 = XMLoadFloat4(&V);
	XMVECTOR X = XMVectorMultiply(v1, v2);
	XMStoreFloat4(this, X);
	return *this;
}

inline Vector4& Vector4::operator*= (float S) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(this);
	XMVECTOR X = XMVectorScale(v1, S);
	XMStoreFloat4(this, X);
	return *this;
}

inline Vector4& Vector4::operator/= (float S) noexcept
{
	using namespace DirectX;
	assert(S != 0.0f);
	XMVECTOR v1 = XMLoadFloat4(this);
	XMVECTOR X = XMVectorScale(v1, 1.f / S);
	XMStoreFloat4(this, X);
	return *this;
}

//------------------------------------------------------------------------------
// Urnary operators
//------------------------------------------------------------------------------

inline Vector4 Vector4::operator- () const noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(this);
	XMVECTOR X = XMVectorNegate(v1);
	Vector4 R;
	XMStoreFloat4(&R, X);
	return R;
}

//------------------------------------------------------------------------------
// Binary operators
//------------------------------------------------------------------------------

inline Vector4 operator+ (const Vector4& V1, const Vector4& V2) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(&V1);
	XMVECTOR v2 = XMLoadFloat4(&V2);
	XMVECTOR X = XMVectorAdd(v1, v2);
	Vector4 R;
	XMStoreFloat4(&R, X);
	return R;
}

inline Vector4 operator- (const Vector4& V1, const Vector4& V2) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(&V1);
	XMVECTOR v2 = XMLoadFloat4(&V2);
	XMVECTOR X = XMVectorSubtract(v1, v2);
	Vector4 R;
	XMStoreFloat4(&R, X);
	return R;
}

inline Vector4 operator* (const Vector4& V1, const Vector4& V2) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(&V1);
	XMVECTOR v2 = XMLoadFloat4(&V2);
	XMVECTOR X = XMVectorMultiply(v1, v2);
	Vector4 R;
	XMStoreFloat4(&R, X);
	return R;
}

inline Vector4 operator* (const Vector4& V, float S) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(&V);
	XMVECTOR X = XMVectorScale(v1, S);
	Vector4 R;
	XMStoreFloat4(&R, X);
	return R;
}

inline Vector4 operator/ (const Vector4& V1, const Vector4& V2) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(&V1);
	XMVECTOR v2 = XMLoadFloat4(&V2);
	XMVECTOR X = XMVectorDivide(v1, v2);
	Vector4 R;
	XMStoreFloat4(&R, X);
	return R;
}

inline Vector4 operator/ (const Vector4& V, float S) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(&V);
	XMVECTOR X = XMVectorScale(v1, 1.f / S);
	Vector4 R;
	XMStoreFloat4(&R, X);
	return R;
}

inline Vector4 operator* (float S, const Vector4& V) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(&V);
	XMVECTOR X = XMVectorScale(v1, S);
	Vector4 R;
	XMStoreFloat4(&R, X);
	return R;
}

//------------------------------------------------------------------------------
// Vector operations
//------------------------------------------------------------------------------

inline bool Vector4::InBounds(const Vector4& Bounds) const noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(this);
	XMVECTOR v2 = XMLoadFloat4(&Bounds);
	return XMVector4InBounds(v1, v2);
}

inline float Vector4::Length() const noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(this);
	XMVECTOR X = XMVector4Length(v1);
	return XMVectorGetX(X);
}

inline float Vector4::LengthSquared() const noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(this);
	XMVECTOR X = XMVector4LengthSq(v1);
	return XMVectorGetX(X);
}

inline float Vector4::Dot(const Vector4& V) const noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(this);
	XMVECTOR v2 = XMLoadFloat4(&V);
	XMVECTOR X = XMVector4Dot(v1, v2);
	return XMVectorGetX(X);
}

inline void Vector4::Cross(const Vector4& v1, const Vector4& v2, Vector4& result) const noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(this);
	XMVECTOR x2 = XMLoadFloat4(&v1);
	XMVECTOR x3 = XMLoadFloat4(&v2);
	XMVECTOR R = XMVector4Cross(x1, x2, x3);
	XMStoreFloat4(&result, R);
}

inline Vector4 Vector4::Cross(const Vector4& v1, const Vector4& v2) const noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(this);
	XMVECTOR x2 = XMLoadFloat4(&v1);
	XMVECTOR x3 = XMLoadFloat4(&v2);
	XMVECTOR R = XMVector4Cross(x1, x2, x3);

	Vector4 result;
	XMStoreFloat4(&result, R);
	return result;
}

inline void Vector4::Normalize() noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(this);
	XMVECTOR X = XMVector4Normalize(v1);
	XMStoreFloat4(this, X);
}

inline void Vector4::Normalize(Vector4& result) const noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(this);
	XMVECTOR X = XMVector4Normalize(v1);
	XMStoreFloat4(&result, X);
}

inline void Vector4::Clamp(const Vector4& vmin, const Vector4& vmax) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(this);
	XMVECTOR v2 = XMLoadFloat4(&vmin);
	XMVECTOR v3 = XMLoadFloat4(&vmax);
	XMVECTOR X = XMVectorClamp(v1, v2, v3);
	XMStoreFloat4(this, X);
}

inline void Vector4::Clamp(const Vector4& vmin, const Vector4& vmax, Vector4& result) const noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(this);
	XMVECTOR v2 = XMLoadFloat4(&vmin);
	XMVECTOR v3 = XMLoadFloat4(&vmax);
	XMVECTOR X = XMVectorClamp(v1, v2, v3);
	XMStoreFloat4(&result, X);
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

inline float Vector4::Distance(const Vector4& v1, const Vector4& v2) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(&v1);
	XMVECTOR x2 = XMLoadFloat4(&v2);
	XMVECTOR V = XMVectorSubtract(x2, x1);
	XMVECTOR X = XMVector4Length(V);
	return XMVectorGetX(X);
}

inline float Vector4::DistanceSquared(const Vector4& v1, const Vector4& v2) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(&v1);
	XMVECTOR x2 = XMLoadFloat4(&v2);
	XMVECTOR V = XMVectorSubtract(x2, x1);
	XMVECTOR X = XMVector4LengthSq(V);
	return XMVectorGetX(X);
}

inline void Vector4::Min(const Vector4& v1, const Vector4& v2, Vector4& result) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(&v1);
	XMVECTOR x2 = XMLoadFloat4(&v2);
	XMVECTOR X = XMVectorMin(x1, x2);
	XMStoreFloat4(&result, X);
}

inline Vector4 Vector4::Min(const Vector4& v1, const Vector4& v2) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(&v1);
	XMVECTOR x2 = XMLoadFloat4(&v2);
	XMVECTOR X = XMVectorMin(x1, x2);

	Vector4 result;
	XMStoreFloat4(&result, X);
	return result;
}

inline void Vector4::Max(const Vector4& v1, const Vector4& v2, Vector4& result) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(&v1);
	XMVECTOR x2 = XMLoadFloat4(&v2);
	XMVECTOR X = XMVectorMax(x1, x2);
	XMStoreFloat4(&result, X);
}

inline Vector4 Vector4::Max(const Vector4& v1, const Vector4& v2) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(&v1);
	XMVECTOR x2 = XMLoadFloat4(&v2);
	XMVECTOR X = XMVectorMax(x1, x2);

	Vector4 result;
	XMStoreFloat4(&result, X);
	return result;
}

inline void Vector4::Lerp(const Vector4& v1, const Vector4& v2, float t, Vector4& result) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(&v1);
	XMVECTOR x2 = XMLoadFloat4(&v2);
	XMVECTOR X = XMVectorLerp(x1, x2, t);
	XMStoreFloat4(&result, X);
}

inline Vector4 Vector4::Lerp(const Vector4& v1, const Vector4& v2, float t) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(&v1);
	XMVECTOR x2 = XMLoadFloat4(&v2);
	XMVECTOR X = XMVectorLerp(x1, x2, t);

	Vector4 result;
	XMStoreFloat4(&result, X);
	return result;
}

inline void Vector4::SmoothStep(const Vector4& v1, const Vector4& v2, float t, Vector4& result) noexcept
{
	using namespace DirectX;
	t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
	t = t * t * (3.f - 2.f * t);
	XMVECTOR x1 = XMLoadFloat4(&v1);
	XMVECTOR x2 = XMLoadFloat4(&v2);
	XMVECTOR X = XMVectorLerp(x1, x2, t);
	XMStoreFloat4(&result, X);
}

inline Vector4 Vector4::SmoothStep(const Vector4& v1, const Vector4& v2, float t) noexcept
{
	using namespace DirectX;
	t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
	t = t * t * (3.f - 2.f * t);
	XMVECTOR x1 = XMLoadFloat4(&v1);
	XMVECTOR x2 = XMLoadFloat4(&v2);
	XMVECTOR X = XMVectorLerp(x1, x2, t);

	Vector4 result;
	XMStoreFloat4(&result, X);
	return result;
}

inline void Vector4::Barycentric(const Vector4& v1, const Vector4& v2, const Vector4& v3, float f, float g, Vector4& result) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(&v1);
	XMVECTOR x2 = XMLoadFloat4(&v2);
	XMVECTOR x3 = XMLoadFloat4(&v3);
	XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);
	XMStoreFloat4(&result, X);
}

inline Vector4 Vector4::Barycentric(const Vector4& v1, const Vector4& v2, const Vector4& v3, float f, float g) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(&v1);
	XMVECTOR x2 = XMLoadFloat4(&v2);
	XMVECTOR x3 = XMLoadFloat4(&v3);
	XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);

	Vector4 result;
	XMStoreFloat4(&result, X);
	return result;
}

inline void Vector4::CatmullRom(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4, float t, Vector4& result) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(&v1);
	XMVECTOR x2 = XMLoadFloat4(&v2);
	XMVECTOR x3 = XMLoadFloat4(&v3);
	XMVECTOR x4 = XMLoadFloat4(&v4);
	XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);
	XMStoreFloat4(&result, X);
}

inline Vector4 Vector4::CatmullRom(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4, float t) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(&v1);
	XMVECTOR x2 = XMLoadFloat4(&v2);
	XMVECTOR x3 = XMLoadFloat4(&v3);
	XMVECTOR x4 = XMLoadFloat4(&v4);
	XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);

	Vector4 result;
	XMStoreFloat4(&result, X);
	return result;
}

inline void Vector4::Hermite(const Vector4& v1, const Vector4& t1, const Vector4& v2, const Vector4& t2, float t, Vector4& result) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(&v1);
	XMVECTOR x2 = XMLoadFloat4(&t1);
	XMVECTOR x3 = XMLoadFloat4(&v2);
	XMVECTOR x4 = XMLoadFloat4(&t2);
	XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);
	XMStoreFloat4(&result, X);
}

inline Vector4 Vector4::Hermite(const Vector4& v1, const Vector4& t1, const Vector4& v2, const Vector4& t2, float t) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat4(&v1);
	XMVECTOR x2 = XMLoadFloat4(&t1);
	XMVECTOR x3 = XMLoadFloat4(&v2);
	XMVECTOR x4 = XMLoadFloat4(&t2);
	XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);

	Vector4 result;
	XMStoreFloat4(&result, X);
	return result;
}

inline void Vector4::Reflect(const Vector4& ivec, const Vector4& nvec, Vector4& result) noexcept
{
	using namespace DirectX;
	XMVECTOR i = XMLoadFloat4(&ivec);
	XMVECTOR n = XMLoadFloat4(&nvec);
	XMVECTOR X = XMVector4Reflect(i, n);
	XMStoreFloat4(&result, X);
}

inline Vector4 Vector4::Reflect(const Vector4& ivec, const Vector4& nvec) noexcept
{
	using namespace DirectX;
	XMVECTOR i = XMLoadFloat4(&ivec);
	XMVECTOR n = XMLoadFloat4(&nvec);
	XMVECTOR X = XMVector4Reflect(i, n);

	Vector4 result;
	XMStoreFloat4(&result, X);
	return result;
}

inline void Vector4::Refract(const Vector4& ivec, const Vector4& nvec, float refractionIndex, Vector4& result) noexcept
{
	using namespace DirectX;
	XMVECTOR i = XMLoadFloat4(&ivec);
	XMVECTOR n = XMLoadFloat4(&nvec);
	XMVECTOR X = XMVector4Refract(i, n, refractionIndex);
	XMStoreFloat4(&result, X);
}

inline Vector4 Vector4::Refract(const Vector4& ivec, const Vector4& nvec, float refractionIndex) noexcept
{
	using namespace DirectX;
	XMVECTOR i = XMLoadFloat4(&ivec);
	XMVECTOR n = XMLoadFloat4(&nvec);
	XMVECTOR X = XMVector4Refract(i, n, refractionIndex);

	Vector4 result;
	XMStoreFloat4(&result, X);
	return result;
}