#include "String.h"
#include "Method.h"
#include "ExString.h"
#include "ConstScope.h"


STRING_CLASS::STRING_CLASS()
{
	CurCheckUses = 0;
	if(!ListStrings.New(ListStrings, 5))
		THROW_UNHANDLED_EXCEPTION("STRING_CLASS: not alloc memory for list strings.", UNHANDLED_EXCEPTION::NOT_ALLOC_MEMORY);
	ListStrings.Init(5);

	NullAsString = RegisterString("null");
	EXECUTE_CONTEXT::MainConstScope.WriteElement(NullAsString);

	EmptyString = RegisterString("");
	EXECUTE_CONTEXT::MainConstScope.WriteElement(EmptyString);

	ObjectAsString = RegisterString("[Object]");
	EXECUTE_CONTEXT::MainConstScope.WriteElement(ObjectAsString);

	NumberAsString = RegisterString("[Number]");
	EXECUTE_CONTEXT::MainConstScope.WriteElement(NumberAsString);

	StringAsString = RegisterString("[String]");
	EXECUTE_CONTEXT::MainConstScope.WriteElement(StringAsString);

	FunctionAsString = RegisterString("[Function]");
	EXECUTE_CONTEXT::MainConstScope.WriteElement(FunctionAsString);

	ExceptionHandlerAsString = RegisterString("[ExceHandler]");
	EXECUTE_CONTEXT::MainConstScope.WriteElement(ExceptionHandlerAsString);

	Name = RegisterString("String");

	EXECUTE_CONTEXT::MainConstScope.WriteElement(Name);

	EXECUTE_CONTEXT::FunctionClass.Name = RegisterString("Function");
	EXECUTE_CONTEXT::MainConstScope.WriteElement(EXECUTE_CONTEXT::FunctionClass.Name);

	EXECUTE_CONTEXT::MainConstScope.Name = RegisterString("__ConstScope");
	EXECUTE_CONTEXT::MainConstScope.WriteElement(EXECUTE_CONTEXT::MainConstScope.Name);

	STRING_CLASS::InitDefaultMethods(&EXECUTE_CONTEXT::FunctionClass);
}

void STRING_CLASS::InitDefaultMethods(LPMETHOD_CLASS RegClass)
{
	AddMember("Cat", RegClass->RegisterMethod(Cat));
	AddMember("IndexOf", RegClass->RegisterMethod(IndexOf));
}

void STRING_CLASS::AddMember(LPINTERNAL_CHAR Name, INSIDE_DATA Val)
{
	LPHEADER_STRING NameMethod = RegisterString(Name);
	INSIDE_DATA Data(NameMethod, this);
	WriteMember(OBJECT::Null, &Data, &Val);
}

LPHEADER_STRING STRING_CLASS::TypeOf(LPINSIDE_DATA Type)
{
	switch(Type->TypeData)
	{
	case OBJECT_CASE:
		return RegisterCommonString(Type->Class->Name);
	case INSIDE_DATA::TYPEDATA_EXCEPT:
		return ExceptionHandlerAsString;
	case INSIDE_DATA::TYPEDATA_NULL:
		return NullAsString;
	case INSIDE_DATA::TYPEDATA_INTEGER:
	default:
		return NumberAsString;
	}
}

LPHEADER_STRING STRING_CLASS::ToString(LPINSIDE_DATA Source)
{
	INTERNAL_CHAR Buf[15];
	TSIZE_STR LenBuf;
	switch(Source->TypeData)
	{
	case INSIDE_DATA::TYPEDATA_NULL:
		return NullAsString;
	case INSIDE_DATA::TYPEDATA_INTEGER:
		LenBuf = NumberToString(Source->Integer, Buf, sizeof(Buf) / sizeof(INTERNAL_CHAR)).Result;
		break;
	case INSIDE_DATA::TYPEDATA_STRING:
		return (LPHEADER_STRING)Source->Object;
	case INSIDE_DATA::TYPEDATA_OBJECT:
		return ObjectAsString;
	case INSIDE_DATA::TYPEDATA_EXCEPT:
		return ExceptionHandlerAsString;
	default://double
		LenBuf = NumberToString(Source->Double, Buf, sizeof(Buf) / sizeof(INTERNAL_CHAR)).Result;
	}
	return OBJECT::New(this, RegisterString(Buf, LenBuf));
};

LPINTERNAL_CHAR STRING_CLASS::ToString(LPINSIDE_DATA Source,LPINTERNAL_CHAR Buf, LPTSIZE_STR Len)
{
	LPINTERNAL_CHAR EndChar;
	switch(Source->TypeData)
	{
	case INSIDE_DATA::TYPEDATA_NULL:
		*Len = NullAsString->Len;
		return NullAsString->Str;
	case INSIDE_DATA::TYPEDATA_INTEGER:
		EndChar = Buf + NumberToString(Source->Integer, Buf, *Len);
		break;
	case INSIDE_DATA::TYPEDATA_STRING:
		*Len = ((LPHEADER_STRING)Source->Object)->Len;
		return ((LPHEADER_STRING)Source->Object)->Str;
	case INSIDE_DATA::TYPEDATA_OBJECT:
		*Len = ObjectAsString->Len;
		return ObjectAsString->Str;
	case INSIDE_DATA::TYPEDATA_EXCEPT:
		*Len = ExceptionHandlerAsString->Len;
		return ExceptionHandlerAsString->Str;
	default:
		EndChar = Buf + NumberToString(Source->Double, Buf, *Len).Result;
		break;
	}
	*Len = (unsigned)(EndChar - Buf) / sizeof(INTERNAL_CHAR);
	return Buf;
};


INSIDE_DATA STRING_CLASS::CreateInstance(LPEXECUTE_CONTEXT Context, LPARG_FUNC Arg)
{
	if(Arg->CountArg == 0)
		return OBJECT::New(this, EmptyString);

	LPINSIDE_DATA Source = Context->GetArg(Arg->Args[0]);
	INTERNAL_CHAR Buf[15];
	LPINTERNAL_CHAR EndChar;

	switch(Source->TypeData)
	{
	case INSIDE_DATA::TYPEDATA_NULL:
		return OBJECT::New(this, NullAsString);
	case INSIDE_DATA::TYPEDATA_INTEGER:
		EndChar = Buf + NumberToString(Source->Integer, Buf, sizeof(Buf) / sizeof(INTERNAL_CHAR)).Result;
		break;
	case INSIDE_DATA::TYPEDATA_STRING:
		return Source->Object;
	case INSIDE_DATA::TYPEDATA_OBJECT:
		return OBJECT::New(this,ObjectAsString);
	case INSIDE_DATA::TYPEDATA_EXCEPT:
		return OBJECT::New(this,ExceptionHandlerAsString);
	default:
		EndChar = Buf + NumberToString(Source->Double,  Buf, sizeof(Buf) / sizeof(INTERNAL_CHAR)).Result;
	}
	return OBJECT::New(this, RegisterString(Buf));
}

void STRING_CLASS::FreeAllUnusedInstance()
{
	ListStrings.EnumDelete(
			[](void* CheckUses, STRING_CLASS::_HASH_ELEM_STRING* Element) -> bool
			{
				if(Element->Val->Uses != (CHECK_USES)CheckUses)
				{
					MEM_FREE(Element->Val);
					return true;
				}
				return false;
			}, 
			(void*)CurCheckUses);
	if(!HASH_STRINGS::ResizeAfterRemove(ListStrings))
		THROW_UNHANDLED_EXCEPTION("Not realloc common string table.", UNHANDLED_EXCEPTION::NOT_REALLOC);
};


ZELLI_DOUBLE STRING_CLASS::OperatorToDouble(INSTANCE_CLASS Object)
{
	ZELLI_DOUBLE Result;
	StringToNumber(&Result, ((LPHEADER_STRING)Object)->Str, ((LPHEADER_STRING)Object)->Len);
	return Result;
}

ZELLI_INTEGER STRING_CLASS::OperatorToInt(INSTANCE_CLASS Object)
{
	ZELLI_INTEGER Result;
	StringToNumber(&Result, ((LPHEADER_STRING)Object)->Str, ((LPHEADER_STRING)Object)->Len);
	return Result;
}

LPHEADER_STRING STRING_CLASS::RegisterCommonString(LPHEADER_STRING String)
{
	if(!HASH_STRINGS::ResizeBeforeInsert(ListStrings))
		THROW_UNHANDLED_EXCEPTION("Not alloc memory for new string.", UNHANDLED_EXCEPTION::NOT_ALLOC_MEMORY);
	auto NewHeader = ListStrings.Insert(String)->Val;
	NewHeader->Uses = CurCheckUses;
	return NewHeader;
}

LPHEADER_STRING STRING_CLASS::RegisterString(LPHEADER_STRING String)
{
	if(!HASH_STRINGS::ResizeBeforeInsert(ListStrings))
		THROW_UNHANDLED_EXCEPTION("Not alloc memory for new string.", UNHANDLED_EXCEPTION::NOT_ALLOC_MEMORY);
	auto NewHeader = ListStrings.Insert(String)->Val;
	NewHeader->Uses = CurCheckUses;
	return NewHeader;
}

LPHEADER_STRING STRING_CLASS::RegisterString(LPINTERNAL_CHAR String)
{
	if(!HASH_STRINGS::ResizeBeforeInsert(ListStrings))
		THROW_UNHANDLED_EXCEPTION("Not alloc memory for new string.", UNHANDLED_EXCEPTION::NOT_ALLOC_MEMORY);
	TSIZE_STR Len;
	HASH_VAL Hash = HEADER_STRING::Hash(String, &Len);
	_FOR_SET_ROW_STRING v = { String, Len, Hash};
	auto NewHeader = ListStrings.Insert(&v)->Val;
	NewHeader->Uses = CurCheckUses;
	return NewHeader;
}

LPHEADER_STRING STRING_CLASS::RegisterString(LPINTERNAL_CHAR String, TSIZE_STR Len)
{
	if(!HASH_STRINGS::ResizeBeforeInsert(ListStrings))
		THROW_UNHANDLED_EXCEPTION("Not alloc memory for new string.", UNHANDLED_EXCEPTION::NOT_ALLOC_MEMORY);
	_FOR_SET_ROW_STRING v = { String, Len, HEADER_STRING::Hash(String, Len)};
	auto NewHeader = ListStrings.Insert(&v)->Val;
	NewHeader->Uses = CurCheckUses;
	return NewHeader;
}

LPHEADER_STRING STRING_CLASS::IsHaveString(LPINTERNAL_CHAR String)
{
	TSIZE_STR Len;
	HASH_VAL Hash = HEADER_STRING::Hash(String, &Len);
	_FOR_SET_ROW_STRING v = { String, Len, Hash};
	auto Cell = ListStrings.Search(&v);
	if(Cell == nullptr)
		return nullptr;
	return Cell->Val;
}

LPHEADER_STRING STRING_CLASS::IsHaveString(LPHEADER_STRING String)
{
	auto Cell = ListStrings.Search(String);
	if(Cell == nullptr)
		return nullptr;
	return Cell->Val;
}

void STRING_CLASS::MarkAsUsed() { CLASS_SCOPE::MarkAllMembers(); }

void STRING_CLASS::MarkAsUsed(INSTANCE_CLASS Object) { ((LPHEADER_STRING)Object)->Uses = CurCheckUses; }

ZELLI_INTEGER STRING_CLASS::GetLength(INSTANCE_CLASS Object) { return ((LPHEADER_STRING)Object)->Len; }

INSIDE_DATA STRING_CLASS::OperatorAdd(INSTANCE_CLASS Object, const LPINSIDE_DATA Var2)
{
	INTERNAL_CHAR Buffer2[10];
	TSIZE_STR CountInBuff1, CountInBuff2;
	LPINTERNAL_CHAR Str1 = ((LPHEADER_STRING)Object)->Str;
	CountInBuff1 = ((LPHEADER_STRING)Object)->Len;
	LPINTERNAL_CHAR Str2 = ToString(Var2,Buffer2,&CountInBuff2);
	HASH_VAL Hash = 0;
	Hash = HEADER_STRING::Hash(Hash, Str1, CountInBuff1);
	Hash = HEADER_STRING::Hash(Hash, Str2, CountInBuff2);
	_FOR_CAT fc = { Str1, Str2, CountInBuff1, CountInBuff2, Hash};
	if(!HASH_STRINGS::ResizeBeforeInsert(ListStrings))
		THROW_UNHANDLED_EXCEPTION("Not alloc memory for new string.", UNHANDLED_EXCEPTION::NOT_ALLOC_MEMORY);
	auto NewHeader = ListStrings.Insert(&fc)->Val;
	NewHeader->Uses = CurCheckUses;
	return OBJECT::New(this, NewHeader);
}

//Стандартные методы класса String

INSIDE_DATA ZELLI_API STRING_CLASS::Cat(LPINSIDE_DATA ThisStr, LPEXECUTE_CONTEXT CurContext, LPARG_FUNC Arg)
{
	INTERNAL_CHAR Buffer2[10];
	TSIZE_STR CountInBuff1, CountInBuff2;
	if(!ThisStr->IsString && (Arg->CountArg == 0))
		return INSIDE_DATA::Null;
	LPINSIDE_DATA Var2 = CurContext->GetArg(Arg->Args[0]);
	LPSTRING_CLASS CurClass = ThisStr->Class;

	LPINTERNAL_CHAR Str1 = ((LPHEADER_STRING)ThisStr->Object)->Str;
	CountInBuff1 = ((LPHEADER_STRING)ThisStr->Object)->Len;
	LPINTERNAL_CHAR Str2 = CurClass->ToString(Var2, Buffer2, &CountInBuff2);
	
	//Последовательно хешируем 2 строки
	HASH_VAL Hash = 0;
	Hash = HEADER_STRING::Hash(Hash, Str1, CountInBuff1);
	Hash = HEADER_STRING::Hash(Hash, Str2, CountInBuff2);
	_FOR_CAT fc = { Str1, Str2, CountInBuff1, CountInBuff2, Hash};
	if(!HASH_STRINGS::ResizeBeforeInsert(CurClass->ListStrings))
		THROW_UNHANDLED_EXCEPTION("Not alloc memory for new string.", UNHANDLED_EXCEPTION::NOT_ALLOC_MEMORY);
	auto NewHeader = CurClass->ListStrings.Insert(&fc)->Val;
	NewHeader->Uses = CurClass->CurCheckUses;
	return OBJECT::New(CurClass, NewHeader);
}

INSIDE_DATA ZELLI_API STRING_CLASS::IndexOf(LPINSIDE_DATA ThisStr, LPEXECUTE_CONTEXT CurContext, LPARG_FUNC Arg)
{
	INTERNAL_CHAR Buffer2[10];
	TSIZE_STR CountInBuff1, CountInBuff2;
	if(!ThisStr->IsString || (Arg->CountArg == 0))
		return -1;
	LPINSIDE_DATA Var2 = CurContext->GetArg(Arg->Args[0]);
	LPSTRING_CLASS CurClass = ThisStr->Class;

	LPINTERNAL_CHAR Str1 = ((LPHEADER_STRING)ThisStr->Object)->Str;
	CountInBuff1 = ((LPHEADER_STRING)ThisStr->Object)->Len;

	LPINTERNAL_CHAR Str2 = CurClass->ToString(Var2,Buffer2,&CountInBuff2);
	LPINTERNAL_CHAR Result = (LPINTERNAL_CHAR)MemSrch(Str1, CountInBuff1, Str2, CountInBuff2);
	if(Result == nullptr)
		return -1;
	return (ZELLI_INTEGER)(((unsigned)Result - (unsigned)Str1) / sizeof(INTERNAL_CHAR));
}


INSIDE_DATA STRING_CLASS::Clone(INSTANCE_CLASS ThisObject) { return OBJECT::New(this, ThisObject); }

bool STRING_CLASS::OperatorEq(INSTANCE_CLASS ThisObj, LPINSIDE_DATA SecondVal)
{
	if(SecondVal->IsString)
		return *(LPHEADER_STRING)ThisObj == *(LPHEADER_STRING)SecondVal->Object;
	return false;
}

bool STRING_CLASS::OperatorNeq(INSTANCE_CLASS ThisObj, LPINSIDE_DATA SecondVal)
{
	if(SecondVal->IsString)
		return *(LPHEADER_STRING)ThisObj != *(LPHEADER_STRING)SecondVal->Object;
	return true;
}

inline STRING_CLASS::INDEX_OBJ_ELEMENT STRING_CLASS::_HASH_ELEM_STRING::IndexByKey(const LPHEADER_STRING k, INDEX_OBJ_ELEMENT MaxCount) { return k->Key % MaxCount; }

inline STRING_CLASS::INDEX_OBJ_ELEMENT STRING_CLASS::_HASH_ELEM_STRING::IndexByKey(STRING_CLASS::_FOR_SET_ROW_STRING* k, INDEX_OBJ_ELEMENT MaxCount) { return k->Hash % MaxCount; }

inline STRING_CLASS::INDEX_OBJ_ELEMENT STRING_CLASS::_HASH_ELEM_STRING::IndexByKey(STRING_CLASS::_FOR_CAT* k, INDEX_OBJ_ELEMENT MaxCount) {  return k->Hash % MaxCount; }

inline bool STRING_CLASS::_HASH_ELEM_STRING::CmpKey(STRING_CLASS::_FOR_SET_ROW_STRING* EnotherKeyVal) 
{ 
	if(Val->Len == EnotherKeyVal->Len) 
		return StringCompare(Val->Str, EnotherKeyVal->Str, Val->Len) == 0;
	return false;
}

inline bool STRING_CLASS::_HASH_ELEM_STRING::CmpKey(STRING_CLASS::_FOR_CAT* EnotherKeyVal) 
{ 
	if((Val->Key == EnotherKeyVal->Hash) && (Val->Len == (EnotherKeyVal->Len1 + EnotherKeyVal->Len2))) 
	{
		if((memcmp(EnotherKeyVal->Str1, Val->Str, EnotherKeyVal->Len1) == 0) && 
		(memcmp(EnotherKeyVal->Str2, Val->Str + EnotherKeyVal->Len1, EnotherKeyVal->Len2) == 0))
		return true;
	}
	return false;
}

bool STRING_CLASS::_HASH_ELEM_STRING::SetKey(STRING_CLASS::_FOR_SET_ROW_STRING* k) 
{ 
	LPHEADER_STRING NewHeader = HEADER_STRING::AllocNew(k->Len);
	if(!NewHeader)
		THROW_UNHANDLED_EXCEPTION("Not alloc memory for new string.", UNHANDLED_EXCEPTION::NOT_ALLOC_MEMORY);
	memcpy(NewHeader->Str, k->Str, NewHeader->Len = k->Len);
	NewHeader->Key = k->Hash;
	Val = NewHeader;
	return true;
}


bool STRING_CLASS::_HASH_ELEM_STRING::SetKey(STRING_CLASS::_FOR_CAT* k) 
{ 
	LPHEADER_STRING NewHeader = HEADER_STRING::AllocNew(k->Len1 + k->Len2);
	if(!NewHeader)
		THROW_UNHANDLED_EXCEPTION("Not alloc memory for new string.", UNHANDLED_EXCEPTION::NOT_ALLOC_MEMORY);
	memcpy(NewHeader->Str, k->Str1, k->Len1);
	memcpy(NewHeader->Str + k->Len1, k->Str2, k->Len2);
	NewHeader->Len = k->Len1 + k->Len2;
	NewHeader->Key = k->Hash;
	Val = NewHeader;
	return true;
}

LPHEADER_STRING HEADER_STRING::AllocNew(TSIZE_STR SizeAllocString)
{
	unsigned NewSize = OFFSET_FIELD(HEADER_STRING, Str) + SizeAllocString;
	void * v;
	if((v = MEM_ALLOC(NewSize)) == NULL)
	{
		if(EXECUTE_CONTEXT::StaticCleaning())
			return  (LPHEADER_STRING)MEM_ALLOC(NewSize);
		else
			return NULL;
	}
	return (LPHEADER_STRING)v;
} 

SIZE_STR STRING_CLASS::InfoObject(INSTANCE_CLASS Obj, LPINTERNAL_CHAR Buffer, SIZE_STR LenInBuf)
{
	HEADER_STRING * Hs = (LPHEADER_STRING)Obj;
	return toz(sprintf_s(Buffer, LenInBuf, "STRING: \"%.*s\" HASH: %u", Hs->Len, Hs->Str, Hs->Key));
}

SIZE_STR STRING_CLASS::InfoClass(LPINTERNAL_CHAR Buffer, SIZE_STR LenInBuf)
{
	SIZE_STR CurLen = LenInBuf, Len2;
	LPINTERNAL_CHAR Cur = Buffer;

	Len2 = toz(sprintf_s(Buffer, CurLen, "%.*s\n", Name->Len, Name->Str));
	CurLen -= Len2;
	Buffer += Len2;

	Len2 = ListStrings.QualityInfo(Buffer, CurLen);
	CurLen -= Len2;
	Buffer += Len2;
	Len2 = toz(sprintf_s(Buffer, CurLen, "\nAll elements:\n"));
	CurLen -= Len2;
	Buffer += Len2;

	for(HASH_STRINGS::LPCELL Cur = ListStrings.GetTable(), Max = Cur + ListStrings.MaxCount;Cur < Max;Cur++)
		for(HASH_STRINGS::TINDEX p = Cur->iStart; p != HASH_STRINGS::EmptyElement;p = ListStrings[p]->iNext)
		{
			Len2 = InfoObject(ListStrings[p]->Val,Buffer, CurLen);
			CurLen -= Len2;
			Buffer += Len2;
			Len2 = toz(sprintf_s(Buffer, CurLen, "\n"));
			CurLen -= Len2;
			Buffer += Len2;
		}

	return LenInBuf - CurLen;
}