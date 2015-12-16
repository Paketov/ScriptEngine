#ifndef __STRING_H__
#define __STRING_H__

#include "HashTable.h"
#include "Object.h"
#include "AdditionalLib.h"
#include "Class.h"
#include "ClassScope.h"

class STRING_CLASS: virtual public HEADER_CLASS, public CLASS_SCOPE
{
protected:

	friend HEADER_STRING;
	friend EXECUTE_CONTEXT;
	typedef unsigned short INDEX_OBJ_ELEMENT, *LPINDEX_OBJ_ELEMENT;

	struct _FOR_SET_ROW_STRING
	{
	    LPINTERNAL_CHAR Str;
		TSIZE_STR		Len;
		HASH_VAL        Hash;
	};

	struct _FOR_CAT
	{
	    LPINTERNAL_CHAR Str1;
		LPINTERNAL_CHAR Str2;
		TSIZE_STR		Len1;
		TSIZE_STR		Len2;
		HASH_VAL        Hash;
	};

	struct _HASH_ELEM_STRING
	{
		LPHEADER_STRING Val;
		inline INDEX_OBJ_ELEMENT IndexInBound(INDEX_OBJ_ELEMENT MaxCount) { return IndexByKey(Val, MaxCount); }

		inline bool SetKey(LPHEADER_STRING k) { Val = k; return true; }
		bool SetKey(_FOR_SET_ROW_STRING* k);
		bool SetKey(_FOR_CAT* k);

		inline static INDEX_OBJ_ELEMENT IndexByKey(const LPHEADER_STRING k, INDEX_OBJ_ELEMENT MaxCount);
		inline static INDEX_OBJ_ELEMENT IndexByKey(_FOR_SET_ROW_STRING* k, INDEX_OBJ_ELEMENT MaxCount);
		inline static INDEX_OBJ_ELEMENT IndexByKey(_FOR_CAT* k, INDEX_OBJ_ELEMENT MaxCount);

		inline bool CmpKey(const LPHEADER_STRING EnotherKeyVal) const { return Val == EnotherKeyVal; }
		inline bool CmpKey(_FOR_SET_ROW_STRING* EnotherKeyVal);
		inline bool CmpKey(_FOR_CAT* EnotherKeyVal);
	};

	typedef HASH_TABLE<_HASH_ELEM_STRING, false, INDEX_OBJ_ELEMENT> HASH_STRINGS, *LPHASH_STRINGS;

	HASH_STRINGS            ListStrings;	//Общий список строк
	CHECK_USES			    CurCheckUses;	//Для сборщика мусора

	void InitDefaultMethods(LPMETHOD_CLASS);

public:

	virtual INSIDE_DATA CreateInstance(LPEXECUTE_CONTEXT, LPARG_FUNC);

	//Garbage Collector

	virtual void SetAllInstanceToUnused() { CurCheckUses++; }

	virtual void MarkAsUsed(INSTANCE_CLASS);

	virtual void MarkAsUsed();

	virtual void FreeAllUnusedInstance();

	virtual INSIDE_DATA OperatorAdd(INSTANCE_CLASS, const LPINSIDE_DATA);

	virtual ZELLI_INTEGER GetLength(INSTANCE_CLASS);

	virtual bool OperatorEq(INSTANCE_CLASS, const LPINSIDE_DATA);

	virtual ZELLI_DOUBLE OperatorToDouble(INSTANCE_CLASS);

	virtual ZELLI_INTEGER OperatorToInt(INSTANCE_CLASS);

	virtual INSIDE_DATA Clone(INSTANCE_CLASS);

	virtual bool OperatorNeq(INSTANCE_CLASS, const LPINSIDE_DATA);

	virtual SIZE_STR InfoObject(INSTANCE_CLASS, LPINTERNAL_CHAR, SIZE_STR);

	virtual SIZE_STR InfoClass(LPINTERNAL_CHAR, SIZE_STR);

	void AddMember(LPINTERNAL_CHAR Name, INSIDE_DATA Val);

	//Стандартные константные строки
	LPHEADER_STRING NullAsString;
	LPHEADER_STRING ObjectAsString;
	LPHEADER_STRING EmptyString;
	LPHEADER_STRING NumberAsString;
	LPHEADER_STRING StringAsString;
	LPHEADER_STRING FunctionAsString;
	LPHEADER_STRING ExceptionHandlerAsString;
	LPHEADER_STRING InitConstDefinitions();

	static INSIDE_DATA ZELLI_API Cat(LPINSIDE_DATA ThisStr, LPEXECUTE_CONTEXT CurContext, LPARG_FUNC Arg);

	static INSIDE_DATA ZELLI_API IndexOf(LPINSIDE_DATA ThisStr, LPEXECUTE_CONTEXT CurContext, LPARG_FUNC Arg);

	/*
	Регистрирование статической строки для STRING_CLASS
	*/
	LPHEADER_STRING RegisterCommonString(LPHEADER_STRING String);

	LPHEADER_STRING RegisterString(LPHEADER_STRING String);

	LPHEADER_STRING RegisterString(LPINTERNAL_CHAR String);

	LPHEADER_STRING RegisterString(LPINTERNAL_CHAR String, TSIZE_STR Len);

	LPHEADER_STRING IsHaveString(LPINTERNAL_CHAR TestString);

	LPHEADER_STRING IsHaveString(LPHEADER_STRING TestString);

	LPHEADER_STRING TypeOf(LPINSIDE_DATA Type);

	LPHEADER_STRING ToString(LPINSIDE_DATA Source);

	LPINTERNAL_CHAR ToString(LPINSIDE_DATA Source, LPINTERNAL_CHAR Buf, LPTSIZE_STR Len);

	STRING_CLASS();

	inline void * operator new(size_t Size) { return MEM_ALLOC(Size); }
};

//Заголовок строки
class HEADER_STRING
{
public:
	typedef HASH_VAL	TKEY, *LPTKEY;

	CHECK_USES			Uses;
	HASH_VAL			Key;		//Hash code
	TSIZE_STR			Len;		//Length
	INTERNAL_CHAR		Str[1];		//String pascal type

	#define STATIC_INIT_STR(Str) {1,/*HEADER_STRING::Hash(Str,sizeof(Str))*/0,sizeof(Str),Str}

	template<const TSIZE_STR LenStr = 1>
	struct STATIC_INIT
	{
		unsigned char	Flags;
		HASH_VAL		Key;
		TSIZE_STR		Len;
		INTERNAL_CHAR	Str[LenStr];

		inline LPHEADER_STRING operator &() { return (LPHEADER_STRING)this; }

		inline operator HEADER_STRING &() { return *(LPHEADER_STRING)this; }

		inline LPHEADER_STRING operator ->() { return (LPHEADER_STRING)this; }
	}; 

	inline static unsigned AllocSize(TSIZE_STR LenAllocString) { return OFFSET_FIELD(HEADER_STRING, Str) + sizeof(INTERNAL_CHAR) * LenAllocString; }

	inline static unsigned AllocSizeBySizeStr(unsigned SizeAllocString) { return OFFSET_FIELD(HEADER_STRING, Str) + SizeAllocString; } 

	static LPHEADER_STRING AllocNew(TSIZE_STR SizeAllocString);

	inline LPHEADER_STRING Clone()
	{
		LPHEADER_STRING NewStr = AllocNew(Len);
		if(NewStr == NULL)
			return NULL;
		memcpy(NewStr, this, OFFSET_FIELD(HEADER_STRING, Str) + Len);
		return NewStr;
	}

	inline void Rehash()
	{
		register HASH_VAL h = 0;
		for (register LPINTERNAL_CHAR end = Str + Len,s = Str; s < end; s++) 
			h = 31 * h + (unsigned char)*s;
		Key = h;
	}

	static inline HASH_VAL Hash(HASH_VAL PrevId, LPINTERNAL_CHAR Str, unsigned Len)
	{
		for (LPINTERNAL_CHAR end = Str + Len,s = Str; s < end; s++) 
			PrevId = 31 * PrevId + (unsigned char)*s;
		return PrevId;
	}

	static inline HASH_VAL Hash(LPINTERNAL_CHAR Str, unsigned Len)
	{
		HASH_VAL h = 0;
		for (LPINTERNAL_CHAR end = Str + Len,s = Str; s < end; s++) 
			h = 31 * h + (unsigned char)*s;
		return h;
	}

	static inline HASH_VAL Hash(LPINTERNAL_CHAR Str, LPINTERNAL_CHAR End)
	{
		HASH_VAL h = 0;
		for (LPINTERNAL_CHAR s = Str; s < End; s++) 
			h = 31 * h + (unsigned char)*s;
		return h;
	}

	static inline HASH_VAL Hash(LPINTERNAL_CHAR Str)
	{
		HASH_VAL h = 0;
		for (LPINTERNAL_CHAR s = Str; *s; s++) 
			h = 31 * h + (unsigned char)*s;
		return h;
	}

	static inline HASH_VAL Hash(LPINTERNAL_CHAR Str, LPTSIZE_STR Len)
	{
		HASH_VAL h = 0;
		register LPINTERNAL_CHAR s = Str;
		for (; *s; s++) 
			h = 31 * h + (unsigned char)*s;
		*Len = (TSIZE_STR)((unsigned)s - (unsigned)Str);
		return h;
	}

	static inline HASH_VAL Hash(LPINTERNAL_CHAR Str, LPINTERNAL_CHAR * End)
	{
		HASH_VAL h = 0;
		register LPINTERNAL_CHAR s = Str;
		for (; *s; s++) 
			h = 31 * h + (unsigned char)*s;
		*End = s;
		return h;
	}

	inline unsigned Size()
	{
		return (sizeof(HEADER_STRING) - sizeof(INTERNAL_CHAR)) + sizeof(INTERNAL_CHAR) * Len;
	}

	INTERNAL_CHAR operator[](const TSIZE_STR IndexChar)
	{
		return Str[IndexChar];
	}

	inline bool Cmp(const LPINTERNAL_CHAR Val, const HASH_VAL Hash)
	{
		if(Key != Hash)
			return false;
		LPINTERNAL_CHAR ic = Val, tc = Str;
		for(;*tc == *ic; tc++, ic++)
			if(*ic == '\0')
				return true;
		return *ic == '\0';
	}

	inline bool operator==(LPINTERNAL_CHAR Val)
	{
		LPINTERNAL_CHAR ic = Val, tc = Str;
		for(;*tc == *ic;tc++, ic++)
			if(*ic == '\0')
				return true;
		return *ic == '\0';
	}

	//inline unsigned int HashString(char* s, unsigned len) 
	//{
	//	unsigned int h = 0; 
	//	for (l1=l; l1>=step; l1-=step)  /* compute hash */
	//    h = h ^ ((h<<5) + (h>>2) + (unsigned char)str[l1-1]);
	//	return h;
	//}
};


inline bool operator==(const HEADER_STRING & Str, const HEADER_STRING & CmpStr)
{
	if(&Str == &CmpStr)
		return true;
	if((CmpStr.Key == Str.Key) && (CmpStr.Len == Str.Len))
		return memcmp(CmpStr.Str, Str.Str, CmpStr.Len) == 0;
	return false;
}

inline bool operator!=(const HEADER_STRING & Str, const HEADER_STRING & CmpStr)
{
	if(&Str != &CmpStr)
		return true;
	if((CmpStr.Key == Str.Key) && (CmpStr.Len == Str.Len))
		return memcmp(CmpStr.Str, Str.Str, CmpStr.Len) != 0;
	return true;
}

#endif