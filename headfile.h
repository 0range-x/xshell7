#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<Windows.h>
#include<tchar.h>
#include <cassert>
#include<sddl.h>
#include <stddef.h>
#include "sha256.h"

BYTE* RC4(char* pDate, int pData_len, BYTE* rc4_key, int rc4_key_len);
void print_usage();

char* base64_decode(char* code);

char* Getsid();

void path_add(char* tmp);
void trave_path();
void free_path();
void find(char* path, char* name);
char* become_char(WCHAR* source, int size);
char* getfileall(char* fname, int MODEL);
int get_len(char* ptr);
char* extract(char* txt, char* name);

#define WCHAR_MODEL 1
#define CHAR_MODEL 0
#define file_Len 128 //文件的最大长度
#define file_Num 256 //文件的最大个数


static void _bswapw(uint32_t* p, uint32_t i);
void* __cdecl _memcp(void* d, const void* s, uint32_t sz);
static void _rtrf(uint32_t* b, uint32_t* p, uint32_t i, uint32_t j);
static void _hash(sha256_context* ctx);
void sha256_init(sha256_context* ctx);
void sha256_hash(sha256_context* ctx, uint8_t* dat, uint32_t sz);
void sha256_done(sha256_context* ctx, uint8_t* buf);



