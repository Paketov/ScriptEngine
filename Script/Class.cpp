#include <malloc.h>
#include <string.h>

#include "Class.h"
#include "Object.h"
#include "ExeptionHandler.h"
#include "String.h"
#include "Context.h"

void HEADER_CLASS::Init(LPINTERNAL_CHAR NewNameForThisClass, LPSTRING_CLASS StringClassForNewName)
{
	IndexInClassArr = OBJECT::ClassList.AddElement();
	if(IndexInClassArr == OBJECT::ClassList.NullIndex)
		THROW_UNHANDLED_EXCEPTION("Not get ne index global table.", UNHANDLED_EXCEPTION::NOT_ALLOC_MEMORY);
	OBJECT::ClassList[IndexInClassArr] = this;
	if(NewNameForThisClass != nullptr)
	{
		if(StringClassForNewName == nullptr)
			StringClassForNewName = EXECUTE_CONTEXT::ForThisThread->StringClass;
		Name = OBJECT::New(StringClassForNewName, StringClassForNewName->RegisterString(NewNameForThisClass));
	}
#ifdef _FAST_CHECK_OBJ
	INSIDE_DATA ForCheck = OBJECT::New(this, INSTANCE_CLASS(0));
	__CheckVal = *((unsigned*)&ForCheck + 1);
#endif
}


HEADER_CLASS::~HEADER_CLASS() { OBJECT::ClassList.RemoveAllInplement(this); }

INSIDE_DATA HEADER_CLASS::CreateInstance(LPARG_FUNC Arg) { return CreateInstance(EXECUTE_CONTEXT::ForThisThread, Arg); }

INSIDE_DATA CLASS_MANAGER::CreateObject(LPINTERNAL_CHAR NameClass, LPARG_FUNC Arg) { return CreateObject(NameClass, EXECUTE_CONTEXT::ForThisThread, Arg); }

INSIDE_DATA CLASS_MANAGER::CreateObject(LPHEADER_STRING NameClass, LPEXECUTE_CONTEXT Context, LPARG_FUNC Arg)
{
	auto Cell = ClassList.Search(NameClass);
	if(Cell != nullptr)
		return Cell->Val->CreateInstance(Context, Arg);
	return INSIDE_DATA::Null;
}

INSIDE_DATA CLASS_MANAGER::CreateObject(LPINSIDE_DATA IdClass, LPEXECUTE_CONTEXT Context, LPARG_FUNC Arg)
{
	if(IdClass->IsString)
		return CreateObject((LPHEADER_STRING)IdClass->Object, Context, Arg);

	return INSIDE_DATA::Null;
}

INSIDE_DATA CLASS_MANAGER::CreateObject(LPHEADER_STRING NameClass, LPARG_FUNC Arg) { return CreateObject(NameClass, EXECUTE_CONTEXT::ForThisThread, Arg); }

INSIDE_DATA CLASS_MANAGER::CreateObject(LPINSIDE_DATA IdClass, LPARG_FUNC Arg)
{
	if(IdClass->IsString)
		return CreateObject((LPHEADER_STRING)IdClass->Object, EXECUTE_CONTEXT::ForThisThread, Arg);
	return INSIDE_DATA::Null;
}

INSIDE_DATA CLASS_MANAGER::CreateObject(LPINTERNAL_CHAR NameClass, LPEXECUTE_CONTEXT Context, LPARG_FUNC Arg)
{
	auto Cell = ClassList.Search(NameClass);
	if(Cell != nullptr)
		return Cell->Val->CreateInstance(Context, Arg);
	return INSIDE_DATA::Null;
}

INSIDE_DATA CLASS_MANAGER::CreateObjectWithException(LPHEADER_STRING NameClass, LPEXECUTE_CONTEXT Context, LPARG_FUNC Arg) 
{
	auto Cell = ClassList.Search(NameClass);
	if(Cell != nullptr)
		return Cell->Val->CreateInstance(Context, Arg);
	THROW_NATIVE_EXCEPT("CLASS_MANAGER: Not have object with this name.", HEADER_EXCEPTION::NOT_HAVE_THIS_OBJECT);
}

inline LPHEADER_CLASS CLASS_MANAGER::GetClass(LPHEADER_STRING NameClass) 
{
	auto Cell = ClassList.Search(NameClass);
	if(Cell != nullptr)
		return Cell->Val;
	return nullptr;
}

inline LPHEADER_CLASS CLASS_MANAGER::GetClassWithException(LPHEADER_STRING NameClass) 
{
	auto Cell = ClassList.Search(NameClass);
	if(Cell != nullptr)
		return Cell->Val;
	THROW_NATIVE_EXCEPT("CLASS_MANAGER: Not have object with this name.", HEADER_EXCEPTION::NOT_HAVE_THIS_OBJECT);
}

LPHEADER_CLASS CLASS_MANAGER::ClassRegister(LPHEADER_CLASS Class) HAS_THROW
{	
	if(!HASH_OBJECTS::ResizeBeforeInsert(ClassList))
		THROW_UNHANDLED_EXCEPTION("Not alloc memory for new register class.", UNHANDLED_EXCEPTION::NOT_ALLOC_MEMORY);
	return ClassList.Insert(Class)->Val;
}

LPHEADER_CLASS CLASS_MANAGER::ClassUnregister(LPHEADER_STRING NameClass)
{
	auto Cell = ClassList.Remove(NameClass);
	if(Cell == nullptr)
		return nullptr;
	HASH_OBJECTS::ResizeAfterRemove(ClassList);
	return Cell->Val;
}

LPHEADER_CLASS CLASS_MANAGER::ClassUnregisterWithException(LPHEADER_STRING NameClass)
{
	auto Cell = ClassList.Remove(NameClass);
	if(Cell == nullptr)
		THROW_NATIVE_EXCEPT("CLASS_MANAGER: Not have object with this name.", HEADER_EXCEPTION::NOT_HAVE_THIS_OBJECT);
	if(!HASH_OBJECTS::ResizeAfterRemove(ClassList))
		THROW_UNHANDLED_EXCEPTION("Not relloc memory for deleted class.", UNHANDLED_EXCEPTION::NOT_ALLOC_MEMORY);
	return Cell->Val;
}

LPHEADER_CLASS CLASS_MANAGER::operator[](const LPINTERNAL_CHAR Id)
{
	auto Cell = ClassList.Search(Id);
	if(Cell == nullptr)
		return nullptr;
	return Cell->Val;
}

inline unsigned short CLASS_MANAGER::_HASH_ELEM_OBJECTS::IndexByKey(LPHEADER_CLASS k, unsigned short MaxCount) { return k->Name.Hash % MaxCount;}

inline unsigned short CLASS_MANAGER::_HASH_ELEM_OBJECTS::IndexByKey(LPHEADER_STRING k, unsigned short MaxCount) { return k->Key % MaxCount; }

inline unsigned short CLASS_MANAGER::_HASH_ELEM_OBJECTS::IndexByKey(const LPINTERNAL_CHAR k, unsigned short MaxCount) { return HEADER_STRING::Hash(k) %  MaxCount; }

inline unsigned short CLASS_MANAGER::_HASH_ELEM_OBJECTS::IndexInBound(unsigned short MaxCount) const { return Val->Name.Hash % MaxCount; }

inline bool CLASS_MANAGER::_HASH_ELEM_OBJECTS::CmpKey(LPHEADER_STRING EnotherKeyVal) { return *(LPHEADER_STRING)Val->Name == *EnotherKeyVal; }

inline bool CLASS_MANAGER::_HASH_ELEM_OBJECTS::CmpKey(const LPINTERNAL_CHAR EnotherKeyVal) { return *(LPHEADER_STRING)Val->Name == EnotherKeyVal; }

INSIDE_DATA DefaultCreateInstance(LPHEADER_CLASS , LPEXECUTE_CONTEXT , LPARG_FUNC ) { return INSIDE_DATA::Null; }

CLASS_MANAGER::CLASS_MANAGER()
{
	if(!HASH_OBJECTS::New(ClassList, 5))
		THROW_UNHANDLED_EXCEPTION("CLASS_MANAGER: not alloc mem for class list.", UNHANDLED_EXCEPTION::NOT_ALLOC_MEMORY);
	ClassList.Init(5);
}


//Create instance by arg list
INSIDE_DATA HEADER_CLASS::CreateInstance(LPINSIDE_DATA Args, NUMBER_ARG CountArg) { return INSIDE_DATA::Null; }

//Create instance by internal represent class 
INSIDE_DATA HEADER_CLASS::CreateInstance(void* InternalRepresentForClass) { return INSIDE_DATA::Null; }


//for gc
void HEADER_CLASS::SetAllInstanceToUnused(){}

inline void HEADER_CLASS::MarkAsUsed()
{
	Name.MarkAsUsed(); 
	MarkClassAsUsed();
}

inline void HEADER_CLASS::MarkInstanceAsUsed(INSTANCE_CLASS) { }
inline void HEADER_CLASS::MarkClassAsUsed() { }

void HEADER_CLASS::FreeAllUnused(){}


void HEADER_CLASS::SetName(LPINTERNAL_CHAR NewNameForThisClass, LPSTRING_CLASS StringClassForNewName)
{
	if(StringClassForNewName == nullptr)
		StringClassForNewName = EXECUTE_CONTEXT::ForThisThread->StringClass;
	Name = OBJECT::New(StringClassForNewName, StringClassForNewName->RegisterString(NewNameForThisClass));
}

//operators

// = Object.MemberIndex
INSIDE_DATA HEADER_CLASS::ReadMember(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex) { return OperatorReadByIndex(Object, MemberIndex); }

//Object.MemberIndex = Source
void HEADER_CLASS::WriteMember(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex, const LPINSIDE_DATA Source) { OperatorWriteByIndex(Object, MemberIndex, Source); }

//delete Object.MemberIndex
void HEADER_CLASS::RemoveMember(INSTANCE_CLASS Object, const LPINSIDE_DATA MemberIndex) { OperatorRemoveByIndex(Object, MemberIndex); }

// = Object[MemberIndex]
INSIDE_DATA HEADER_CLASS::OperatorReadByIndex(INSTANCE_CLASS, const LPINSIDE_DATA) { return INSIDE_DATA::Null; }
INSIDE_DATA HEADER_CLASS::OperatorReadByIndex(INSTANCE_CLASS Object, ZELLI_INTEGER MemberIndex) 
{ 	
	INSIDE_DATA Data = MemberIndex;
	return OperatorReadByIndex(Object, &Data); 
}

//Object[MemberIndex] = Source
void HEADER_CLASS::OperatorWriteByIndex(INSTANCE_CLASS, const LPINSIDE_DATA, const LPINSIDE_DATA) {}
void HEADER_CLASS::OperatorWriteByIndex(INSTANCE_CLASS Object, ZELLI_INTEGER MemberIndex, const LPINSIDE_DATA Source)
{ 
	INSIDE_DATA Data = MemberIndex;
	OperatorWriteByIndex(Object, &Data, Source); 
}

//delete Object[MemberIndex]
void HEADER_CLASS::OperatorRemoveByIndex(INSTANCE_CLASS, const LPINSIDE_DATA){}
void HEADER_CLASS::OperatorRemoveByIndex(INSTANCE_CLASS Object, ZELLI_INTEGER MemberIndex) 
{ 
	INSIDE_DATA Data = MemberIndex;
	OperatorRemoveByIndex(Object, &Data); 
}

//Object == AnotherVar
bool HEADER_CLASS::OperatorEq(INSTANCE_CLASS, LPINSIDE_DATA) { return false; }

//Object != AnotherVar
bool HEADER_CLASS::OperatorNeq(INSTANCE_CLASS, LPINSIDE_DATA) { return false; }

//Object < AnotherVar
bool HEADER_CLASS::OperatorLt(INSTANCE_CLASS, LPINSIDE_DATA) { return false; }

//Object <= AnotherVar
bool HEADER_CLASS::OperatorLe(INSTANCE_CLASS, LPINSIDE_DATA) { return false; }

//Object > AnotherVar
bool HEADER_CLASS::OperatorGt(INSTANCE_CLASS, LPINSIDE_DATA) { return false; }

//Object >= AnotherVar
bool HEADER_CLASS::OperatorGe(INSTANCE_CLASS, LPINSIDE_DATA) { return false; }

//Object + AnotherVar
INSIDE_DATA HEADER_CLASS::OperatorAdd(INSTANCE_CLASS, LPINSIDE_DATA) { return false; }

//Object - AnotherVar
INSIDE_DATA HEADER_CLASS::OperatorSub(INSTANCE_CLASS, LPINSIDE_DATA) { return INSIDE_DATA::Null; }

//Object * AnotherVar
INSIDE_DATA HEADER_CLASS::OperatorMul(INSTANCE_CLASS, LPINSIDE_DATA) { return INSIDE_DATA::Null; }

//Object / AnotherVar
INSIDE_DATA HEADER_CLASS::OperatorDiv(INSTANCE_CLASS, LPINSIDE_DATA) { return INSIDE_DATA::Null; }

//Object % AnotherVar
INSIDE_DATA HEADER_CLASS::OperatorMod(INSTANCE_CLASS, LPINSIDE_DATA) { return INSIDE_DATA::Null; }

INSIDE_DATA HEADER_CLASS::OperatorAnd(INSTANCE_CLASS, LPINSIDE_DATA) { return INSIDE_DATA::Null; }

//Object | AnotherVar
INSIDE_DATA HEADER_CLASS::OperatorOr(INSTANCE_CLASS Object, LPINSIDE_DATA AnotherVar) { return INSIDE_DATA::Null; }

//Object ^ AnotherVar
INSIDE_DATA HEADER_CLASS::OperatorXor(INSTANCE_CLASS Object, LPINSIDE_DATA AnotherVar) { return INSIDE_DATA::Null; }

//Object >> AnotherVar
INSIDE_DATA HEADER_CLASS::OperatorShr(INSTANCE_CLASS Object, LPINSIDE_DATA AnotherVar) { return INSIDE_DATA::Null; }

//Object << AnotherVar
INSIDE_DATA HEADER_CLASS::OperatorShl(INSTANCE_CLASS Object, LPINSIDE_DATA AnotherVar) { return INSIDE_DATA::Null; }

//-Object
INSIDE_DATA HEADER_CLASS::OperatorInvers(INSTANCE_CLASS Object) { return INSIDE_DATA::Null; }

//!Object
INSIDE_DATA HEADER_CLASS::OperatorNot(INSTANCE_CLASS Object) { return INSIDE_DATA::Null; }

//~Object
INSIDE_DATA HEADER_CLASS::OperatorBitNot(INSTANCE_CLASS Object) { return INSIDE_DATA::Null; }

//Object++
void HEADER_CLASS::OperatorInc(INSTANCE_CLASS) {}

//Object--
void HEADER_CLASS::OperatorDec(INSTANCE_CLASS) {}

//Object.clone()
INSIDE_DATA HEADER_CLASS::Clone(INSTANCE_CLASS) { return INSIDE_DATA::Null; }

//Object.length
ZELLI_INTEGER HEADER_CLASS::GetLength(INSTANCE_CLASS) { return 0; }


HASH_VAL HEADER_CLASS::GetHash(INSTANCE_CLASS Instance) { return (HASH_VAL)Instance;}

//CurKey in Object
void HEADER_CLASS::EnumKey(INSTANCE_CLASS, LPINSIDE_DATA CurKey) { CurKey->SetNull(); }

//Object()
INSIDE_DATA HEADER_CLASS::OperatorCall(INSTANCE_CLASS Object, LPINSIDE_DATA This, LPEXECUTE_CONTEXT CurContext, LPARG_FUNC Arg) { return INSIDE_DATA::Null; }

//(double)Object
ZELLI_DOUBLE HEADER_CLASS::OperatorToDouble(INSTANCE_CLASS Object) { return 0.0; }

//(int)Object
ZELLI_INTEGER HEADER_CLASS::OperatorToInt(INSTANCE_CLASS) { return 0; }

//(bool)Object
bool HEADER_CLASS::OperatorToBool(INSTANCE_CLASS) { return false; }


INSIDE_DATA HEADER_CLASS::FromString(const INTERNAL_CHAR* InputString, TSIZE_STR StrLen) { return INSIDE_DATA::Null; }

//Convert object to string. Result in internal style string. String registred in BasicStringClass.
LPHEADER_STRING HEADER_CLASS::ToString(INSTANCE_CLASS Object, LPSTRING_CLASS BasicStringClass) 
{ 
	std::basic_string<INTERNAL_CHAR> s;
	ToString(Object, s);
	return BasicStringClass->RegisterString(s.c_str());
}

//Convert object to string. Result place in c style string.
void HEADER_CLASS::ToString(INSTANCE_CLASS Object, LPINTERNAL_CHAR Buf, TSIZE_STR SizeBuf)
{
	std::basic_string<INTERNAL_CHAR> s;
	ToString(Object, s);
	memcpy(Buf, s.c_str(), ((s.length() > SizeBuf)? SizeBuf: s.length()) * sizeof(INTERNAL_CHAR));
}

//Convert object to string. Result place in c++ style string.
void HEADER_CLASS::ToString(INSTANCE_CLASS Object, std::basic_string<INTERNAL_CHAR>& Buf)
{
	std::basic_string<INTERNAL_CHAR> s;
	TypeOf(s);
	Buf = "<Instance of " + s + ">";
}

//Get type of object. Result in internal style string. String registred in BasicStringClass.
LPHEADER_STRING HEADER_CLASS::TypeOf(LPSTRING_CLASS BasicStringClass)
{
	std::basic_string<INTERNAL_CHAR> s;
	TypeOf(s);
	return BasicStringClass->RegisterString(s.c_str());
}

//Get type of object. Result place in c style string.
void HEADER_CLASS::TypeOf(LPINTERNAL_CHAR Buf, TSIZE_STR SizeBuf)
{
	std::basic_string<INTERNAL_CHAR> s;
	TypeOf(s);
	memcpy(Buf, s.c_str(), ((s.length() > SizeBuf)? SizeBuf: s.length()) * sizeof(INTERNAL_CHAR));
}

//Get type of object. Result place in c++ style string.
void HEADER_CLASS::TypeOf(std::basic_string<INTERNAL_CHAR>& Buf) 
{
	Buf = "[";
	Buf.append(LPHEADER_STRING(Name.Object)->Str, LPHEADER_STRING(Name.Object)->Len);
	Buf += "]";
}

//Debug info
SIZE_STR HEADER_CLASS::InfoObject(INSTANCE_CLASS, LPINTERNAL_CHAR Buffer, SIZE_STR LenInBuf)
{
	int s = sprintf_s(Buffer, LenInBuf, "Not have info for this object.");
	return (s > 0)?s:0;
}

SIZE_STR HEADER_CLASS::InfoClass(LPINTERNAL_CHAR Buffer, SIZE_STR LenInBuf)
{
	int s = sprintf_s(Buffer,LenInBuf, "Not have info for this class.");
	return (s > 0)?s:0;
}

void CLASS_MANAGER::SetAllInstanceToUnused()
{
	ClassList.EnumValues( 
		[](void*, CLASS_MANAGER::_HASH_ELEM_OBJECTS* Element)
		{
			Element->Val->SetAllInstanceToUnused();
			return true;
		});
}

void CLASS_MANAGER::FreeAllUnusedInstance()
{
	ClassList.EnumValues( 
		[](void*, CLASS_MANAGER::_HASH_ELEM_OBJECTS* Element)
		{
			Element->Val->FreeAllUnused();
			return true;
		});
}

void CLASS_MANAGER::MarkAllClassesAsUsed()
{
	ClassList.EnumValues( 
		[](void*, CLASS_MANAGER::_HASH_ELEM_OBJECTS* Element)
		{
			Element->Val->MarkAsUsed();
			return true;
		});
}

