#ifndef __LOAD_FROM_FILE_H__
#define __LOAD_FROM_FILE_H__

#include "TypeData.h"


class BYTECODE_FILE
{
	typedef struct SIGNATURE_IMAGE
	{
		bool CheckSignature()
		{
		   if((memcmp(Sign,"ZELBC", 5) == 0) && (SizeFile > 8))
			   return true;
		   return false;
		}
		char	 Sign[5];
		unsigned SizeFile;
		unsigned CheckSumm;
		unsigned CountSections;
		unsigned IndexMainMethod;
	} SIGNATURE_IMAGE, *LPSIGNATURE_IMAGE;

	typedef struct SECTION_IMAGE
	{
	     unsigned char	Type;
		 unsigned		Size;
		 unsigned		CountElements;
	} SECTION_IMAGE, *LPSECTION_IMAGE;

	enum
	{
	   STRING_SECTION,
	   METHOD_SECTION,
	   INTERATOR_SECTION,
	   DYNAMIC_OFFSET_TABLE
	};


public:	
	enum ERRORS
	{
		SUCCESS,
	    NOT_OPEN_FILE,
		NOT_READ_FILE,
		BAD_SIGNATURE,
		NOT_ALLOC_STRING,
		NOT_ALLOC_METHODS,
		NOT_ALLOC_INTERATORS,
		BAD_INTERATOR_SECTION,
		BAD_OFFSET_POINTER_TO_DYNAMIC_DATA,
		NO_ONE_INTERATOR_SECTION,
		NO_ONE_OFFSET_INTERATOR_SECTION,
		BAD_ENTRY,
		BAD_NUMBER_VAR,
		BAD_REL_ADDRESS,
		BAD_ARG_FUNCTION,
		BAD_STRING_INDEX,
		BAD_METHOD_INDEX,
		BAD_OFFSET_INTERATOR,
		BAD_OPCODE
	};

	typedef struct REG_METHOD
	{
	    TSIZE_METHOD    Size;
		LPHEADER_METHOD Header;
	} REG_METHOD, *LPREG_METHOD;


	TINDEX_STRING     CountInStringArr;
	LPHEADER_STRING * StringArr;
	TINDEX_METHOD     CountInMetodArr;
	LPREG_METHOD      Methods;

	TOFFSET_INTERATOR SizeInteratorSection;
	TINDEX_INTERATOR  CountInterators;
	void * InteratorSection;
	unsigned SizeLpInteratorSect;
	void * LpInteratorSect;

	LPSTRING_CLASS StringClass;
	LPMETHOD_CLASS MethodClass;

	SIGNATURE_IMAGE Signature;
	LPHEADER_METHOD MainMethod;
	LPEXTERNAL_CHAR FileName;

	OFFSET_METHOD  BadOffset;
	LPREG_METHOD   BadMethod;

	BYTECODE_FILE(LPSTRING_CLASS StrClass, LPMETHOD_CLASS MtdClass)
	{
		StringClass = StrClass;
		MethodClass = MtdClass;
		CountInterators = SizeInteratorSection = SizeLpInteratorSect = CountInMetodArr = CountInStringArr = 0;
		StringArr = NULL;
		Methods = NULL;
		InteratorSection = NULL;
		LpInteratorSect = NULL;
	}

	~BYTECODE_FILE()
	{
		if(StringArr)
			MEM_FREE(StringArr);
		if(Methods)
			MEM_FREE(Methods);
		if(LpInteratorSect)
			MEM_FREE(LpInteratorSect);
	}

	ERRORS LoadFile(LPEXTERNAL_CHAR Name);

	BYTECODE_FILE::ERRORS CheckMethod(LPHEADER_METHOD Method, TSIZE_METHOD SizeMethod);
};

#endif