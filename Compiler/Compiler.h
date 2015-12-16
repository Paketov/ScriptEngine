

#include <string.h>
#include "..\Script\TypeData.h"
#include "..\Script\HashTable.h"
#include "..\Script\Method.h"
#include "..\Script\Opcode.h"

#include "Keywords.h"


#define CHAR_TYPE(Type, Char)  ((sizeof(Type) == sizeof(char))?(Char):(L ## Char))
#define min(a, b) (((a) < (b))?(a):(b))
#define LEN_ARRAY(a) (sizeof(a) / sizeof((a)[0]))
#define ISALPHA(c) isalpha(c)
#define ISDIGIT(c) ((c >= CHAR_TYPE(INTERNAL_CHAR,'0')) && (c <= CHAR_TYPE(INTERNAL_CHAR,'9')))

#define T(a) CHAR_TYPE(INTERNAL_CHAR,a)

class TRANSLATOR
{	
	enum ERRORS
	{
		SUCCESS,
		NOT_OPEN_INPUT_FILE,
		BAD_NUMBER,
		UNEXPECTED_END_FILE,
		OUT_MAX_LEN_NUMBER,
		EXPECTED_NAME_FILE
	};


	typedef unsigned short INDEX_STRING, *LPINDEX_STRING; 

	typedef struct 
	{
		HASH_VAL        vKey;
		INDEX_STRING	Id;
		TSIZE_STR		Len;
		LPINTERNAL_CHAR OutStr;

		HASH_VAL Key()
		{
		    return vKey;
		}
	} STRING_ITEM, *LPSTRING_ITEM;

	
	typedef struct 
	{
		inline HASH_VAL Key()
		{
		   return GenKey(El->NameOpcode);
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
			return strcmp(Str, El->NameOpcode) == 0;
		}
		LPCOMMAND_PROPERTIES El;
	}__COMMAND_ELEMENT;

	typedef struct
	{
        enum
		{
		  CONSTANT,
		  LOCAL_VAR

		};
		unsigned char Type;

		LPINTERNAL_CHAR Name;

		LPHEADER_METHOD BufMethod;
		unsigned LenCode;
		unsigned LenName;
		
	} METHOD_ITEM, *LPMETHOD_ITEM;


	typedef HASH_TABLE<STRING_ITEM, false, INDEX_STRING, (INDEX_STRING)-1>   OUT_STRIG_TABLE, *LPOUT_STRIG_TABLE;

	typedef HASH_TABLE<__COMMAND_ELEMENT, false, unsigned char>   COMMANDS_TABLE, *LPCOMMANDS_TABLE;

	COMMANDS_TABLE	CommandsTable;
	KEYWORD_TABLE   KeywordsTable;
	 
	FILE *			InFile;
	FILE *			ErrOut;
	INTERNAL_CHAR	WordBuf[1000];

	unsigned		LenWordBuf;
	unsigned		Line;

	bool			isHaveDigits;

	bool			isInt;
	double			dNum;
	long long		iNum;
	unsigned char	MaxLenExp;

	unsigned		MaxLenNumber;

	//текущая глубина пространства
	unsigned		CurScopeDeep;

	ERRORS			LastErr;

	LPMETHOD_ITEM	MethodList;

	OUT_STRIG_TABLE OutStrings;

	void OutError(ERRORS Error, LPINTERNAL_CHAR FormatMsg, ...);

	bool InitCommandTable();

	bool InitKeywordTable();

	bool InterpretAsKeyword(LPINTERNAL_CHAR Lecsem);
	bool InterpretAsIdentificator(LPINTERNAL_CHAR Lecsem);

public:

	TRANSLATOR(LPINTERNAL_CHAR NameInFile);

	bool ReadGlobalScope();


	INTERNAL_CHAR ReadWord(INTERNAL_CHAR FirstChar);

	INTERNAL_CHAR ReadNumber(INTERNAL_CHAR FirstChar);

	void Compile();

};