#ifndef _TYPEDATA_H_
#define _TYPEDATA_H_
//Определения всех кодов типов данных
#include <string.h>
#include <string>
#include <type_traits>
#define ENABLE_DOUBLE_LONG_TYPEDATA
#define SKIP_BYTES(Count, Ident)	unsigned char __skiping_array__ ## Ident[Count]
#define OFFSET_FIELD(Type, Field)   ((unsigned)&((Type*)0)->Field)
inline unsigned toz(int a) { return (a > 0)?a:0; }


#define GOLDEN_RATIO 1.61803398875f

#if defined(WIN32)
#include <windows.h>
#define MEM_ALLOC_PAGE(size)		VirtualAlloc(NULL,size, MEM_COMMIT, PAGE_READWRITE)
#define MEM_FREE_PAGE(ptr)			VirtualFree(ptr, 0,MEM_RELEASE)
#define DYNAMIC_MEMORY_GET_BLOCK(size)	MEM_ALLOC_PAGE(size)
#define DYNAMIC_MEMORY_FREE_BLOCK(ptr)	MEM_FREE_PAGE(ptr)
extern HANDLE  ProcessHeapHandle;
#define MEM_ALLOC(Size)				HeapAlloc(GetProcessHeap(),0, Size)
#define MEM_ALLOC_ZERO(Size)        HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY, Size)
#define MEM_REALLOC(Ptr, NewSize)	HeapReAlloc(GetProcessHeap(), 0, Ptr, NewSize)
#define MEM_ZERO_INIT_ALLOC(Size)	HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY, Size)
#define MEM_FREE(Ptr)				HeapFree(GetProcessHeap(), 0, Ptr)
#define THREAD_DATA					__declspec(thread)
#elif defined(POSIX) 
#include <sys/mman.h>
#define MEM_ALLOC_PAGE(size)		mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
#define MEM_ALLOC(Size)				malloc(Size)
#define MEM_REALLOC(Ptr, NewSize)	realloc(Ptr,NewSize)
#define MEM_ZERO_INIT_ALLOC(Size)	calloc(1, Size)
#define MEM_FREE(Ptr)				free(Ptr)
#endif

#ifdef FAST_THROW
#define SET_EXCEPT				if(__SetExept()){
#define CATCH_EXCEPT			;POP_EXCEPT;}else{
#define END_EXCEPT				}
#define POP_EXCEPT				__PopExcept()
#define HAS_THROW
#else
#ifdef _DEBUG
#define THROW_NATIVE_EXCEPT(Description, Id) {\
												static const HEADER_STRING::STATIC_INIT<sizeof(Description)> s = STATIC_INIT_STR(Description);\
												static HEADER_STRING::STATIC_INIT<sizeof(__FILE__)> f = STATIC_INIT_STR(__FILE__);\
												static const HEADER_EXCEPTION::STATIC_INIT e = {Id, &s, &f, __LINE__};\
												throw (const LPHEADER_EXCEPTION)&e;\
											 }
#define THROW_UNHANDLED_EXCEPTION(Description, Id) {\
												static const UNHANDLED_EXCEPTION e = {Id, Description, __FILE__, __LINE__};\
												throw &e;\
													}
#else

#define THROW_NATIVE_EXCEPT(Description, Id) {\
												static const HEADER_STRING::STATIC_INIT<sizeof(Description)> s = STATIC_INIT_STR(Description);\
												static const HEADER_EXCEPTION::STATIC_INIT e = {Id, &s};\
												throw (const LPHEADER_EXCEPTION)&e;\
											 }
#define THROW_UNHANDLED_EXCEPTION(Description, Id) {\
												static const UNHANDLED_EXCEPTION e = {Id, Description};\
												throw &e;\
													}
#endif
#define SET_EXCEPT				try{	
#define CATCH_EXCEPT			}catch(...){
#define END_EXCEPT				}
#define POP_EXCEPT
// #define HAS_THROW throw(...)
// #define HAS_THROW throw(const LPHEADER_EXCEPTION)
// #define HAS_THROW throw(const LPHEADER_EXCEPTION, const LPUNHANDLED_EXCEPTION)
#define HAS_THROW
#endif

class						EXECUTE_CONTEXT;
class						CLASS_MANAGER;
class						STACK_LOCAL_VAR;
class						OPCODE;
class						BYTECODE_FILE;
class						TRANSLATOR;
struct						INSIDE_DATA;		//Нестрого типизированный тип данных с заголовком типа данных
struct						INTERATOR;
struct						INSIDE_POINTER_INTERATOR;
//Заголовки динамических данных
class						HEADER_STRING;
class						HEADER_METHOD;
class						HEADER_CLASS;
class						HEADER_EXCEPTION;
class						EXEPTION_CLASS;


typedef						HEADER_EXCEPTION *LPHEADER_EXCEPTION;
typedef						EXEPTION_CLASS *LPEXEPTION_CLASS;
class						GLOBAL_SCOPE_INSTANCE;
class						GLOBAL_SCOPE_CLASS;
typedef						GLOBAL_SCOPE_INSTANCE *LPGLOBAL_SCOPE_INSTANCE;
typedef						GLOBAL_SCOPE_CLASS  *LPGLOBAL_SCOPE_CLASS;

struct						ZELLI_EXCEPT_HANDLER;
struct						UNHANDLED_EXCEPTION;
typedef UNHANDLED_EXCEPTION LPUNHANDLED_EXCEPTION;

typedef int 				ZELLI_INTEGER, *LPZELLI_INTEGER;
typedef short				ZELLI_INTEGER16, *LPZELLI_INTEGER16;
typedef char				ZELLI_INTEGER8, *LPZELLI_INTEGER8;

typedef double				ZELLI_DOUBLE, *LPZELLI_DOUBLE;
typedef float				ZELLI_FLOAT,  *LPZELLI_FLOAT;

//Скриптовый тип данных
typedef BYTECODE_FILE		*LPBYTECODE_FILE;

typedef HEADER_METHOD		*LPHEADER_METHOD;

typedef INTERATOR			*LPINTERATOR;

typedef EXECUTE_CONTEXT		*LPEXECUTE_CONTEXT;
typedef STACK_LOCAL_VAR		*LPSTACK_LOCAL_VAR;
typedef CLASS_MANAGER		*LPCLASS_MANAGER;
typedef INSIDE_POINTER_INTERATOR *LPINSIDE_POINTER_INTERATOR;
typedef char				INTERNAL_CHAR, *LPINTERNAL_CHAR;
typedef char				EXTERNAL_CHAR, *LPEXTERNAL_CHAR;
typedef unsigned char		TYPE_INTERNAL_DATA;
typedef unsigned int		SIZE_STR;
typedef OPCODE				*LPOPCODE;

typedef void				*INSTANCE_CLASS;
typedef	HEADER_CLASS		*LPHEADER_CLASS;

typedef INSIDE_DATA			*LPINSIDE_DATA;

typedef std::conditional<bool(sizeof(void*) > 4), unsigned long long, unsigned>::type PLATFORM_WORD;
typedef PLATFORM_WORD HASH_VAL; 

typedef unsigned short		NUMBER_VAR, *LPNUMBER_VAR;
typedef unsigned char		SHORT_NUMBER_VAR,*LPSHORT_NUMBER_VAR;
typedef unsigned short		NUMBER_ARG,	*LPNUMBER_ARG;

typedef unsigned char		TDEEP, *LPTDEEP;

#ifdef WIN32
#pragma pack(push, 1)
#endif

typedef struct ARG_FUNC
{
	union
	{
		NUMBER_VAR CountArg;
		class
		{
			NUMBER_VAR CountArg;
		public:
			operator bool() { return CountArg == 0; }
		} IsEmpty;
	};
	NUMBER_VAR Args[1];
	inline NUMBER_VAR operator[] (NUMBER_VAR n) { return Args[n]; }
	static ARG_FUNC EmptyArgs;
} ARG_FUNC, *LPARG_FUNC;

typedef unsigned short		TCOUNT_VAR, *LPTCOUNT_VAR;
typedef unsigned char		SHORT_NUMBER_OBJMEMBER, *LPSHORT_NUMBER_OBJMEMBER;
typedef unsigned short		EXCEPTION_TYPE, *LPEXCEPTION_TYPE;
typedef char				CHECK_USES;
typedef short				REL_ADDRESS;
typedef char				SHORT_REL_ADDRESS;
typedef PLATFORM_WORD		TSIZE_STR, *LPTSIZE_STR;
typedef PLATFORM_WORD       TSIZE_METHOD, *LPTSIZE_METHOD;
typedef unsigned int        OFFSET_METHOD, *LPOFFSET_METHOD;
typedef unsigned int        TINDEX_METHOD, *LPTINDEX_METHOD;
typedef unsigned int        TINDEX_STRING, *LPTINDEX_STRING;
typedef unsigned int        TINDEX_INTERATOR, *LPTINDEX_INTERATOR;
typedef unsigned int		TOFFSET_INTERATOR, *LPTOFFSET_INTERATOR;

typedef std::conditional<bool(sizeof(void*) > 4), unsigned long long, unsigned>::type ZELLI_OFFSET; 

#define ZELLI_INTEGER_HASH(Integer) ((HASH_VAL)(Integer))
#define ZELLI_DOUBLE_HASH(Double) (*((std::conditional<bool(sizeof(ZELLI_DOUBLE) == 8), unsigned long long*, unsigned*>::type)&(Double)))

#define MIN_DEEP_INTERATOR 1

//Расширенные опреранды

#define MAX_COUNT_OPERANDS 3

#ifndef NULL
#define NULL 0
#endif

#ifndef max
#define max(a,b) (((a)>(b))?(a):(b))
#endif

#define SETBITS(type,count)			((type)(-1) >> ((sizeof(type) * 8) - count))
#define REVSETBITS(type,count)		((type)(-1) << ((sizeof(type) * 8) - count))
#define CLEARBITS(type,count)		(~SETBITS(type,count))
#define REVCLEARBITS(type,count)	(~REVSETBITS(type,count))
#define GETMINTYPE(type)			(1 << ((sizeof(type) * 8) - 1))
//Api Zelli
#define ZELLI_API			__fastcall


struct ZELLI_EXCEPT_HANDLER
{
	enum
	{
		NONE_VARIABLE = (NUMBER_VAR)-1
	};
	NUMBER_VAR				NextHandler;
	LPOPCODE				CatchAddress;
};
typedef INSIDE_DATA (ZELLI_API * NATIVE_FUNCTION)(LPINSIDE_DATA This, LPEXECUTE_CONTEXT CurContext, LPARG_FUNC Arg);

#define INSIDE_MAIN_TYPES						\
union{											\
	ZELLI_INTEGER			Integer;			\
	ZELLI_EXCEPT_HANDLER	ExcHand;			\
	ZELLI_DOUBLE			Double;				\
	OBJECT					Object;				\
struct{											\
	SKIP_BYTES(6,0);							\
	unsigned short			TypeData;			\
};												\
}

class	STRING_CLASS;
class	HEADER_STRING;

typedef STRING_CLASS		*LPSTRING_CLASS;
typedef HEADER_STRING		*LPHEADER_STRING;

class	METHOD_CLASS;
class	HEADER_METHOD;

typedef METHOD_CLASS *LPMETHOD_CLASS;
typedef HEADER_METHOD *LPHEADER_METHOD;


template<class T>
class FOR_UNION_DATA
{
	char _Val[sizeof(T)];
public:
	inline operator T &() { return *(T*)_Val; }

	inline T * operator &() { return (T*)_Val; }

	inline T * operator ->() { return (T*)_Val; }

	inline T & operator =(T & Val)
	{
		*((T*)_Val = Val);
		return *(T*)_Val;
	}
};


#define DEF_IS_PROPERTY(Name, CommonData, Expression) class{CommonData;public:inline operator bool(){return Expression;}}Name

//#define OBJECT_CASE INSIDE_DATA::TYPEDATA_STRING: case INSIDE_DATA::TYPEDATA_OBJECT: case INSIDE_DATA::TYPEDATA_FUNCTION
#define OBJECT_CASE INSIDE_DATA::TYPEDATA_OBJECT
#define GC_MARK_VAR(Var) 	switch((Var).TypeData){case OBJECT_CASE:(Var).Object.MarkAsUsed();}



#include "Object.h"

struct INSIDE_DATA
{
private:

	class __LEN_PROP_DEF{
		INSIDE_MAIN_TYPES;
	public:
		operator ZELLI_INTEGER();
		operator INSIDE_DATA() { return operator ZELLI_INTEGER(); }
	};

	class __ISSIGNED_PROP_DEF{
		INSIDE_MAIN_TYPES;
	public:
		operator bool();
	};

	class __GETCLASS_PROP_DEF{
		INSIDE_MAIN_TYPES;
	public:
		operator LPHEADER_CLASS();

		template<class T>
		operator T * () { return (T*)Object.Prototype; }

		LPHEADER_CLASS operator ->();
	};

	class __FOR_GETADDRESS_OPERATOR{
		INSIDE_MAIN_TYPES;
	public:
		inline LPINSIDE_DATA operator ->() { return (LPINSIDE_DATA)this; }

		inline operator LPINSIDE_DATA() { return (LPINSIDE_DATA)this; }

		inline operator LPZELLI_INTEGER()
		{
			TypeData = TYPEDATA::TYPEDATA_INTEGER;
			return &Integer;
		}

		inline operator LPZELLI_DOUBLE() { return &Double; }

		inline operator bool*()
		{
			TypeData = TYPEDATA::TYPEDATA_INTEGER;
			Integer = 0;
			return (bool*)&Integer;
		}

		inline operator LPOBJECT()
		{
			TypeData = TYPEDATA::TYPEDATA_OBJECT;
			return &Object;
		}

		template <class T>
        inline operator T *() { return (T*)this; }
	};

	class __ISZERO_PROP_DEF{
		INSIDE_MAIN_TYPES;
	public:
		operator bool();
	};

	class __QUERY_MEMBER_BY_INDEX;

	
	class __QUERY_MEMBER{
		friend INSIDE_DATA;
		LPINSIDE_DATA			Obj;
		struct T{INSIDE_MAIN_TYPES;};
		char MemberIndex[sizeof(T)];

		inline __QUERY_MEMBER(LPINSIDE_DATA NewObj, LPINSIDE_DATA Member)
		{
		    Obj = NewObj;
		   *(LPINSIDE_DATA)MemberIndex = *Member;
		}
	public:
	    INSIDE_DATA operator=(INSIDE_DATA);
		operator INSIDE_DATA();

		__QUERY_MEMBER operator [](INSIDE_DATA Member);
		__QUERY_MEMBER_BY_INDEX operator [](ZELLI_INTEGER Member);
	};

	class __QUERY_MEMBER_BY_INDEX{
		friend INSIDE_DATA;
		LPINSIDE_DATA			Obj;
		ZELLI_INTEGER			MemberIndex;

		inline __QUERY_MEMBER_BY_INDEX(LPINSIDE_DATA Obj, ZELLI_INTEGER Member)
		{
		   this->Obj = Obj;
		   this->MemberIndex = Member;
		}
	public:
	    INSIDE_DATA operator=(INSIDE_DATA);
		operator INSIDE_DATA();
		__QUERY_MEMBER operator [](INSIDE_DATA Member);
		__QUERY_MEMBER_BY_INDEX operator [](ZELLI_INTEGER Member);
	};

	class __GET_HASH{
		INSIDE_MAIN_TYPES;
	public:
		operator HASH_VAL();
	};

	class __PROP_IS_STRING{
		INSIDE_MAIN_TYPES;
	public:
		operator bool();
	};

public:
	enum
	{
		NAN_DOUBLE = 0xFFF8
	};

	enum TYPEDATA
	{			
		TYPEDATA_INTEGER = NAN_DOUBLE + 0x2,	//Целое число	
		//Состояние
		TYPEDATA_NULL = NAN_DOUBLE + 0x3,		//Пустой тип данных
		//Объекты
		TYPEDATA_OBJECT = NAN_DOUBLE + 0x4,		//Объект
		//Дескрипторы исключений
		TYPEDATA_EXCEPT	= NAN_DOUBLE + 0x5
	};


	union
	{
		INSIDE_MAIN_TYPES;

		DEF_IS_PROPERTY(IsObject, INSIDE_MAIN_TYPES, TypeData == TYPEDATA_OBJECT);

		DEF_IS_PROPERTY(IsExceptionHandler, INSIDE_MAIN_TYPES, TypeData == TYPEDATA_EXCEPT);

		DEF_IS_PROPERTY(IsNumber, INSIDE_MAIN_TYPES, TypeData <= TYPEDATA_INTEGER);

		DEF_IS_PROPERTY(IsDouble, INSIDE_MAIN_TYPES, TypeData < TYPEDATA_INTEGER);

		DEF_IS_PROPERTY(IsInt, INSIDE_MAIN_TYPES, TypeData == TYPEDATA_INTEGER);

		DEF_IS_PROPERTY(IsNull, INSIDE_MAIN_TYPES, TypeData == TYPEDATA_NULL);

		DEF_IS_PROPERTY(IsNotNull, INSIDE_MAIN_TYPES, TypeData != TYPEDATA_NULL);

		__PROP_IS_STRING		IsString;

		__LEN_PROP_DEF			Length;

		__ISSIGNED_PROP_DEF		IsSigned;

		__ISZERO_PROP_DEF		IsZero;

		__GETCLASS_PROP_DEF		Class;

		__GET_HASH				Hash;
	};


	inline __FOR_GETADDRESS_OPERATOR & operator&() { return (__FOR_GETADDRESS_OPERATOR&)*this;}

	INSIDE_DATA(){}

	inline INSIDE_DATA(const ZELLI_INTEGER Val) : TypeData(TYPEDATA_INTEGER), Integer(Val) {}

	inline INSIDE_DATA(const unsigned int Val) : TypeData(TYPEDATA_INTEGER), Integer(Val) {}

	inline INSIDE_DATA(const ZELLI_DOUBLE Val)
	{
		register ZELLI_INTEGER Tst;
		if(Val == (Tst = (ZELLI_INTEGER)Val))
		{
			TypeData = TYPEDATA_INTEGER;
			Integer = Tst;
		}else
		{
			Double = Val;
		}
	}

	inline INSIDE_DATA(const bool Val) : TypeData(TYPEDATA_INTEGER), Integer((ZELLI_INTEGER)Val){}

	INSIDE_DATA(const OBJECT);

	INSIDE_DATA(const LPOBJECT);

	INSIDE_DATA(const INSTANCE_CLASS, LPHEADER_CLASS);

	INSIDE_DATA(const NATIVE_FUNCTION);

	operator bool();

	operator ZELLI_INTEGER();

	operator ZELLI_DOUBLE();

	bool operator ==(INSIDE_DATA & CompareData);

	bool operator !=(INSIDE_DATA & CompareData);

	bool operator >(INSIDE_DATA & CompareData);

	bool operator >=(INSIDE_DATA & CompareData);

	bool operator <(INSIDE_DATA & CompareData);

	bool operator <=(INSIDE_DATA & CompareData);

	void operator ++();

	void operator --();

	INSIDE_DATA operator &(INSIDE_DATA & Val2);

	INSIDE_DATA operator |(INSIDE_DATA & Val2);

	INSIDE_DATA operator ^(INSIDE_DATA & Val2);

	INSIDE_DATA operator <<(INSIDE_DATA & Val2);

	INSIDE_DATA operator >>(INSIDE_DATA & Val2);

	INSIDE_DATA operator +(INSIDE_DATA & Val2);

	INSIDE_DATA operator -(INSIDE_DATA & Val2);

	INSIDE_DATA operator *(INSIDE_DATA & Val2);

	INSIDE_DATA operator /(INSIDE_DATA & Val2);

	INSIDE_DATA operator %(INSIDE_DATA & Val2);

	INSIDE_DATA operator -();

	INSIDE_DATA operator ~();

	inline __QUERY_MEMBER_BY_INDEX operator[](ZELLI_INTEGER Index) {return __QUERY_MEMBER_BY_INDEX(this, Index);}

	inline __QUERY_MEMBER operator[](INSIDE_DATA Member) {return __QUERY_MEMBER(this, &Member);}

	INSIDE_DATA operator()(LPEXECUTE_CONTEXT Context, LPARG_FUNC Arg = &ARG_FUNC::EmptyArgs) HAS_THROW;

	INSIDE_DATA operator()(LPEXECUTE_CONTEXT Context, LPINSIDE_DATA ThisScope, LPARG_FUNC Arg = &ARG_FUNC::EmptyArgs) HAS_THROW;

	INSIDE_DATA operator()(LPINSIDE_DATA ThisScope, LPARG_FUNC Arg = &ARG_FUNC::EmptyArgs) HAS_THROW;

	INSIDE_DATA operator()(LPARG_FUNC Arg = &ARG_FUNC::EmptyArgs) HAS_THROW;

	SIZE_STR Info(LPINTERNAL_CHAR Buffer, SIZE_STR Len);

	void In(LPINSIDE_DATA Var);

	OBJECT ToString(LPSTRING_CLASS BasicClass);

	void ToString(std::basic_string<INTERNAL_CHAR>& Str);

	void ToString(INTERNAL_CHAR* StrBuf, TSIZE_STR BufLen);

	OBJECT TypeOf(LPSTRING_CLASS BasicClass);

	void TypeOf(std::basic_string<INTERNAL_CHAR>& Str);

	void TypeOf(INTERNAL_CHAR* StrBuf, TSIZE_STR BufLen);

	unsigned TypeId();

	inline void SetNull() { TypeData = TYPEDATA_NULL; }

	static INSIDE_DATA Null;
	static INSIDE_DATA Zero;
};


template <class TypeData>
struct LIST_HEADER
{
	TypeData * Next;
	TypeData * Previous;

	static inline void AddToRight(TypeData * ThisList, TypeData * NewElem)
	{
		(NewElem->Next = ThisList->Next)->Previous = NewElem;
		(ThisList->Next = NewElem)->Previous = ThisList;
	}

	static inline void AddToLeft(TypeData * ThisList, TypeData * NewElem)
	{
		(NewElem->Previous = ThisList->Previous)->Next = NewElem;
		(ThisList->Previous = NewElem)->Next = ThisList;
	}

	static inline void BindList(TypeData * ThisList, TypeData * NewList)
	{
		TypeData * PrevNewList = NewList->Previous;
		NewList->Next->Previous = ThisList;
		PrevNewList->Next = ThisList->Next;

		ThisList->Next->Previous = NewList->Previous;
		ThisList->Next = NewList->Next;
	}

	static inline void MoveToListRight(TypeData * ThisList, TypeData * ListDest)
	{
		(ThisList->Previous->Next = ThisList->Next)->Previous = ThisList->Previous;

		(ThisList->Next = ListDest->Next)->Previous = ThisList;
		(ListDest->Next = ThisList)->Previous = ListDest;
	}

	static inline void MoveToListLeft(TypeData * ThisList, TypeData * ListDest)
	{
		(ThisList->Previous->Next = Next)->Previous = ThisList->Previous;
		(ThisList->Previous = ListDest->Previous)->Next = ThisList;
		(ListDest->Previous = ThisList)->Next = ListDest;
	}

	static inline void Close(TypeData * ThisList)
	{
		ThisList->Previous = ThisList->Next = ThisList;
	}

	static inline void Remove(TypeData * ThisElem)
	{
		(ThisElem->Previous->Next = ThisElem->Next)->Previous = ThisElem->Previous;
	}

	static void OnMoveToNewPlace(TypeData * ThisElem)
	{
		ThisElem->Previous->Next = ThisElem;
		ThisElem->Next->Previous = ThisElem;
	}
};

#ifdef WIN32
#pragma pack(pop)
#endif

#endif