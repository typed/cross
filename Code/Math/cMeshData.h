#ifndef cmeshdata_h
#define cmeshdata_h
#include "CommonDef.h"
#include "MathBase.h"
#include "cVector2D.h"
#include "cVector3D.h"
#include "cMatrix4X4.h"
#include "cvector.h"
#include "cMap.h"
#include "iStream_DS.h"
#include "cstring.h"

namespace cross
{

struct stVertex
{
	cVector3Df			m_pos;
	cVector3Df			m_nor;
	cVector2Df			m_tex;
	ui32				m_nWOffset;
	ui32				m_nWNum;
};

struct stWeight
{
	ui32				m_nVIndex;
	f32					m_fWeights;
	ui32				m_nBoneIdx;
};

struct stBone
{
	cstring			m_name;
	ui32				m_idxParent;		//父结点
	cMatrix4X4			m_mxInit;			//初始距阵
	cMatrix4X4			m_mxOffset;			//偏移距阵
	bool IsSameName(const cstring& s) const {return m_name == s;}
};

class cMeshData
{
public:
	static const ui32 c_type = 'MD00';
	cMeshData();
	cMeshData(const cMeshData& md);
	cMeshData& operator = (const cMeshData& md);
	bool ToStream(cOStream& os) const;
	bool FromStream(cIStream& is);
	cvector<stVertex>	m_aVertex;
	cvector<cVector3Dus>	m_aFace;
	cvector<stWeight>	m_aWeight;
};

struct stAniBone
{
	ui32				idxBone;			//被控制的骨头的index
	ui32				nKey;				//Ani对应的key的个数
	ui32				nKeyOffset;			//开始Key Index
};

struct stAniKey
{
	ui32				nTime;				//时间点
	cMatrix4X4			mat;				//变换
};

class cAni
{
public:
	cAni();
	cAni(const cAni& ani);
	cAni& operator = (const cAni& ani);
	bool ToStream(cOStream& os) const;
	bool FromStream(cIStream& is);
	cvector<stAniBone>				m_aBone;
	cvector<stAniKey>				m_aKey;
	ui32							m_sumTime;
	cstring						m_name;
};

class cAniSet
{
public:
	cAniSet();
	cAniSet(const cAniSet& ani);
	cAniSet& operator = (const cAniSet& ani);
	bool AddAni(cstring, const cAni& ani);
	bool DelAni(cstring);
	void DelAllAni();
	const cAni* GetAni(cstring) const;
private:
	cMap<cstring, cAni>			m_mpAni;
};

class cSkeletonData
{
public:
	cSkeletonData();
	cSkeletonData(const cSkeletonData& sd);
	cSkeletonData& operator = (const cSkeletonData& sd);
	bool ToStream(cOStream& os) const;
	bool FromStream(cIStream& is);
	cvector<stBone>					m_aSkl;				//初始骨架
	cAniSet							m_aniset;			//动作
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//cMeshData
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline cMeshData::cMeshData()
{
}

inline cMeshData::cMeshData(const cMeshData& md)
{
	*this = md;
}

inline cMeshData& cMeshData::operator = (const cMeshData& md)
{
	m_aVertex = md.m_aVertex;
	m_aFace = md.m_aFace;
	m_aWeight = md.m_aWeight;
	return *this;
}

inline bool cMeshData::ToStream(cOStream& os) const
{
	os << c_type;
	const ui32 c_version = 1;
	os << c_version;
	array_to_stream(os, m_aVertex);
	array_to_stream(os, m_aFace);
	array_to_stream(os, m_aWeight);
	return os.IsOk();
}

inline bool cMeshData::FromStream(cIStream& is)
{
	ui32 type = 0;
	is >> type;
	if (type != c_type)
		return false;
	ui32 version = 0;
	is >> version;
	if (version >= 1) {
		array_from_stream(is, m_aVertex);
		array_from_stream(is, m_aFace);
		array_from_stream(is, m_aWeight);
	}
	return is.IsOk();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//cAni
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline cAni::cAni()
{
}

inline cAni::cAni(const cAni& ani)
{
	*this = ani;
}

inline cAni& cAni::operator = (const cAni& ani)
{
	m_aBone = ani.m_aBone;
	m_aKey = ani.m_aKey;
	m_name = ani.m_name;
	m_sumTime = ani.m_sumTime;
	return *this;
}

inline bool cAni::ToStream(cOStream& os) const
{
	const ui32 c_version = 1;
	os << c_version;
	os << m_name << m_sumTime;
	array_to_stream(os, m_aBone);
	array_to_stream(os, m_aKey);
	return os.IsOk();
}

inline bool cAni::FromStream(cIStream& is)
{
	ui32 version = 0;
	is >> version;
	if (version >= 1) {
		is >> m_name >> m_sumTime;
		array_from_stream(is, m_aBone);
		array_from_stream(is, m_aKey);
	}
	return is.IsOk();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//cAniSet
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline cAniSet::cAniSet()
{
}

inline cAniSet::cAniSet(const cAniSet& ani)
{
	*this = ani;
}

inline cAniSet& cAniSet::operator = (const cAniSet& ani)
{
	ani.m_mpAni.copy(m_mpAni);
	return *this;
}

inline bool cAniSet::AddAni(cstring sKey, const cAni& ani)
{
	if (m_mpAni.find(sKey))
		return false;
	m_mpAni.set(sKey, ani);
	return true;
}

inline bool cAniSet::DelAni(cstring sKey)
{
	return m_mpAni.remove(sKey);
}

inline void cAniSet::DelAllAni()
{
	m_mpAni.clear();
}

inline const cAni* cAniSet::GetAni(cstring sKey) const
{
	cMap<cstring, cAni>::iterator pn = m_mpAni.find(sKey);
	if (pn == m_mpAni.end())
		return 0;
	return &pn->getValue();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//cSkeletonData
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline cSkeletonData::cSkeletonData()
{
}

inline cSkeletonData::cSkeletonData(const cSkeletonData& sd)
{
	*this = sd;
}

inline cSkeletonData& cSkeletonData::operator = (const cSkeletonData& sd)
{
	m_aSkl = sd.m_aSkl;
	m_aniset = sd.m_aniset;
	return *this;
}

inline bool cSkeletonData::ToStream(cOStream& os) const
{
	array_to_stream(os, m_aSkl);
	return os.IsOk();
}

inline bool cSkeletonData::FromStream(cIStream& is)
{
	array_from_stream(is, m_aSkl);
	return is.IsOk();
}

}
#endif
