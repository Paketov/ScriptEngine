
#ifndef __CLASSMANAGER_H_HAS_INCLUDED__
#define  __CLASSMANAGER_H_HAS_INCLUDED__

#include "HashTable.h"
#include "Object.h"
#include "AdditionalLib.h"
#include "Class.h"
#include "ClassScope.h"

class CLASS_MANAGER: public HEADER_CLASS
{
protected:

	typedef unsigned short INDEX_OBJ_ELEMENT, *LPINDEX_OBJ_ELEMENT;

	struct _HASH_ELEM_STRING
	{
		OBJECT::PROTOTYPE Val;

		inline INDEX_OBJ_ELEMENT IndexInBound(INDEX_OBJ_ELEMENT MaxCount) { return IndexByKey(Val, MaxCount); }

		inline bool SetKey(OBJECT::PROTOTYPE k) { Val = k; return true; }

		static INDEX_OBJ_ELEMENT IndexByKey(OBJECT::PROTOTYPE k, INDEX_OBJ_ELEMENT MaxCount);

		inline bool CmpKey(OBJECT::PROTOTYPE k) const { return Val == k; }
	};

	typedef HASH_TABLE<_HASH_ELEM_STRING, false, INDEX_OBJ_ELEMENT> HASH_CLASSES;

	HASH_CLASSES Table;

	CLASS_MANAGER(LPSTRING_CLASS StrClass);


		// = Object.MemberIndex
	virtual INSIDE_DATA ReadMember(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex);
	//Object.MemberIndex = Source
	virtual void WriteMember(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex, const LPINSIDE_DATA Source);
	//delete Object.MemberIndex
	virtual void RemoveMember(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex);

	// = Object[MemberIndex]
	virtual INSIDE_DATA OperatorReadByIndex(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex);
	virtual INSIDE_DATA OperatorReadByIndex(INSTANCE_CLASS Object, ZELLI_INTEGER MemberIndex);

	//Object[MemberIndex] = Source
	virtual void OperatorWriteByIndex(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex, const LPINSIDE_DATA Source);
	virtual void OperatorWriteByIndex(INSTANCE_CLASS Object, ZELLI_INTEGER MemberIndex, const LPINSIDE_DATA Source);

	//delete Object[MemberIndex]
	virtual void OperatorRemoveByIndex(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex);
	virtual void OperatorRemoveByIndex(INSTANCE_CLASS Object, ZELLI_INTEGER MemberIndex);

	//CurKey in Object
	virtual void EnumKey(INSTANCE_CLASS Object, LPINSIDE_DATA CurKey);
};


#endif