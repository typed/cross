#ifndef crectset_h
#define crectset_h

#include "../CommonDef.h"
#include "cRect.h"
#include "vector"

namespace cm
{

template<class TR>
class cRectSetTemplate
{
public:

	typedef std::vector< cRectTemplate<TR> > aRect;

	cRectSetTemplate()
	{
	}

	cRectSetTemplate(const cRectSetTemplate<TR>& rs)
	{
		*this = (rs);
	}

	cRectSetTemplate<TR>& operator = (const cRectSetTemplate<TR>& rs) {
		m_aRect = rs.m_aRect;
		m_side = rs.m_side;
		return *this;
	}

	void Clear() {
		m_side.Set0();
	}

	bool AddRect(const cRectTemplate<TR>& r) {
		if (r.IsEmpty())
			return false;
		m_aRect.push_back(r);
		m_side = m_side.Union(r);
		return true;
	}

	const cRectSetTemplate<TR>& GetRect(ui32 idx) const {
		return m_aRect[idx];
	}

	ui32 GetRectCount() const {
		return m_aRect.size();
	}

	const cRectTemplate<TR>& GetRectSide() const {
		return m_side;
	}

	void SetScale(f32 xscale, f32 yscale) {

	}

protected:
	aRect				m_aRect;
	cRectTemplate<TR>	m_side;
};

typedef cRectSetTemplate<i32> cRectSet;
typedef cRectSetTemplate<f32> cRectSetf;

}
#endif
