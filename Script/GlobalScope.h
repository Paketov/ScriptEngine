#ifndef __GLOBALSCOPE_H__
#define __GLOBALSCOPE_H__

#include "TypeData.h"
#include "Object.h"
#include "HashTable.h"
#include "Context.h"
#include "AdditionalLib.h"
#include "ExeptionHandler.h"

/*
* Класс по работе с глобальными переменными.
*/

class GLOBAL_SCOPE_CLASS: public HEADER_CLASS
{	
private:
	OBJECT				Instance;
	OBJECT::PROTOTYPE	StringClass;

	typedef unsigned short INDEX_OBJ_ELEMENT;

	struct HASH_ELEMENT
	{
		LPHEADER_STRING Key;
		INSIDE_DATA Val;

		inline INDEX_OBJ_ELEMENT IndexInBound(INDEX_OBJ_ELEMENT MaxCount) { return IndexByKey(Key, MaxCount); }
		inline bool SetKey(LPHEADER_STRING k) { Key = k; return true; }
		static inline INDEX_OBJ_ELEMENT IndexByKey(LPHEADER_STRING k, INDEX_OBJ_ELEMENT MaxCount);
		inline bool CmpKey(LPHEADER_STRING k);
	};

	typedef HASH_TABLE<HASH_ELEMENT, false, INDEX_OBJ_ELEMENT>   NODE_TABLE_HEADER, *LPNODE_TABLE_HEADER;
	NODE_TABLE_HEADER	NodeTable;
public:

	inline void * operator new(size_t Size) { return MEM_ALLOC(Size); }

	virtual INSIDE_DATA CreateInstance(LPEXECUTE_CONTEXT, LPARG_FUNC Arg = &ARG_FUNC::EmptyArgs);

	virtual INSIDE_DATA ReadMember(INSTANCE_CLASS, const LPINSIDE_DATA);

	virtual void WriteMember(INSTANCE_CLASS, const LPINSIDE_DATA, const LPINSIDE_DATA);

	virtual void RemoveMember(INSTANCE_CLASS, const LPINSIDE_DATA);

	// = Object[MemberIndex]
	virtual INSIDE_DATA OperatorReadByIndex(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex);

	//Object[MemberIndex] = Source
	virtual void OperatorWriteByIndex(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex, const LPINSIDE_DATA Source);

	//delete Object[MemberIndex]
	virtual void OperatorRemoveByIndex(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex);

	virtual void MarkClassAsUsed();

	virtual ZELLI_INTEGER GetLength(INSTANCE_CLASS);

	virtual void EnumKey(INSTANCE_CLASS, LPINSIDE_DATA );

	virtual bool OperatorEq(INSTANCE_CLASS, const LPINSIDE_DATA );

	virtual SIZE_STR InfoClass(LPINTERNAL_CHAR, SIZE_STR);

	GLOBAL_SCOPE_CLASS(LPSTRING_CLASS);

};


#endif