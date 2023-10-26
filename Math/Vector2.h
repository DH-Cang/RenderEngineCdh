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

// 2D vector
struct Vector2 : public DirectX::XMFLOAT2
{
	Vector2() noexcept : XMFLOAT2(0.f, 0.f) {}
	constexpr explicit Vector2(float ix) noexcept : XMFLOAT2(ix, ix) {}
	constexpr Vector2(float ix, float iy) noexcept : XMFLOAT2(ix, iy) {}
	explicit Vector2(_In_reads_(2) const float* pArray) noexcept : XMFLOAT2(pArray) {}
	Vector2(DirectX::FXMVECTOR V) noexcept { XMStoreFloat2(this, V); }
	Vector2(const XMFLOAT2& V) noexcept { this->x = V.x; this->y = V.y; }
	explicit Vector2(const DirectX::XMVECTORF32& F) noexcept { this->x = F.f[0]; this->y = F.f[1]; }

	Vector2(const Vector2&) = default;
	Vector2& operator=(const Vector2&) = default;

	Vector2(Vector2&&) = default;
	Vector2& operator=(Vector2&&) = default;

	operator DirectX::XMVECTOR() const noexcept { return XMLoadFloat2(this); }

	// Comparison operators
	bool operator == (const Vector2& V) const noexcept;
	bool operator != (const Vector2& V) const noexcept;

	// Assignment operators
	Vector2& operator= (const DirectX::XMVECTORF32& F) noexcept { x = F.f[0]; y = F.f[1]; return *this; }
	Vector2& operator+= (const Vector2& V) noexcept;
	Vector2& operator-= (const Vector2& V) noexcept;
	Vector2& operator*= (const Vector2& V) noexcept;
	Vector2& operator*= (float S) noexcept;
	Vector2& operator/= (float S) noexcept;

	// Unary operators
	Vector2 operator+ () const noexcept { return *this; }
	Vector2 operator- () const noexcept { return Vector2(-x, -y); }

	// Vector operations
	bool InBounds(const Vector2& Bounds) const noexcept;

	float Length() const noexcept;
	float LengthSquared() const noexcept;

	float Dot(const Vector2& V) const noexcept;
	void Cross(const Vector2& V, Vector2& result) const noexcept;
	Vector2 Cross(const Vector2& V) const noexcept;

	void Normalize() noexcept;
	void Normalize(Vector2& result) const noexcept;

	void Clamp(const Vector2& vmin, const Vector2& vmax) noexcept;
	void Clamp(const Vector2& vmin, const Vector2& vmax, Vector2& result) const noexcept;

	// Static functions
	static float Distance(const Vector2& v1, const Vector2& v2) noexcept;
	static float DistanceSquared(const Vector2& v1, const Vector2& v2) noexcept;

	static void Min(const Vector2& v1, const Vector2& v2, Vector2& result) noexcept;
	static Vector2 Min(const Vector2& v1, const Vector2& v2) noexcept;

	static void Max(const Vector2& v1, const Vector2& v2, Vector2& result) noexcept;
	static Vector2 Max(const Vector2& v1, const Vector2& v2) noexcept;

	static void Lerp(const Vector2& v1, const Vector2& v2, float t, Vector2& result) noexcept;
	static Vector2 Lerp(const Vector2& v1, const Vector2& v2, float t) noexcept;

	static void SmoothStep(const Vector2& v1, const Vector2& v2, float t, Vector2& result) noexcept;
	static Vector2 SmoothStep(const Vector2& v1, const Vector2& v2, float t) noexcept;

	static void Barycentric(const Vector2& v1, const Vector2& v2, const Vector2& v3, float f, float g, Vector2& result) noexcept;
	static Vector2 Barycentric(const Vector2& v1, const Vector2& v2, const Vector2& v3, float f, float g) noexcept;

	static void CatmullRom(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector2& v4, float t, Vector2& result) noexcept;
	static Vector2 CatmullRom(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector2& v4, float t) noexcept;

	static void Hermite(const Vector2& v1, const Vector2& t1, const Vector2& v2, const Vector2& t2, float t, Vector2& result) noexcept;
	static Vector2 Hermite(const Vector2& v1, const Vector2& t1, const Vector2& v2, const Vector2& t2, float t) noexcept;

	static void Reflect(const Vector2& ivec, const Vector2& nvec, Vector2& result) noexcept;
	static Vector2 Reflect(const Vector2& ivec, const Vector2& nvec) noexcept;

	static void Refract(const Vector2& ivec, const Vector2& nvec, float refractionIndex, Vector2& result) noexcept;
	static Vector2 Refract(const Vector2& ivec, const Vector2& nvec, float refractionIndex) noexcept;

	// Constants
	static const Vector2 Zero;
	static const Vector2 One;
	static const Vector2 UnitX;
	static const Vector2 UnitY;
};

//------------------------------------------------------------------------------
// Comparision operators
//------------------------------------------------------------------------------

inline bool Vector2::operator == (const Vector2& V) const noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(this);
	XMVECTOR v2 = XMLoadFloat2(&V);
	return XMVector2Equal(v1, v2);
}

inline bool Vector2::operator != (const Vector2& V) const noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(this);
	XMVECTOR v2 = XMLoadFloat2(&V);
	return XMVector2NotEqual(v1, v2);
}

//------------------------------------------------------------------------------
// Assignment operators
//------------------------------------------------------------------------------

inline Vector2& Vector2::operator+= (const Vector2& V) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(this);
	XMVECTOR v2 = XMLoadFloat2(&V);
	XMVECTOR X = XMVectorAdd(v1, v2);
	XMStoreFloat2(this, X);
	return *this;
}

inline Vector2& Vector2::operator-= (const Vector2& V) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(this);
	XMVECTOR v2 = XMLoadFloat2(&V);
	XMVECTOR X = XMVectorSubtract(v1, v2);
	XMStoreFloat2(this, X);
	return *this;
}

inline Vector2& Vector2::operator*= (const Vector2& V) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(this);
	XMVECTOR v2 = XMLoadFloat2(&V);
	XMVECTOR X = XMVectorMultiply(v1, v2);
	XMStoreFloat2(this, X);
	return *this;
}

inline Vector2& Vector2::operator*= (float S) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(this);
	XMVECTOR X = XMVectorScale(v1, S);
	XMStoreFloat2(this, X);
	return *this;
}

inline Vector2& Vector2::operator/= (float S) noexcept
{
	using namespace DirectX;
	assert(S != 0.0f);
	XMVECTOR v1 = XMLoadFloat2(this);
	XMVECTOR X = XMVectorScale(v1, 1.f / S);
	XMStoreFloat2(this, X);
	return *this;
}

//------------------------------------------------------------------------------
// Binary operators
//------------------------------------------------------------------------------

inline Vector2 operator+ (const Vector2& V1, const Vector2& V2) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(&V1);
	XMVECTOR v2 = XMLoadFloat2(&V2);
	XMVECTOR X = XMVectorAdd(v1, v2);
	Vector2 R;
	XMStoreFloat2(&R, X);
	return R;
}

inline Vector2 operator- (const Vector2& V1, const Vector2& V2) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(&V1);
	XMVECTOR v2 = XMLoadFloat2(&V2);
	XMVECTOR X = XMVectorSubtract(v1, v2);
	Vector2 R;
	XMStoreFloat2(&R, X);
	return R;
}

inline Vector2 operator* (const Vector2& V1, const Vector2& V2) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(&V1);
	XMVECTOR v2 = XMLoadFloat2(&V2);
	XMVECTOR X = XMVectorMultiply(v1, v2);
	Vector2 R;
	XMStoreFloat2(&R, X);
	return R;
}

inline Vector2 operator* (const Vector2& V, float S) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(&V);
	XMVECTOR X = XMVectorScale(v1, S);
	Vector2 R;
	XMStoreFloat2(&R, X);
	return R;
}

inline Vector2 operator/ (const Vector2& V1, const Vector2& V2) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(&V1);
	XMVECTOR v2 = XMLoadFloat2(&V2);
	XMVECTOR X = XMVectorDivide(v1, v2);
	Vector2 R;
	XMStoreFloat2(&R, X);
	return R;
}

inline Vector2 operator/ (const Vector2& V, float S) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(&V);
	XMVECTOR X = XMVectorScale(v1, 1.f / S);
	Vector2 R;
	XMStoreFloat2(&R, X);
	return R;
}

inline Vector2 operator* (float S, const Vector2& V) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(&V);
	XMVECTOR X = XMVectorScale(v1, S);
	Vector2 R;
	XMStoreFloat2(&R, X);
	return R;
}

//------------------------------------------------------------------------------
// Vector operations
//------------------------------------------------------------------------------

inline bool Vector2::InBounds(const Vector2& Bounds) const noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(this);
	XMVECTOR v2 = XMLoadFloat2(&Bounds);
	return XMVector2InBounds(v1, v2);
}

inline float Vector2::Length() const noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(this);
	XMVECTOR X = XMVector2Length(v1);
	return XMVectorGetX(X);
}

inline float Vector2::LengthSquared() const noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(this);
	XMVECTOR X = XMVector2LengthSq(v1);
	return XMVectorGetX(X);
}

inline float Vector2::Dot(const Vector2& V) const noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(this);
	XMVECTOR v2 = XMLoadFloat2(&V);
	XMVECTOR X = XMVector2Dot(v1, v2);
	return XMVectorGetX(X);
}

inline void Vector2::Cross(const Vector2& V, Vector2& result) const noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(this);
	XMVECTOR v2 = XMLoadFloat2(&V);
	XMVECTOR R = XMVector2Cross(v1, v2);
	XMStoreFloat2(&result, R);
}

inline Vector2 Vector2::Cross(const Vector2& V) const noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(this);
	XMVECTOR v2 = XMLoadFloat2(&V);
	XMVECTOR R = XMVector2Cross(v1, v2);

	Vector2 result;
	XMStoreFloat2(&result, R);
	return result;
}

inline void Vector2::Normalize() noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(this);
	XMVECTOR X = XMVector2Normalize(v1);
	XMStoreFloat2(this, X);
}

inline void Vector2::Normalize(Vector2& result) const noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(this);
	XMVECTOR X = XMVector2Normalize(v1);
	XMStoreFloat2(&result, X);
}

inline void Vector2::Clamp(const Vector2& vmin, const Vector2& vmax) noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(this);
	XMVECTOR v2 = XMLoadFloat2(&vmin);
	XMVECTOR v3 = XMLoadFloat2(&vmax);
	XMVECTOR X = XMVectorClamp(v1, v2, v3);
	XMStoreFloat2(this, X);
}

inline void Vector2::Clamp(const Vector2& vmin, const Vector2& vmax, Vector2& result) const noexcept
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(this);
	XMVECTOR v2 = XMLoadFloat2(&vmin);
	XMVECTOR v3 = XMLoadFloat2(&vmax);
	XMVECTOR X = XMVectorClamp(v1, v2, v3);
	XMStoreFloat2(&result, X);
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

inline float Vector2::Distance(const Vector2& v1, const Vector2& v2) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat2(&v1);
	XMVECTOR x2 = XMLoadFloat2(&v2);
	XMVECTOR V = XMVectorSubtract(x2, x1);
	XMVECTOR X = XMVector2Length(V);
	return XMVectorGetX(X);
}

inline float Vector2::DistanceSquared(const Vector2& v1, const Vector2& v2) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat2(&v1);
	XMVECTOR x2 = XMLoadFloat2(&v2);
	XMVECTOR V = XMVectorSubtract(x2, x1);
	XMVECTOR X = XMVector2LengthSq(V);
	return XMVectorGetX(X);
}

inline void Vector2::Min(const Vector2& v1, const Vector2& v2, Vector2& result) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat2(&v1);
	XMVECTOR x2 = XMLoadFloat2(&v2);
	XMVECTOR X = XMVectorMin(x1, x2);
	XMStoreFloat2(&result, X);
}

inline Vector2 Vector2::Min(const Vector2& v1, const Vector2& v2) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat2(&v1);
	XMVECTOR x2 = XMLoadFloat2(&v2);
	XMVECTOR X = XMVectorMin(x1, x2);

	Vector2 result;
	XMStoreFloat2(&result, X);
	return result;
}

inline void Vector2::Max(const Vector2& v1, const Vector2& v2, Vector2& result) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat2(&v1);
	XMVECTOR x2 = XMLoadFloat2(&v2);
	XMVECTOR X = XMVectorMax(x1, x2);
	XMStoreFloat2(&result, X);
}

inline Vector2 Vector2::Max(const Vector2& v1, const Vector2& v2) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat2(&v1);
	XMVECTOR x2 = XMLoadFloat2(&v2);
	XMVECTOR X = XMVectorMax(x1, x2);

	Vector2 result;
	XMStoreFloat2(&result, X);
	return result;
}

inline void Vector2::Lerp(const Vector2& v1, const Vector2& v2, float t, Vector2& result) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat2(&v1);
	XMVECTOR x2 = XMLoadFloat2(&v2);
	XMVECTOR X = XMVectorLerp(x1, x2, t);
	XMStoreFloat2(&result, X);
}

inline Vector2 Vector2::Lerp(const Vector2& v1, const Vector2& v2, float t) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat2(&v1);
	XMVECTOR x2 = XMLoadFloat2(&v2);
	XMVECTOR X = XMVectorLerp(x1, x2, t);

	Vector2 result;
	XMStoreFloat2(&result, X);
	return result;
}

inline void Vector2::SmoothStep(const Vector2& v1, const Vector2& v2, float t, Vector2& result) noexcept
{
	using namespace DirectX;
	t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
	t = t * t * (3.f - 2.f * t);
	XMVECTOR x1 = XMLoadFloat2(&v1);
	XMVECTOR x2 = XMLoadFloat2(&v2);
	XMVECTOR X = XMVectorLerp(x1, x2, t);
	XMStoreFloat2(&result, X);
}

inline Vector2 Vector2::SmoothStep(const Vector2& v1, const Vector2& v2, float t) noexcept
{
	using namespace DirectX;
	t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
	t = t * t * (3.f - 2.f * t);
	XMVECTOR x1 = XMLoadFloat2(&v1);
	XMVECTOR x2 = XMLoadFloat2(&v2);
	XMVECTOR X = XMVectorLerp(x1, x2, t);

	Vector2 result;
	XMStoreFloat2(&result, X);
	return result;
}

inline void Vector2::Barycentric(const Vector2& v1, const Vector2& v2, const Vector2& v3, float f, float g, Vector2& result) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat2(&v1);
	XMVECTOR x2 = XMLoadFloat2(&v2);
	XMVECTOR x3 = XMLoadFloat2(&v3);
	XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);
	XMStoreFloat2(&result, X);
}

inline Vector2 Vector2::Barycentric(const Vector2& v1, const Vector2& v2, const Vector2& v3, float f, float g) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat2(&v1);
	XMVECTOR x2 = XMLoadFloat2(&v2);
	XMVECTOR x3 = XMLoadFloat2(&v3);
	XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);

	Vector2 result;
	XMStoreFloat2(&result, X);
	return result;
}

inline void Vector2::CatmullRom(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector2& v4, float t, Vector2& result) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat2(&v1);
	XMVECTOR x2 = XMLoadFloat2(&v2);
	XMVECTOR x3 = XMLoadFloat2(&v3);
	XMVECTOR x4 = XMLoadFloat2(&v4);
	XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);
	XMStoreFloat2(&result, X);
}

inline Vector2 Vector2::CatmullRom(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector2& v4, float t) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat2(&v1);
	XMVECTOR x2 = XMLoadFloat2(&v2);
	XMVECTOR x3 = XMLoadFloat2(&v3);
	XMVECTOR x4 = XMLoadFloat2(&v4);
	XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);

	Vector2 result;
	XMStoreFloat2(&result, X);
	return result;
}

inline void Vector2::Hermite(const Vector2& v1, const Vector2& t1, const Vector2& v2, const Vector2& t2, float t, Vector2& result) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat2(&v1);
	XMVECTOR x2 = XMLoadFloat2(&t1);
	XMVECTOR x3 = XMLoadFloat2(&v2);
	XMVECTOR x4 = XMLoadFloat2(&t2);
	XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);
	XMStoreFloat2(&result, X);
}

inline Vector2 Vector2::Hermite(const Vector2& v1, const Vector2& t1, const Vector2& v2, const Vector2& t2, float t) noexcept
{
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat2(&v1);
	XMVECTOR x2 = XMLoadFloat2(&t1);
	XMVECTOR x3 = XMLoadFloat2(&v2);
	XMVECTOR x4 = XMLoadFloat2(&t2);
	XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);

	Vector2 result;
	XMStoreFloat2(&result, X);
	return result;
}

inline void Vector2::Reflect(const Vector2& ivec, const Vector2& nvec, Vector2& result) noexcept
{
	using namespace DirectX;
	XMVECTOR i = XMLoadFloat2(&ivec);
	XMVECTOR n = XMLoadFloat2(&nvec);
	XMVECTOR X = XMVector2Reflect(i, n);
	XMStoreFloat2(&result, X);
}

inline Vector2 Vector2::Reflect(const Vector2& ivec, const Vector2& nvec) noexcept
{
	using namespace DirectX;
	XMVECTOR i = XMLoadFloat2(&ivec);
	XMVECTOR n = XMLoadFloat2(&nvec);
	XMVECTOR X = XMVector2Reflect(i, n);

	Vector2 result;
	XMStoreFloat2(&result, X);
	return result;
}

inline void Vector2::Refract(const Vector2& ivec, const Vector2& nvec, float refractionIndex, Vector2& result) noexcept
{
	using namespace DirectX;
	XMVECTOR i = XMLoadFloat2(&ivec);
	XMVECTOR n = XMLoadFloat2(&nvec);
	XMVECTOR X = XMVector2Refract(i, n, refractionIndex);
	XMStoreFloat2(&result, X);
}

inline Vector2 Vector2::Refract(const Vector2& ivec, const Vector2& nvec, float refractionIndex) noexcept
{
	using namespace DirectX;
	XMVECTOR i = XMLoadFloat2(&ivec);
	XMVECTOR n = XMLoadFloat2(&nvec);
	XMVECTOR X = XMVector2Refract(i, n, refractionIndex);

	Vector2 result;
	XMStoreFloat2(&result, X);
	return result;
}