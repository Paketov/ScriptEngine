#ifndef __CLASSSCOPE_H__
#define __CLASSSCOPE_H__

#include "HashTable.h"
#include "Object.h"
#include "Class.h"



class CLASS_SCOPE: public virtual HEADER_CLASS
{
protected:

	friend EXECUTE_CONTEXT;
	typedef unsigned short INDEX_OBJ_ELEMENT, *LPINDEX_OBJ_ELEMENT;

	struct HASH_ELEMENT
	{
		INSIDE_DATA Key;
		INSIDE_DATA Val;

		inline INDEX_OBJ_ELEMENT IndexInBound(INDEX_OBJ_ELEMENT MaxCount) { return IndexByKey(&Key, MaxCount); }

		inline bool SetKey(LPINSIDE_DATA k) { Key = *k; return true; }
		inline bool SetKey(ZELLI_INTEGER k) { Key = k; return true; }

		inline static INDEX_OBJ_ELEMENT IndexByKey(INSIDE_DATA* k, INDEX_OBJ_ELEMENT MaxCount) { return k->Hash % MaxCount; }
		inline static INDEX_OBJ_ELEMENT IndexByKey(ZELLI_INTEGER k, INDEX_OBJ_ELEMENT MaxCount) { return ZELLI_INTEGER_HASH(k) % MaxCount; }

		inline bool CmpKey(LPINSIDE_DATA k) { return Key == *k; }
		inline bool CmpKey(ZELLI_INTEGER k) { return Key.IsInt && (Key.Integer == k); }
	};

	typedef HASH_TABLE<HASH_ELEMENT, false, INDEX_OBJ_ELEMENT> NODE_TABLE_HEADER, *LPNODE_TABLE_HEADER;

	NODE_TABLE_HEADER		NodeTable;

	CLASS_SCOPE();

	~CLASS_SCOPE();

	// = Object.MemberIndex
	virtual INSIDE_DATA ReadMember(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex);
	//Object.MemberIndex = Source
	virtual void WriteMember(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex, const LPINSIDE_DATA Source);
	//delete Object.MemberIndex
	virtual void RemoveMember(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex);

	// = Object[MemberIndex]
	virtual INSIDE_DATA OperatorReadByIndex(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex);
	virtual INSIDE_DATA OperatorReadByIndex(INSTANCE_CLASS Object, ZELLI_INTEGER MemberId);

	//Object[MemberIndex] = Source
	virtual void OperatorWriteByIndex(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex, const LPINSIDE_DATA Source);
	virtual void OperatorWriteByIndex(INSTANCE_CLASS Object, ZELLI_INTEGER MemberIndex, const LPINSIDE_DATA Source);

	//delete Object[MemberIndex]
	virtual void OperatorRemoveByIndex(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex);
	virtual void OperatorRemoveByIndex(INSTANCE_CLASS Object, ZELLI_INTEGER MemberIndex);

	virtual void EnumKey(INSTANCE_CLASS Object, LPINSIDE_DATA CurKey);

	void MarkAllMembers();

	void AddMember(LPINTERNAL_CHAR Name, INSIDE_DATA Val);
};

#endif