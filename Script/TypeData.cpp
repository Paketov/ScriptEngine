#include "TypeData.h"
#include "Object.h"
#include "String.h"
#include "AdditionalLib.h"
#include "Method.h"
#include "InlineTypeData.h"

//Property


INSIDE_DATA::__GET_HASH::operator HASH_VAL()
{
	switch(TypeData)
	{	
	case TYPEDATA_INTEGER:
		return ZELLI_INTEGER_HASH(Integer);
	case OBJECT_CASE:
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
	case OBJECT_CASE:
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
	case OBJECT_CASE:
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
		return Obj->Object.Prototype->OperatorReadByIndex(Obj, MemberIndex);
	THROW_NATIVE_EXCEPT("This val is not object", HEADER_EXCEPTION::VAL_IS_NOT_OBJECT);
}

INSIDE_DATA INSIDE_DATA::__QUERY_MEMBER::operator=(INSIDE_DATA Val)
{
	if(Obj->IsObject)
		return Obj->Object.Prototype->OperatorReadByIndex(Obj, MemberIndex);
	THROW_NATIVE_EXCEPT("This val is not object", HEADER_EXCEPTION::VAL_IS_NOT_OBJECT);
}

INSIDE_DATA::__QUERY_MEMBER INSIDE_DATA::__QUERY_MEMBER::operator [](INSIDE_DATA & Member)
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
		return Obj->Object.Prototype->OperatorReadByIndex(Obj, MemberIndex);
	THROW_NATIVE_EXCEPT("This val is not object", HEADER_EXCEPTION::VAL_IS_NOT_OBJECT);
}

INSIDE_DATA INSIDE_DATA::__QUERY_MEMBER_BY_INDEX::operator=(INSIDE_DATA Val)
{
	if(Obj->IsObject)
		return Obj->Object.Prototype->OperatorReadByIndex(Obj, MemberIndex);
	THROW_NATIVE_EXCEPT("This val is not object", HEADER_EXCEPTION::VAL_IS_NOT_OBJECT);
}

INSIDE_DATA::__QUERY_MEMBER INSIDE_DATA::__QUERY_MEMBER_BY_INDEX::operator [](INSIDE_DATA & Member)
{
	INSIDE_DATA t = operator INSIDE_DATA();
	return  __QUERY_MEMBER(&t, &Member);
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