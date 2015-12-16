#ifndef __ACSELERATE_INTERATOR_H__
#define __ACSELERATE_INTERATOR_H__

/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Interator table.
Used for access on objects from single variable.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	+========================================================================+
	|					HEADER_FILE_INTERATOR_LIST							 |
	|																		 |
	|========================================================================|
	|HEADER_INTERATORS|INTERATOR|INTERATOR|...|INTERATOR|					 |	
	|________________________________________________________________________|
	|HEADER_INTERATORS|INTERATOR|INTERATOR|...|INTERATOR|					 |
	|________________________________________________________________________|
	|							...											 |
	|________________________________________________________________________|
 _.>|HEADER_INTERATORS|INTERATOR|INTERATOR|...|INTERATOR|					 |
| i	+________________________________________________________________________+
l n							^					^					^______________.
p d							|					\________________________________. |
i e							\__________________________________________________. | |
n x																			   | | |
t |_+========================================================================+ | | |
e  _|							METHOD_HEADER                                | | | |
r | |========================================================================| | | |
a | |							CODE	....								 | | | |
t | +________________________________________________________________________+ | | |
o |	                                                                           | | |
r |	                                                                           | | |
| `>+========================================================================+ | | |
`-->|							INTERATOR_CLASS								 | | | |
	|========================================================================| | | |
	|							LPINTERATOR									 |_| | |
	|________________________________________________________________________|   | |
	|							LPINTERATOR									 |___| |
	|________________________________________________________________________|	   |
	|							...											 |     |
	|________________________________________________________________________|     |
	|							LPINTERATOR									 |_____|
	+________________________________________________________________________+

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When interpreter called method he, set InteratorTable pointer in internal registers, for cases when reqwested get same 
object fields.

*/

#include "TypeData.h"

struct INTERATOR
{
    unsigned char       Deep;
    LPHEADER_STRING		Element[1];

	inline unsigned GetSize()
	{
		return sizeof(Deep) + Deep * sizeof(LPHEADER_STRING);
	}

	inline LPINTERATOR GetNext()
	{
	   return (LPINTERATOR)((char*)this + GetSize());
	}

	bool InitAll(LPBYTECODE_FILE CurLoad);
}; 


struct INSIDE_POINTER_INTERATOR
{
	TINDEX_METHOD iMethod;
	unsigned	  CountPointers;
	TSIZE_METHOD  OffsetPointer[1];
	unsigned GetSize()
	{
	   return CountPointers * sizeof(TSIZE_METHOD)  + sizeof(TINDEX_METHOD);
	}
	LPINSIDE_POINTER_INTERATOR GetNext()
	{
	   return (LPINSIDE_POINTER_INTERATOR)((char*)this + GetSize());
	}
	bool InitAll(LPBYTECODE_FILE CurLoad);

};

#endif