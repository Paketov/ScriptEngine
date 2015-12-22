
#include "ExeptionHandler.h"
#include "String.h"
#include "Context.h"


void EXEPTION_CLASS::MarkClassAsUsed()
{
	propId.MarkAsUsed();
	propDescription.MarkAsUsed();
#ifdef _DEBUG
	PropCppFile.MarkAsUsed();
	PropLine.MarkAsUsed();
#endif
}

EXEPTION_CLASS::EXEPTION_CLASS(LPSTRING_CLASS StrClass) : HEADER_CLASS(this, "Exception", StrClass)
{
	propId = OBJECT::New(StrClass, StrClass->RegisterString("Id"));
	propDescription = OBJECT::New(StrClass, StrClass->RegisterString("Description"));
#ifdef _DEBUG
	PropCppFile = OBJECT::New(StrClass, StrClass->RegisterString("CppFile"));
	PropLine = OBJECT::New(StrClass, StrClass->RegisterString("Line"));
#endif
}

INSIDE_DATA EXEPTION_CLASS::ReadMember(INSTANCE_CLASS Object, const LPINSIDE_DATA Member)
{
	if(Member->IsString)
	{
		LPHEADER_STRING MemberStr = Member->Object;
		if(*MemberStr == *((LPHEADER_STRING)propId))
			return ((LPHEADER_EXCEPTION)Object)->ThrowId;
		else if(*MemberStr == *(LPHEADER_STRING)propDescription)
			return EXECUTE_CONTEXT::ForThisThread->StringClass->RegisterCommonString(((LPHEADER_EXCEPTION)Object)->Description);
#ifdef _DEBUG
		else if(*MemberStr == *(LPHEADER_STRING)PropCppFile)
			return EXECUTE_CONTEXT::ForThisThread->StringClass->RegisterCommonString(((LPHEADER_EXCEPTION)Object)->CppFile);
		else if(*MemberStr == *(LPHEADER_STRING)PropLine)
			return ((LPHEADER_EXCEPTION)Object)->Line;
#endif
	}
	return INSIDE_DATA::Null;
}


void EXEPTION_CLASS::WriteMember(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex, const LPINSIDE_DATA Source)
{	
	THROW_NATIVE_EXCEPT("EXEPTION_CLASS: Not write in const object", HEADER_EXCEPTION::NOT_WRITE_IN_CONST_OBJECT);
};

void EXEPTION_CLASS::RemoveMember(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex)
{
	THROW_NATIVE_EXCEPT("EXEPTION_CLASS: Not remove in const object", HEADER_EXCEPTION::NOT_REMOVE_FROM_CONST_OBJ);
}

void EXEPTION_CLASS::EnumKey(INSTANCE_CLASS Object, LPINSIDE_DATA CurKey)
{
	if(CurKey->IsString)
	{
		if(*(LPHEADER_STRING)CurKey->Object == *(LPHEADER_STRING)propId)
		{
			*CurKey = EXECUTE_CONTEXT::ForThisThread->StringClass->RegisterCommonString(propDescription);
			return;
		}
#ifdef _DEBUG
		else if(*(LPHEADER_STRING)CurKey->Object == *(LPHEADER_STRING)propDescription)
		{
			*CurKey = EXECUTE_CONTEXT::ForThisThread->StringClass->RegisterCommonString(PropCppFile);
			return;
		}
		else if(*(LPHEADER_STRING)CurKey->Object == *(LPHEADER_STRING)PropCppFile)
		{
			*CurKey = EXECUTE_CONTEXT::ForThisThread->StringClass->RegisterCommonString(PropLine);
			return;
		}else if(*(LPHEADER_STRING)CurKey->Object == *(LPHEADER_STRING)PropLine)
		{
			CurKey->SetNull();
			return;
		}
#else
		else if(*(LPHEADER_STRING)CurKey->Object == *propDescription)
		{
			CurKey->SetNull();
			return;
		}
#endif
	}
	*CurKey = EXECUTE_CONTEXT::ForThisThread->StringClass->RegisterCommonString(propId);
}

ZELLI_INTEGER EXEPTION_CLASS::OperatorToInt(INSTANCE_CLASS Object) { return 1; }

ZELLI_DOUBLE EXEPTION_CLASS::OperatorToDouble(INSTANCE_CLASS Object) { return 1.0; }

INSIDE_DATA EXEPTION_CLASS::CreateInstance(LPEXECUTE_CONTEXT Context,LPARG_FUNC Arg) { return INSIDE_DATA::Null; }

ZELLI_INTEGER EXEPTION_CLASS::GetLength(INSTANCE_CLASS Object) { return 4; }

bool EXEPTION_CLASS::OperatorEq(INSTANCE_CLASS ThisObj, const LPINSIDE_DATA SecondObj)
{
	if(SecondObj->IsExceptionHandler)
		return SecondObj->Object.Object == ThisObj;
	return false;
}

SIZE_STR EXEPTION_CLASS::InfoObject(INSTANCE_CLASS Obj, LPINTERNAL_CHAR Buffer, SIZE_STR LenInBuf)
{
	LPHEADER_EXCEPTION Hs = (LPHEADER_EXCEPTION)Obj;
	LPINTERNAL_CHAR Cur = Buffer;
	SIZE_STR CurLen = LenInBuf, Len2 = toz(sprintf_s(Buffer, LenInBuf, "EXCEPTION: \"%.*s\" ID: %u", Hs->Description->Len, Hs->Description->Str, Hs->ThrowId));
	CurLen -= Len2;
	Buffer += Len2;
#ifdef _DEBUG
	Len2 = toz(sprintf_s(Buffer, LenInBuf, "CPP_FILE: \"%.*s\" LINE: %u", Hs->CppFile->Len, Hs->CppFile->Str, Hs->Line));
	CurLen -= Len2;
	Buffer += Len2;
#endif
	return LenInBuf - CurLen;
}
