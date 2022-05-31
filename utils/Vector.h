#pragma once
#include <sstream>
#include "Color.h"
#define DEG2RAD(x)  ((float)(x) * (float)(3.14f / 180.f))
class Vector
{
public:
	Vector(void)
	{
		Invalidate();
	}
	Vector(float X, float Y, float Z)
	{
		x = X;
		y = Y;
		z = Z;
	}
	Vector(float X, float Y)
	{
		x = X;
		y = Y;
		z = 0;
	}

	Vector(const float* clr)
	{
		x = clr[0];
		y = clr[1];
		z = clr[2];
	}

	void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f)
	{
		x = ix; y = iy; z = iz;
	}
	bool IsValid() const
	{
		return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
	}
	void Invalidate()
	{
		x = y = z = std::numeric_limits<float>::infinity();
	}
	Vector(Color col)
	{
		this->x = col.r;
		this->y = col.g;
		this->z = col.b;
	}
	Vector operator-(int i)
	{
		this->x = x - i;
		this->y = y - i;
		this->z = z - i;
		return *this;
	}
	Vector operator-(float i)
	{
		this->x = x - i;
		this->y = y - i;
		this->z = z - i;
		return *this;
	}
	Vector operator+(int i)
	{
		this->x = x + i;
		this->y = y + i;
		this->z = z + i;
		return *this;
	}
	Vector operator+(float i)
	{
		this->x = x + i;
		this->y = y + i;
		this->z = z + i;
		return *this;
	}
	float& operator[](int i)
	{
		return ((float*)this)[i];
	}
	float operator[](int i) const
	{
		return ((float*)this)[i];
	}

	void Zero()
	{
		x = y = z = 0.0f;
	}

	bool operator==(const Vector& src) const
	{
		return (src.x == x) && (src.y == y) && (src.z == z);
	}
	bool operator!=(const Vector& src) const
	{
		return (src.x != x) || (src.y != y) || (src.z != z);
	}

	Vector& operator+=(const Vector& v)
	{
		x += v.x; y += v.y; z += v.z;
		return *this;
	}
	Vector& operator-=(const Vector& v)
	{
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}
	Vector& operator*=(float fl)
	{
		x *= fl;
		y *= fl;
		z *= fl;
		return *this;
	}
	Vector& operator*=(const Vector& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}
	Vector& operator/=(const Vector& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}
	Vector& operator+=(float fl)
	{
		x += fl;
		y += fl;
		z += fl;
		return *this;
	}
	Vector& operator/=(float fl)
	{
		x /= fl;
		y /= fl;
		z /= fl;
		return *this;
	}
	Vector& operator-=(float fl)
	{
		x -= fl;
		y -= fl;
		z -= fl;
		return *this;
	}

	void NormalizeInPlace()
	{
		*this = Normalized();
	}
	Vector Normalized() const
	{
		Vector res = *this;
		float l = res.Length();
		if (l != 0.0f) {
			res /= l;
		}
		else {
			res.x = res.y = res.z = 0.0f;
		}
		return res;
	}
	Vector VectorAngles()
	{
		float tmp, yaw, pitch;

		if (this->y == 0 && this->x == 0)
		{
			yaw = 0;
			if (this->z > 0)
				pitch = 270;
			else
				pitch = 90;
		}
		else
		{
			yaw = (atan2(this->y, this->x) * 180.f / 3.14f);
			if (yaw < 0)
				yaw += 360;

			tmp = sqrt(this->x * this->x + this->y * this->y);
			pitch = (atan2(-this->z, tmp) * 180.f / 3.14f);
			if (pitch < 0)
				pitch += 360;
		}

		this->x = pitch;
		this->y = yaw;
		this->z = 0;
		return *this;
	}
	Vector angle_vector()
	{
		auto sy = sin(y / 180.f * static_cast<float>(3.14f));
		auto cy = cos(y / 180.f * static_cast<float>(3.14f));

		auto sp = sin(x / 180.f * static_cast<float>(3.14f));
		auto cp = cos(x / 180.f * static_cast<float>(3.14f));

		 Vector buffer = Vector(cp * cy, cp * sy, -sp);
		 *this = buffer;
		 return *this;
	}
	float DistTo(const Vector& vOther) const
	{
		Vector delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;

		return delta.Length();
	}
	float DistToSqr(const Vector& vOther) const
	{
		Vector delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;

		return delta.LengthSqr();
	}
	float Dot(const Vector& vOther) const
	{
		return (x * vOther.x + y * vOther.y + z * vOther.z);
	}
	float Length() const
	{
		return sqrt(x * x + y * y + z * z);
	}
	float LengthSqr(void) const
	{
		return (x * x + y * y + z * z);
	}
	float Length2D() const
	{
		return sqrt(x * x + y * y);
	}

	Vector& operator=(const Vector& vOther)
	{
		x = vOther.x; y = vOther.y; z = vOther.z;
		return *this;
	}

	Vector operator-(void) const
	{
		return Vector(-x, -y, -z);
	}
	Vector operator+(const Vector& v) const
	{
		return Vector(x + v.x, y + v.y, z + v.z);
	}
	Vector operator-(const Vector& v) const
	{
		return Vector(x - v.x, y - v.y, z - v.z);
	}
	Vector operator*(float fl) const
	{
		return Vector(x * fl, y * fl, z * fl);
	}
	Vector operator*(const Vector& v) const
	{
		return Vector(x * v.x, y * v.y, z * v.z);
	}
	Vector operator/(float fl) const
	{
		return Vector(x / fl, y / fl, z / fl);
	}
	Vector operator/(const Vector& v) const
	{
		return Vector(x / v.x, y / v.y, z / v.z);
	}
	static inline void SinCos(const float& r, float& s, float& c)
	{
		s = sin(r);
		c = cos(r);
	}
	inline void Rotate2D( float& f)
	{
		float _x, _y;

		float s, c;

		SinCos(DEG2RAD(f), s, c);

		_x = x;
		_y = y;

		x = (_x * c) - (_y * s);
		y = (_x * s) + (_y * c);
	}
	Vector normalize() {
		while (this->y <= -180.f)this->y += 360.f;
		while (this->y >= 180.f)this->y -= 360.f;
		if (this->x > 89.f)this->x = 89.f;
		if (this->x < -89.f)this->x = -89.f;

		this->z = 0;
		return *this;
	}
	Vector clamp() {
		if (this->y >= 180.f)this->y = 179.9f;
		if (this->y <= -180.f)this->y = -179.9f;
		if (this->x < -89.f)this->x = -89.0f;
		if (this->x > 89.f)this->x = 89.0f;
		this->z = 0;
		return *this;
	}
	float x, y, z;
};
inline Vector operator*(float lhs, const Vector& rhs)
{
	Vector rez;
	rez.x = rhs.x * lhs;
	rez.y = rhs.y * lhs;
	return rez;
}
inline Vector operator/(float lhs, const Vector& rhs)
{
	Vector rez;
	rez.x = rhs.x / lhs;
	rez.y = rhs.y / lhs;
	return rez;
}
class __declspec(align(16)) VectorAligned : public Vector
{
public:
	inline VectorAligned(void) {};
	inline VectorAligned(float X, float Y, float Z)
	{
		Init(X, Y, Z);
	}

public:
	explicit VectorAligned(const Vector& vOther)
	{
		Init(vOther.x, vOther.y, vOther.z);
	}

	VectorAligned& operator=(const Vector& vOther)
	{
		Init(vOther.x, vOther.y, vOther.z);
		return *this;
	}

	VectorAligned& operator=(const VectorAligned& vOther)
	{
		Init(vOther.x, vOther.y, vOther.z);
		return *this;
	}

	float w;
};
class vec2_t {
public:
	float x, y;

	vec2_t() {
		x = 0; y = 0;
	};
	vec2_t(float X, float Y) {
		x = X; y = Y;
	};
	vec2_t(Vector vec) {
		x = vec.x; y = vec.y;
	}

	inline vec2_t operator*(const float n) const {
		return vec2_t(x * n, y * n);
	}
	inline vec2_t operator/(const float n) const {
		return vec2_t(x / n, y / n);
	}
	inline vec2_t operator+(const vec2_t& v) const {
		return vec2_t(x + v.x, y + v.y);
	}
	inline vec2_t operator-(const vec2_t& v) const {
		return vec2_t(x - v.x, y - v.y);
	}
	inline void operator+=(const vec2_t& v) {
		x += v.x;
		y += v.y;
	}
	inline void operator-=(const vec2_t& v) {
		x -= v.x;
		y -= v.y;
	}

	bool operator==(const vec2_t& v) const {
		return (v.x == x && v.y == y);
	}
	bool operator!=(const vec2_t& v) const {
		return (v.x != x || v.y != y);
	}

	inline float length() {
		return std::sqrt((x * x) + (y * y));
	}
};
class QAngle
{
public:
	QAngle(Vector f)
	{
		this->pitch = f.x;
		this->yaw = f.y;
		this->roll = f.z;
	}
	QAngle(void)
	{
		Init();
	}
	QAngle(float X, float Y, float Z)
	{
		Init(X, Y, Z);
	}
	QAngle(const float* clr)
	{
		Init(clr[0], clr[1], clr[2]);
	}

	void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f)
	{
		pitch = ix;
		yaw = iy;
		roll = iz;
	}
	QAngle operator=(const Vector f)
	{
		return QAngle(f.x, f.y, f.z);
	}
	float operator[](int i) const
	{
		return ((float*)this)[i];
	}
	float& operator[](int i)
	{
		return ((float*)this)[i];
	}

	QAngle& operator+=(const QAngle& v)
	{
		pitch += v.pitch; yaw += v.yaw; roll += v.roll;
		return *this;
	}
	QAngle& operator-=(const QAngle& v)
	{
		pitch -= v.pitch; yaw -= v.yaw; roll -= v.roll;
		return *this;
	}
	QAngle& operator*=(float fl)
	{
		pitch *= fl;
		yaw *= fl;
		roll *= fl;
		return *this;
	}
	QAngle& operator*=(const QAngle& v)
	{
		pitch *= v.pitch;
		yaw *= v.yaw;
		roll *= v.roll;
		return *this;
	}
	QAngle& operator/=(const QAngle& v)
	{
		pitch /= v.pitch;
		yaw /= v.yaw;
		roll /= v.roll;
		return *this;
	}
	QAngle& operator+=(float fl)
	{
		pitch += fl;
		yaw += fl;
		roll += fl;
		return *this;
	}
	QAngle& operator/=(float fl)
	{
		pitch /= fl;
		yaw /= fl;
		roll /= fl;
		return *this;
	}
	QAngle& operator-=(float fl)
	{
		pitch -= fl;
		yaw -= fl;
		roll -= fl;
		return *this;
	}
	QAngle operator+=(const Vector v)
	{
	this->pitch = this->pitch + v.x, this->yaw = this->yaw + v.y, this->roll = this->roll + v.z;
	return *this;
	}
	QAngle operator-=(const Vector v)
	{
		this->pitch = this->pitch - v.x, this->yaw = this->yaw - v.y, this->roll = this->roll - v.z;
		return *this;
	}
	QAngle& operator=(const QAngle& vOther)
	{
		pitch = vOther.pitch; yaw = vOther.yaw; roll = vOther.roll;
		return *this;
	}

	QAngle operator-(void) const
	{
		return QAngle(-pitch, -yaw, -roll);
	}
	QAngle operator+(const QAngle& v) const
	{
		return QAngle(pitch + v.pitch, yaw + v.yaw, roll + v.roll);
	}
	QAngle operator-(const QAngle& v) const
	{
		return QAngle(pitch - v.pitch, yaw - v.yaw, roll - v.roll);
	}
	QAngle operator*(float fl) const
	{
		return QAngle(pitch * fl, yaw * fl, roll * fl);
	}
	QAngle operator*(const QAngle& v) const
	{
		return QAngle(pitch * v.pitch, yaw * v.yaw, roll * v.roll);
	}
	QAngle operator/(float fl) const
	{
		return QAngle(pitch / fl, yaw / fl, roll / fl);
	}
	QAngle operator/(const QAngle& v) const
	{
		return QAngle(pitch / v.pitch, yaw / v.yaw, roll / v.roll);
	}
	QAngle operator+(Vector f)
	{
		return QAngle(this->pitch + f.x, this->yaw + f.y, this->roll + f.z);
	}
	
	float Length() const
	{
		return sqrt(pitch * pitch + yaw * yaw + roll * roll);
	}
	float LengthSqr(void) const
	{
		return (pitch * pitch + yaw * yaw + roll * roll);
	}
	bool IsZero(float tolerance = 0.01f) const
	{
		return (pitch > -tolerance && pitch < tolerance&&
			yaw > -tolerance && yaw < tolerance&&
			roll > -tolerance && roll < tolerance);
	}

	float Normalize() const
	{
		QAngle res = *this;
		float l = res.Length();
		if (l != 0.0f)
		{
			res /= l;
		}
		else
		{
			res[0] = res[1] = res[2] = 0.0f;
		}
		return l;
	}

	float pitch;
	float yaw;
	float roll;
	void ClampAngles(QAngle& angles)
	{
		if (angles.pitch > 89.0f) angles.pitch = 89.0f;
		else if (angles.pitch < -89.0f) angles.pitch = -89.0f;

		if (angles.yaw > 180.0f) angles.yaw = 180.0f;
		else if (angles.yaw < -180.0f) angles.yaw = -180.0f;

		angles.roll = 0;
	}
	void NormalizeAng(QAngle& angles)
	{
		while (angles.yaw < -189.f)angles.yaw += 180.f;
		while (angles.yaw > 189.f)angles.yaw -= 180.f;
		if (angles.pitch > 89.f)angles.pitch = 89.f;
		if (angles.pitch < -89.f)angles.pitch = -89.f;

		angles.roll = 0;
	}
};
inline QAngle operator*(float lhs, const QAngle& rhs)
{
	return rhs * lhs;
}
inline QAngle operator/(float lhs, const QAngle& rhs)
{
	return rhs / lhs;
}
