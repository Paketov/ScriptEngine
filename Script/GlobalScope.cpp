#include "GlobalScope.h"
#include "String.h"


void GLOBAL_SCOPE_CLASS::MarkClassAsUsed()
{
		NodeTable.EnumValues(
		[](void* StringClass, HASH_ELEMENT* Element)
		{
			GC_MARK_VAR(Element->Val);
			((LPSTRING_CLASS)StringClass)->MarkAsUsed(Element->Key);
			return true;
		}, StringClass);
}


bool GLOBAL_SCOPE_CLASS::OperatorEq(INSTANCE_CLASS ThisObj, const LPINSIDE_DATA SecondObj)
{
	if(SecondObj->IsObject)
		return ThisObj == SecondObj->Object;
	return false;
}

INSIDE_DATA GLOBAL_SCOPE_CLASS::ReadMember(INSTANCE_CLASS Object, const LPINSIDE_DATA Member)
{
	if(Member->IsString)
	{
		auto Cell = NodeTable.Search((LPHEADER_STRING)Member->Object);
		if(Cell != nullptr)
			return Cell->Val;
	}
	return INSIDE_DATA::Null;
}

void GLOBAL_SCOPE_CLASS::WriteMember(INSTANCE_CLASS Object, const LPINSIDE_DATA Member, const LPINSIDE_DATA Source)
{
	if(Member->IsString)
	{
		if(!NODE_TABLE_HEADER::ResizeBeforeInsert(NodeTable))
			THROW_UNHANDLED_EXCEPTION("GLOBAL_SCOPE_CLASS: Not realloc table.", UNHANDLED_EXCEPTION::NOT_REALLOC);
		NodeTable.Insert((LPHEADER_STRING)Member->Object)->Val = Source;
	}else
		THROW_NATIVE_EXCEPT("Not get from global scope by not string value.", HEADER_EXCEPTION::BAD_TYPE_ARG);
}

void GLOBAL_SCOPE_CLASS::RemoveMember(INSTANCE_CLASS Object, const LPINSIDE_DATA Member)
{
	if(Member->IsString)
	{
		NodeTable.Remove((LPHEADER_STRING)Member->Object);
		if(!NODE_TABLE_HEADER::ResizeAfterRemove(NodeTable))
			THROW_UNHANDLED_EXCEPTION("GLOBAL_SCOPE_CLASS: Not realloc table.", UNHANDLED_EXCEPTION::NOT_REALLOC);
	}else
		THROW_NATIVE_EXCEPT("GLOBAL_SCOPE_CLASS: Not remove by by not string value.", HEADER_EXCEPTION::BAD_TYPE_ARG);
}

	// = Object[MemberIndex]
INSIDE_DATA GLOBAL_SCOPE_CLASS::OperatorReadByIndex(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex)
{
	return ReadMember(Object, MemberIndex);
}

	//Object[MemberIndex] = Source
void GLOBAL_SCOPE_CLASS::OperatorWriteByIndex(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex, const LPINSIDE_DATA Source)
{
	WriteMember(Object, MemberIndex, Source);
}

	//delete Object[MemberIndex]
void GLOBAL_SCOPE_CLASS::OperatorRemoveByIndex(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex)
{
	RemoveMember(Object, MemberIndex);
}

ZELLI_INTEGER GLOBAL_SCOPE_CLASS::GetLength(INSTANCE_CLASS Object) { return NodeTable.CountUsed; }


void GLOBAL_SCOPE_CLASS::EnumKey(INSTANCE_CLASS Object, LPINSIDE_DATA CurKey)
{
	NODE_TABLE_HEADER::LPCELL Cell;
	if(CurKey->IsString)
		Cell = NodeTable.GetNextCellByKey((LPHEADER_STRING)CurKey->Object);
	else
		Cell = NodeTable.GetStartCell();	
	if(Cell != nullptr)
	{
		*CurKey = Cell->Key;
		return;
	}
	CurKey->SetNull();
}

INSIDE_DATA GLOBAL_SCOPE_CLASS::CreateInstance(LPEXECUTE_CONTEXT Context, LPARG_FUNC Arg)
{
	INSIDE_DATA NewInstance;
	NewInstance.TypeData = INSIDE_DATA::TYPEDATA_OBJECT;
	NewInstance.Object = Instance;
	return NewInstance;
}

GLOBAL_SCOPE_CLASS::GLOBAL_SCOPE_CLASS(LPSTRING_CLASS ListStrings) : HEADER_CLASS(this, "__GlobalScope", ListStrings)
{
	StringClass = ListStrings;
	if(!NODE_TABLE_HEADER::New(NodeTable, 15))
		THROW_UNHANDLED_EXCEPTION("GLOBAL_SCOPE: Not alloc memory.", UNHANDLED_EXCEPTION::NOT_ALLOC_MEMORY);
	NodeTable.Init(15);
	Instance.Prototype = this;
}
inline GLOBAL_SCOPE_CLASS::INDEX_OBJ_ELEMENT GLOBAL_SCOPE_CLASS::HASH_ELEMENT::IndexByKey(LPHEADER_STRING k, INDEX_OBJ_ELEMENT MaxCount) { return k->Key % MaxCount; }
bool  GLOBAL_SCOPE_CLASS::HASH_ELEMENT::CmpKey(LPHEADER_STRING k) { return *Key == *k; }

SIZE_STR GLOBAL_SCOPE_CLASS::InfoClass(LPINTERNAL_CHAR Buffer, SIZE_STR LenInBuf)
{

	SIZE_STR CurLen = LenInBuf, Len2;
	LPINTERNAL_CHAR Cur = Buffer;
	INSIDE_DATA Key = INSIDE_DATA::Null, Data;
	Len2 = toz(sprintf_s(Buffer, CurLen, "%.*s\n", ((LPHEADER_STRING)Name)->Len, ((LPHEADER_STRING)Name)->Str));
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
		EnumKey(Instance, &Key);
		if(Key.IsNull)
			break;

		Len2 = Key.Info(Buffer, CurLen);
		CurLen -= Len2;
		Buffer += Len2;

		Len2 = toz(sprintf_s(Buffer, CurLen, " -> "));
		CurLen -= Len2;
		Buffer += Len2;

		Data = ReadMember(Instance, &Key);
		Len2 = Data.Info(Buffer, CurLen);
		CurLen -= Len2;
		Buffer += Len2;

		Len2 = toz(sprintf_s(Buffer, CurLen, "\n"));
		CurLen -= Len2;
		Buffer += Len2;
	}

	return LenInBuf - CurLen;
}