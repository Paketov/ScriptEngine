//#include "Object.h"
//#include "Class.h"
//
//	class PROTOTYPE
//	{
//		friend					OBJECT;
//		unsigned short			Index;
//	public:
//		LPHEADER_CLASS operator ->()
//		{
//			return ClassList[Index];
//		}
//
//		operator LPHEADER_CLASS()
//		{
//			return ClassList[Index];
//		}
//
//		inline operator unsigned short()
//		{
//			return Index; 
//		}
//
//		unsigned short operator =(const unsigned short i)
//		{
//			return Index = i;
//		}
//
//		LPHEADER_CLASS operator =(const LPHEADER_CLASS h)
//		{
//			Index = ClassList.Search(h);
//			if(Index != ClassList.NullIndex)
//				return h;
//			return NULL;
//		}
//
//		inline bool operator ==(LPHEADER_CLASS Enother)
//		{
//			return ClassList[Index] == Enother;
//		}
//
//		inline bool operator ==(PROTOTYPE & Enother)
//		{
//			return Index == Enother.Index;
//		}
//
//		inline bool operator !=(PROTOTYPE & Enother)
//		{
//			return Index != Enother.Index;
//		}
//
//		inline bool operator !=(LPHEADER_CLASS Enother)
//		{
//			return ClassList[Index] != Enother;
//		}
//	};
//
//
//	union
//	{
//		unsigned short	 IndexClass;
//		INSTANCE_CLASS   Object;
//
//		PROTOTYPE Prototype;
//
//		class
//		{
//			PROTOTYPE			Prototype;
//			INSTANCE_CLASS	    Object;
//		public:
//			inline operator INSIDE_DATA()
//			{
//				return Prototype->GetLength(Object);
//			}
//
//			inline operator ZELLI_INTEGER()
//			{
//				return Prototype->GetLength(Object);
//			}
//		} Length;
//
//		class
//		{
//			PROTOTYPE Prototype;
//		public:
//
//			inline operator INSIDE_DATA()
//			{
//				return Prototype->Name;
//			}
//
//			inline operator LPHEADER_STRING()
//			{
//				return Prototype->Name;
//			}
//		} NamePrototype;
//
//		class
//		{
//			unsigned short	 IndexClass;
//			INSTANCE_CLASS	 Object;
//		public:
//			inline operator unsigned()
//			{
//				return IndexClass * 31 + (unsigned)Object;
//			}
//		} Hash;
//	};
//
//	inline operator INSTANCE_CLASS()
//	{
//		return Object;
//	}
//
//	inline operator LPOBJECT()
//	{
//		return this;
//	}
//
//	template<typename T>
//	inline operator T()
//	{
//		return (T)Object;
//	}
//
//	inline INSIDE_DATA Clone()
//	{
//		return Prototype->Clone(Object);
//	}
//
//	inline INSIDE_DATA ReadMember(LPINSIDE_DATA MemberIndex)
//	{
//		return Prototype->ReadMember(Object, MemberIndex);
//	}
//
//	inline INSIDE_DATA ReadMember(ID_OBJMEMBER MemberId)
//	{
//		return Prototype->ReadMemberById(Object, MemberId);
//	}
//
//	inline void WriteMember(LPINSIDE_DATA MemberIndex, LPINSIDE_DATA Source)
//	{
//		Prototype->WriteMember(Object, MemberIndex, Source);
//	}
//
//	inline void WriteMember(ID_OBJMEMBER MemberId, LPINSIDE_DATA Source)
//	{
//		Prototype->WriteMemberById(Object, MemberId, Source);
//	}
//
//	inline void RemoveElement(LPINSIDE_DATA MemberIndex)
//	{
//		Prototype->RemoveElement(Object, MemberIndex);
//	}
//
//	inline void RemoveElement(ID_OBJMEMBER MemberId)
//	{
//		Prototype->RemoveElementById(Object, MemberId);
//	}
//
//	inline void MarkAsUsed()
//	{
//		Prototype->MarkAsUsed(Object);
//	}
//
//	inline void EnumKey(LPINSIDE_DATA CurKey)
//	{
//		Prototype->EnumKey(Object, CurKey);
//	}
//
//	inline operator ZELLI_DOUBLE()
//	{
//		return Prototype->OperatorToDouble(Object);
//	}
//
//	inline operator ZELLI_INTEGER()
//	{
//		return Prototype->OperatorToInt(Object);	
//	}
//
//	inline operator bool()
//	{
//		return Prototype->OperatorToBool(Object);	
//	}
//
//	inline bool operator ==(INSIDE_DATA & SecondObj)
//	{
//		return Prototype->OperatorEq(Object, &SecondObj);
//	}
//
//	inline bool operator ==(OBJECT & SecondObj)
//	{
//		return (Prototype == SecondObj.Prototype) && (Object == SecondObj.Object);
//	}
//
//	inline bool operator !=(INSIDE_DATA & SecondObj)
//	{
//		return Prototype->OperatorNeq(Object, &SecondObj);
//	}
//
//	inline bool operator !=(OBJECT & SecondObj)
//	{
//		return (Prototype != SecondObj.Prototype) || (Object != SecondObj.Object);
//	}
//
//	inline bool operator <(INSIDE_DATA & SecondObj)
//	{
//		return Prototype->OperatorLt(Object, &SecondObj);
//	}
//
//	inline bool operator <=(INSIDE_DATA & SecondObj)
//	{
//		return Prototype->OperatorLe(Object, &SecondObj);
//	}
//
//	inline bool operator >(INSIDE_DATA & SecondObj)
//	{
//		return Prototype->OperatorGt(Object, &SecondObj);
//	}
//
//	inline bool operator >=(INSIDE_DATA & SecondObj)
//	{
//		return Prototype->OperatorGe(Object, &SecondObj);
//	}
//
//	inline INSIDE_DATA operator -()
//	{
//		return Prototype->OperatorInvers(Object);
//	}
//
//	inline void operator ++()
//	{
//		Prototype->OperatorInc(Object);
//	}
//
//	inline void operator --()
//	{
//		Prototype->OperatorDec(Object);
//	}
//
//	inline INSIDE_DATA operator +(INSIDE_DATA & SecondObj)
//	{
//		return Prototype->OperatorAdd(Object, &SecondObj);
//	}
//
//	inline INSIDE_DATA operator -(INSIDE_DATA & SecondObj)
//	{
//		return Prototype->OperatorSub(Object, &SecondObj);
//	}
//
//	inline INSIDE_DATA operator *(INSIDE_DATA & SecondObj)
//	{
//		return Prototype->OperatorMul(Object, &SecondObj);
//	}
//
//	inline INSIDE_DATA operator /(INSIDE_DATA & SecondObj)
//	{
//		return Prototype->OperatorDiv(Object, &SecondObj);
//	}
//
//	inline INSIDE_DATA operator %(INSIDE_DATA & SecondObj)
//	{
//		return Prototype->OperatorMod(Object, &SecondObj);
//	}
//
//	inline INSIDE_DATA operator >>(INSIDE_DATA & SecondObj)
//	{
//		return Prototype->OperatorShr(Object, &SecondObj);
//	}
//
//	inline INSIDE_DATA operator <<(INSIDE_DATA & SecondObj)
//	{
//		return Prototype->OperatorShl(Object, &SecondObj);
//	}
//
//	inline INSIDE_DATA operator &(INSIDE_DATA & SecondObj)
//	{
//		return Prototype->OperatorAnd(Object, &SecondObj);
//	}
//
//	inline INSIDE_DATA operator |(INSIDE_DATA & SecondObj)
//	{
//		return Prototype->OperatorOr(Object, &SecondObj);
//	}
//
//	inline INSIDE_DATA operator ^(INSIDE_DATA & SecondObj)
//	{
//		return Prototype->OperatorXor(Object, &SecondObj);
//	}
//
//	inline INSIDE_DATA operator()(LPINSIDE_DATA This, LPEXECUTE_CONTEXT CurContext, LPARG_FUNC Arg)
//	{
//		return Prototype->OperatorCall(Object, This, CurContext, Arg);
//	}
//
//	inline INSIDE_DATA operator()(LPEXECUTE_CONTEXT CurContext, LPARG_FUNC Arg)
//	{
//		INSIDE_DATA NullVal;
//		NullVal.SetNull();
//		return Prototype->OperatorCall(Object, &NullVal, CurContext, Arg);
//	}
//
//	inline void In(LPINSIDE_DATA CurKey)
//	{
//		return Prototype->EnumKey(Object,  CurKey);
//	}
//
//	inline INSIDE_DATA operator !()
//	{
//		return Prototype->OperatorNot(Object);
//	}
//
//	inline INSIDE_DATA operator ~()
//	{
//		return Prototype->OperatorBitNot(Object);
//	}
//
//	inline INSIDE_DATA operator[](const ID_OBJMEMBER Id)
//	{
//		return Prototype->OperatorReadByIndexId(Object, Id);
//	}
//
//	inline INSIDE_DATA operator[](const LPINSIDE_DATA Index)
//	{
//		return Prototype->OperatorReadByIndex(Object, Index);
//	}
//
//	inline INSIDE_DATA operator[](const INSIDE_DATA & Index)
//	{
//		return Prototype->OperatorReadByIndex(Object, (const LPINSIDE_DATA)&Index);
//	}
//
//	inline SIZE_STR Info(LPINTERNAL_CHAR Buffer, SIZE_STR LenInBuf)
//	{
//		return Prototype->InfoObject(Object, Buffer, LenInBuf);
//	}