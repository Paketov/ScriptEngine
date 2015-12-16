#ifndef __KEYWORDS_H__
#define __KEYWORDS_H__

#include "..\Script\TypeData.h"





typedef bool (*TKEYWORD_PROC)(TRANSLATOR *);

typedef struct 
{
	inline HASH_VAL Key()
	{
		return GenKey(Name);
	}
	inline static HASH_VAL GenKey(const char * Str)
	{
		HASH_VAL h = 0;
		for (LPINTERNAL_CHAR s = (LPINTERNAL_CHAR)Str; *s; s++) 
			h = 31 * h + (unsigned)*s;
		return h;
	}

	inline bool Cmp(const char * Str)
	{
		return strcmp(Str, Name) == 0;
	}

	LPINTERNAL_CHAR Name;
	TKEYWORD_PROC   KeywordProc;
} __KEYWORD_ELEMENT;

typedef HASH_ARRAY<HASH_VAL, __KEYWORD_ELEMENT, false, unsigned char>   KEYWORD_TABLE, *LPKEYWORD_TABLE;

#ifndef __KEYWORD_CPP__
extern unsigned CountKeywords;
extern KEYWORD_TABLE::TSTATIC_VAL Keywords[];
#endif

#endif