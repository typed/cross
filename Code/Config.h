#ifndef _cross_config_h_
#define _cross_config_h_

typedef unsigned long long	ui64;
typedef long long			i64;
typedef unsigned int		ui32;
typedef int					i32;
typedef unsigned long		ul32;
typedef long				l32;
typedef unsigned short		ui16;
typedef short				i16;
typedef unsigned char		ui8;
typedef char				i8;
typedef bool				b8;
typedef float				f32;
typedef double				f64;
typedef int					b32;
typedef void				vd;

typedef vd*					pvd;
typedef const vd*			cpvd;
typedef i8*					pi8;
typedef pi8					pstr;
typedef const i8*			cpi8;
typedef cpi8				cpstr;
typedef ui8*				pui8;
typedef const ui8*			cpui8;
typedef i16*				pi16;
typedef const i16*			cpi16;
typedef ui16*				pui16;
typedef const ui16*			cpui16;
typedef i32*				pi32;
typedef const i32*			cpi32;
typedef ui32*				pui32;
typedef const ui32*			cpui32;
typedef i64*				pi64;
typedef const i64*			cpi64;
typedef ui64*				pui64;
typedef const ui64*			cpui64;
typedef f32*				pf32;
typedef const f32*			cpf32;
typedef f64*				pf64;
typedef const f64*			cpf64;
typedef ui8					byte;
typedef pui8				pbyte;
typedef cpui8				cpbyte;
typedef ui64				tTime;
typedef ui32				commonid;
typedef i32					tDate;
typedef ui32				color32;
typedef ui16				color16;

#define T32 1
#define F32 0

#ifdef NULL
#	undef NULL
#endif
#define NULL 0

#define Succeeded(Status) ((Status)>=0)
#define Failed(Status) ((Status)<0)
#define SOK 0
#define EFAIL -1

#define UI32_MAX 0xffffffff
#define I32_MIN 0x80000000
#define I32_MAX 0x7fffffff
#define I32_MAX_HALF 0x3fffffff
#define F32_BIG 9999999.f
#define F32_SIMALL 0.0000001f

#ifdef REL
#	undef REL
#endif
#define REL(p) if ((p)) {(p)->Release(); (p) = 0;}

#ifdef DEL
#	undef DEL
#endif
#define DEL(p) if ((p)) { delete (p); (p) = 0;}

#ifdef DEL_ARRAY
#	undef DEL_ARRAY
#endif
#define DEL_ARRAY(p) if ((p)) { delete[] (p); (p) = 0;}

//对齐
#define Ali(num,align) (((num)+((align)-1))&(~((align)-1)))

#endif
