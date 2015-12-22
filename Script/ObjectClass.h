#include "TypeData.h"
#include "Object.h"
#include "HashTable.h"
#include "Context.h"
#include "AdditionalLib.h"
#include "ExeptionHandler.h"

class OBJECT_HEADER;
class OBJECT_CLASS;

typedef OBJECT_HEADER *LPOBJECT_HEADER;
typedef OBJECT_CLASS  *LPOBJECT_CLASS;



class OBJECT_CLASS: public HEADER_CLASS
{	
	typedef unsigned short INDEX_OBJ_ELEMENT, *LPINDEX_OBJ_ELEMENT;
	friend OBJECT_HEADER;
public:

	struct HASH_ELEMENT
	{
		INSIDE_DATA Key;
		INSIDE_DATA Val;

		inline INDEX_OBJ_ELEMENT IndexInBound(INDEX_OBJ_ELEMENT MaxCount) { return IndexByKey(&Key, MaxCount); }

		inline bool SetKey(LPINSIDE_DATA k) { Key = *k; return true; }
		inline bool SetKey(ZELLI_INTEGER k) { Key = k; return true; }

		static inline INDEX_OBJ_ELEMENT IndexByKey(INSIDE_DATA* k, INDEX_OBJ_ELEMENT MaxCount) { return k->Hash % MaxCount; }
		static inline INDEX_OBJ_ELEMENT IndexByKey(ZELLI_INTEGER k, INDEX_OBJ_ELEMENT MaxCount) { return ZELLI_INTEGER_HASH(k) % MaxCount; }
		
		inline bool CmpKey(LPINSIDE_DATA k) { return Key == *k; }
		inline bool CmpKey(ZELLI_INTEGER k) { return Key.IsInt && (Key.Integer == k); }
	};

	typedef HASH_TABLE<HASH_ELEMENT, false, INDEX_OBJ_ELEMENT> NODE_TABLE_HEADER, *LPNODE_TABLE_HEADER;
private:
	CHECK_USES					CurCheckUses;	//Для сборщика мусора
	LPOBJECT_HEADER				HeaderObjQueue;
	unsigned					CountHeaderInQueue;
	unsigned					MaxCountHeaderInQueue;

	void (__fastcall * AddInQueueHeader)(LPOBJECT_CLASS This, LPOBJECT_HEADER Header);

	LPOBJECT_HEADER (__fastcall * GetHeaderFromQueue)(LPOBJECT_CLASS This);

	static void __fastcall __AddInQueue(LPOBJECT_CLASS This, LPOBJECT_HEADER Header);

	static void __fastcall __DeleteObj(LPOBJECT_CLASS This, LPOBJECT_HEADER Header);

	static LPOBJECT_HEADER __fastcall __GetFromQueue(LPOBJECT_CLASS This);

	static LPOBJECT_HEADER __fastcall __AllocHeader(LPOBJECT_CLASS This);

	void SwitchToAddMode()
	{
		AddInQueueHeader = __AddInQueue;
		GetHeaderFromQueue = __GetFromQueue;
	}

	struct LISTS_OBJECTS: public LIST_HEADER<OBJECT_HEADER>
	{
		LPOBJECT_HEADER operator &() { return (LPOBJECT_HEADER)((unsigned)this - sizeof(INSTANCE_CLASS)); }
	};

	LISTS_OBJECTS	ListObject;
	LISTS_OBJECTS	ListUnusedObject;

public:
	virtual INSIDE_DATA CreateInstance(LPEXECUTE_CONTEXT Context,LPARG_FUNC Arg);

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

	HASH_VAL GetHash(INSTANCE_CLASS Instance);

	virtual void FreeAllUnused();

	virtual void SetAllInstanceToUnused(LPHEADER_CLASS Class)
	{
		CurCheckUses++;
		LIST_HEADER<OBJECT_HEADER>::BindList(&ListUnusedObject, &ListObject);
		LIST_HEADER<OBJECT_HEADER>::Close(&ListObject);
	}

	virtual void MarkInstanceAsUsed(INSTANCE_CLASS Object);

	virtual void MarkClassAsUsed(INSTANCE_CLASS Object);

	virtual ZELLI_INTEGER GetLength(INSTANCE_CLASS Object);

	virtual void EnumKey(INSTANCE_CLASS Object, LPINSIDE_DATA CurKey);

	virtual bool OperatorEq(INSTANCE_CLASS ThisObj, const LPINSIDE_DATA SecondObj);

	OBJECT_CLASS(LPSTRING_CLASS ListStrings);
};


class OBJECT_HEADER: public LIST_HEADER<OBJECT_HEADER>
{
public:
	OBJECT_CLASS::NODE_TABLE_HEADER			NodeTable;
	CHECK_USES								ForCheckUses;
};



