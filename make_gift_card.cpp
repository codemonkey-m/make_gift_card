#include <iostream>
#include <string.h>
#include <time.h>
using namespace std;

#define ArrayCount(a)	(sizeof(a) / sizeof((a)[0]))

#define ReturnErr(err, str) {\
	if(err != 0)\
		cout << str << endl;\
	exit(err);\
}

#define DICT_SIZE 32
const char AwardCodeDict[DICT_SIZE] = {
	'V','W','X','Y','Z','U','P','J',
	'K','M','5','6','Q','R','S','T',
	'1','E','F','G','7','8','9','N',
	'A','B','C','H','2','3','4','D'
};

typedef unsigned long long  UINT64;

#define AWARD_CODE_BIT 5
#define AWARD_CODE_NUM 10

UINT64 GenerateAwardCodeVaule(UINT64 awardId)
{
    UINT64 codeVal = 0;
    for(int i = 0; i < AWARD_CODE_NUM; i++)
    {
        UINT64 key = rand() % DICT_SIZE;    
        codeVal |= key << (AWARD_CODE_BIT * i);
    }
    codeVal |= awardId << (AWARD_CODE_BIT * AWARD_CODE_NUM);
    return codeVal;
}

int DecodeAwardCodeValue(UINT64 codeVal, char* code)
{
    int pos = 0;
    while(codeVal)
    {
        int key = codeVal & 0x1F;
        code[pos++] = AwardCodeDict[key];
        codeVal = codeVal >> AWARD_CODE_BIT;
    }
    return pos;
}

int GetKeyFromDict(char ch)
{
    int key = -1;
    for(int i = 0; i < DICT_SIZE; i++)
    {
        if(toupper(ch) == AwardCodeDict[i]){
            key = i;
            break;
        }
    }
    return key;
}

UINT64 GetAwardID(const char* code)
{
    if(!code || strlen(code) <= AWARD_CODE_NUM) return 0;

    UINT64 awardId = 0;
    for(int i = AWARD_CODE_NUM; i < strlen(code); i++)
    {
        UINT64 val = GetKeyFromDict(code[i]);
        awardId |= val << (AWARD_CODE_BIT * (i - AWARD_CODE_NUM));
    }
    return awardId;
}

UINT64 GetAwardCodeVaule(const char* code)
{
    if(!code) return 0;

    UINT64 codeVal = 0;
	for (int i = strlen(code) - 1; i >= 0; i--)
    {
        UINT64 key = GetKeyFromDict(code[i]);       
        codeVal |= key << (AWARD_CODE_BIT * i);
	}
	
	return codeVal;
}

int MakeGiftCard(int argc, char** argv)
{
	if (argc != 4)
		ReturnErr(-101, "make key param not matching!");

	//id限制
	unsigned nID = atoi(argv[2]);
	if (nID >= unsigned(1 << (sizeof(UINT64) * 8 - AWARD_CODE_BIT * AWARD_CODE_NUM)))
		ReturnErr(-102, "gift id is too big!");

	//数量限制
	unsigned nNum = atoi(argv[3]);
	if (nNum > 100000)
		ReturnErr(-1, "gift count is too much!");

	srand((unsigned)time(NULL));
	char code[32] = { 0 };
	for (int i = 0; i < nNum; ++i)
	{
		DecodeAwardCodeValue(GenerateAwardCodeVaule(nID), code);
		cout << code << endl;
		memset(code, 0, 32);
	}

	ReturnErr(0, "");
}

int GetID(int argc, char** argv)
{
	if (argc != 3)
		ReturnErr(-101, "get id, param count not matching!");

	cout << GetAwardID(argv[2]) << endl;

	ReturnErr(0, "");
}

struct FunctionInfo
{
	char	szName[32];
	int(*handle)(int argc, char** argv);
};

FunctionInfo AllFunction[] = {
	{"make", &MakeGiftCard,},
	{"getid", &GetID,},
};

int main(int argc, char** argv){
	if (argc < 2)
		ReturnErr(-1, "param count not enough!");

	for (size_t i = 0; i < ArrayCount(AllFunction); i++)
	{
		if (strcmp(AllFunction[i].szName, argv[1]) == 0)
			return AllFunction[i].handle(argc, argv);
	}

	ReturnErr(-2, "can't find operation");
}
