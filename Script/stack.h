#ifndef __STACK_H__
#define __STACK_H__
#include <stdlib.h>

template <class TYPE_ELEMENT>
class STACK
{
protected:
	TYPE_ELEMENT * Bottom, *Top, *MaxSize;

public:
	bool Resize(unsigned int NewCount)
	{
		unsigned int Differ = (unsigned int)Top - (unsigned int)Bottom;
		if(Differ > (NewCount * sizeof(TYPE_ELEMENT)))
			return false;
		
		void * Tst = realloc(Bottom,NewCount * sizeof(TYPE_ELEMENT));
		if(Tst)
		{
			Bottom = (TYPE_ELEMENT *)Tst;
			Top = (TYPE_ELEMENT *)((unsigned int)Bottom + Differ);
			MaxSize =(TYPE_ELEMENT *)((unsigned int)Bottom + (NewCount - 1) * sizeof(TYPE_ELEMENT));
			return true;
		}else
			return false;
	}
	STACK()
	{
		MaxSize = Top = Bottom = NULL;
	}
	STACK(unsigned int Count)
	{
		Bottom = NULL;
		Resize(Count);
	}
	~STACK()
	{
	  if(Bottom)
		  free(Bottom);
	}

	bool Push(TYPE_ELEMENT * Source)
	{
		if(Top == MaxSize)
			return false;
		memcpy(Top,Source,sizeof(TYPE_ELEMENT));
		Top++;
		return true;
	}
	TYPE_ELEMENT * Push()
	{
		if(Top == MaxSize)
			return NULL;
		return Top++;
	}

	bool Pop(TYPE_ELEMENT * Dest)
	{
		if(Top == Bottom)
			return false;
		Top--;
		memcpy(Dest,Top,sizeof(TYPE_ELEMENT));
		return true;

	}
	TYPE_ELEMENT * Pop()
	{
		if(Top == Bottom)
			return NULL;
		return --Top;
	}
	TYPE_ELEMENT * GetTop()
	{
		if(Top == Bottom)
			return NULL;
		return Top - 1;
	}

	bool isFull()
	{
		return Top == MaxSize;
	}

	bool isEmpty()
	{
		return Top == Bottom;
	}
	unsigned int CurCount()
	{
		unsigned int Differ = (unsigned int)Top - (unsigned int)Bottom;
		return Differ / sizeof(TYPE_ELEMENT);
	}

};

#endif