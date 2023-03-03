#include "headfile.h"

#define MAX_NAME 260

char* Getsid();

LPWSTR wSid = NULL;
TCHAR UserName[64], DomainName[64];

char* Getsid() {
	HANDLE hToken;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken))
	{
		printf("[!]OpenProcessToken error\n");
		return 0;
	}

	DWORD Size, UserSize, DomainSize;
	SID* sid;
	SID_NAME_USE SidType;


	TOKEN_USER* User;
	Size = 0;

	//查看当前用户的token
	GetTokenInformation(hToken, TokenUser, NULL, 0, &Size);
	if (!Size)
		return 0;

	User = (TOKEN_USER*)malloc(Size);
	assert(User);
	GetTokenInformation(hToken, TokenUser, User, Size, &Size);
	assert(Size);
	Size = GetLengthSid(User->User.Sid);
	assert(Size);
	sid = (SID*)malloc(Size);
	assert(sid);

	CopySid(Size, sid, User->User.Sid);
	UserSize = (sizeof UserName / sizeof * UserName) - 1;
	DomainSize = (sizeof DomainName / sizeof * DomainName) - 1;
	LookupAccountSid(NULL, sid, UserName, &UserSize, DomainName, &DomainSize, &SidType);


	int ret = ConvertSidToStringSid(User->User.Sid, &wSid);
	if (FAILED(ret)) {
		printf("Failed to return ret.");
		printf("Error code=0x");
		printf("%4x", ret);
	}
	//printf("whoami/user:%S\\%S ", DomainName, UserName);
	//printf(" %S\n", wSid);
	char* result = strcat(UserName, wSid);
	free(sid);
	free(User);
	return result;
}