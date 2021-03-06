#include "TypeData.h"
#include "Object.h"
#include "String.h"
#include "AdditionalLib.h"
#include "Method.h"
#include "InlineTypeData.h"
#include "ExString.h"
#include "ExTypeTraits.h"

//Property


INSIDE_DATA::__PROP_IS_STRING::operator bool()
{
   return (TypeData == TYPEDATA_OBJECT) && (Object.Prototype->IdClass == STRING_CLASS::Id);
}

INSIDE_DATA::__GET_HASH::operator HASH_VAL()
{
	switch(TypeData)
	{	
	case TYPEDATA_INTEGER:
		return ZELLI_INTEGER_HASH(Integer);
	case TYPEDATA_OBJECT:
		return Object.Hash;
	case TYPEDATA_NULL:
	case TYPEDATA_EXCEPT:
		return 0;
	default:
		return ZELLI_DOUBLE_HASH(Double);
	}
}

INSIDE_DATA::__ISZERO_PROP_DEF::operator bool() { return ((LPINSIDE_DATA)this)->operator ==(INSIDE_DATA::Zero); }

INSIDE_DATA::__LEN_PROP_DEF::operator ZELLI_INTEGER()
{
	switch(TypeData)
	{	
	case TYPEDATA_NULL:
		return 0;
	case TYPEDATA_OBJECT:
		return Object.Length;
	}
	return 1;
}

INSIDE_DATA::__ISSIGNED_PROP_DEF::operator bool()
{
	switch(TypeData)
	{	
	case TYPEDATA_INTEGER:
		return Integer > 0;
	case TYPEDATA_OBJECT:
		return Object.operator>(INSIDE_DATA::Zero);
	case TYPEDATA_NULL:
	case TYPEDATA_EXCEPT:
		return false;
	default:
		return Double > 0.0;
	}
}


INSIDE_DATA::__QUERY_MEMBER::operator INSIDE_DATA()
{
	if(Obj->IsObject)
		return Obj->Object.Prototype->OperatorReadByIndex(Obj->Object, (LPINSIDE_DATA)MemberIndex);
	THROW_NATIVE_EXCEPT("This val is not object", HEADER_EXCEPTION::VAL_IS_NOT_OBJECT);
}

INSIDE_DATA INSIDE_DATA::__QUERY_MEMBER::operator=(INSIDE_DATA Val)
{
	if(Obj->IsObject)
	{
		Obj->Object.Prototype->OperatorWriteByIndex(Obj->Object, (LPINSIDE_DATA)MemberIndex, &Val);
		return Val;
	}
	THROW_NATIVE_EXCEPT("This val is not object", HEADER_EXCEPTION::VAL_IS_NOT_OBJECT);
}

INSIDE_DATA::__QUERY_MEMBER INSIDE_DATA::__QUERY_MEMBER::operator [](INSIDE_DATA Member)
{
	INSIDE_DATA t = operator INSIDE_DATA();
	return  __QUERY_MEMBER(&t, &Member);
}

INSIDE_DATA::__QUERY_MEMBER_BY_INDEX INSIDE_DATA::__QUERY_MEMBER::operator [](ZELLI_INTEGER Member)
{
	INSIDE_DATA t = operator INSIDE_DATA();
	return  __QUERY_MEMBER_BY_INDEX(&t, Member);
}

INSIDE_DATA::__QUERY_MEMBER_BY_INDEX::operator INSIDE_DATA()
{
	if(Obj->IsObject)
		return Obj->Object.Prototype->OperatorReadByIndex(Obj->Object, MemberIndex);
	THROW_NATIVE_EXCEPT("This val is not object", HEADER_EXCEPTION::VAL_IS_NOT_OBJECT);
}

INSIDE_DATA INSIDE_DATA::__QUERY_MEMBER_BY_INDEX::operator=(INSIDE_DATA Val)
{
	if(Obj->IsObject)
	{
		Obj->Object.Prototype->OperatorWriteByIndex(Obj->Object, MemberIndex, &Val);
		return Val;
	}
	THROW_NATIVE_EXCEPT("This val is not object", HEADER_EXCEPTION::VAL_IS_NOT_OBJECT);
}

INSIDE_DATA::__QUERY_MEMBER INSIDE_DATA::__QUERY_MEMBER_BY_INDEX::operator [](INSIDE_DATA Member)
{
	INSIDE_DATA t = operator INSIDE_DATA();
	return  INSIDE_DATA::__QUERY_MEMBER(&t, &Member);
}

INSIDE_DATA::__QUERY_MEMBER_BY_INDEX INSIDE_DATA::__QUERY_MEMBER_BY_INDEX::operator [](ZELLI_INTEGER Member)
{
	INSIDE_DATA t = operator INSIDE_DATA();
	return  __QUERY_MEMBER_BY_INDEX(&t, Member);
}

INSIDE_DATA::__GETCLASS_PROP_DEF::operator LPHEADER_CLASS() { return Object.Prototype; }

LPHEADER_CLASS INSIDE_DATA::__GETCLASS_PROP_DEF::operator ->() { return Object.Prototype; }


void INSIDE_DATA::MarkAsUsed()
{
	if(IsObject)
		Object.Prototype->MarkAsUsed(Object);
}

//Methods
INSIDE_DATA::operator bool() { return ((LPINLINE_INSIDE_DATA)this)->operator bool(); }

INSIDE_DATA::operator ZELLI_INTEGER() { return ((LPINLINE_INSIDE_DATA)this)->operator ZELLI_INTEGER(); }

INSIDE_DATA::operator ZELLI_DOUBLE() { return ((LPINLINE_INSIDE_DATA)this)->operator ZELLI_DOUBLE(); }

INSIDE_DATA::INSIDE_DATA(const OBJECT Val)
{
	TypeData = TYPEDATA_OBJECT;
	Object = Val;
}

INSIDE_DATA::INSIDE_DATA(const LPOBJECT Val)
{
	TypeData = TYPEDATA_OBJECT;
	Object = *Val;
}

INSIDE_DATA::INSIDE_DATA(const INSTANCE_CLASS Obj, const LPHEADER_CLASS Cla)
{
	TypeData = TYPEDATA_OBJECT;
	Object.Prototype = Cla;
	Object.Object = Obj;
}

INSIDE_DATA::INSIDE_DATA(const NATIVE_FUNCTION NativeMethod)
{
	TypeData = TYPEDATA_OBJECT;
	Object.Prototype = &EXECUTE_CONTEXT::NativeMethodClass;
	Object.Object = NativeMethod;
}

bool INSIDE_DATA::operator ==(INSIDE_DATA & CompareData) { return ((LPINLINE_INSIDE_DATA)this)->operator==(CompareData); }

bool INSIDE_DATA::operator !=(INSIDE_DATA & CompareData) { return ((LPINLINE_INSIDE_DATA)this)->operator==(CompareData); }

bool INSIDE_DATA::operator >(INSIDE_DATA & CompareData) { return ((LPINLINE_INSIDE_DATA)this)->operator&(CompareData); }

bool INSIDE_DATA::operator >=(INSIDE_DATA & CompareData) { return ((LPINLINE_INSIDE_DATA)this)->operator>=(CompareData); }

bool INSIDE_DATA::operator <(INSIDE_DATA & CompareData) { return ((LPINLINE_INSIDE_DATA)this)->operator<(CompareData); }

bool INSIDE_DATA::operator <=(INSIDE_DATA & CompareData) { return ((LPINLINE_INSIDE_DATA)this)->operator<=(CompareData); }

void INSIDE_DATA::operator ++() { return ((LPINLINE_INSIDE_DATA)this)->operator++(); }

void INSIDE_DATA::operator --() { return ((LPINLINE_INSIDE_DATA)this)->operator--(); }

INSIDE_DATA INSIDE_DATA::operator &(INSIDE_DATA & Val2) { return ((LPINLINE_INSIDE_DATA)this)->operator&(Val2); }

INSIDE_DATA INSIDE_DATA::operator |(INSIDE_DATA & Val2) { return ((LPINLINE_INSIDE_DATA)this)->operator|(Val2); }

INSIDE_DATA INSIDE_DATA::operator ^(INSIDE_DATA & Val2) { return ((LPINLINE_INSIDE_DATA)this)->operator^(Val2); }

INSIDE_DATA INSIDE_DATA::operator <<(INSIDE_DATA & Val2) { return ((LPINLINE_INSIDE_DATA)this)->operator<<(Val2); }

INSIDE_DATA INSIDE_DATA::operator >>(INSIDE_DATA & Val2) { return ((LPINLINE_INSIDE_DATA)this)->operator>>(Val2); }

INSIDE_DATA INSIDE_DATA::operator +(INSIDE_DATA & Val2) { return ((LPINLINE_INSIDE_DATA)this)->operator+(Val2); }

INSIDE_DATA INSIDE_DATA::operator -(INSIDE_DATA & Val2) { return ((LPINLINE_INSIDE_DATA)this)->operator-(Val2); }

INSIDE_DATA INSIDE_DATA::operator *(INSIDE_DATA & Val2) { return ((LPINLINE_INSIDE_DATA)this)->operator*(Val2); }

INSIDE_DATA INSIDE_DATA::operator /(INSIDE_DATA & Val2) { return ((LPINLINE_INSIDE_DATA)this)->operator/(Val2); }

INSIDE_DATA INSIDE_DATA::operator %(INSIDE_DATA & Val2) { return ((LPINLINE_INSIDE_DATA)this)->operator%(Val2); }

INSIDE_DATA INSIDE_DATA::operator -() { return ((LPINLINE_INSIDE_DATA)this)->operator-(); }

INSIDE_DATA INSIDE_DATA::operator ~() { return ((LPINLINE_INSIDE_DATA)this)->operator~(); }

INSIDE_DATA INSIDE_DATA::operator()(LPEXECUTE_CONTEXT Context, LPARG_FUNC Arg) HAS_THROW { return ((LPINLINE_INSIDE_DATA)this)->operator()(Context, Arg); }

INSIDE_DATA INSIDE_DATA::operator()(LPEXECUTE_CONTEXT Context, LPINSIDE_DATA ThisScope, LPARG_FUNC Arg) HAS_THROW { return ((LPINLINE_INSIDE_DATA)this)->operator()(Context, ThisScope, Arg); }

INSIDE_DATA INSIDE_DATA::operator()(LPINSIDE_DATA ThisScope, LPARG_FUNC Arg) HAS_THROW { return ((LPINLINE_INSIDE_DATA)this)->operator()(EXECUTE_CONTEXT::ForThisThread, Arg); }

INSIDE_DATA INSIDE_DATA::operator()(LPARG_FUNC Arg) HAS_THROW { return ((LPINLINE_INSIDE_DATA)this)->operator()(EXECUTE_CONTEXT::ForThisThread, Arg); }

void INSIDE_DATA::In(LPINSIDE_DATA Var) { return ((LPINLINE_INSIDE_DATA)this)->In(Var); }


OBJECT INSIDE_DATA::ToString(LPSTRING_CLASS BasicClass)
{
	switch(TypeData)
	{	
	case TYPEDATA_INTEGER:
		{
			INTERNAL_CHAR Buf[40];
			NumberToString(Integer, Buf);
			return OBJECT::New(BasicClass, BasicClass->RegisterString(Buf));
		}
	case TYPEDATA_OBJECT:
		return Object.ToString(BasicClass);
	case TYPEDATA_NULL:
		return OBJECT::New(BasicClass, BasicClass->RegisterString("null"));
	case TYPEDATA_EXCEPT:
		return OBJECT::New(BasicClass, BasicClass->RegisterString("[ExceptionHandler]"));
	default:
		{
			INTERNAL_CHAR Buf[40];
			NumberToString(Double, Buf);
			return OBJECT::New(BasicClass, BasicClass->RegisterString(Buf));
		}
	}
}

void INSIDE_DATA::ToString(std::basic_string<INTERNAL_CHAR>& Str)
{
	switch(TypeData)
	{	
	case TYPEDATA_INTEGER:
		NumberToString(Integer, Str);
		return;
	case TYPEDATA_OBJECT:
		Object.ToString(Str);
		return;
	case TYPEDATA_NULL:
		Str = "null";
		return;
	case TYPEDATA_EXCEPT:		
		Str = "[ExceptionHandler]";
		return;
	default:
		NumberToString(Double, Str);
		return;
	}
}

void INSIDE_DATA::ToString(INTERNAL_CHAR* StrBuf, TSIZE_STR BufLen)
{
	switch(TypeData)
	{	
	case TYPEDATA_INTEGER:
		NumberToString(Integer, StrBuf, BufLen);
		return;
	case TYPEDATA_OBJECT:
		Object.ToString(StrBuf, BufLen);
		return;
	case TYPEDATA_NULL:
		StringCopy(StrBuf, "null", BufLen);
		return;
	case TYPEDATA_EXCEPT:
		StringCopy(StrBuf, "[ExceptionHandler]", BufLen);
		return;
	default:
		NumberToString(Double, StrBuf, BufLen);
		return;
	}
}

OBJECT INSIDE_DATA::TypeOf(LPSTRING_CLASS BasicClass)
{
	switch(TypeData)
	{	
	case TYPEDATA_OBJECT:
		return Object.TypeOf(BasicClass);
	case TYPEDATA_NULL:
		return OBJECT::New(BasicClass, BasicClass->RegisterString("[Null]"));
	case TYPEDATA_EXCEPT:
		return OBJECT::New(BasicClass, BasicClass->RegisterString("[ExceptionHandler]"));
	case TYPEDATA_INTEGER:
	default:
		return OBJECT::New(BasicClass, BasicClass->RegisterString("[Number]"));
	}
}

void INSIDE_DATA::TypeOf(std::basic_string<INTERNAL_CHAR>& Str)
{
	switch(TypeData)
	{	
	case TYPEDATA_OBJECT:
		Object.TypeOf(Str);
		return;
	case TYPEDATA_NULL:
		Str = "[Null]";
		return;
	case TYPEDATA_EXCEPT:
		Str = "[ExceptionHandler]";
		return;
	case TYPEDATA_INTEGER:
	default:
		Str = "[Number]";
		return;
	}
}

void INSIDE_DATA::TypeOf(INTERNAL_CHAR* StrBuf, TSIZE_STR BufLen)
{
	INTERNAL_CHAR* s;
	switch(TypeData)
	{	
	case TYPEDATA_OBJECT:
		Object.TypeOf(StrBuf, BufLen);
		return;
	case TYPEDATA_NULL:
		s = "[Null]";
		break;
	case TYPEDATA_EXCEPT:
		s = "[ExceptionHandler]";
		break;
	case TYPEDATA_INTEGER:
	default:
		s = "[Number]";
	}
	StringCopy(StrBuf, s, BufLen);
}

unsigned INSIDE_DATA::TypeId()
{
	switch(TypeData)
	{	
	case TYPEDATA_INTEGER:
		return std::get_id_for_type<decltype(Integer)>();
	case TYPEDATA_OBJECT:
		return Object.Prototype->IdClass;
	case TYPEDATA_NULL:
		return std::get_id_for_type<std::empty_type>();
	case TYPEDATA_EXCEPT:
		{
			struct e{};
			return std::get_id_for_type<e>();
		}
	default:
		return std::get_id_for_type<decltype(Double)>();
	}
}


SIZE_STR INSIDE_DATA::Info(LPINTERNAL_CHAR Buffer, SIZE_STR Len)
{
	int r;
	switch(TypeData)
	{
	case INSIDE_DATA::TYPEDATA_NULL:
		r = sprintf_s(Buffer, Len, "NULL");
		break;
	case INSIDE_DATA::TYPEDATA_INTEGER:
		r = sprintf_s(Buffer, Len, "INTEGER: %i", Integer);
		break;
	case OBJECT_CASE:
		r = Object.Info(Buffer, Len);
		break;
	case INSIDE_DATA::TYPEDATA_EXCEPT:
		r = sprintf_s(Buffer, Len, "EXCEPTION_HANDLER");
		break;
	default:
		r = sprintf_s(Buffer, Len, "DOUBLE: %L", Double);
	}
	return (r > 0)?r:0;
}                                                                                                                                       