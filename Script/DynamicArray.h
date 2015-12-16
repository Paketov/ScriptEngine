#ifndef __DYNAMIC_ARRAY_H__
#define __DYNAMIC_ARRAY_H__

#include "TypeData.h"

template <typename T, typename I = unsigned short, T NullV = (T)-1, I NullI = (I)-1>
class DYNAMIC_ARRAY
{	
public:

#define FIELDS_				\
	struct					\
	{						\
		T * Array;			\
		I Count_;			\
		I AllocCount_;		\
		I MinEmpty;			\
		I MinAllocCount;	\
	}

	union
	{
		class{
			FIELDS_;
		public:
			inline operator bool() { return (I)((float)Count * GOLDEN_RATIO) < AllocCount; }
			inline operator I() { return (I)((float)Count * GOLDEN_RATIO); }
		} IsRequiredCompress;

		class{
			friend DYNAMIC_ARRAY;
			FIELDS_;
		public:
			inline operator I() { return Count_; }
		} Count;

		class{
			FIELDS_;
		public:
			inline operator I() { return AllocCount_; }
		} AllocCount; 

		class 
		{
			FIELDS_;
			inline static T RetNull()
			{
			    static T NullValue_ = NullV;
				return NullValue_;
			}
		public:
			inline operator T() { return RetNull(); }
		} NullValue;

	};
private:
	void ClearRegion(I StartIndex, I EndIndex)
	{
		T * Arr = Count.Array + StartIndex;
		T * Max = Arr + EndIndex;
		for(;Arr < Max;Arr++)
			*Arr = NullV;
	}

	I GetEmptyIndex()
	{
		
		I Ret = Count.MinEmpty;
		if(Ret == NullI)
			return NullI;
		Count.MinEmpty = NullI;
		T * Arr = Count.Array;
		for(I i = Ret + 1, Max = Count.AllocCount_;i < Max;i++)
			if(Arr[i] == NullV)
			{
			   Count.MinEmpty = i;
			   break;
			}
		Count.Count_++;
		return Ret;
	}

	void Turnindex(I Index)
	{
	  if((Index < Count.MinEmpty) || (Count.MinEmpty == NullI))
	     Count.MinEmpty = Index;
	  Count.Array[Index] = NullV;
	  Count.Count_--;
	}


public:
	typedef T TELEMENT,		*LPTELEMENT;
	typedef I TINDEX,		*LPTINDEX;

	static const I NullIndex = NullI;

	DYNAMIC_ARRAY() { DYNAMIC_ARRAY(50); }

	DYNAMIC_ARRAY(I StartCount)
	{
		Count.Array = (T*)MEM_ALLOC(sizeof(T) * (Count.MinAllocCount = StartCount));
		if(Count.Array == NULL)
			return;
		Count.MinEmpty = 0;
		Count.Count_ = 0;
		Count.AllocCount_ = StartCount;
		ClearRegion(0, StartCount);
	}

	~DYNAMIC_ARRAY()
	{
		if(Count.Array)
			MEM_FREE(Count.Array);
	}

	I AddElement()
	{
		if(Count.Count_ >= Count.AllocCount_)
		{
			I NewCount = (I)((float)Count.AllocCount_ * GOLDEN_RATIO);
			T * NewArray = (T*)MEM_REALLOC(Count.Array, sizeof(T) * NewCount);
			if(NewArray == NULL)
				return NullI;
			ClearRegion(Count.AllocCount_, NewCount);
			Count.AllocCount_ = NewCount;
		}
		return GetEmptyIndex();
	}

	void RemoveAllInplement(T Val)
	{
		T * Arr = Count.Array;
		for(I i = 0, Max = Count.AllocCount_;i < Max;i++)
			if(Arr[i] == Val)
			    RemoveElement(i);
	}

	void RemoveElement(I Index) { Turnindex(Index); }

	void RemoveWithCompress(I Index)
	{
		Turnindex(Index);
		if(IsRequiredCompress)
		   Compress();
	}

	void Reinit()
	{
		T * Arr = Count.Array;
		T * Max = Count.AllocCount_;
		for(;Arr < Max;Arr++)
			*Arr = NullV;
	}

	T & operator[](I Index) { return Count.Array[Index]; }

	I Search(T Val)
	{
		T * Arr = Count.Array;
		for(I i = 0, Max = Count.AllocCount_;i < Max;i++)
			if(Arr[i] == Val)
				return i;
	   return NullI;
	}

	bool Compress()
	{
		if(Count.MinEmpty == NullI)
			return false;

		T * Arr = Count.Array;
		for(I i = Count.MinEmpty, j = Count.AllocCount_ - 1;i <= j ;i++)
		{
			if(Arr[i] != NullV)
				continue;
			for(;i <= j; j--)
			{
				if(Arr[i] == NullV)
					continue;
				Arr[i] = Arr[j];
				break;
			}
		}
		I NewCount = max(Count.MinAllocCount, Count.Count_);
		T * NewArray = (T*)MEM_REALLOC(Arr, sizeof(T) * NewCount);
		if(NewArray == NULL)
			return false;
		Count.Array = NewArray;
		Count.AllocCount_ = NewCount;
		return true;
	}

};


#endif