// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

// 添加要在此处预编译的标头
#include "framework.h"

#endif //PCH_H

#if 1
# define _DLLExport __declspec (dllexport) //定义该函数的dll
# else  
# define _DLLExport __declspec (dllimport)  //使用该函数
#endif  
typedef int*(__stdcall* MyDelegate)(int value);
typedef void (__stdcall* MyDelegate_error)(int value);
//代表c风格的
extern "C" {
    // int _DLLExport Add(int x, int y, char* bbbb, std::vector<std::string> bbc);
    int _DLLExport Add(int x, int y, char* bbbb, char* bbc[]);
    int _DLLExport Max(int x, int y);
    int _DLLExport GenerateItems(int* itemCount, double* itemsFound);
    int _DLLExport GenerateItems1(int itemCount, const char** stringArray);

    int _DLLExport CreateFlatBuffersFileTest(const char* filename,bool isCreateFile);

    void _DLLExport readgunserializedGuid(const char* filename,int itemCount,  char** stringArray);
    void _DLLExport readgunserializedGuidSize(const char* filename, int* outGuidSize);

    void _DLLExport readgunserializedDependencyHash(const char* filename, int itemCount, char** stringArray);
    void _DLLExport readgunserializedDependencyHashSize(const char* filename, int* outGuidSize);

    void _DLLExport readgunserializedDenpendencies(const char* filename, int itemCount, int itemCount2, int** stringArray , MyDelegate callbackfun, MyDelegate_error delegatefun);
    void _DLLExport readgunserializedDenpendenciesSize(const char* filename, int* outGuidSize);
    int _DLLExport readgunserializedDenpendenciesIntArraySize(const char* filename, int GuidSizes, void* outGuidSize);

}
// isCreateFile 如果文件不存在是否创建文件
int creagame(const char* filename, bool isCreateFile);
const char* readFlatBuffers(const char* filename);

bool isFileExists_stat(const char* filename);
bool isFileExists_ifstream(const char* filename);
bool isFileExists_fopen(const char* filename);

bool isFileExists_access(const char* filename);



