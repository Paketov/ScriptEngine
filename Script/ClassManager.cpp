#include "ClassManager.h"
#include "String.h"
#include "Method.h"


CLASS_MANAGER::CLASS_MANAGER(LPSTRING_CLASS StrClass): HEADER_CLASS(this, "___ClassManager", StrClass) 
{
	if(!HASH_CLASSES::New(Table, 5))
		THROW_UNHANDLED_EXCEPTION("CLASS_MANAGER: not alloc memory for list classes.", UNHANDLED_EXCEPTION::NOT_ALLOC_MEMORY);
	Table.Init(5);

}

CLASS_MANAGER::INDEX_OBJ_ELEMENT CLASS_MANAGER::_HASH_ELEM_STRING::IndexByKey(OBJECT::PROTOTYPE k, INDEX_OBJ_ELEMENT MaxCount)
{
	 return LPHEADER_STRING(k->Name.Object)->Key % MaxCount;
}

// = Object.MemberIndex
INSIDE_DATA CLASS_MANAGER::ReadMember(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex)
{


}

//Object.MemberIndex = Source
void CLASS_MANAGER::WriteMember(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex, const LPINSIDE_DATA Source)
{
	if(MemberIndex->IsObject)
	{
		if(!HASH_CLASSES::ResizeBeforeInsert(Table))
			THROW_UNHANDLED_EXCEPTION("CLASS_MANAGER: Not realloc table.", UNHANDLED_EXCEPTION::NOT_REALLOC);
		Table.Insert(MemberIndex->Object.Prototype);
	}
}

//delete Object.MemberIndex
void CLASS_MANAGER::RemoveMember(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex)
{

}

// = Object[MemberIndex]
INSIDE_DATA CLASS_MANAGER::OperatorReadByIndex(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex)
{
	if(MemberIndex->IsObject)
	{
		if(!HASH_CLASSES::ResizeBeforeInsert(Table))
			THROW_UNHANDLED_EXCEPTION("CLASS_MANAGER: Not realloc table.", UNHANDLED_EXCEPTION::NOT_REALLOC);
		auto Cell = Table.Search(MemberIndex->Object.Prototype);
		if(Cell == nullptr)
			return INSIDE_DATA::Null;
		return OBJECT::New(Cell->Val, OBJECT::Null);
	}
	return INSIDE_DATA::Null;
}

INSIDE_DATA CLASS_MANAGER::OperatorReadByIndex(INSTANCE_CLASS Object, ZELLI_INTEGER MemberIndex)
{
	return INSIDE_DATA::Null;
}

//Object[MemberIndex] = Source
void CLASS_MANAGER::OperatorWriteByIndex(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex, const LPINSIDE_DATA Source)
{
	if(MemberIndex->IsObject)
	{
		if(!HASH_CLASSES::ResizeBeforeInsert(Table))
			THROW_UNHANDLED_EXCEPTION("CLASS_MANAGER: Not realloc table.", UNHANDLED_EXCEPTION::NOT_REALLOC);
		Table.Insert(MemberIndex->Object.Prototype);
	}
}


void CLASS_MANAGER::OperatorWriteByIndex(INSTANCE_CLASS Object, ZELLI_INTEGER MemberIndex, const LPINSIDE_DATA Source) {}

//delete Object[MemberIndex]
void CLASS_MANAGER::OperatorRemoveByIndex(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex)
{

}


void CLASS_MANAGER::OperatorRemoveByIndex(INSTANCE_CLASS Object, ZELLI_INTEGER MemberIndex)
{

}

//CurKey in Object
void CLASS_MANAGER::EnumKey(INSTANCE_CLASS Object, LPINSIDE_DATA CurKey)
{

}