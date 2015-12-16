
#include <stdio.h>
#include "ConstScope.h"

INSIDE_DATA CONST_SCOPE_CLASS::GetElement(OBJECT & Member)
{
	auto Cell = NodeTable.Search(&Member);
	if(Cell != nullptr)
		return Cell->Key;
	return INSIDE_DATA::Null;
}

void CONST_SCOPE_CLASS::WriteElement(OBJECT & Member)
{		
	if(!NODE_TABLE_HEADER::ResizeBeforeInsert(NodeTable))
		THROW_UNHANDLED_EXCEPTION("Not alloc memory for const scope element.", UNHANDLED_EXCEPTION::NOT_ALLOC_MEMORY);
	NodeTable.Insert(&Member);
}

void CONST_SCOPE_CLASS::WriteElement(LPHEADER_STRING String) { WriteElement((OBJECT&)OBJECT::New(StringClass, String)); }

INSIDE_DATA CONST_SCOPE_CLASS::ReadMember(INSTANCE_CLASS Obj, const LPINSIDE_DATA Data)
{
	if(Data->IsObject)
		return GetElement(Data->Object);
	return INSIDE_DATA::Null;
}

void CONST_SCOPE_CLASS::MarkAsUsed(INSTANCE_CLASS)
{
	NodeTable.EnumValues(
		[](CONST_SCOPE_CLASS::_HASH_ELEMENT* HashElem)
		{
			HashElem->Key.MarkAsUsed();
			return true;
		});
}

void CONST_SCOPE_CLASS::WriteMember(INSTANCE_CLASS, const LPINSIDE_DATA Member, const LPINSIDE_DATA)
{
	if(Member->IsObject)
		WriteElement(Member->Object);
}


ZELLI_INTEGER CONST_SCOPE_CLASS::GetLength(INSTANCE_CLASS Obj) { return NodeTable.CountUsed; }

void CONST_SCOPE_CLASS::EnumKey(INSTANCE_CLASS Obj, LPINSIDE_DATA CurKey)
{
	NODE_TABLE_HEADER::LPCELL Cell;
	if(CurKey->IsNotNull)
		Cell = NodeTable.GetNextCellByKey(CurKey->Object);
	else
		Cell = NodeTable.GetStartCell();	
	if(Cell != nullptr)
	{
		*CurKey = Cell->Key;
		return;
	}
	CurKey->SetNull();
}

bool CONST_SCOPE_CLASS::OperatorEq(INSTANCE_CLASS Obj, LPINSIDE_DATA SecondObj)
{
	if(SecondObj->IsObject)
		return Obj == SecondObj->Object;
	return false;
}

void CONST_SCOPE_CLASS::RemoveElement(LPOBJECT Member)
{
	NodeTable.Remove(Member);
	if(!NODE_TABLE_HEADER::ResizeAfterRemove(NodeTable))
		THROW_UNHANDLED_EXCEPTION("Not alloc memory for global scope variable.", UNHANDLED_EXCEPTION::NOT_ALLOC_MEMORY);
}

void CONST_SCOPE_CLASS::RemoveElement(INSTANCE_CLASS Obj, LPINSIDE_DATA Member)
{
	if(Member->IsObject)
		RemoveElement(Member->Object);
}

INSIDE_DATA CONST_SCOPE_CLASS::CreateInstance(LPEXECUTE_CONTEXT,LPARG_FUNC)
{
	INSIDE_DATA NewInstance;
	NewInstance.TypeData = INSIDE_DATA::TYPEDATA_OBJECT;
	NewInstance.Object = Instance;
	return NewInstance;
}


SIZE_STR CONST_SCOPE_CLASS::InfoClass(LPINTERNAL_CHAR Buffer, SIZE_STR LenInBuf)
{
	SIZE_STR CurLen = LenInBuf, Len2;
	LPINTERNAL_CHAR Cur = Buffer;
	INSIDE_DATA Key = INSIDE_DATA::Null;
	Len2 = toz(sprintf_s(Buffer, CurLen, "%.*s\n", Name->Len, Name->Str));
	CurLen -= Len2;
	Buffer += Len2;
	Len2 = NodeTable.QualityInfo(Buffer, CurLen);
	CurLen -= Len2;
	Buffer += Len2;
	Len2 = toz(sprintf_s(Buffer, CurLen, "\nAll elements:\n"));
	CurLen -= Len2;
	Buffer += Len2;
	while(CurLen > 0)
	{
		EnumKey(&Instance, &Key);
		if(Key.IsNull)
			break;
		Len2 = Key.Info(Buffer, CurLen);
		CurLen -= Len2;
		Buffer += Len2;
		Len2 = toz(sprintf_s(Buffer, CurLen, "\n"));
		CurLen -= Len2;
		Buffer += Len2;
	}
	return LenInBuf - CurLen;
}