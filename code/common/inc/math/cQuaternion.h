#ifndef _common_quaternion_h_
#define _common_quaternion_h_

//四元数
#include "../CommonDef.h"
#include "cVector3D.h"
#include "cMatrix4X4.h"

namespace cm
{

class cQuaternion
{
public:
	cQuaternion(f32 fx = 0.f, f32 fy = 0.f, f32 fz = 0.f, f32 fw = 1.f) : x(fx), y(fy), z(fz), w(fw) {}
	cQuaternion(const cQuaternion& other) { *this = (other); }
	cQuaternion(const cMatrix4X4& m) { FromMatrix(m); }
	cQuaternion& operator = (const cQuaternion& other) { w = other.w; x = other.x; y = other.y; z = other.z; return *this; }
	void Set(f32 fx, f32 fy, f32 fz, f32 fw) { x = fx; y = fy; z = fz; w = fw; }
	bool operator == (const cQuaternion& other) const { return w == other.w && x == other.x && y == other.y && z == other.z; }
	bool operator != (const cQuaternion& other) const { return !operator == (other); }
	cQuaternion operator * (const cQuaternion& other) const;
	cQuaternion operator * (f32 s) const { cQuaternion q; q.x = x * s; q.y = y * s; q.z = z * s; q.w = w * s; return q; }
	cQuaternion& operator *= (f32 s) {x *= s; y *= s; z *= s; w *= s; return *this;}
	cQuaternion operator + (const cQuaternion& q) const {return cQuaternion(x+q.x,y+q.y,z+q.z,w+q.w);}
	cQuaternion operator - (const cQuaternion& q) const {return cQuaternion(x-q.x,y-q.y,z-q.z,w-q.w);}
	cQuaternion& Normalize();
	cQuaternion& FromAngleAxis(f32 angle, const cVector3Df& rkAxis);
	cQuaternion& FromMatrix(const cMatrix4X4& m);
	f32 DotProduct(const cQuaternion& q2) const;
	cQuaternion& Slerp(cQuaternion q1, cQuaternion q2, f32 time);
	void GetMatrix_Transposed(cMatrix4X4& dest) const;
	cMatrix4X4 GetMatrix() const {cMatrix4X4 m; GetMatrix_Transposed(m); return m;}
	//设置为单位四元数
	void Identity() { w = 1.f; x = y = z = 0.f; }
	f32	w,x,y,z;
};

inline cQuaternion cQuaternion::operator * (const cQuaternion& other) const
{
	cQuaternion tmp;
	tmp.w = (other.w * w) - (other.x * x) - (other.y * y) - (other.z * z);
	tmp.x = (other.w * x) + (other.x * w) + (other.y * z) - (other.z * y);
	tmp.y = (other.w * y) + (other.y * w) + (other.z * x) - (other.x * z);
	tmp.z = (other.w * z) + (other.z * w) + (other.x * y) - (other.y * x);
	return tmp;
}

inline cQuaternion& cQuaternion::FromAngleAxis(f32 rfRadian, const cVector3Df& rkAxis)
{
	f32 fHalfRadian = 0.5f * rfRadian;
	f32 fSin = CrossMath::Sin(fHalfRadian);
	w = CrossMath::Cos(fHalfRadian);
	x = fSin * rkAxis.x;
	y = fSin * rkAxis.y;
	z = fSin * rkAxis.z;
	return *this;
}

inline cQuaternion& cQuaternion::FromMatrix(const cMatrix4X4& m)
{
	const f32 diag = m(0,0) + m(1,1) + m(2,2) + 1.f;
	if( diag > 0.0f )
	{
		const f32 scale = CrossMath::Sqrt(diag) * 2.0f; // get scale from diagonal
		// TODO: speed this up
		x = ( m(2,1) - m(1,2)) / scale;
		y = ( m(0,2) - m(2,0)) / scale;
		z = ( m(1,0) - m(0,1)) / scale;
		w = 0.25f * scale;
	}
	else
	{
		if ( m(0,0) > m(1,1) && m(0,0) > m(2,2))
		{
			// 1st element of diag is greatest value
			// find scale according to 1st element, and double it
			const f32 scale = CrossMath::Sqrt( 1.0f + m(0,0) - m(1,1) - m(2,2)) * 2.0f;

			// TODO: speed this up
			x = 0.25f * scale;
			y = (m(0,1) + m(1,0)) / scale;
			z = (m(2,0) + m(0,2)) / scale;
			w = (m(2,1) - m(1,2)) / scale;
		}
		else if ( m(1,1) > m(2,2))
		{
			// 2nd element of diag is greatest value
			// find scale according to 2nd element, and double it
			const f32 scale = CrossMath::Sqrt( 1.0f + m(1,1) - m(0,0) - m(2,2)) * 2.0f;

			// TODO: speed this up
			x = (m(0,1) + m(1,0) ) / scale;
			y = 0.25f * scale;
			z = (m(1,2) + m(2,1) ) / scale;
			w = (m(0,2) - m(2,0) ) / scale;
		}
		else
		{
			// 3rd element of diag is greatest value
			// find scale according to 3rd element, and double it
			const f32 scale = CrossMath::Sqrt( 1.0f + m(2,2) - m(0,0) - m(1,1)) * 2.0f;

			// TODO: speed this up
			x = (m(0,2) + m(2,0)) / scale;
			y = (m(1,2) + m(2,1)) / scale;
			z = 0.25f * scale;
			w = (m(1,0) - m(0,1)) / scale;
		}
	}
	return Normalize();
}

inline cQuaternion& cQuaternion::Normalize()
{
	const f32 n = x*x + y*y + z*z + w*w;
	if (n == 1)
		return *this;
	return (*this *= (1.0f / CrossMath::Sqrt(n)));
}

inline void cQuaternion::GetMatrix_Transposed(cMatrix4X4& dest) const
{
	dest[0] = 1.0f - 2.0f*y*y - 2.0f*z*z;
	dest[4] = 2.0f*x*y + 2.0f*z*w;
	dest[8] = 2.0f*x*z - 2.0f*y*w;
	dest[12] = 0.0f;

	dest[1] = 2.0f*x*y - 2.0f*z*w;
	dest[5] = 1.0f - 2.0f*x*x - 2.0f*z*z;
	dest[9] = 2.0f*z*y + 2.0f*x*w;
	dest[13] = 0.0f;

	dest[2] = 2.0f*x*z + 2.0f*y*w;
	dest[6] = 2.0f*z*y - 2.0f*x*w;
	dest[10] = 1.0f - 2.0f*x*x - 2.0f*y*y;
	dest[14] = 0.0f;

	dest[3] = 0.f;
	dest[7] = 0.f;
	dest[11] = 0.f;
	dest[15] = 1.f;
}

inline f32 cQuaternion::DotProduct(const cQuaternion& q2) const
{
	return (x * q2.x) + (y * q2.y) + (z * q2.z) + (w * q2.w);
}

inline cQuaternion& cQuaternion::Slerp(cQuaternion q1, cQuaternion q2, f32 time)
{
	f32 angle = q1.DotProduct(q2);
	if (angle < 0.0f) {
		q1 *= -1.0f;
		angle *= -1.0f;
	}
	f32 scale;
	f32 invscale;
	if ((angle + 1.0f) > 0.05f)
	{
		if ((1.0f - angle) >= 0.05f) // spherical interpolation
		{
			const f32 theta = CrossMath::Acos(angle);
			const f32 invsintheta = 1.f / CrossMath::Sin(theta);
			scale = CrossMath::Sin(theta * (1.0f - time)) * invsintheta;
			invscale = CrossMath::Sin(theta * time) * invsintheta;
		}
		else // linear interploation
		{
			scale = 1.0f - time;
			invscale = time;
		}
	}
	else
	{
		q2.Set(-q1.y, q1.x, -q1.w, q1.z);
		scale = CrossMath::Sin(c_fPI * (0.5f - time));
		invscale = CrossMath::Sin(c_fPI * time);
	}
	return *this = ( q1 * scale + q2 * invscale );
}

}

typedef cm::cQuaternion	quat;

#endif
