#include "ClassScope.h"
#include "String.h"
#include "Method.h"
#include "Context.h"


// = Object.MemberIndex
INSIDE_DATA CLASS_SCOPE::ReadMember(INSTANCE_CLASS, const LPINSIDE_DATA MemberIndex)
{
	auto Cell = NodeTable.Search(MemberIndex);
	if(Cell == nullptr)
		return INSIDE_DATA::Null;
	return Cell->Val;
}

//Object.MemberIndex = Source
void CLASS_SCOPE::WriteMember(INSTANCE_CLASS, const LPINSIDE_DATA MemberIndex, const LPINSIDE_DATA Source)
{
	if(!NODE_TABLE_HEADER::ResizeBeforeInsert(NodeTable))
		THROW_UNHANDLED_EXCEPTION("CLASS_SCOPE: Not realloc table.", UNHANDLED_EXCEPTION::NOT_REALLOC);
	NodeTable.Insert(MemberIndex)->Val = *Source;
}

//delete Object.MemberIndex
void CLASS_SCOPE::RemoveMember(INSTANCE_CLASS, const LPINSIDE_DATA MemberIndex)
{
	NodeTable.Remove(MemberIndex);
	if(!NODE_TABLE_HEADER::ResizeAfterRemove(NodeTable))
		THROW_UNHANDLED_EXCEPTION("CLASS_SCOPE: Not realloc table.", UNHANDLED_EXCEPTION::NOT_REALLOC);
}

// = Object[MemberIndex]
INSIDE_DATA CLASS_SCOPE::OperatorReadByIndex(INSTANCE_CLASS, const LPINSIDE_DATA MemberIndex)
{
	auto Cell = NodeTable.Search(MemberIndex);
	if(Cell == nullptr)
		return INSIDE_DATA::Null;
	return Cell->Val;
}

INSIDE_DATA CLASS_SCOPE::OperatorReadByIndex(INSTANCE_CLASS, ZELLI_INTEGER MemberIndex)
{
	auto Cell = NodeTable.Search(MemberIndex);
	if(Cell == nullptr)
		return INSIDE_DATA::Null;
	return Cell->Val;
}

//Object[MemberIndex] = Source
void CLASS_SCOPE::OperatorWriteByIndex(INSTANCE_CLASS, const LPINSIDE_DATA MemberIndex, const LPINSIDE_DATA Source)
{
	if(!NODE_TABLE_HEADER::ResizeBeforeInsert(NodeTable))
		THROW_UNHANDLED_EXCEPTION("CLASS_SCOPE: Not realloc table.", UNHANDLED_EXCEPTION::NOT_REALLOC);
	NodeTable.Insert(MemberIndex)->Val = *Source;
}

void CLASS_SCOPE::OperatorWriteByIndex(INSTANCE_CLASS, ZELLI_INTEGER MemberIndex, const LPINSIDE_DATA Source)
{
	if(!NODE_TABLE_HEADER::ResizeBeforeInsert(NodeTable))
		THROW_UNHANDLED_EXCEPTION("CLASS_SCOPE: Not realloc table.", UNHANDLED_EXCEPTION::NOT_REALLOC);
	NodeTable.Insert(MemberIndex)->Val = *Source;
}

//delete Object[MemberIndex]
void CLASS_SCOPE::OperatorRemoveByIndex(INSTANCE_CLASS, const LPINSIDE_DATA MemberIndex)
{
	NodeTable.Remove(MemberIndex);
	if(!NODE_TABLE_HEADER::ResizeAfterRemove(NodeTable))
		THROW_UNHANDLED_EXCEPTION("CLASS_SCOPE: Not realloc table.", UNHANDLED_EXCEPTION::NOT_REALLOC);
}

void CLASS_SCOPE::OperatorRemoveByIndex(INSTANCE_CLASS, ZELLI_INTEGER MemberIndex)
{
	NodeTable.Remove(MemberIndex);
	if(!NODE_TABLE_HEADER::ResizeAfterRemove(NodeTable))
		THROW_UNHANDLED_EXCEPTION("CLASS_SCOPE: Not realloc table.", UNHANDLED_EXCEPTION::NOT_REALLOC);
}

void CLASS_SCOPE::EnumKey(INSTANCE_CLASS, LPINSIDE_DATA CurKey)
{
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

CLASS_SCOPE::CLASS_SCOPE() : HEADER_CLASS(this)
{
	if(!NODE_TABLE_HEADER::New(NodeTable,5))
		THROW_UNHANDLED_EXCEPTION("CLASS_SCOPE: not alloc memory for properties.", UNHANDLED_EXCEPTION::NOT_ALLOC_MEMORY);
	NodeTable.Init(5);
}

CLASS_SCOPE::~CLASS_SCOPE() { NODE_TABLE_HEADER::Free(NodeTable); }

void CLASS_SCOPE::MarkAllMembers()
{
	NodeTable.EnumValues(
		[](HASH_ELEMENT* Element)
		{
			GC_MARK_VAR(Element->Val);
			GC_MARK_VAR(Element->Key);
			return true;
		});
}

void CLASS_SCOPE::AddMember(LPINTERNAL_CHAR Name, INSIDE_DATA Val)
{
	LPHEADER_STRING NameMethod = EXECUTE_CONTEXT::ForThisThread->StringClass->RegisterString(Name);
	INSIDE_DATA Data(NameMethod, this);
	WriteMember(OBJECT::Null, &Data, &Val);
}