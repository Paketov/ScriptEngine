#include "ObjectClass.h"
#include "String.h"
#include "Method.h"




// = Object.MemberIndex
inline INSIDE_DATA OBJECT_CLASS::ReadMember(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex)
{
	auto Cell = ((LPOBJECT_HEADER)Object)->NodeTable.Search(MemberIndex);
	if(Cell != nullptr)
		return Cell->Val;
	return INSIDE_DATA::Null;
}

//Object.MemberIndex = Source
void OBJECT_CLASS::WriteMember(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex, const LPINSIDE_DATA Source)
{
	NODE_TABLE_HEADER& NodeTable = ((LPOBJECT_HEADER)Object)->NodeTable;
	if(!NODE_TABLE_HEADER::ResizeBeforeInsert(NodeTable))
		THROW_UNHANDLED_EXCEPTION("OBJECT_CLASS: Not realloc table.", UNHANDLED_EXCEPTION::NOT_REALLOC);
	NodeTable.Insert(MemberIndex)->Val = *Source;
}

//delete Object.MemberIndex
void OBJECT_CLASS::RemoveMember(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex)
{
	NODE_TABLE_HEADER& NodeTable = ((LPOBJECT_HEADER)Object)->NodeTable;
	NodeTable.Remove(MemberIndex);
	if(!NODE_TABLE_HEADER::ResizeAfterRemove(NodeTable))
		THROW_UNHANDLED_EXCEPTION("OBJECT_CLASS: Not realloc table.", UNHANDLED_EXCEPTION::NOT_REALLOC);
}

// = Object[MemberIndex]
INSIDE_DATA OBJECT_CLASS::OperatorReadByIndex(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex)
{
	auto Cell = ((LPOBJECT_HEADER)Object)->NodeTable.Search(MemberIndex);
	if(Cell != nullptr)
		return Cell->Val;
	return INSIDE_DATA::Null;
}

INSIDE_DATA OBJECT_CLASS::OperatorReadByIndex(INSTANCE_CLASS Object, ZELLI_INTEGER MemberIndex)
{
	auto Cell = ((LPOBJECT_HEADER)Object)->NodeTable.Search(MemberIndex);
	if(Cell != nullptr)
		return Cell->Val;
	return INSIDE_DATA::Null;
}

//Object[MemberIndex] = Source
void OBJECT_CLASS::OperatorWriteByIndex(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex, const LPINSIDE_DATA Source)
{
	NODE_TABLE_HEADER& NodeTable = ((LPOBJECT_HEADER)Object)->NodeTable;
	if(!NODE_TABLE_HEADER::ResizeBeforeInsert(NodeTable))
		THROW_UNHANDLED_EXCEPTION("OBJECT_CLASS: Not realloc table.", UNHANDLED_EXCEPTION::NOT_REALLOC);
	NodeTable.Insert(MemberIndex)->Val = *Source;
}

void OBJECT_CLASS::OperatorWriteByIndex(INSTANCE_CLASS Object, ZELLI_INTEGER MemberIndex, const LPINSIDE_DATA Source)
{
	NODE_TABLE_HEADER& NodeTable = ((LPOBJECT_HEADER)Object)->NodeTable;
	if(!NODE_TABLE_HEADER::ResizeBeforeInsert(NodeTable))
		THROW_UNHANDLED_EXCEPTION("OBJECT_CLASS: Not realloc table.", UNHANDLED_EXCEPTION::NOT_REALLOC);
	NodeTable.Insert(MemberIndex)->Val = *Source;
}

//delete Object[MemberIndex]
void OBJECT_CLASS::OperatorRemoveByIndex(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex)
{
	NODE_TABLE_HEADER& NodeTable = ((LPOBJECT_HEADER)Object)->NodeTable;
	NodeTable.Remove(MemberIndex);
	if(!NODE_TABLE_HEADER::ResizeAfterRemove(NodeTable))
		THROW_UNHANDLED_EXCEPTION("OBJECT_CLASS: Not realloc table.", UNHANDLED_EXCEPTION::NOT_REALLOC);
}

void OBJECT_CLASS::OperatorRemoveByIndex(INSTANCE_CLASS Object, ZELLI_INTEGER MemberIndex)
{
	NODE_TABLE_HEADER& NodeTable = ((LPOBJECT_HEADER)Object)->NodeTable;
	NodeTable.Remove(MemberIndex);
	if(!NODE_TABLE_HEADER::ResizeAfterRemove(NodeTable))
		THROW_UNHANDLED_EXCEPTION("OBJECT_CLASS: Not realloc table.", UNHANDLED_EXCEPTION::NOT_REALLOC);
}

ZELLI_INTEGER OBJECT_CLASS::GetLength(INSTANCE_CLASS Object) { return ((LPOBJECT_HEADER)Object)->NodeTable.CountUsed; }

void OBJECT_CLASS::MarkInstanceAsUsed(INSTANCE_CLASS Object)
{
	LPOBJECT_HEADER Obj = ((LPOBJECT_HEADER)Object);
	if(Obj->ForCheckUses == CurCheckUses)
		return;
	Obj->ForCheckUses = CurCheckUses;
	LIST_HEADER<OBJECT_HEADER>::MoveToListRight(Obj, &ListObject);
	Obj->NodeTable.EnumValues
	(
		[](HASH_ELEMENT* El)
		{
			GC_MARK_VAR(El->Key);
			GC_MARK_VAR(El->Val);
			return true;
		}
	);
}

void OBJECT_CLASS::MarkClassAsUsed(INSTANCE_CLASS Object)
{
}

void OBJECT_CLASS::FreeAllUnused()
{
	SwitchToAddMode();
	LPOBJECT_HEADER ListUnusedObject = &this->ListUnusedObject;

	for(LPOBJECT_HEADER CurElem = (LPOBJECT_HEADER)ListUnusedObject->Next;ListUnusedObject != CurElem;)
	{
		NODE_TABLE_HEADER::Free(CurElem->NodeTable);
		CurElem = (LPOBJECT_HEADER)CurElem->Next;
		AddInQueueHeader(this, CurElem->Previous);
	}
	LIST_HEADER<OBJECT_HEADER>::Close(ListUnusedObject);	
}

LPOBJECT_HEADER __fastcall OBJECT_CLASS::__AllocHeader(LPOBJECT_CLASS This)
{
	LPOBJECT_HEADER NewHeader = (LPOBJECT_HEADER)MEM_ALLOC(sizeof(OBJECT_HEADER));
	if(NewHeader == NULL)
		THROW_UNHANDLED_EXCEPTION("Not alloc memory for new object.", UNHANDLED_EXCEPTION::NOT_ALLOC_MEMORY);
	return NewHeader;
}

OBJECT_CLASS::OBJECT_CLASS(LPSTRING_CLASS ListStrings): HEADER_CLASS(this, "Object", ListStrings)
{
	CurCheckUses = 0;
	LIST_HEADER<OBJECT_HEADER>::Close(&ListObject);
	LIST_HEADER<OBJECT_HEADER>::Close(&ListUnusedObject);
	HeaderObjQueue = nullptr;
	CountHeaderInQueue = 0;
	MaxCountHeaderInQueue = 100;
	SwitchToAddMode();
}

void OBJECT_CLASS::EnumKey(INSTANCE_CLASS Object, LPINSIDE_DATA CurKey)
{
	OBJECT_CLASS::NODE_TABLE_HEADER& NodeTable = ((LPOBJECT_HEADER)Object)->NodeTable;
	NODE_TABLE_HEADER::LPCELL Cell;
	if(CurKey->IsNotNull)
		Cell = NodeTable.GetNextCellByKey(CurKey);
	else
		Cell = NodeTable.GetStartCell();
	if(Cell != nullptr)
	{
		*CurKey = Cell->Key;
		return;
	}
	CurKey->SetNull();
}

INSIDE_DATA OBJECT_CLASS::CreateInstance(LPEXECUTE_CONTEXT Context,LPARG_FUNC Arg)
{
	LPOBJECT_HEADER NewHeader = GetHeaderFromQueue(this);
	LIST_HEADER<OBJECT_HEADER>::AddToRight(&ListObject, NewHeader);
	NewHeader->ForCheckUses = CurCheckUses;
	if(!NODE_TABLE_HEADER::Realloc(NewHeader->NodeTable, 5))
		THROW_UNHANDLED_EXCEPTION("OBJECT_CLASS: Not alloc table.", UNHANDLED_EXCEPTION::NOT_ALLOC_MEMORY);
	NewHeader->NodeTable.Init(5);
	return NewHeader;
}

LPOBJECT_HEADER __fastcall OBJECT_CLASS::__GetFromQueue(LPOBJECT_CLASS This)
{
	if(This->HeaderObjQueue)
	{
		LPOBJECT_HEADER NewHeader = This->HeaderObjQueue;
		This->HeaderObjQueue = *((LPOBJECT_HEADER*)NewHeader);
		This->CountHeaderInQueue--;
		return NewHeader;
	}
	This->GetHeaderFromQueue = __AllocHeader;
	return __AllocHeader(This);
}

void __fastcall OBJECT_CLASS::__AddInQueue(LPOBJECT_CLASS This, LPOBJECT_HEADER Header)
{	    
	if(This->CountHeaderInQueue < This->MaxCountHeaderInQueue)
	{		
		*((LPOBJECT_HEADER*)Header) = This->HeaderObjQueue;
		This->HeaderObjQueue = Header;
		This->CountHeaderInQueue++;
		return;
	}
	This->AddInQueueHeader = __DeleteObj;
	MEM_FREE(Header);
}

void __fastcall OBJECT_CLASS::__DeleteObj(LPOBJECT_CLASS This, LPOBJECT_HEADER Header) { MEM_FREE(Header); }

bool OBJECT_CLASS::OperatorEq(INSTANCE_CLASS ThisObj, LPINSIDE_DATA SecondObj)
{
	if(SecondObj->IsObject)
		return ThisObj == SecondObj->Object;
	return false;
}


HASH_VAL OBJECT_CLASS::GetHash(INSTANCE_CLASS Instance)
{
	return (HASH_VAL)Instance;
}