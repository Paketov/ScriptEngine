#include "NativeMethod.h"
#include "String.h"
#include <stdio.h>


bool NATIVEMETHOD_CLASS::OperatorEq(INSTANCE_CLASS Instance, const LPINSIDE_DATA Enother)
{
	if(Enother->IsObject)
		return (Enother->Object.Prototype == this) && (Enother->Object == Instance);
	return false;
}

bool NATIVEMETHOD_CLASS::OperatorNeq(INSTANCE_CLASS Instance, LPINSIDE_DATA Enother)
{
	if(Enother->IsObject)
		return (Enother->Object.Prototype != this) || (Enother->Object != Instance);
	return true;
}


ZELLI_INTEGER NATIVEMETHOD_CLASS::OperatorToInt(INSTANCE_CLASS)
{
	return 1;
}

ZELLI_DOUBLE NATIVEMETHOD_CLASS::OperatorToDouble(INSTANCE_CLASS)
{
	return 1.0;
}

INSIDE_DATA NATIVEMETHOD_CLASS::OperatorCall(INSTANCE_CLASS Instance, LPINSIDE_DATA This, LPEXECUTE_CONTEXT CurContext, LPARG_FUNC Arg)
{
	return ((NATIVE_FUNCTION)Instance)(This, CurContext, Arg);
}

SIZE_STR NATIVEMETHOD_CLASS::InfoObject(INSTANCE_CLASS Obj, LPINTERNAL_CHAR Buffer, SIZE_STR LenInBuf)
{
	return toz(sprintf_s(Buffer, LenInBuf, "NATIVE_METHOD"));
}


NATIVEMETHOD_CLASS::NATIVEMETHOD_CLASS(LPSTRING_CLASS StrClass)
{
	Name = StrClass->RegisterString("__NativeMethod");
}