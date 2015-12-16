#ifndef __CONSTSCOPE_H__
#define __CONSTSCOPE_H__

#include "TypeData.h"
#include "Object.h"
#include "HashTable.h"
#include "AdditionalLib.h"
#include "ExeptionHandler.h"

/*
* Класс для константных объектов (чтобы сборщик мусора игнорировал их).
*/


class CONST_SCOPE_CLASS;

typedef CONST_SCOPE_CLASS  *LPCONST_SCOPE_CLASS;

class CONST_SCOPE_CLASS: public HEADER_CLASS
{	
private:
	OBJECT				Instance;
	OBJECT::PROTOTYPE   StringClass;

	typedef unsigned short INDEX_OBJ_ELEMENT;

	typedef struct _HASH_ELEMENT
	{
		OBJECT Key;

		inline bool SetKey(LPOBJECT k) { Key = *k; return true; }
		static inline INDEX_OBJ_ELEMENT IndexByKey(OBJECT* k, INDEX_OBJ_ELEMENT MaxCount) { return k->Hash % MaxCount; }
		inline INDEX_OBJ_ELEMENT IndexInBound(INDEX_OBJ_ELEMENT MaxCount) { return IndexByKey(&Key, MaxCount); }
		inline bool CmpKey(OBJECT* EnotherKeyVal) { return Key == *EnotherKeyVal; }
	}  _HASH_ELEMENT;


	typedef HASH_TABLE<_HASH_ELEMENT, false, INDEX_OBJ_ELEMENT>   NODE_TABLE_HEADER, *LPNODE_TABLE_HEADER;
	NODE_TABLE_HEADER	NodeTable;

public:

	inline void * operator new(size_t Size) { return MEM_ALLOC(Size); }

	INSIDE_DATA GetElement(OBJECT &);

	void WriteElement(OBJECT &);

	virtual INSIDE_DATA CreateInstance(LPEXECUTE_CONTEXT, LPARG_FUNC Arg = &ARG_FUNC::EmptyArgs);

	void WriteElement(LPHEADER_STRING);

	virtual INSIDE_DATA ReadMember(INSTANCE_CLASS, const LPINSIDE_DATA);

	virtual void WriteMember(INSTANCE_CLASS, const LPINSIDE_DATA, const LPINSIDE_DATA);

	virtual void RemoveElement(INSTANCE_CLASS, const LPINSIDE_DATA);

	void RemoveElement(LPOBJECT);

	virtual void MarkAsUsed(INSTANCE_CLASS);

	virtual ZELLI_INTEGER GetLength(INSTANCE_CLASS);

	virtual void EnumKey(INSTANCE_CLASS, LPINSIDE_DATA);

	virtual bool OperatorEq(INSTANCE_CLASS, const LPINSIDE_DATA);


public:

	virtual SIZE_STR InfoClass(LPINTERNAL_CHAR, SIZE_STR);

	CONST_SCOPE_CLASS(LPSTRING_CLASS ListStrings)
	{
	   Name = ListStrings->RegisterString("__ConstScope");
	   StringClass = ListStrings;
	   CONST_SCOPE_CLASS();
	}


	CONST_SCOPE_CLASS()
	{
		//Name = ListStrings->RegisterString("__ConstScope");
		if(!NODE_TABLE_HEADER::New(NodeTable, 15))
			THROW_UNHANDLED_EXCEPTION("CONST_SCOPE: Not alloc memory.", UNHANDLED_EXCEPTION::NOT_ALLOC_MEMORY);
		NodeTable.Init(15);
		Instance.Prototype = this;
	}

};


#endif