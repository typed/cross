#ifndef _cross_math_posrotscl_h_
#define _cross_math_posrotscl_h_

#include "../CommonDef.h"
#include "MathBase.h"
#include "cVector3D.h"
#include "cQuaternion.h"
#include "cMatrix4X4.h"

namespace cross
{

class cPosRotScl
{
public:
	cPosRotScl() {}
	cPosRotScl(const vector3d& vPos, const quat& qRot, const vector3d& vScl) : m_pos(vPos), m_rot(qRot), m_scl(vScl) {}
	cPosRotScl(const cPosRotScl& prs) { *this = prs; }
	cPosRotScl& operator = (const cPosRotScl& prs) { m_pos = prs.m_pos; m_rot = prs.m_rot; m_scl = prs.m_scl; return *this; }
	bool operator == (const cPosRotScl& prs) const { return m_pos == prs.m_pos && m_rot == prs.m_rot && m_scl == prs.m_scl; }
	bool operator != (const cPosRotScl& prs) const { return !operator == (prs); }
	matrix GetMatrix() const;
	vector3d	m_pos;
	quat		m_rot;
	vector3d	m_scl;
};

inline matrix cPosRotScl::GetMatrix() const
{
	matrix tm;
	matrix m = m_rot.GetMatrix();
	tm.SetTranslation(m_pos);
	m = tm * m;
	tm.Identity();
	tm.SetScale(m_scl);
	m = tm * m;
	return m;
}

}

#endif