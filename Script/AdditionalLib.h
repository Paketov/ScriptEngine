#ifndef __ADDITIONAL_LIB__
#define __ADDITIONAL_LIB__
#ifdef WIN32
#include <winnt.h>
#endif
#include <math.h>

//+NAN 7FFF F0000000 00000000
#define LN Ln
#define POW Pow

#define CHAR_TYPE(Type, Char)  ((sizeof(Type) == sizeof(char))?(Char):(L ## Char))
#define CMP_CHARS(Arr1, Arr2) (sizeof(Arr1[0]) == sizeof(char)?\
	(*(unsigned short*)(Arr1) == *(unsigned short*)(Arr2)):\
	(*(unsigned int*)(Arr1) == *(unsigned int*)(Arr2)))

#ifdef ARCH_X86
FORCEINLINE double Fabs(double x)
{
	double Result;
	__asm
	{
			FLD x					
			FABS
			FSTP Result
	}
	return Result;
}

FORCEINLINE double Round(double x)
{
	double Result;
	__asm
	{
			FLD x					
		    FRNDINT
			FSTP Result
	}
	return Result;
}

FORCEINLINE double Sqrt(double x)
{
	double Result;
	__asm
	{
			FLD x					
			FSQRT
			FSTP Result
	}
	return Result;
}

FORCEINLINE double Cos(double x)
{
	double Result;
	__asm
	{
		FLD x					
		FCOS
		FSTP Result
	}
	return Result;
}

FORCEINLINE double Sin(double x)
{
	double Result;
	__asm
	{
		FLD x					
		FSIN
		FSTP Result
	}
	return Result;
}

FORCEINLINE double Ln(double x)
{
	double Result;
	__asm
	{
		FLDLN2					//st(0) = ln(2)
		FLD x					//st(0) = arg, st(1) = ln(2)
		FYL2X					//st(0) = ST(1)*log2(ST(0))
		FSTP Result
	}
	return Result;
}

inline double Pow(double x, double y)
{
	int inty = (int)y;
	double Result;
	double _X = x;
	unsigned int _N = (inty >= 0)?(unsigned int)inty: (unsigned int)-inty;
	for (Result = 1.0; ; _X *= _X)
	{
		if ((_N & 1) != 0)
			Result *= _X;
		if ((_N >>= 1) == 0)
		{
			if(inty < 0)
				Result = 1.0 / Result; 
			break;
		}
	}
	if((_X = y - inty) != 0.0)
	{
		__asm
		{
			FLD _X		//st(0) = _X
				FLD x		//st(0) = x
				FYL2X		//st(0) = ST(1)*log2(ST(0))
				F2XM1		//st(0) = 2^st(0) - 1
				FLD1		//st(0) = 1.0
				FADD		//st(0) += st(1)
				FMUL Result //st(0) *= Result
				FST Result
		}
	}
	return Result;
}

#else
#define Fabs(x) (fabs(x))
#define Round(x) ((long long)x)
#define Sqrt(x) (sqrt(x))
#define Cos(x) (cos(x))
#define Sin(x) (sin(x))
#define Ln(x) (log((long double)x))
inline double Pow(double x, double y) { return pow(x, y);}
#endif

inline void * MemSrch(const void * InStr, size_t StrLen, const void *SubStr, size_t SubStrLen)
{
	if(SubStrLen > StrLen)
		return NULL;
	unsigned int * Sub = (unsigned int*)SubStr;
	unsigned int * Str = (unsigned int*)InStr;
	unsigned int * MaxStr = (unsigned int*)((char*)Str + (StrLen - SubStrLen + 1));
	unsigned int * MaxSub = (unsigned int*)((char*)SubStr + SubStrLen);
	if(SubStrLen >= 4)
	{
		for(;Str < MaxStr;Str = (unsigned int*)((unsigned char*)Str + 1))
		{
			if(*Str == *Sub)
			{
				unsigned int* Found = Str;
				Str++; Sub++;
				for(unsigned int * m = MaxSub - 1;Sub <= m;Str++, Sub++)
					if(*Sub != *Str)
						goto lblExtrLoop;
				for(;Sub < MaxSub;Str = (unsigned int*)((unsigned char*)Str + 1), Sub = (unsigned int*)((unsigned char*)Sub + 1))
					if(*(unsigned char*)Sub != *(unsigned char*)Str)
						goto lblExtrLoop;
				return Found;	
lblExtrLoop:
				Str = Found;
				Sub = (unsigned int*)SubStr;
			}
		}
	}else if(SubStrLen > 0)
	{
		for(;Str < MaxStr;Str = (unsigned int*)((unsigned char*)Str + 1))
		{
			if(*(unsigned char*)Str == *(unsigned char*)Sub)
			{
				unsigned int* Found = Str;
				Str = (unsigned int*)((unsigned char*)Str + 1);
				Sub = (unsigned int*)((unsigned char*)Sub + 1);
				for(;Sub < MaxSub;Str = (unsigned int*)((unsigned char*)Str + 1), Sub = (unsigned int*)((unsigned char*)Sub + 1))
					if(*(unsigned char*)Sub != *(unsigned char*)Str)
						goto lblExtr2Loop;
				return Found;	
lblExtr2Loop:
				Str = Found;
				Sub = (unsigned int*)SubStr;
			}
		}
	}
	return NULL;

	//__asm
	//{
	//	MOVAPS XMM0, DQWORD PTR DS:[EDI]
	//	MOVNTPS DQWORD PTR DS:[EDI],XMM0
	//		movntq  qword [EDI], MM0
	//}
}



template<class _Ty> inline
	_Ty Pow(_Ty _X, int _Y)
{
	unsigned int _N = (_Y >= 0) ?(unsigned int)_Y: (unsigned int)(-_Y);
	for (_Ty _Z = _Ty(1); ; _X *= _X)
	{
		if ((_N & 1) != 0)
			_Z *= _X;
		if ((_N >>= 1) == 0)
			return (_Y < 0 ? _Ty(1) / _Z : _Z); 
	}
}

#endif