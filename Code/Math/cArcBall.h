#ifndef _cross_math_carcball_h
#define _cross_math_carcball_h

#include "Config.h"
#include "MathFunc.h"
#include "cVector3D.h"
#include "cRect.h"
#include "cMatrix4X4.h"
#include "cQuaternion.h"

namespace cross
{

class cArcBall
{
public:

	cArcBall() {
		Reset();
	}

	void Reset() {
		m_vDownPt.Set0();
		m_vCurrentPt.Set0();
		m_qNow.Identity();
		m_qDown.Identity();
		m_matrix.Identity();
		m_bDrag = false;
		m_fRadius = 1.f;
	}

	void SetView(const cRect& rc) {
		m_rcView = rc;
	}

	void OnBegin(i32 x, i32 y) {
		if (m_rcView.IsIn(cPoint(x, y))) {
			m_bDrag = true;
			m_qDown = m_qNow;
			m_vDownPt = ScreenToVector( x, y );
		}
	}
	void OnMove(i32 x, i32 y) {
		if (m_bDrag) { 
			m_vCurrentPt = ScreenToVector( x, y );
			m_qNow = m_qDown * QuatFromBallPoints( m_vDownPt, m_vCurrentPt );
		}
	}
	void OnEnd() {
		m_bDrag = false;
	}

	bool IsBeingDragged() const {
		return m_bDrag;
	}

	cVector3Df ScreenToVector( i32 fScreenPtX, i32 fScreenPtY ) {
		// Scale to screen
		f32 x   = -	(fScreenPtX - m_rcView.left - m_rcView.GetWidth() / 2.f)  / (m_fRadius * m_rcView.GetWidth() / 2.f);
		f32 y   =		(fScreenPtY - m_rcView.top - m_rcView.GetHeight() / 2.f) / (m_fRadius * m_rcView.GetHeight() / 2.f);

		f32 z   = 0.0f;
		f32 mag = x * x + y * y;

		if( mag > 1.0f )
		{
			f32 scale = 1.f / MathFunc::Sqrt(mag);
			x *= scale;
			y *= scale;
		}
		else
			z = MathFunc::Sqrt(1.f - mag);

		// Return vector
		return cVector3Df( x, y, z );
	}

	cQuaternion QuatFromBallPoints(const cVector3Df& vFrom, const cVector3Df& vTo) {
		f32 fDot = vFrom.DotProduct(vTo);
		cVector3Df vPart = vFrom.CrossProduct(vTo);
		return cQuaternion(vPart.x, vPart.y, vPart.z, fDot);
	}

	//const cMatrix4X4& GetRotationMatrix() const;
	//const cMatrix4X4& GetTranslationMatrix() const;
	//const cMatrix4X4& GetMatrix() {
	//	m_matrix.SetQuaternion(m_qNow);
	//	return m_matrix;
	//}

protected:

	bool		m_bDrag;

	cMatrix4X4	m_matrix;

	cRect		m_rcView;

	f32		m_fRadius;

	cVector3Df	m_vCurrentPt;
	cVector3Df	m_vDownPt;

	cQuaternion	m_qNow;
	cQuaternion	m_qDown;

};

}
#endif
