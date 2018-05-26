#ifndef _common_matrix4x4_h_
#define _common_matrix4x4_h_

#include "../CommonDef.h"
#include "MathBase.h"
#include "cVector3D.h"
#include "cPlane3D.h"
#include "cRect.h"
#include "cAabbox.h"
#include "../CrossMemory.h"

namespace cm
{

class cMatrix4X4
{
public:

	//默认为单位距阵
	cMatrix4X4() { Identity(); }
	cMatrix4X4(const cMatrix4X4& other) { *this = other; }
	cMatrix4X4& operator = (const cMatrix4X4& other);
	void Set0();
	bool operator == (const cMatrix4X4& other) const;
	bool operator != (const cMatrix4X4& other) const { return !operator == (other); }
	f32& operator [] (ui32 id) {return m[id];}
	const f32& operator [] (ui32 id) const {return m[id];}
	f32& operator () (const ui32 row, const ui32 col) {return m[row * 4 + col];}
	const f32& operator () (const ui32 row, const ui32 col) const {return m[row * 4 + col];}
	//设置为单位距阵
	void Identity();
	bool IsIdentity() const;
	//距阵相乘
	cMatrix4X4 operator * (const cMatrix4X4& in) const;
	cMatrix4X4& operator *= (const cMatrix4X4& in);
	void Multiply(cMatrix4X4& out, const cMatrix4X4& in) const;

	void Interpolate(cMatrix4X4& out, const cMatrix4X4& in, f32 time) const;

	void TransformVect(cVector3Df& out, const cVector3Df& in) const;
	cVector3Df TransformVect(const cVector3Df& in) const;

	//用于向量的变换,向量和位置无关系，所以不用计算第四维
	void TransformVecNormal(cVector3Df& out, const cVector3Df& in) const;

	//坐标
	void SetTranslation(const cVector3Df& in) { m[12] = in.x; m[13] = in.y; m[14] = in.z; }

	void SetInverseTranslation(const cVector3Df& in) { m[12] = -in.x; m[13] = -in.y; m[14] = -in.z; }

	cVector3Df GetTranslation() const { return cVector3Df(m[12], m[13], m[14]); }

	//旋转
	void SetRotationDegree(const cVector3Df& v) { SetRotationRadian( v * c_fDegToRad ); }
	cVector3Df GetRotationDegree() const;
	void SetRotationRadian(const cVector3Df& v);
	cVector3Df GetRotationRadian() const;

	void TransformBox(cAabboxf& out, const cAabboxf& in) const {TransformVect(out.m_min, in.m_min); TransformVect(out.m_max, in.m_max);}

	//test add by yfw

	//绕坐标轴轴旋转yaw	

	//绕x轴旋转
	//1,			0,				0,			0
	//0,			CrossMath::Cos(fAngle),	CrossMath::Sin(fAngle),0
	//0,			-CrossMath::Sin(fAngle),	CrossMath::Cos(fAngle),0
	//0,			0,				0,			1
	void MatrixRotationX(f32 fAngle);

	//绕Y轴旋转
	//CrossMath::Cos(fAngle),	0,	-CrossMath::Sin(fAngle), 0
	//0,			1,			0,	  0
	//CrossMath::Sin(fAngle),	0,	CrossMath::Cos(fAngle),  0
	//0,			0,			0,	  1		
	void MatrixRotationY(f32 fAngle);

	//绕Z轴旋转yaw
	//绕y旋转矩阵
	//CrossMath::Cos(fAngle), CrossMath::Sin(fAngle),	0, 0
	//-CrossMath::Sin(fAngle),CrossMath::Cos(fAngle),	0, 0
	//0,			0,			1, 0
	//0,			0,			0, 1
	void MatrixRotationZ(f32 fAngle);

	//The order of transformations is roll first, then pitch, then yaw.
	// Yaw around the y-axis; Pitch around the x-axis, in radians. Roll around the z-axis, in radians.
	//Out:一般为单位举行
	void MatrixRotationYawPitchRoll(f32 fYaw, f32 fPitch, f32 fRoll);

	void MatrixRotationAxis(const cVector3Df& vAxis, f32 fRad);

	//缩放
	void SetScale(const cVector3Df& v) { m[0] = v.x; m[5] = v.y; m[10] = v.z; }
	cVector3Df GetScale() const { return cVector3Df(m[0], m[5], m[10]); }

	//观察变换
	void LookAtLH(const cVector3Df& position, const cVector3Df& target, const cVector3Df& upVector);

	//正交投影
	void OrthoProject(f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar);

	void OrthoOffCenterProject(f32 l, f32 t, f32 r, f32 b, f32 zNear, f32 zFar);

	//透视投影
	void PerspectiveProject(f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar);
	void PerspectiveFovProject(f32 fovY, f32 aspect, f32 zNear, f32 zFar);

	//视区变换
	void SetViewport(const cRectf& rc, f32 minZ, f32 maxZ);

	//创建一个阴影矩阵
	void MatrixShadow(const cVector3Df &light,			//nLightType为0，表示光的方向，nLightType为1表示光的位置
					  int nLightType,					//光线的类型0,表示平行光，1表示点光源
					  f32 a,f32 b,f32 c,f32 d   //投影平面方程ax + by + cz + d = 0的各个参数。
					  );

	//求矩阵的逆矩阵
	/************************************************************************
	* 参考ogre的实现
	*A的逆矩阵 = 1/|A| * {d00,d10,d20,d30,
	*					   d01,d11,d21,d31,
	*                      d02,d12,d22,d32,
	*	                   d03,d13,d23,d33}		 	
	*其中dij为元素mij的余子式，即由行列式A划去第i行，第j列以后剩下的3行3列所
	*组成的行列式。
	*所以问题转化为求A的行列式以及A的16个余子式的行列式
	*先将四阶行列式化为3阶，再将化为2阶，最后可求得四阶行列式的值
	**************************************************************************/
	void Inverse(cMatrix4X4& out) const;

	//3D顶点变换为2D顶点，true 在视区，false 不在视区
	static bool Transform3DTo2D(cVector2Df& out,
						const cVector3Df& in,												//世界坐标
						const cVector3Df& eye, const cVector3Df& to, const cVector3Df& up,	//摄像机
						f32 fovY, f32 zNear, f32 zFar,								//投影
						f32 width, f32 height											//视区
						);

	//将3D世界坐标变换为2D屏幕坐标
	static void Transform3DTo2D(cVector2Df& out,           //屏幕坐标
		                  const cVector3Df& inWorld,       //世界坐标
						  const cRect& viewPort,           //视区
						  const cMatrix4X4& matrixProject, //投影矩阵
						  const cMatrix4X4& matrixView,    //视图变换矩阵
						  bool  bOrtho                     //是否为正交投影
						  );
	
	//将2D屏幕坐标变换为世界坐标
	//将2D屏幕坐标变换为3D空间中近平面上投影的坐标
	//pick算法发生改变
	//正交投影 Wp = 1
	static void OrthoTransform2DTo3D(cVector3Df &eye,		//eye相机位置(视区到世界坐标)
		                  cVector3Df &dir,					//dir相机方向(视区到世界坐标)
						  const cRect& viewport,			//视区
						  const cMatrix4X4 &matrixproject,	//投影矩阵
						  const cMatrix4X4 &matrixview		//视图变换矩阵
						  );

	//透视投影
	static void ProjectTransform2DTo3D(cVector3Df &eye,	//eye相机位置(视区到世界坐标)
		cVector3Df &dir,							//dir相机方向(视区到世界坐标)
		const cRect &viewport,						//视区
		const cMatrix4X4 &matrixproject,			//投影矩阵
		const cMatrix4X4 &matrixview				//视图变换矩阵
		);

	//根据平面获得指定的矩阵
	static cMatrix4X4 buildReflectionMatrix(const cPlane3Df& p);

	f32 m[16];

};

inline void cMatrix4X4::Set0()
{
	m[0] = m[1] = m[2] = m[3] = m[4] = m[5] = m[6] = m[7] = m[8] = 
		m[9] = m[10] = m[11] = m[12] = m[13] = m[14] = m[15] = 0.f;
}

inline void cMatrix4X4::Identity()
{
	m[0] = m[5] = m[10] = m[15] = 1.f;
	m[1] = m[2] = m[3] = m[4] = m[6] = m[7] = m[8] = m[9] = m[11] = m[12] = m[13] = m[14] = 0.f;
}

inline bool cMatrix4X4::IsIdentity() const
{
	if (m[0] != 1.f || m[5] != 1.f || m[10] != 1.f || m[15] != 1.f)
		return false;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (j != i && m[ i * 4 + j] != 0.f)
				return false;
	return true;
}

inline bool cMatrix4X4::operator == (const cMatrix4X4& other) const
{
	for (int i = 0; i < 16; i++)
		if (m[i] != other.m[i])
			return false;
	return true;
}

inline cMatrix4X4& cMatrix4X4::operator = (const cMatrix4X4& other)
{
	m[0] = other.m[0]; m[1] = other.m[1]; m[2] = other.m[2]; m[3] = other.m[3];
	m[4] = other.m[4]; m[5] = other.m[5]; m[6] = other.m[6]; m[7] = other.m[7];
	m[8] = other.m[8]; m[9] = other.m[9]; m[10] = other.m[10]; m[11] = other.m[11];
	m[12] = other.m[12]; m[13] = other.m[13]; m[14] = other.m[14]; m[15] = other.m[15];
	return *this;
}

inline cMatrix4X4 cMatrix4X4::operator * (const cMatrix4X4& in) const
{
	cMatrix4X4 out;
	f32* m3 = out.m;
	const f32* m1 = m;
	const f32* m2 = in.m;

	m3[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
	m3[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
	m3[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
	m3[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];

	m3[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
	m3[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
	m3[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
	m3[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];

	m3[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
	m3[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
	m3[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
	m3[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];

	m3[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
	m3[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
	m3[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
	m3[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];

	return out;
}

inline cMatrix4X4& cMatrix4X4::operator *= (const cMatrix4X4& in)
{
	f32* m3 = m;
	const f32* m1 = m;
	const f32* m2 = in.m;

	m3[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
	m3[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
	m3[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
	m3[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];

	m3[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
	m3[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
	m3[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
	m3[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];

	m3[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
	m3[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
	m3[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
	m3[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];

	m3[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
	m3[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
	m3[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
	m3[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];

	return *this;
}

inline void cMatrix4X4::Multiply(cMatrix4X4& out, const cMatrix4X4& in) const
{
	f32* m3 = out.m;
	const f32* m1 = m;
	const f32* m2 = in.m;

	m3[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
	m3[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
	m3[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
	m3[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];

	m3[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
	m3[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
	m3[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
	m3[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];

	m3[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
	m3[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
	m3[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
	m3[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];

	m3[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
	m3[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
	m3[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
	m3[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];

	return;
}

inline void cMatrix4X4::Interpolate(cMatrix4X4& out, const cMatrix4X4& in2, f32 time) const
{
	int num = sizeof(m) / sizeof(m[0]);
	for (int i = 0; i < num; i++)
		out.m[i] = m[i] + ( in2.m[i] - m[i] ) * time;
}

inline void cMatrix4X4::TransformVect(cVector3Df& out, const cVector3Df& in) const
{
	out.x = in.x * m[0] + in.y * m[4] + in.z * m[8] + m[12];
	out.y = in.x * m[1] + in.y * m[5] + in.z * m[9] + m[13];
	out.z = in.x * m[2] + in.y * m[6] + in.z * m[10] + m[14];
}

inline cVector3Df cMatrix4X4::TransformVect(const cVector3Df& in) const
{
	cVector3Df out;
	TransformVect(out, in);
	return out;
}

//用于向量的变换,向量和位置无关系，所以不用计算第四维
inline void cMatrix4X4::TransformVecNormal(cVector3Df& out, const cVector3Df& in) const
{
	out.x = in.x * m[0] + in.y * m[4] + in.z * m[8];
	out.y = in.x * m[1] + in.y * m[5] + in.z * m[9];
	out.z = in.x * m[2] + in.y * m[6] + in.z * m[10];
}

inline cVector3Df cMatrix4X4::GetRotationDegree() const
{
	cVector3Df v = GetRotationRadian();
	
	v *= c_fRadToDeg;

	if (v.x < 0.f)
		v.x += 360.f;
	if (v.y < 0.f)
		v.y += 360.f;
	if (v.z < 0.f)
		v.z += 360.f;

	return v;
}

inline void cMatrix4X4::SetRotationRadian(const cVector3Df& v)
{
	f32 cr = CrossMath::Cos( v.x );
	f32 sr = CrossMath::Sin( v.x );
	f32 cp = CrossMath::Cos( v.y );
	f32 sp = CrossMath::Sin( v.y );
	f32 cy = CrossMath::Cos( v.z );
	f32 sy = CrossMath::Sin( v.z );

	m[0] = cp * cy;
	m[1] = cp * sy;
	m[2] = -sp;

	f32 srsp = sr * sp;
	f32 crsp = cr * sp;

	m[4] = srsp * cy - cr * sy;
	m[5] = srsp * sy + cr * cy;
	m[6] = sr * cp;

	m[8] = crsp * cy + sr * sy;
	m[9] = crsp * sy - sr * cy;
	m[10] = cr * cp;
}

inline cVector3Df cMatrix4X4::GetRotationRadian() const
{
	f32 Y = -CrossMath::Asin(m[2]);
	f32 C = CrossMath::Cos(Y);

	f32 rotx, roty, X, Z;

	if (CrossMath::Abs(C)>0.0005f)
	{
		rotx = m[10] / C;
		roty = m[6] / C;
		X = CrossMath::Atan2( roty, rotx );
		rotx = m[0] / C;
		roty = m[1] / C;
		Z = CrossMath::Atan2( roty, rotx );
	}
	else
	{
		X = 0.f;
		rotx = m[5];
		roty = -m[4];
		Z = CrossMath::Atan2( roty, rotx );
	}

	return cVector3Df(X, Y, Z);
}

inline void cMatrix4X4::MatrixRotationX(f32 fAngle)
{
	Set0();
	m[0] = 1;
	m[5] = CrossMath::Cos(fAngle);
	m[6] = CrossMath::Sin(fAngle);
	m[9] = -CrossMath::Sin(fAngle);
	m[10] = CrossMath::Cos(fAngle);
	m[15] = 1.0f;
}

//绕Y轴旋转
//CrossMath::Cos(fAngle),	0,	-CrossMath::Sin(fAngle), 0
//0,			1,			0,	  0
//CrossMath::Sin(fAngle),	0,	CrossMath::Cos(fAngle),  0
//0,			0,			0,	  1		
inline void cMatrix4X4::MatrixRotationY(f32 fAngle)
{
	Set0();
	m[0] = CrossMath::Cos(fAngle);
	m[2] = -CrossMath::Sin(fAngle);
	m[5] = 1.0f;
	m[8] = CrossMath::Sin(fAngle);
	m[10] = CrossMath::Cos(fAngle);
	m[15] = 1.0f;
}

//绕Z轴旋转yaw
//绕y旋转矩阵
//CrossMath::Cos(fAngle), CrossMath::Sin(fAngle),	0, 0
//-CrossMath::Sin(fAngle),CrossMath::Cos(fAngle),	0, 0
//0,			0,			1, 0
//0,			0,			0, 1
inline void cMatrix4X4::MatrixRotationZ(f32 fAngle)
{
	Set0();
	m[0] = CrossMath::Cos(fAngle);
	m[1] = CrossMath::Sin(fAngle);
	m[4] = -CrossMath::Sin(fAngle);
	m[5] = CrossMath::Cos(fAngle);
	m[10] = 1.0f;
	m[15] = 1.0f;
}

//The order of transformations is roll first, then pitch, then yaw.
// Yaw around the y-axis; Pitch around the x-axis, in radians. Roll around the z-axis, in radians.
//Out:一般为单位举行
inline void cMatrix4X4::MatrixRotationYawPitchRoll(f32 fYaw, f32 fPitch, f32 fRoll)
{
	cMatrix4X4 Out;
	cMatrix4X4 mRotationM;
	cMatrix4X4 mOutTemp;

	mOutTemp = Out;
	Out.Set0();
	mRotationM.MatrixRotationZ(fRoll);
	mRotationM.Multiply(Out, mOutTemp);

	mOutTemp = Out;
	Out.Set0();
	mRotationM.MatrixRotationX(fPitch);
	//mOutTemp：左乘数； mRotationM:右乘数
	mRotationM.Multiply(Out, mOutTemp);

	mOutTemp = Out;
	Out.Set0();
	mRotationM.MatrixRotationY(fYaw);
	mRotationM.Multiply(Out, mOutTemp);

	*this = Out;

}

inline void cMatrix4X4::MatrixRotationAxis(const cVector3Df& vAxis, f32 fRad)
{

	f32 cost = CrossMath::Cos(fRad), sint = CrossMath::Sin(fRad), one_sub_cost = 1 - cost;
	cVector3Df v(vAxis);
	v.Normalize();

	m[0] = v.x * v.x * one_sub_cost + cost;
	m[1] = v.x * v.y * one_sub_cost + v.z * sint;
	m[2] = v.x * v.z * one_sub_cost - v.y * sint;
	m[3] = 0.f;

	m[4] = v.x * v.y * one_sub_cost - v.z * sint;
	m[5] = v.y * v.y * one_sub_cost + cost;
	m[6] = v.y * v.z * one_sub_cost + v.x * sint;
	m[7] = 0.f;

	m[8] = v.x * v.z * one_sub_cost + v.y * sint;
	m[9] = v.y * v.z * one_sub_cost - v.x * sint;
	m[10] = v.z * v.z * one_sub_cost + cost;
	m[11] = 0.f;

	m[12] = 0.f;
	m[13] = 0.f;
	m[14] = 0.f;
	m[15] = 1.f;

}

//观察变换
inline void cMatrix4X4::LookAtLH(const cVector3Df& position, const cVector3Df& target, const cVector3Df& upVector)
{
	cVector3Df zaxis = target - position;
	zaxis.Normalize();

	cVector3Df xaxis = upVector.CrossProduct(zaxis);
	xaxis.Normalize();

	cVector3Df yaxis = zaxis.CrossProduct(xaxis);

	m[0] = xaxis.x;
	m[1] = yaxis.x;
	m[2] = zaxis.x;
	m[3] = 0;

	m[4] = xaxis.y;
	m[5] = yaxis.y;
	m[6] = zaxis.y;
	m[7] = 0;

	m[8] = xaxis.z;
	m[9] = yaxis.z;
	m[10] = zaxis.z;
	m[11] = 0;

	m[12] = -xaxis.DotProduct(position);
	m[13] = -yaxis.DotProduct(position);
	m[14] = -zaxis.DotProduct(position);
	m[15] = 1;

}

//正交投影
inline void cMatrix4X4::OrthoProject(f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar)
{
	m[0] = 2.f / widthOfViewVolume;
	m[1] = 0;
	m[2] = 0;
	m[3] = 0;

	m[4] = 0;
	m[5] = 2.f / heightOfViewVolume;
	m[6] = 0;
	m[7] = 0;

	m[8] = 0;
	m[9] = 0;
	m[10] = 1 / ( zFar - zNear );
	m[11] = 0;

	m[12] = 0;
	m[13] = 0;
	m[14] = zNear / ( zNear - zFar );
	m[15] = 1;

}

inline void cMatrix4X4::OrthoOffCenterProject(f32 l, f32 t, f32 r, f32 b, f32 zNear, f32 zFar)
{
	m[0] = 2.f / ( r - l);
	m[1] = 0;
	m[2] = 0;
	m[3] = 0;

	m[4] = 0;
	m[5] = 2.f / ( b - t );
	m[6] = 0;
	m[7] = 0;

	m[8] = 0;
	m[9] = 0;
	m[10] = 1.f / ( zFar - zNear );
	m[11] = 0;

	m[12] = ( l + r ) / ( l - r );
	m[13] = ( t + b ) / ( t - b );
	m[14] = zNear / ( zNear - zFar );
	m[15] = 1.f;
}

//透视投影
inline void cMatrix4X4::PerspectiveProject(f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar)
{
	m[0] = 2.f * zNear / widthOfViewVolume;
	m[1] = 0;
	m[2] = 0;
	m[3] = 0;

	m[4] = 0;
	m[5] = 2.f * zNear / heightOfViewVolume;
	m[6] = 0;
	m[7] = 0;

	m[8] = 0;
	m[9] = 0;
	m[10] = zFar / ( zFar - zNear );
	m[11] = 1.f;

	m[12] = 0;
	m[13] = 0;
	m[14] = zNear * zFar / ( zNear - zFar );
	m[15] = 0;

}

inline void cMatrix4X4::PerspectiveFovProject(f32 fovY, f32 aspect, f32 zNear, f32 zFar)
{
	f32 h = 1.f / CrossMath::Tan( fovY / 2.f );
	f32 w = h / aspect;

	m[0] = w;
	m[1] = 0;
	m[2] = 0;
	m[3] = 0;

	m[4] = 0;
	m[5] = h;
	m[6] = 0;
	m[7] = 0;

	m[8] = 0;
	m[9] = 0;
	m[10] = zFar / ( zFar - zNear );
	m[11] = 1.f;

	m[12] = 0;
	m[13] = 0;
	m[14] = -zNear * zFar / ( zFar - zNear );
	m[15] = 0;

}

//视区变换
inline void cMatrix4X4::SetViewport(const cRectf& rc, f32 minZ, f32 maxZ)
{
	m[0] = rc.GetWidth() / 2;
	m[1] = 0;
	m[2] = 0;
	m[3] = 0;

	m[4] = 0;
	m[5] = - rc.GetHeight() / 2;
	m[6] = 0;
	m[7] = 0;

	m[8] = 0;
	m[9] = 0;
	m[10] = maxZ - minZ;
	m[11] = 1.f;

	m[12] = rc.left + rc.GetWidth() / 2;
	m[13] = rc.top + rc.GetHeight() / 2;
	m[14] = minZ;
	m[15] = 1;
}

//创建一个阴影矩阵
inline void cMatrix4X4::MatrixShadow(const cVector3Df &light,			//nLightType为0，表示光的方向，nLightType为1表示光的位置
					int nLightType,					//光线的类型0,表示平行光，1表示点光源
					f32 a,f32 b,f32 c,f32 d   //投影平面方程ax + by + cz + d = 0的各个参数。
					)
{
	f32  dot;
	f32  norm;

	norm = CrossMath::Sqrt(a * a + b * b + c * c);
	if ( norm ){
		a = a / norm;
		b = b / norm;
		c = c / norm;
		d = d / norm;
	}
	else{
		a = 0.0f;
		b = 0.0f;
		c = 0.0f;
		d = 0.0f;
	}

	dot = a * light.x + b * light.y + c * light.z + d * nLightType;
	m[0] = dot - a * light.x;
	m[1] = -a * light.y;
	m[2] = -a * light.z;
	m[3] = -a * nLightType;
	m[4] = -b * light.x;
	m[5] = dot - b * light.y;
	m[6] = -b * light.z;
	m[7] = -b * nLightType;
	m[8] = -c * light.x;
	m[9] = -c * light.y;
	m[10] = dot - c * light.z;
	m[11] = -c * nLightType;
	m[12] = -d * light.x;
	m[13] = -d * light.y;
	m[14] = -d * light.z;
	m[15] = dot - d * nLightType;
}

//求矩阵的逆矩阵
/************************************************************************
* 参考ogre的实现
*A的逆矩阵 = 1/|A| * {d00,d10,d20,d30,
*					   d01,d11,d21,d31,
*                      d02,d12,d22,d32,
*	                   d03,d13,d23,d33}		 	
*其中dij为元素mij的余子式，即由行列式A划去第i行，第j列以后剩下的3行3列所
*组成的行列式。
*所以问题转化为求A的行列式以及A的16个余子式的行列式
*先将四阶行列式化为3阶，再将化为2阶，最后可求得四阶行列式的值
**************************************************************************/
inline void cMatrix4X4::Inverse(cMatrix4X4& out) const
{
	f32* pMatrixOut = out.m;

	f32 m00 = m[0], m01 = m[1], m02 = m[2], m03 = m[3];
	f32 m10 = m[4], m11 = m[5], m12 = m[6], m13 = m[7];
	f32 m20 = m[8], m21 = m[9], m22 = m[10], m23 = m[11];
	f32 m30 = m[12], m31 = m[13], m32 = m[14], m33 = m[15];

	f32 v0 = m20 * m31 - m21 * m30;
	f32 v1 = m20 * m32 - m22 * m30;
	f32 v2 = m20 * m33 - m23 * m30;
	f32 v3 = m21 * m32 - m22 * m31;
	f32 v4 = m21 * m33 - m23 * m31;
	f32 v5 = m22 * m33 - m23 * m32;

	f32 t00 = + (v5 * m11 - v4 * m12 + v3 * m13);
	f32 t10 = - (v5 * m10 - v2 * m12 + v1 * m13);
	f32 t20 = + (v4 * m10 - v2 * m11 + v0 * m13);
	f32 t30 = - (v3 * m10 - v1 * m11 + v0 * m12);

	f32 invDet = 1 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

	f32 d00 = t00 * invDet;
	f32 d10 = t10 * invDet;
	f32 d20 = t20 * invDet;
	f32 d30 = t30 * invDet;

	f32 d01 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
	f32 d11 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
	f32 d21 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
	f32 d31 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

	v0 = m10 * m31 - m11 * m30;
	v1 = m10 * m32 - m12 * m30;
	v2 = m10 * m33 - m13 * m30;
	v3 = m11 * m32 - m12 * m31;
	v4 = m11 * m33 - m13 * m31;
	v5 = m12 * m33 - m13 * m32;

	f32 d02 = + (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
	f32 d12 = - (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
	f32 d22 = + (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
	f32 d32 = - (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

	v0 = m21 * m10 - m20 * m11;
	v1 = m22 * m10 - m20 * m12;
	v2 = m23 * m10 - m20 * m13;
	v3 = m22 * m11 - m21 * m12;
	v4 = m23 * m11 - m21 * m13;
	v5 = m23 * m12 - m22 * m13;

	f32 d03 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
	f32 d13 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
	f32 d23 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
	f32 d33 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

	pMatrixOut[0] = d00;
	pMatrixOut[1] = d01;
	pMatrixOut[2] = d02;
	pMatrixOut[3] = d03;

	pMatrixOut[4] = d10;
	pMatrixOut[5] = d11;
	pMatrixOut[6] = d12;
	pMatrixOut[7] = d13;

	pMatrixOut[8] = d20;
	pMatrixOut[9] = d21;
	pMatrixOut[10] = d22;
	pMatrixOut[11] = d23;

	pMatrixOut[12] = d30;
	pMatrixOut[13] = d31;
	pMatrixOut[14] = d32;
	pMatrixOut[15] = d33;
}

inline bool cMatrix4X4::Transform3DTo2D(cVector2Df& out,
					const cVector3Df& in,												//世界坐标
					const cVector3Df& eye, const cVector3Df& to, const cVector3Df& up,	//摄像机
					f32 fovY, f32 zNear, f32 zFar,								//投影
					f32 width, f32 height											//视区
					)
{

	cMatrix4X4 mView;						//观察矩阵
	mView.LookAtLH(eye, to, up);

	cVector3Df temp1, temp2;
	mView.TransformVect(temp1, in);

	temp2.y = temp1.y / ( temp1.z * CrossMath::Tan( fovY / 2 ) );
	temp2.x = temp1.x / ( temp1.z * CrossMath::Tan( fovY / 2 * ( width / height ) ) );

	out.x = ( 1.f + temp2.x ) * width / 2;
	out.y = ( 1.f - temp2.y ) * height / 2;

	return ! ( out.x > width || out.x < 0 ||
		out.y > height || out.y < 0 ||
		temp1.z < zNear || temp1.z > zFar );

}

inline void cMatrix4X4::Transform3DTo2D(cVector2Df& out,           //屏幕坐标
		                const cVector3Df& inWorld,       //世界坐标
						const cRect& viewPort,           //视区
						const cMatrix4X4& matrixProject, //投影矩阵
						const cMatrix4X4& matrixView,    //视图变换矩阵
						bool  bOrtho                     //是否为正交投影
						)
{
	/*cVector3Df posProject;
	cMatrix4X4 projViewMatrix =  matrixProject * matrixView;
	projViewMatrix.TransformVect(posProject,inWorld);*/
	cVector3Df posView, posProject;
	matrixView.TransformVect(posView, inWorld);
	matrixProject.TransformVect(posProject, posView);
	if (!bOrtho && posView.z > 1.0f)
		posProject /= posView.z;
	out.x = (posProject.x + 1.0f) * viewPort.GetWidth() * 0.5f + viewPort.GetLeft();
	out.y = (1.0f - posProject.y) * viewPort.GetHeight() * 0.5f + viewPort.GetTop();
}
	
//将2D屏幕坐标变换为世界坐标
//将2D屏幕坐标变换为3D空间中近平面上投影的坐标
//pick算法发生改变
//正交投影 Wp = 1
inline void cMatrix4X4::OrthoTransform2DTo3D(cVector3Df &eye,		//eye相机位置(视区到世界坐标)
		                cVector3Df &dir,					//dir相机方向(视区到世界坐标)
						const cRect& viewport,			//视区
						const cMatrix4X4 &matrixproject,	//投影矩阵
						const cMatrix4X4 &matrixview		//视图变换矩阵
						)
{
	cMatrix4X4 projectviewmatrix =  /*matrixproject * */matrixview;
	cMatrix4X4 inversematrix;

	//求得视图变换矩阵和投影变换矩阵乘积的逆矩阵
	projectviewmatrix.Inverse(inversematrix);

	//屏幕坐标到投影平面
	cVector3Df vecproj;
	//vecproj.x = (2.0f * (dir.x - viewport.GetLeft()) / viewport.GetWidth() - 1.0f) / matrixproject.m[0] + 1 / matrixproject.m[0];
	//vecproj.y = (1.0f - 2.0f * (dir.y -  viewport.GetTop()) / viewport.GetHeight()) / matrixproject.m[5] - 1 / matrixproject.m[5];
	//vecproj.z = (dir.z - 0) / (1 - 0) / matrixproject.m[10] - matrixproject.m[14] / matrixproject.m[10]/* * 1 / matrixproject.m[10]*/;
	vecproj.x = (2.0f * (dir.x - viewport.GetLeft()) / viewport.GetWidth() - 1.0f) / matrixproject.m[0];
	vecproj.y = (1.0f - 2.0f * (dir.y -  viewport.GetTop()) / viewport.GetHeight()) / matrixproject.m[5];
	vecproj.z = (dir.z - 0) / (1 - 0);

	cVector3Df vecT(vecproj);
	//将投影平面坐标变换为世界坐标
	inversematrix.TransformVect(eye, vecT);
	//相机方向到世界坐标
	vecproj = cVector3Df(0, 0, 1);

	projectviewmatrix = matrixview;
	projectviewmatrix.Inverse(inversematrix);
	inversematrix.TransformVecNormal(dir, vecproj);
	dir.Normalize();
}

//透视投影
inline void cMatrix4X4::ProjectTransform2DTo3D(cVector3Df &eye,	//eye相机位置(视区到世界坐标)
	cVector3Df &dir,							//dir相机方向(视区到世界坐标)
	const cRect &viewport,						//视区
	const cMatrix4X4 &matrixproject,			//投影矩阵
	const cMatrix4X4 &matrixview				//视图变换矩阵
	)
{
	cMatrix4X4 projectviewmatrix =  /*matrixproject * */matrixview;
	cMatrix4X4 inversematrix;

	//求得视图变换矩阵和投影变换矩阵乘积的逆矩阵
	projectviewmatrix.Inverse(inversematrix);

	//屏幕坐标到投影平面
	cVector3Df vecproj;
	vecproj.x = (2.0f * (dir.x - viewport.GetLeft()) / viewport.GetWidth() - 1.0f) / matrixproject.m[0];
	vecproj.y = (1.0f - 2.0f * (dir.y -  viewport.GetTop()) / viewport.GetHeight()) / matrixproject.m[5];
	vecproj.z = (dir.z - 0) / (1 - 0);

	//将投影平面坐标变换为世界坐标
	inversematrix.TransformVecNormal(dir, vecproj);
	//相机方向到世界坐标
	dir.Normalize();
	cVector3Df vecT;
	inversematrix.TransformVect(eye, vecT);
}
//-----------------------------------------------------------------------
//根据平面获得指定的矩阵
inline  cMatrix4X4 cMatrix4X4::buildReflectionMatrix(const cPlane3Df& p)
{
	cMatrix4X4 mTemp;
	//return Matrix4(
	//	-2 * p.m_vNormal.x * p.m_vNormal.x + 1,   -2 * p.m_vNormal.x * p.m_vNormal.y,       -2 * p.m_vNormal.x * p.m_vNormal.z,       -2 * p.m_vNormal.x * p.m_d, 
	//	-2 * p.m_vNormal.y * p.m_vNormal.x,       -2 * p.m_vNormal.y * p.m_vNormal.y + 1,   -2 * p.m_vNormal.y * p.m_vNormal.z,       -2 * p.m_vNormal.y * p.m_d, 
	//	-2 * p.m_vNormal.z * p.m_vNormal.x,       -2 * p.m_vNormal.z * p.m_vNormal.y,       -2 * p.m_vNormal.z * p.m_vNormal.z + 1,   -2 * p.m_vNormal.z * p.m_d, 
	//	0,                                  0,                                  0,                                  1);

	mTemp.m[0] = -2 * p.m_vNormal.x * p.m_vNormal.x + 1;
	mTemp.m[1] = -2 * p.m_vNormal.x * p.m_vNormal.y;
	mTemp.m[2] = -2 * p.m_vNormal.x * p.m_vNormal.z;
	mTemp.m[3] = -2 * p.m_vNormal.x * p.m_d;

	mTemp.m[4] = -2 * p.m_vNormal.y * p.m_vNormal.x;
	mTemp.m[5] = -2 * p.m_vNormal.y * p.m_vNormal.y + 1;
	mTemp.m[6] = -2 * p.m_vNormal.y * p.m_vNormal.z;
	mTemp.m[7] = -2 * p.m_vNormal.y * p.m_d;

	mTemp.m[8] = -2 * p.m_vNormal.z * p.m_vNormal.x;
	mTemp.m[9] = -2 * p.m_vNormal.z * p.m_vNormal.y;
	mTemp.m[10] = -2 * p.m_vNormal.z * p.m_vNormal.z + 1;
	mTemp.m[11] = -2 * p.m_vNormal.z * p.m_d;

	mTemp.m[12] = 0;
	mTemp.m[13] = 0;
	mTemp.m[14] = 0;
	mTemp.m[15] = 1;


	return mTemp;
}

}

typedef cm::cMatrix4X4	matrix;

#endif
