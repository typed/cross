#ifndef _common_color_h_
#define _common_color_h_

#include "../CommonDef.h"
#include "cRect.h"
#include "cVector4D.h"
#include "Random.h"

namespace cm
{

#define SMOOTH(v) \
	{ \
		if ( (v) > 1.f ) \
		{\
			(v) = 1.f; \
		}\
		else if ( (v) < 0.f) \
		{\
			(v) = 0.f; \
		}\
	}


class COMMON_API cColor
{
public:

	cColor() : m_fAlpha(0.f), m_fRed(0.f), m_fGreen(0.f), m_fBlue(0.f) {}

	cColor(f32 r, f32 g, f32 b, f32 a) {
		SetColor(r, g, b, a);
	}

	cColor(ui32 col) {
		SetColor8888(col);
	}

	cColor(ui32 col, bool /*bRGB*/) {      // r g b   低位->高位   add by jiangzhijun 10-9-28
		SetColor888(col);
	}

	cColor(const cColor& col) {
		*this = col;
	}

	bool operator == (const cColor& col) const {
		return Equals(m_fAlpha, col.m_fAlpha) &&
			Equals(m_fRed, col.m_fRed) &&
			Equals(m_fGreen, col.m_fGreen) &&
			Equals(m_fBlue, col.m_fBlue);
	}

	bool operator != (const cColor& col) const {
		return !operator == (col);
	}

	cColor& operator = (const cColor& col) {
		m_fAlpha = col.m_fAlpha;
		m_fRed = col.m_fRed;
		m_fGreen = col.m_fGreen;
		m_fBlue = col.m_fBlue;
		return *this;
	}

	cColor& operator += (const cColor& col) {
		m_fAlpha += col.m_fAlpha;
		m_fRed += col.m_fRed;
		m_fGreen += col.m_fGreen;
		m_fBlue += col.m_fBlue;
		//SMOOTH(m_fAlpha);
		//SMOOTH(m_fRed);
		//SMOOTH(m_fGreen);
		//SMOOTH(m_fBlue);
		return *this;
	}

	void Interpolate(cColor& out, const cColor& in, f32 fChangePer)
	{
		out.m_fRed = m_fRed + (in.m_fRed - m_fRed) * fChangePer;
		out.m_fAlpha = m_fAlpha + (in.m_fAlpha - m_fAlpha) * fChangePer;
		out.m_fBlue  = m_fBlue + (in.m_fBlue - m_fBlue) * fChangePer;
		out.m_fGreen = m_fGreen + (in.m_fGreen - m_fGreen) * fChangePer;
	}

	//灰度值 = 0.3*R+0.59*G+0.11*B
	cColor Gradation()
	{
		cColor col;
		col.m_fRed = m_fRed * 0.3f;
		col.m_fGreen = m_fGreen * 0.59f;
		col.m_fBlue  = m_fBlue * 0.11f;
		return col;
	}

	cColor operator + (const cColor& t) const {
		cColor col;
		col.m_fAlpha = m_fAlpha + t.m_fAlpha;
		col.m_fRed = m_fRed + t.m_fRed;
		col.m_fGreen = m_fGreen + t.m_fGreen;
		col.m_fBlue = m_fBlue + t.m_fBlue;
		//SMOOTH(col.m_fAlpha);
		//SMOOTH(col.m_fRed);
		//SMOOTH(col.m_fGreen);
		//SMOOTH(col.m_fBlue);
		return col;
	}

	cColor operator * (f32 t) const {
		cColor col;
		col.m_fAlpha = m_fAlpha * t;
		col.m_fRed = m_fRed * t;
		col.m_fGreen = m_fGreen * t;
		col.m_fBlue = m_fBlue * t;
		//SMOOTH(col.m_fAlpha);
		//SMOOTH(col.m_fRed);
		//SMOOTH(col.m_fGreen);
		//SMOOTH(col.m_fBlue);
		return col;
	}

    void SetColor888(ui32 col) {
		m_fRed = (ui8)col / 255.f;
		m_fGreen = (ui8)(col >> 8) / 255.f;
		m_fBlue = (ui8)(col >> 16) / 255.f;
		m_fAlpha = 1.0;
	}

	void SetColor8888(ui32 col) {
		m_fAlpha = ( col >> 24 ) / 255.f;
		m_fRed = ( ( col & 0x00ff0000 ) >> 16 ) / 255.f;
		m_fGreen = ( ( col & 0x0000ff00 ) >> 8 ) / 255.f;
		m_fBlue = ( col & 0x000000ff ) / 255.f;
	}

	ui32 GetColor8888() const {
		ui32 t;
		ui32 re = 0;
		t = (ui32)(m_fAlpha * 0xff);
		re |= (t << 24);
		t = (ui32)(m_fRed * 0xff);
		re |= (t << 16);
		t = (ui32)(m_fGreen * 0xff);
		re |= (t << 8);
		t = (ui32)(m_fBlue * 0xff);
		return re | t;
	}

	ui32 GetColor888() const {
		ui32 t;
		ui32 re = 0;
		t = (ui32)(m_fBlue * 0xff);
		re |= (t << 16);
		t = (ui32)(m_fGreen * 0xff);
		re |= (t << 8);
		t = (ui32)(m_fRed * 0xff);
		return re | t;
	}

	void SetColor(f32 r, f32 g, f32 b, f32 a) {
		m_fRed = r; m_fGreen = g; m_fBlue = b; m_fAlpha = a;
	}

	void GetColor(f32& r, f32& g, f32& b, f32& a) const {
		r = m_fRed; g = m_fGreen; b = m_fBlue; a = m_fAlpha;
	}

	void SetRed(f32 r) {m_fRed = r;}
	f32 GetRed() const {return m_fRed;}
	void SetGreen(f32 g) {m_fGreen = g;}
	f32 GetGreen() const {return m_fGreen;}
	void SetBlue(f32 b) {m_fBlue = b;}
	f32 GetBlue() const {return m_fBlue;}
	void SetAlpha(f32 a) {m_fAlpha = a;}
	f32 GetAlpha() const {return m_fAlpha;}

	cVector4Df GetVector4D() const {return cVector4Df(m_fRed, m_fGreen, m_fBlue, m_fAlpha);}

	f32 m_fAlpha;
	f32 m_fRed;
    f32 m_fGreen;
    f32 m_fBlue;

};

class COMMON_API cColor32_4
{
public:

	cColor32_4() {
		SetColor(0xffffffff);
	}

	cColor32_4(ui32 c) {
		SetColor(c);
	}

	cColor32_4(ui32 tl, ui32 tr, ui32 bl, ui32 br) {
		col[DR_TopLeft] = tl;
		col[DR_TopRight] = tr;
		col[DR_BottomLeft] = bl;
		col[DR_BottomRight] = br;
	}

	void SetColor(ui32 c) {
		for (i32 i = 0; i < cm::DR_Num; i++) {
			col[i] = c;
		}
	}

	void SetColor(cm::eDirRect e, ui32 c) {
		col[e] = c;
	}

	void SetAlpha(cm::eDirRect e, ui8 c) {
		ui32 cc = c << 24;
		col[e] = (col[e] & 0x00ffffff) | cc;
	}

	void SetAlpha(ui8 c) {
		ui32 cc = c << 24;
		for (i32 i = 0; i < cm::DR_Num; i++) {
			col[i] = (col[i] & 0x00ffffff) | cc;
		}
	}

	ui8 GetAlpha(cm::eDirRect e = cm::DR_TopLeft) const {
		return (ui8) ( col[e] >> 24 );
	}

	ui32 GetColor(cm::eDirRect e) const {
		return col[e];
	}

	ui32 GetColor() const {
		return GetColor(DR_TopLeft);
	}

	bool operator == (const cColor32_4& c) const {
		for (int i = 0; i < cm::DR_Num; i++)
			if (col[i] != c.col[i])
				return false;
		return true;
	}

	bool operator != (const cColor32_4& c) const {
		return !operator == (c);
	}

	cColor32_4& operator = (const cColor32_4& c) {
		for (int i = 0; i < cm::DR_Num; i++)
			col[i] = c.col[i];
		return *this;
	}
protected:
	ui32 col[cm::DR_Num];
};

inline ui32 RandomColor32(ui32 colMin, ui32 colMax)
{
	return ( RandomInt( colMin & 0xff000000, colMax & 0xff000000 ) & 0xff000000 ) |
		( RandomInt( colMin & 0x00ff0000, colMax & 0x00ff0000 ) & 0x00ff0000 ) |
		( RandomInt( colMin & 0x0000ff00, colMax & 0x0000ff00 ) & 0x0000ff00 ) |
		RandomInt( colMin & 0x000000ff, colMax & 0x000000ff );
}

//inline ui32 GetColor32Delta(ui32 colMax, ui32 colMin)
//{
//
//}

}

typedef cm::cColor		color;

typedef cm::cColor32_4	color32_4;

#endif
