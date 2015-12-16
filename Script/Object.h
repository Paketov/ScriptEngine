#ifndef __OBJECT_H__
#define __OBJECT_H__


#include "TypeData.h"
#include "HashTable.h"
#include "DynamicArray.h"

//Шаблон экземпляра объекта
template <class INSIDE_T>
class OBJECT_
{
	typedef INSIDE_T *LPINSIDE_T;
public:

	typedef DYNAMIC_ARRAY<void*, unsigned short, nullptr> ARR_FOR_CLASS, *LPARR_FOR_CLASS;
	static ARR_FOR_CLASS ClassList;

	class PROTOTYPE
	{
		friend					OBJECT_;
		ARR_FOR_CLASS::TINDEX	Index;
	public:
		inline LPHEADER_CLASS operator ->() { return (LPHEADER_CLASS)ClassList[Index]; }
		inline operator LPHEADER_CLASS() { return (LPHEADER_CLASS)ClassList[Index]; }
		template<typename T>
		inline operator T*() { return (T*)ClassList[Index]; }
		inline operator ARR_FOR_CLASS::TINDEX() { return Index; }
		inline unsigned short operator =(const ARR_FOR_CLASS::TINDEX i) { return Index = i; }
		inline LPHEADER_CLASS operator =(const LPHEADER_CLASS h) { Index = h->IndexInClassArr; return h; }
		inline bool operator ==(PROTOTYPE & Enother) { return Index == Enother.Index; }
		inline bool operator !=(PROTOTYPE & Enother) { return Index != Enother.Index; }
	};

#define __OBJECT_FILEDS_DEF			\
	struct							\
	{								\
		INSTANCE_CLASS   Object;	\
		PROTOTYPE		 Prototype; \
	}								

	static const INSTANCE_CLASS Null;
	union
	{
		__OBJECT_FILEDS_DEF;

		class{
			__OBJECT_FILEDS_DEF;
		public:
			inline operator INSIDE_T() { return Prototype->GetLength(Object); }
			inline operator ZELLI_INTEGER() { return Prototype->GetLength(Object); }
		} Length;

		class{
			__OBJECT_FILEDS_DEF;
		public:
			inline operator INSIDE_T() { return Prototype->Name; }
			inline operator LPHEADER_STRING() { return Prototype->Name; }
		} NamePrototype;

		class{
			__OBJECT_FILEDS_DEF;
		public:
			inline operator HASH_VAL() { return Prototype->GetHash(Object); }
		} Hash;
	};

	inline static OBJECT_ New(const LPHEADER_CLASS Class, const INSTANCE_CLASS Instance)
	{
	   OBJECT_ Obj;
	   Obj.Prototype = Class;
	   Obj.Object = Instance;
	   return Obj;
	}

	inline static OBJECT_ New(const PROTOTYPE Class, const INSTANCE_CLASS Instance)
	{
	   OBJECT_ Obj;
	   Obj.Prototype = Class;
	   Obj.Object = Instance;
	   return Obj;
	}

	inline void Set(const PROTOTYPE Class, const INSTANCE_CLASS Instance)
	{
	   Prototype = Class;
	   Object = Instance; 
	}

	inline INSIDE_T Clone() { return Prototype->Clone(Object); }
	inline INSIDE_T ReadMember(LPINSIDE_T MemberIndex) { return Prototype->ReadMember(Object, MemberIndex); }
	inline void WriteMember(LPINSIDE_T MemberIndex, LPINSIDE_T Source) { Prototype->WriteMember(Object, MemberIndex, Source); }
	inline void RemoveMember(LPINSIDE_T MemberIndex) { Prototype->RemoveMember(Object, MemberIndex); }
	inline void MarkAsUsed() { Prototype->MarkAsUsed(Object); }
	inline void EnumKey(LPINSIDE_T CurKey) { Prototype->EnumKey(Object, CurKey); }
	inline operator ZELLI_DOUBLE() { return Prototype->OperatorToDouble(Object); }
	inline operator ZELLI_INTEGER() { return Prototype->OperatorToInt(Object); }
	inline operator bool(){ return Prototype->OperatorToBool(Object); }
	template<typename T>
	inline operator T() { return T(Object); }
	inline bool operator ==(INSIDE_T & SecondObj) { return Prototype->OperatorEq(Object, &SecondObj); }
	inline bool operator ==(OBJECT_ & SecondObj) { return (Prototype == SecondObj.Prototype) && (Object == SecondObj.Object);}
	inline bool operator !=(INSIDE_T & SecondObj) { return Prototype->OperatorNeq(Object, &SecondObj);}
	inline bool operator !=(OBJECT_ & SecondObj) { return (Prototype != SecondObj.Prototype) || (Object != SecondObj.Object); }
	inline bool operator <(INSIDE_T & SecondObj) { return Prototype->OperatorLt(Object, &SecondObj);}
	inline bool operator <=(INSIDE_T & SecondObj) { return Prototype->OperatorLe(Object, &SecondObj); }
	inline bool operator >(INSIDE_T & SecondObj) { return Prototype->OperatorGt(Object, &SecondObj); }
	inline bool operator >=(INSIDE_T & SecondObj) { return Prototype->OperatorGe(Object, &SecondObj); }
	inline INSIDE_T operator -() { return Prototype->OperatorInvers(Object); }
	inline void operator ++() { Prototype->OperatorInc(Object); }
	inline void operator --() { Prototype->OperatorDec(Object); }
	inline INSIDE_T operator +(INSIDE_T & SecondObj) { return Prototype->OperatorAdd(Object, &SecondObj); }
	inline INSIDE_T operator -(INSIDE_T & SecondObj) { return Prototype->OperatorSub(Object, &SecondObj); }
	inline INSIDE_T operator *(INSIDE_T & SecondObj) { return Prototype->OperatorMul(Object, &SecondObj); }
	inline INSIDE_T operator /(INSIDE_T & SecondObj) { return Prototype->OperatorDiv(Object, &SecondObj); }
	inline INSIDE_T operator %(INSIDE_T & SecondObj) { return Prototype->OperatorMod(Object, &SecondObj); }
	inline INSIDE_T operator >>(INSIDE_T & SecondObj) { return Prototype->OperatorShr(Object, &SecondObj); }
	inline INSIDE_T operator <<(INSIDE_T & SecondObj) { return Prototype->OperatorShl(Object, &SecondObj); }
	inline INSIDE_T operator &(INSIDE_T & SecondObj) { return Prototype->OperatorAnd(Object, &SecondObj); }
	inline INSIDE_T operator |(INSIDE_T & SecondObj) { return Prototype->OperatorOr(Object, &SecondObj); }
	inline INSIDE_T operator ^(INSIDE_T & SecondObj) { return Prototype->OperatorXor(Object, &SecondObj); }
	inline INSIDE_T operator()(LPINSIDE_T This, LPEXECUTE_CONTEXT CurContext, LPARG_FUNC Arg) { return Prototype->OperatorCall(Object, This, CurContext, Arg); }
	inline INSIDE_T operator()(LPEXECUTE_CONTEXT CurContext, LPARG_FUNC Arg) { INSIDE_T NullVal; NullVal.SetNull(); return Prototype->OperatorCall(Object, &NullVal, CurContext, Arg); }
	inline void In(LPINSIDE_T CurKey) { return Prototype->EnumKey(Object, CurKey); }
	inline INSIDE_T operator !() { return Prototype->OperatorNot(Object); }
	inline INSIDE_T operator ~() { return Prototype->OperatorBitNot(Object); }
	inline INSIDE_T operator[](ZELLI_INTEGER Index) { return Prototype->OperatorReadByIndex(Object, Index); }
	inline INSIDE_T operator[](const INSIDE_T & Index) { return Prototype->OperatorReadByIndex(Object, (const LPINSIDE_T)&Index); }
	inline SIZE_STR Info(LPINTERNAL_CHAR Buffer, SIZE_STR LenInBuf) { return Prototype->InfoObject(Object, Buffer, LenInBuf); }
};

template <class INSIDE_T>
const INSTANCE_CLASS OBJECT_<INSIDE_T>::Null = nullptr;

typedef OBJECT_<INSIDE_DATA> OBJECT, *LPOBJECT;



#endif