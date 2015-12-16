#ifndef __INLINE_INSIDE_DATA_H__
#define __INLINE_INSIDE_DATA_H__

#include "TypeData.h"


template<class T = INSIDE_DATA>
class _INLINE_OPERATORS: public INSIDE_DATA
{
public:

	_INLINE_OPERATORS(){}

	template<class A>
	FORCEINLINE _INLINE_OPERATORS(A v)
	{
		*((LPINSIDE_DATA)this) = v;
	}

	FORCEINLINE operator bool()
	{
		switch(TypeData)
		{	
		case TYPEDATA_NULL:
			return false;
		case TYPEDATA_EXCEPT:	
			return true;
		case TYPEDATA_INTEGER:
			return Integer != 0;
		case TYPEDATA_STRING:
		case TYPEDATA_OBJECT:
			return Object.operator bool();
		default:
			return Double != 0.0;
		}
	}

	FORCEINLINE operator ZELLI_INTEGER()
	{
		switch(TypeData)
		{
		case TYPEDATA_INTEGER:
			return Integer;
		case OBJECT_CASE:
			return Object.operator ZELLI_INTEGER();
		case TYPEDATA_NULL:
		case TYPEDATA_EXCEPT:
			return 0;
		default:	
			return (ZELLI_INTEGER)Double;
		}
	}

	FORCEINLINE operator ZELLI_DOUBLE()
	{
		switch(TypeData)
		{	
		case TYPEDATA_INTEGER:
			return (ZELLI_DOUBLE)Integer;
		case OBJECT_CASE:
			return Object.operator ZELLI_DOUBLE();
		case TYPEDATA_NULL:
		case TYPEDATA_EXCEPT:
			return 0.0;
		default:
			return Double;
		}
	}

	FORCEINLINE INSIDE_DATA operator +(INSIDE_DATA & Val2)
	{
		switch(TypeData)
		{
		case INSIDE_DATA::TYPEDATA_NULL:
			return Val2;
		case INSIDE_DATA::TYPEDATA_INTEGER:
			return Integer + (ZELLI_DOUBLE)Val2;
		case OBJECT_CASE:
			return Object.operator +(Val2);
		case INSIDE_DATA::TYPEDATA_EXCEPT:
			return INSIDE_DATA::Null;
		default:
			return Double + (ZELLI_DOUBLE)Val2;
		}
	}

	FORCEINLINE bool operator ==(INSIDE_DATA & CompareData)
	{
		if(CompareData.IsObject)
			return CompareData.Object.operator==(*this);

		switch(TypeData)
		{
		case TYPEDATA_INTEGER:
			if(CompareData.TypeData == TYPEDATA_INTEGER)
				return Integer == CompareData.Integer;
			else
				return false;
		case OBJECT_CASE:
			//Возвращает true если обе переменных указывают на один и тот же объект
			return Object.operator ==(CompareData);
		case TYPEDATA_EXCEPT: 
			return (LPINSIDE_DATA)this == &CompareData;
		case TYPEDATA_NULL:
			if(TypeData == CompareData.TypeData)
				return true;
			return false;
		default:
			if(TypeData != CompareData.TypeData)
				return false;
			return Double == CompareData.Double;
		}
	}

	FORCEINLINE bool operator!=(INSIDE_DATA & CompareData)
	{
		if(CompareData.IsObject)
			return CompareData.Object.operator!=(*this);

		switch(TypeData)
		{
		case TYPEDATA_INTEGER:
			if(CompareData.TypeData == TYPEDATA_INTEGER)
				return Integer != CompareData.Integer;
			else
				return true;
		case OBJECT_CASE:
			//Возвращает true если обе переменных указывают на один и тот же объект
			return Object.operator !=(CompareData);
		case TYPEDATA_EXCEPT: 
			return this != &CompareData;
		case TYPEDATA_NULL:
			if(TypeData != CompareData.TypeData)
				return true;
			return false;
		default:
			if(TypeData != CompareData.TypeData)
				return true;
			return Double != CompareData.operator ZELLI_DOUBLE();
		}
	}

	FORCEINLINE bool operator >(INSIDE_DATA & CompareData)
	{
		switch(TypeData)
		{
		case TYPEDATA_INTEGER:
			{
				if(CompareData.TypeData == TYPEDATA_INTEGER)
					return Integer > CompareData.Integer;
				else if(CompareData.TypeData < TYPEDATA_INTEGER)
					return (ZELLI_DOUBLE)Integer > CompareData.Double;
				else 
					return false;
			}
		case OBJECT_CASE:
			//Возвращает true если обе переменных указывают на один и тот же объект
			return Object.operator >(CompareData);
		case TYPEDATA_EXCEPT: 
		case TYPEDATA_NULL:
			return false;
		default:
			return Double > CompareData.operator ZELLI_DOUBLE();
		}

	}

	FORCEINLINE bool operator <(INSIDE_DATA & CompareData)
	{
		switch(TypeData)
		{
		case TYPEDATA_INTEGER:
			{
				if(CompareData.TypeData == TYPEDATA_INTEGER)
					return Integer < CompareData.Integer;
				else if(CompareData.TypeData < TYPEDATA_INTEGER)
					return (ZELLI_DOUBLE)Integer < CompareData.Double;
				else 
					return false;
			}
		case OBJECT_CASE:
			//Возвращает true если обе переменных указывают на один и тот же объект
			return Object.operator <(CompareData);
		case TYPEDATA_EXCEPT: 
		case TYPEDATA_NULL:
			return false;
		default:
			return Double < CompareData.operator ZELLI_DOUBLE();
		}
	}

	FORCEINLINE bool operator >=(INSIDE_DATA & CompareData)
	{
		switch(TypeData)
		{
		case TYPEDATA_INTEGER:
			{
				if(CompareData.TypeData == TYPEDATA_INTEGER)
					return Integer >= CompareData.Integer;
				else if(CompareData.TypeData < TYPEDATA_INTEGER)
					return (ZELLI_DOUBLE)Integer >= CompareData.Double;
				else 
					return false;
			}
		case OBJECT_CASE:
			//Возвращает true если обе переменных указывают на один и тот же объект
			return Object.operator >=(CompareData);
		case TYPEDATA_EXCEPT: 
		case TYPEDATA_NULL:
			return false;
		default:
			return Double >= CompareData.operator ZELLI_DOUBLE();
		}
	}

	FORCEINLINE bool operator <=(INSIDE_DATA & CompareData)
	{
		switch(TypeData)
		{
		case TYPEDATA_INTEGER:
			{
				if(CompareData.TypeData == TYPEDATA_INTEGER)
					return Integer <= CompareData.Integer;
				else if(CompareData.TypeData < TYPEDATA_INTEGER)
					return (ZELLI_DOUBLE)Integer <= CompareData.Double;
				else 
					return false;
			}
		case OBJECT_CASE:
			//Возвращает true если обе переменных указывают на один и тот же объект
			return Object.operator <=(CompareData);
		case TYPEDATA_EXCEPT: 
		case TYPEDATA_NULL:
			return false;
		default:
			return Double <= CompareData.Double;
		}
	}

	FORCEINLINE void operator ++()
	{
		switch(TypeData)
		{
		case INSIDE_DATA::TYPEDATA_NULL:
			return;
		case INSIDE_DATA::TYPEDATA_INTEGER:
			Integer++;
			return;
		case OBJECT_CASE:
			Object.operator ++();
			return;
		case INSIDE_DATA::TYPEDATA_EXCEPT:
			return;
		default:
			Double++;
		} 
	}

	FORCEINLINE void operator --()
	{
		switch(TypeData)
		{
		case INSIDE_DATA::TYPEDATA_NULL:
			return;
		case INSIDE_DATA::TYPEDATA_INTEGER:
			Integer--;
			return;
		case OBJECT_CASE:
			Object.operator --();
			return;
		case INSIDE_DATA::TYPEDATA_EXCEPT:
			return;
		default:
			Double--;
		}
	}

	FORCEINLINE INSIDE_DATA operator &(INSIDE_DATA & Val2)
	{
		switch(TypeData)
		{
		case INSIDE_DATA::TYPEDATA_NULL:
			return 0;
		case INSIDE_DATA::TYPEDATA_INTEGER:
			return Integer & (ZELLI_INTEGER)Val2;
		case OBJECT_CASE:
			return Object.operator &(Val2);
		case INSIDE_DATA::TYPEDATA_EXCEPT:
			return INSIDE_DATA::Null;
		default:
			return operator ZELLI_INTEGER() & (ZELLI_INTEGER)Val2;
		}
	}

	FORCEINLINE INSIDE_DATA operator |(INSIDE_DATA & Val2)
	{
		switch(TypeData)
		{
		case INSIDE_DATA::TYPEDATA_NULL:
			return (ZELLI_INTEGER)Val2;
		case INSIDE_DATA::TYPEDATA_INTEGER:
			return Integer | (ZELLI_INTEGER)Val2;
		case OBJECT_CASE:
			return Object.operator |(Val2);
		case INSIDE_DATA::TYPEDATA_EXCEPT:
			return INSIDE_DATA::Null;
		default:
			return operator ZELLI_INTEGER() | (ZELLI_INTEGER)Val2;
		}
	}

	FORCEINLINE INSIDE_DATA operator ^(INSIDE_DATA & Val2)
	{
		switch(TypeData)
		{
		case INSIDE_DATA::TYPEDATA_NULL:
			return 0 ^ (ZELLI_INTEGER)Val2;
		case INSIDE_DATA::TYPEDATA_INTEGER:
			return Integer ^ (ZELLI_INTEGER)Val2;
		case OBJECT_CASE:
			return Object.operator ^(Val2);
		case INSIDE_DATA::TYPEDATA_EXCEPT:
			return INSIDE_DATA::Null;
		default:
			return operator ZELLI_INTEGER() ^ (ZELLI_INTEGER)Val2;
		}
	}

	FORCEINLINE INSIDE_DATA operator <<(INSIDE_DATA & Val2)
	{
		switch(TypeData)
		{
		case INSIDE_DATA::TYPEDATA_NULL:
			return 0;
		case INSIDE_DATA::TYPEDATA_INTEGER:
			return Integer << (ZELLI_INTEGER)Val2;
		case OBJECT_CASE:
			return Object.operator <<(Val2);
		case INSIDE_DATA::TYPEDATA_EXCEPT:
			return INSIDE_DATA::Null;
		default:
			return operator ZELLI_INTEGER() << (ZELLI_INTEGER)Val2;
		}
	}

	FORCEINLINE INSIDE_DATA operator >>(INSIDE_DATA & Val2)
	{
		switch(TypeData)
		{
		case INSIDE_DATA::TYPEDATA_NULL:
			return 0;
		case INSIDE_DATA::TYPEDATA_INTEGER:
			return Integer >> (ZELLI_INTEGER)Val2;
		case OBJECT_CASE:
			return Object.operator >>(Val2);
		case INSIDE_DATA::TYPEDATA_EXCEPT:
			return INSIDE_DATA::Null;
		default:
			return operator ZELLI_INTEGER() >> (ZELLI_INTEGER)Val2;
		}
	}

	FORCEINLINE INSIDE_DATA operator -(INSIDE_DATA & Val2)
	{
		switch(TypeData)
		{
		case INSIDE_DATA::TYPEDATA_NULL:
			return Val2;
		case INSIDE_DATA::TYPEDATA_INTEGER:
			return Integer - (ZELLI_DOUBLE)Val2;
		case OBJECT_CASE:
			return Object.operator -(Val2);
		case INSIDE_DATA::TYPEDATA_EXCEPT:
			return INSIDE_DATA::Null;
		default:
			return Double - (ZELLI_DOUBLE)Val2;
		}
	}

	FORCEINLINE INSIDE_DATA operator *(INSIDE_DATA & Val2)
	{
		switch(TypeData)
		{
		case INSIDE_DATA::TYPEDATA_NULL:
			return Val2;
		case INSIDE_DATA::TYPEDATA_INTEGER:
			return Integer * (ZELLI_DOUBLE)Val2;
		case OBJECT_CASE:
			return Object.operator *(Val2);
		case INSIDE_DATA::TYPEDATA_EXCEPT:
			return INSIDE_DATA::Null;
		default:
			return Double * (ZELLI_DOUBLE)Val2;
		}
	}

	FORCEINLINE INSIDE_DATA operator /(INSIDE_DATA & Val2)
	{
		switch(TypeData)
		{
		case INSIDE_DATA::TYPEDATA_NULL:
			return Val2;
		case INSIDE_DATA::TYPEDATA_INTEGER:
			return (ZELLI_DOUBLE)Integer / (ZELLI_DOUBLE)Val2;
		case OBJECT_CASE:
			return Object.operator /(Val2);
		case INSIDE_DATA::TYPEDATA_EXCEPT:
			return INSIDE_DATA::Null;
		default:
			return Double / (ZELLI_DOUBLE)Val2;
		}
	}

	FORCEINLINE INSIDE_DATA operator %(INSIDE_DATA & Val2)
	{
		switch(TypeData)
		{
		case INSIDE_DATA::TYPEDATA_NULL:
			return Val2;
		case OBJECT_CASE:
			return Object.operator %(Val2);
		case INSIDE_DATA::TYPEDATA_EXCEPT:
			return INSIDE_DATA::Null;
		default:
			if(Val2.IsZero)
				return INSIDE_DATA::Null;
			return fmod(operator ZELLI_DOUBLE(), (ZELLI_DOUBLE)Val2);
		}
	}

	FORCEINLINE INSIDE_DATA operator -()
	{
		switch(TypeData)
		{
		case INSIDE_DATA::TYPEDATA_NULL:
			return *this;
		case INSIDE_DATA::TYPEDATA_INTEGER:
			return -Integer;
		case OBJECT_CASE:
			return Object.operator -();
		case INSIDE_DATA::TYPEDATA_EXCEPT:
			return INSIDE_DATA::Null;
		default:
			return -Double;
		}
	}

	FORCEINLINE INSIDE_DATA operator ~()
	{
		switch(TypeData)
		{
		case INSIDE_DATA::TYPEDATA_NULL:
			return *this;
		case INSIDE_DATA::TYPEDATA_INTEGER:
			return ~Integer;
		case OBJECT_CASE:
			return Object.operator ~();
		case INSIDE_DATA::TYPEDATA_EXCEPT:
			return INSIDE_DATA::Null;
		default:
			return ~((ZELLI_INTEGER)Double);
		}
	}

	FORCEINLINE INSIDE_DATA operator[](LPHEADER_STRING Str)
	{
		if(IsObject)
			return Object.Prototype->ReadMember(Str);
		THROW_NATIVE_EXCEPT("This val is not object", HEADER_EXCEPTION::VAL_IS_NOT_OBJECT);
	}

	FORCEINLINE INSIDE_DATA operator[](LPINSIDE_DATA Member)
	{
		if(IsObject)
			return Object.Prototype->ReadMember(Object, Member);
		THROW_NATIVE_EXCEPT("This val is not object", HEADER_EXCEPTION::VAL_IS_NOT_OBJECT);
	}

	FORCEINLINE INSIDE_DATA operator()(LPEXECUTE_CONTEXT Context, LPARG_FUNC Arg) HAS_THROW
	{	
		if(IsObject)
		{
			INSIDE_DATA NullVal;
			NullVal.SetNull();
			return Object.Prototype->OperatorCall(Object, &NullVal, Context, Arg);
		}
		THROW_NATIVE_EXCEPT("This val is not function", HEADER_EXCEPTION::VAL_IS_NOT_FUNCTION);		
	}

	FORCEINLINE INSIDE_DATA operator()(LPEXECUTE_CONTEXT Context, LPINSIDE_DATA ThisScope, LPARG_FUNC Arg) HAS_THROW
	{	
		if(IsObject)
			return Object.Prototype->OperatorCall(Object, ThisScope, Context, Arg);
		THROW_NATIVE_EXCEPT("This val is not function", HEADER_EXCEPTION::VAL_IS_NOT_FUNCTION);		
	}

	FORCEINLINE void In(LPINSIDE_DATA Var)
	{
		if(IsObject)
			Object.In(Var);
		THROW_NATIVE_EXCEPT("This val is not object", HEADER_EXCEPTION::VAL_IS_NOT_OBJECT);
	}
};

typedef _INLINE_OPERATORS<INSIDE_DATA> INLINE_INSIDE_DATA, *LPINLINE_INSIDE_DATA;

#endif