#ifndef _common_viewfrustum_h_
#define _common_viewfrustum_h_

#include "../CommonDef.h"
#include "cVector3D.h"
#include "cLine3D.h"
#include "cPlane3D.h"
#include "cAabbox.h"
#include "cMatrix4X4.h"
#include "cQuaternion.h"

namespace cm
{
//视锥
enum eVFPlanes
{
	VFP_Far,
	VFP_Near,
	VFP_Left,
	VFP_Right,
	VFP_Bottom,
	VFP_Top,
	VFP_Count
};

class cViewFrustum
{
public:
	cViewFrustum() {}
	cViewFrustum(const cViewFrustum& vf) {*this = vf;}
	cViewFrustum& operator = (const cViewFrustum& vf);
	//cViewFrustum(const cMatrix4X4& mat) {SetFrom(mat);}
	//void Transform(const cMatrix4X4& mat);
	
	cVector3Df GetFarLeftUp() const;
	cVector3Df GetFarLeftDown() const;
	cVector3Df GetFarRightUp() const;
	cVector3Df GetFarRightDown() const;

	cVector3Df GetNearLeftUp() const;
	cVector3Df GetNearLeftDown() const;
	cVector3Df GetNearRightUp() const;
	cVector3Df GetNearRightDown() const;

	const cAabboxf& GetBoundingBox() const {return m_boundingBox;}
	void RecalculateBoundingBox();
	void SetPosition(const cVector3Df& p) {m_cameraPosition = p; Recalculate();}
	const cVector3Df& GetPosition() const {return m_cameraPosition;}
	const cMatrix4X4& GetMView() const {return m_mView;}
	const cMatrix4X4& GetMProject() const {return m_mProject;}
	void SetMView(const cMatrix4X4& m) {m_mView = m; Recalculate();}
	void SetMProject(const cMatrix4X4& m) {m_mProject = m; Recalculate();}
	//
	bool ClipLine(cLine3Df& lineOut, const cLine3Df& lineIn) const;
	//点是否在视锥里
	bool IsPointInside(const cVector3Df& p) const;
	//盒子是否与视锥相交
	bool IntersectsWithBox(const cAabboxf& box) const;
	//从屏幕点到空间射线
	bool GetRayFromScreenCoordinates(const cPoint& pt, cRay3Df& ray, i32 w, i32 h) const;
	//空间中的点转到屏幕上
	bool GetScreenCoordinatesFromVector3D(cPoint& pt, const cVector3Df& v, i32 w, i32 h) const;
	//获取8个顶点
	void GetEdges(cVector3Df* edges) const;
	void GetEdgesIndex(ui16* edgesindex) const;
	//获取12条线
	void GetLines(cLine3Df* lines) const;

	const cPlane3Df& GetPlane(eVFPlanes e) const {return m_planes[e];}

	void BuildView(const cVector3Df& eye, const cVector3Df& lookat, const cVector3Df& up) {cMatrix4X4 m; m.LookAtLH(eye, lookat, up); SetMView(m);}
	void BuildOrthoProject(f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar) {cMatrix4X4 m; m.OrthoProject(widthOfViewVolume, heightOfViewVolume, zNear, zFar); SetMProject(m);}
	void BuildPerspectiveProject(f32 fovY, f32 aspect, f32 zNear, f32 zFar) {cMatrix4X4 m; m.PerspectiveFovProject(fovY, aspect, zNear, zFar); SetMProject(m);}

	bool BuildOrthoViewFrustum(const cAabboxf& box, const cVector3Df& vDir);
	bool BuildPerspectiveViewFrustum(const cAabboxf& box, const cVector3Df& vDir);

private:
	void Recalculate() {SetFrom(m_mProject * m_mView);}
	void SetFrom(const cMatrix4X4& mat);
	cVector3Df		m_cameraPosition;
	cPlane3Df		m_planes[VFP_Count];
	cAabboxf		m_boundingBox;
	cMatrix4X4		m_mView;
	cMatrix4X4		m_mProject;
};

inline cViewFrustum& cViewFrustum::operator = (const cViewFrustum& vf)
{
	m_cameraPosition = vf.m_cameraPosition;
	for (i32 i = 0; i < VFP_Count; i++)
		m_planes[i] = vf.m_planes[i];
	m_boundingBox = vf.m_boundingBox;
	m_mView = vf.m_mView;
	m_mProject = vf.m_mProject;
    return *this;
}

inline void cViewFrustum::SetFrom(const cMatrix4X4& mat)
{
	// left clipping plane
	m_planes[VFP_Left].m_vNormal.x =	mat[3 ] + mat[0];
	m_planes[VFP_Left].m_vNormal.y =	mat[7 ] + mat[4];
	m_planes[VFP_Left].m_vNormal.z =	mat[11] + mat[8];
	m_planes[VFP_Left].m_d =			mat[15] + mat[12];

	// right clipping plane
	m_planes[VFP_Right].m_vNormal.x =	mat[3 ] - mat[0];
	m_planes[VFP_Right].m_vNormal.y =	mat[7 ] - mat[4];
	m_planes[VFP_Right].m_vNormal.z =	mat[11] - mat[8];
	m_planes[VFP_Right].m_d =			mat[15] - mat[12];

	// top clipping plane
	m_planes[VFP_Top].m_vNormal.x =		mat[3 ] - mat[1];
	m_planes[VFP_Top].m_vNormal.y =		mat[7 ] - mat[5];
	m_planes[VFP_Top].m_vNormal.z =		mat[11] - mat[9];
	m_planes[VFP_Top].m_d =				mat[15] - mat[13];

	// bottom clipping plane
	m_planes[VFP_Bottom].m_vNormal.x =	mat[3 ] + mat[1];
	m_planes[VFP_Bottom].m_vNormal.y =	mat[7 ] + mat[5];
	m_planes[VFP_Bottom].m_vNormal.z =	mat[11] + mat[9];
	m_planes[VFP_Bottom].m_d =			mat[15] + mat[13];

	// far clipping plane
	m_planes[VFP_Far].m_vNormal.x =		mat[3 ] - mat[2];
	m_planes[VFP_Far].m_vNormal.y =		mat[7 ] - mat[6];
	m_planes[VFP_Far].m_vNormal.z =		mat[11] - mat[10];
	m_planes[VFP_Far].m_d =				mat[15] - mat[14];

	// near clipping plane
	m_planes[VFP_Near].m_vNormal.x =	mat[2];
	m_planes[VFP_Near].m_vNormal.y =	mat[6];
	m_planes[VFP_Near].m_vNormal.z =	mat[10];
	m_planes[VFP_Near].m_d =			mat[14];

	// normalize normals
	i32 i;
	for ( i = 0; i != VFP_Count; ++i)
	{
		const f32 len = - ( 1.f / m_planes[i].m_vNormal.GetLength() );
		m_planes[i].m_vNormal *= len;
		m_planes[i].m_d *= len;
	}

	// make bounding box
	RecalculateBoundingBox();
}

inline void cViewFrustum::RecalculateBoundingBox()
{
	//m_boundingBox.Reset( m_cameraPosition );

	m_boundingBox.Reset(GetNearLeftUp());
	m_boundingBox.AddInternalPoint(GetNearRightUp());
	m_boundingBox.AddInternalPoint(GetNearLeftDown());
	m_boundingBox.AddInternalPoint(GetNearRightDown());

	m_boundingBox.AddInternalPoint(GetFarLeftUp());
	m_boundingBox.AddInternalPoint(GetFarRightUp());
	m_boundingBox.AddInternalPoint(GetFarLeftDown());
	m_boundingBox.AddInternalPoint(GetFarRightDown());
}

//inline void cViewFrustum::Transform(const cMatrix4X4& mat)
//{
//	for (i32 i = 0; i < VFP_Count; ++i)
//		mat.TransformPlane(m_planes[i]);
//
//	cVector3Df v = m_cameraPosition;
//	mat.TransformVect(v, m_cameraPosition);
//	m_cameraPosition = v;
//	RecalculateBoundingBox();
//}

inline cVector3Df cViewFrustum::GetFarLeftUp() const
{
	cVector3Df p;
	m_planes[VFP_Far].GetIntersectionWithPlanes(m_planes[VFP_Top], m_planes[VFP_Left], p);
	return p;
}

inline cVector3Df cViewFrustum::GetFarLeftDown() const
{
	cVector3Df p;
	m_planes[VFP_Far].GetIntersectionWithPlanes(m_planes[VFP_Bottom], m_planes[VFP_Left], p);
	return p;
}

inline cVector3Df cViewFrustum::GetFarRightUp() const
{
	cVector3Df p;
	m_planes[VFP_Far].GetIntersectionWithPlanes(m_planes[VFP_Top], m_planes[VFP_Right], p);
	return p;
}

inline cVector3Df cViewFrustum::GetFarRightDown() const
{
	cVector3Df p;
	m_planes[VFP_Far].GetIntersectionWithPlanes(m_planes[VFP_Bottom], m_planes[VFP_Right], p);
	return p;
}

inline cVector3Df cViewFrustum::GetNearLeftUp() const
{
	cVector3Df p;
	m_planes[VFP_Near].GetIntersectionWithPlanes(m_planes[VFP_Top], m_planes[VFP_Left], p);
	return p;
}

inline cVector3Df cViewFrustum::GetNearLeftDown() const
{
	cVector3Df p;
	m_planes[VFP_Near].GetIntersectionWithPlanes(m_planes[VFP_Bottom], m_planes[VFP_Left], p);
	return p;
}

inline cVector3Df cViewFrustum::GetNearRightUp() const
{
	cVector3Df p;
	m_planes[VFP_Near].GetIntersectionWithPlanes(m_planes[VFP_Top], m_planes[VFP_Right], p);
	return p;
}

inline cVector3Df cViewFrustum::GetNearRightDown() const
{
	cVector3Df p;
	m_planes[VFP_Near].GetIntersectionWithPlanes(m_planes[VFP_Bottom], m_planes[VFP_Right], p);
	return p;
}

inline bool cViewFrustum::ClipLine(cLine3Df& lineOut, const cLine3Df& lineIn) const
{
	bool wasClipped = false;
	for (i32 i = 0; i < VFP_Count; ++i)
	{
		if (m_planes[i].ClassifyPointRelation(lineIn.from) == IR_Front)
		{
			lineOut.from = lineIn.from.GetInterpolated(lineIn.to, m_planes[i].GetKnownIntersectionWithLine(lineIn.from, lineIn.to));
			wasClipped = true;
		}
		if (m_planes[i].ClassifyPointRelation(lineIn.to) == IR_Front)
		{
			lineOut.to = lineIn.from.GetInterpolated(lineIn.to, m_planes[i].GetKnownIntersectionWithLine(lineIn.from, lineIn.to));
			wasClipped = true;
		}
	}
	return wasClipped;
}

//点是否在视锥里
inline bool cViewFrustum::IsPointInside(const cVector3Df& p) const
{
	for (i32 i = 0; i < VFP_Count; i++)
		if (IR_Front == m_planes[i].ClassifyPointRelation(p))
			return false;
	return true;
}

//盒子是否与视锥相交
inline bool cViewFrustum::IntersectsWithBox(const cAabboxf& box) const
{
	cVector3Df v[8];
	box.GetEdges(v);
	for (i32 i = 0; i < VFP_Count; i++) {
		i32 k = 0;
		for (i32 j = 0; j < 8; j++) {
			if (IR_Front == m_planes[i].ClassifyPointRelation(v[j]))
				k++;
			else
				break;
		}
		if (k == 8)
			return false;
	}
	return true;
}

inline bool cViewFrustum::GetRayFromScreenCoordinates(const cPoint& pt, cRay3Df& ray, i32 w, i32 h) const
{
	//获取视口信息
	cRect viewport;
	viewport.SetData2(0, 0, w, h);
	//cVector3Df vecProject((f32)pt.x, (f32)pt.y, -100000.f);
	cVector3Df vecProject((f32)pt.x, (f32)pt.y, 1.0f);
	//观察坐标系
	cVector3Df vecout;
	cMatrix4X4::ProjectTransform2DTo3D(vecout, vecProject, viewport, m_mProject, m_mView);
	ray = cRay3Df(vecout, vecProject);
	return true;
}

//空间中的点转到屏幕上
inline bool cViewFrustum::GetScreenCoordinatesFromVector3D(cPoint& pt, const cVector3Df& v, i32 w, i32 h) const
{
	cVector2Df vScreen;
	cRect rc(0, 0, w, h);
	cMatrix4X4::Transform3DTo2D(vScreen, v, rc, m_mProject, m_mView, false);
	cPoint ptTemp((i32)(vScreen.x + 0.5f), (i32)(vScreen.y + 0.5f));
	if (!rc.IsIn(ptTemp))
		return false;
	pt = ptTemp;
	return true;
}

//获取8个顶点
inline void cViewFrustum::GetEdges(cVector3Df* edges) const
{
	/*
	Edges are stored in this way:
	Hey, am I an ascii artist, or what? :) niko.
             /3--------/7
            /  |      / |
           /   |     /  |
           1---------5  |
           |   2- - -| -6
           |  /      |  /
           |/        | /
           0---------4/
	*/
	edges[0] = GetNearLeftDown();
	edges[1] = GetNearLeftUp();
	edges[2] = GetFarLeftDown();
	edges[3] = GetFarLeftUp();
	edges[4] = GetNearRightDown();
	edges[5] = GetNearRightUp();
	edges[6] = GetFarRightDown();
	edges[7] = GetFarRightUp();
}

inline void cViewFrustum::GetEdgesIndex(ui16* edgesindex) const
{
	ui16 indexes[] = {
		2, 3, 1, 2, 1, 0,
		0, 1, 5, 0, 5, 4,
		4, 5, 7, 4, 7, 6,
		6, 7, 3, 6, 3, 2,
		1, 3, 7, 1, 7, 5,
		2, 0, 4, 2, 4, 6
	};
	CrossMemory::MemCpy(edgesindex, indexes, sizeof(indexes));
}

//获取12条线
inline void cViewFrustum::GetLines(cLine3Df* lines) const
{
	cVector3Df vs[8];
	GetEdges(vs);
	lines[0].from = vs[0];
	lines[0].to = vs[1];
	lines[1].from = vs[1];
	lines[1].to = vs[5];
	lines[2].from = vs[5];
	lines[2].to = vs[4];
	lines[3].from = vs[4];
	lines[3].to = vs[0];

	lines[4].from = vs[1];
	lines[4].to = vs[3];
	lines[5].from = vs[0];
	lines[5].to = vs[2];
	lines[6].from = vs[4];
	lines[6].to = vs[6];
	lines[7].from = vs[5];
	lines[7].to = vs[7];

	lines[8].from = vs[2];
	lines[8].to = vs[3];
	lines[9].from = vs[3];
	lines[9].to = vs[7];
	lines[10].from = vs[7];
	lines[10].to = vs[6];
	lines[11].from = vs[6];
	lines[11].to = vs[2];
}

inline bool cViewFrustum::BuildOrthoViewFrustum(const cAabboxf& box, const cVector3Df& vDir)
{
	//if (box.IsEmpty() || vDir.IsEmpty())
	//	return false;
	//cVector3Df dirSrc(1.f, 0.f, 0.f);
	//dirSrc.Normalize();
	//cVector3Df dirDes(vDir);
	//dirDes.Normalize();
	//cQuaternion q;
	//q.FromAngleAxis(dirDes.GetTheta(dirSrc), dirDes.CrossProduct(dirSrc));
	//cMatrix4X4 m = q.GetMatrix();
	//m.TransformVect(
	//m.SetRotationRadian(cVector3Df());
	return true;
}

inline bool cViewFrustum::BuildPerspectiveViewFrustum(const cAabboxf& box, const cVector3Df& vDir)
{
	if (box.IsEmpty() || vDir.IsEmpty())
		return false;
	cVector3Df dir(vDir);
	dir.Normalize();
	return true;
}

}

typedef cm::cViewFrustum	viewfrustum;

#endif
