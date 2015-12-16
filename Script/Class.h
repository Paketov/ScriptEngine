#ifndef __CLASS_H__
#define __CLASS_H__

#include "TypeData.h"
#include "HashTable.h"
#include "DynamicArray.h"
#include "Object.h"

/*
*		HEADER_CLASS - Template for all registred classes.
*
*    +------------------------------------------------------------------+
*    |							CLASS_MANAGER							  |
*    |																	  |
*    |							CreateObject("RegistredClass")---------------------+
*	 |																	  |         |
*    +------------------------------------------------------------------+         |
*																					|
*	 +-----------------------RegistredClass-----------------------------+			|
*    |  +-----------------------------------------------------------+   |			|
*	 |	|					HEADER_CLASS		^	     		     |   |			|
*	 |	+--------------------------------------|-------------------+    |			|
*    |   Fields....								|	  					 |			|
*    +-----------------------------------------|------------------------+			|
*												|									|
*												|	          +---------------------+
*		            							|	          v
*												|	  INSIDE_DATA-+
*												|				  |
*			+----------------------------------|--------------+  |
*			|     +----------------------------|-------+ <-------+
*			|     |           INSTANCE_CLASS    |       |      |
*			|     |								|	   	|      |
*			|     |							Prototype   |      |
*			|	  +------------------------------------+      |
*			|												   |
*			|		Fields...								   |
*			|												   |
*			+-------------------------------------------------+
*	
*/

#define _FAST_CHECK_OBJ
class HEADER_CLASS
{	
	friend CLASS_MANAGER;
	friend OBJECT;
public:
	//Name of class.
	LPHEADER_STRING					Name;
	OBJECT::ARR_FOR_CLASS::TINDEX	IndexInClassArr;

	inline bool IsInstance(LPOBJECT lpObject) const { return lpObject->Prototype == IndexInClassArr; }
	inline bool IsInstance(OBJECT & Object) const { return Object.Prototype == IndexInClassArr; }

	//Only when compiler, in not move fields mode
#ifdef _FAST_CHECK_OBJ
	unsigned					    __CheckVal;
	inline bool IsInstance(LPINSIDE_DATA lpData) { return *((unsigned*)lpData + 1) == __CheckVal; }

	inline bool IsInstance(INSIDE_DATA & Data) { return *((unsigned*)&Data + 1) == __CheckVal; }
#else
	inline bool IsInstance(LPINSIDE_DATA lpData) { return lpData->IsObject && (lpData->Object.Prototype == IndexInClassArr); }

	inline bool IsInstance(INSIDE_DATA & Data) { return Data.IsObject && (Data.Object.Prototype == IndexInClassArr); }
#endif 

	HEADER_CLASS();

	~HEADER_CLASS();

	//for gc
	virtual void SetAllInstanceToUnused();
	virtual void MarkAsUsed();
	virtual void MarkAsUsed(INSTANCE_CLASS Object);
	virtual void FreeAllUnused();

	//operators

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

	//Object == AnotherVar
	virtual bool OperatorEq(INSTANCE_CLASS Object, const LPINSIDE_DATA AnotherVar);

	//Object != AnotherVar
	virtual bool OperatorNeq(INSTANCE_CLASS Object, const LPINSIDE_DATA AnotherVar);

	//Object < AnotherVar
	virtual bool OperatorLt(INSTANCE_CLASS Object, const LPINSIDE_DATA AnotherVar);

	//Object <= AnotherVar
	virtual bool OperatorLe(INSTANCE_CLASS Object, const LPINSIDE_DATA AnotherVar);

	//Object > AnotherVar
	virtual bool OperatorGt(INSTANCE_CLASS Object, const LPINSIDE_DATA AnotherVar);

	//Object >= AnotherVar
	virtual bool OperatorGe(INSTANCE_CLASS Object, const LPINSIDE_DATA AnotherVar);

	//Object + AnotherVar
	virtual INSIDE_DATA OperatorAdd(INSTANCE_CLASS Object, const LPINSIDE_DATA AnotherVar);

	//Object - AnotherVar
	virtual INSIDE_DATA OperatorSub(INSTANCE_CLASS Object, const LPINSIDE_DATA AnotherVar);

	//Object * AnotherVar
	virtual INSIDE_DATA OperatorMul(INSTANCE_CLASS Object, const LPINSIDE_DATA AnotherVar);

	//Object / AnotherVar
	virtual INSIDE_DATA OperatorDiv(INSTANCE_CLASS Object, const LPINSIDE_DATA AnotherVar);

	//Object % AnotherVar
	virtual INSIDE_DATA OperatorMod(INSTANCE_CLASS Object, const LPINSIDE_DATA AnotherVar);

	//Object & AnotherVar
	virtual INSIDE_DATA OperatorAnd(INSTANCE_CLASS Object, const LPINSIDE_DATA AnotherVar);

	//Object | AnotherVar
	virtual INSIDE_DATA OperatorOr(INSTANCE_CLASS Object, const LPINSIDE_DATA AnotherVar);

	//Object ^ AnotherVar
	virtual INSIDE_DATA OperatorXor(INSTANCE_CLASS Object, const LPINSIDE_DATA AnotherVar);

	//Object >> AnotherVar
	virtual INSIDE_DATA OperatorShr(INSTANCE_CLASS Object, const LPINSIDE_DATA AnotherVar);

	//Object << AnotherVar
	virtual INSIDE_DATA OperatorShl(INSTANCE_CLASS Object, const LPINSIDE_DATA AnotherVar);

	//-Object
	virtual INSIDE_DATA OperatorInvers(INSTANCE_CLASS Object);

	//!Object
	virtual INSIDE_DATA OperatorNot(INSTANCE_CLASS Object);

	//~Object
	virtual INSIDE_DATA OperatorBitNot(INSTANCE_CLASS Object);

	//Object++
	virtual void OperatorInc(INSTANCE_CLASS Object);

	//Object--
	virtual void OperatorDec(INSTANCE_CLASS Object);

	//Object.clone()
	virtual INSIDE_DATA Clone(INSTANCE_CLASS Object);

	//Object.length
	virtual ZELLI_INTEGER GetLength(INSTANCE_CLASS Object);

	virtual HASH_VAL GetHash(INSTANCE_CLASS Instance);

	//CurKey in Object
	virtual void EnumKey(INSTANCE_CLASS Object, LPINSIDE_DATA CurKey);

	//Object()
	virtual INSIDE_DATA OperatorCall(INSTANCE_CLASS Object, LPINSIDE_DATA This, LPEXECUTE_CONTEXT CurContext, LPARG_FUNC Arg);

	//(double)Object
	virtual ZELLI_DOUBLE OperatorToDouble(INSTANCE_CLASS Object);

	//(int)Object
	virtual ZELLI_INTEGER OperatorToInt(INSTANCE_CLASS Object);

	//(bool)Object
	virtual bool OperatorToBool(INSTANCE_CLASS Object);

	//Constructor
	INSIDE_DATA CreateInstance(LPARG_FUNC Arg = &ARG_FUNC::EmptyArgs);

	virtual INSIDE_DATA CreateInstance(LPEXECUTE_CONTEXT Context, LPARG_FUNC Arg = &ARG_FUNC::EmptyArgs);

	virtual INSIDE_DATA CreateInstance(LPEXECUTE_CONTEXT Context, void * Data);

	//Debug info
	virtual SIZE_STR InfoObject(INSTANCE_CLASS Obj, LPINTERNAL_CHAR Buffer, SIZE_STR LenInBuf);

	virtual SIZE_STR InfoClass(LPINTERNAL_CHAR Buffer, SIZE_STR LenInBuf);
};


class CLASS_MANAGER
{
private:
	//LPSTRING_CLASS StringHeapNameObjects;

	struct _HASH_ELEM_OBJECTS
	{
		LPHEADER_CLASS Val;

		inline bool SetKey(LPHEADER_CLASS v) { Val = v; return true;}

		static inline unsigned short IndexByKey(LPHEADER_STRING k, unsigned short MaxCount);

		static inline unsigned short IndexByKey(const LPINTERNAL_CHAR k, unsigned short MaxCount);

		static inline unsigned short IndexByKey(LPHEADER_CLASS k, unsigned short MaxCount);

		inline unsigned short IndexInBound(unsigned short MaxCount) const;

		inline bool CmpKey(LPHEADER_STRING EnotherKeyVal);

		inline bool CmpKey(const LPINTERNAL_CHAR EnotherKeyVal);

		inline bool CmpKey(LPHEADER_CLASS EnotherKeyVal) { return Val == EnotherKeyVal; }
	};

	typedef HASH_TABLE<_HASH_ELEM_OBJECTS, false, unsigned short> HASH_OBJECTS, *LPHASH_OBJECTS;

	HASH_OBJECTS   ClassList;
public:
	CLASS_MANAGER();

	~CLASS_MANAGER() { HASH_OBJECTS::Free(ClassList); }

	inline void * operator new(size_t Size) { return MEM_ALLOC(Size); }
	/*
	*	������ ������.
	*/
	INSIDE_DATA CreateObject(LPHEADER_STRING NameClass,LPEXECUTE_CONTEXT Context, LPARG_FUNC Arg = &ARG_FUNC::EmptyArgs);

	INSIDE_DATA CreateObject(LPINSIDE_DATA IdClass,LPEXECUTE_CONTEXT Context, LPARG_FUNC Arg = &ARG_FUNC::EmptyArgs);

	INSIDE_DATA CreateObject(LPHEADER_STRING NameClass, LPARG_FUNC Arg = &ARG_FUNC::EmptyArgs);

	INSIDE_DATA CreateObject(LPINSIDE_DATA IdClass, LPARG_FUNC Arg = &ARG_FUNC::EmptyArgs);

	INSIDE_DATA CreateObject(LPINTERNAL_CHAR NameClass, LPEXECUTE_CONTEXT Context, LPARG_FUNC Arg = &ARG_FUNC::EmptyArgs);

	INSIDE_DATA CreateObject(LPINTERNAL_CHAR NameClass, LPARG_FUNC Arg = &ARG_FUNC::EmptyArgs);

	INSIDE_DATA CreateObjectWithException(LPHEADER_STRING NameClass,LPEXECUTE_CONTEXT Context,LPARG_FUNC Arg = &ARG_FUNC::EmptyArgs) HAS_THROW;

	void SetAllInstanceToUnused();

	void FreeAllUnusedInstance();

	inline LPHEADER_CLASS operator[](const LPHEADER_STRING Id) { return GetClass(Id); }

	inline LPHEADER_CLASS operator[](const LPINTERNAL_CHAR Id);

	inline LPHEADER_CLASS GetClass(LPHEADER_STRING NameClass);

	inline LPHEADER_CLASS GetClassWithException(LPHEADER_STRING NameClass) HAS_THROW;

	LPHEADER_CLASS ClassRegister(LPHEADER_CLASS Class) HAS_THROW;

	LPHEADER_CLASS ClassUnregister(LPHEADER_STRING NameClass);

	LPHEADER_CLASS ClassUnregisterWithException(LPHEADER_STRING NameClass)HAS_THROW;

};


#endif
