// dllmain.cpp : 定义 DLL 应用程序的入口点。
#define  _CRT_SECURE_NO_WARNINGS 1
#include "pch.h"


// #include <stdio.h>//引入C的库函数
#include <fstream>
#include<iostream>
#include<vector>
#include<limits>
#include<io.h>
#include <algorithm>
#include "SaveSchema_generated.h"
#include "ssssss_generated.h"
#include "fast_generated.h"


#include<iostream>
#include<vector>
// dllmain.cpp : 定义 DLL 应用程序的入口点。



//宏定义  
#define  EXPORTBUILD 
using namespace PiNet;
void test(const char* filename, const  std::string giuid[]) {
    flatbuffers::FlatBufferBuilder builder(1024);

    // table ConvT
    auto ConvT = new PiNet::ConvT;
    ConvT->kernelX = 3;
    ConvT->kernelY = 3;
    // union ConvUnionOpParameter
    OpParameterUnion ConvUnionOpParameter;
    ConvUnionOpParameter.type = OpParameter_Conv;
    ConvUnionOpParameter.value = ConvT;
    // table OpT
    auto ConvTableOpt = new PiNet::OpT;
    ConvTableOpt->name = "Conv";
    ConvTableOpt->inputIndexes = { 0 };
    ConvTableOpt->outputIndexes = { 1 };
    ConvTableOpt->type = OpType_Conv;
    ConvTableOpt->parameter = ConvUnionOpParameter;

    // table PoolT
    auto PoolT = new PiNet::PoolT;
    PoolT->padX = 3;
    PoolT->padY = 3;
    // union OpParameterUnion
    OpParameterUnion PoolUnionOpParameter;
    PoolUnionOpParameter.type = OpParameter_Pool;
    PoolUnionOpParameter.value = PoolT;
    // table Opt
    auto PoolTableOpt = new PiNet::OpT;
    PoolTableOpt->name = "Pool";
    PoolTableOpt->inputIndexes = { 1 };
    PoolTableOpt->outputIndexes = { 2 };
    PoolTableOpt->type = OpType_Pool;
    PoolTableOpt->parameter = PoolUnionOpParameter;

    // table NetT
    auto netT = new PiNet::NetT;
    netT->oplists.emplace_back(ConvTableOpt);
    netT->oplists.emplace_back(PoolTableOpt);
    netT->tensorName = { "conv_in", "conv_out", "pool_out" };
    netT->outputName = { "pool_out" };
    // table Net
    auto net = CreateNet(builder, netT);
    builder.Finish(net);

    // This must be called after `Finish()`.
    uint8_t* buf = builder.GetBufferPointer();
    int size = builder.GetSize();  // Returns the size of the buffer that
    //`GetBufferPointer()` points to.
    std::ofstream output(filename, std::ofstream::binary);
    output.write((const char*)buf, size);
}

void readflatbuff(const char* filename) {
	
    std::ifstream infile;
    infile.open(filename, std::ios::binary | std::ios::in);
    infile.seekg(0, std::ios::end);
    std::streampos length = infile.tellg();
    // std::streampos;
    infile.seekg(0, std::ios::beg);
    char* buffer_pointer = new char[length];
    infile.read(buffer_pointer, length);
    infile.close();

    auto net = GetNet(buffer_pointer);

    auto ConvOp = net->oplists()->Get(0);
    auto ConvOpT = ConvOp->UnPack();

    auto PoolOp = net->oplists()->Get(1);
    auto PoolOpT = PoolOp->UnPack();

    auto inputIndexes = ConvOpT->inputIndexes;
    auto outputIndexes = ConvOpT->outputIndexes;
    auto type = ConvOpT->type;
    std::cout << "inputIndexes: " << inputIndexes[0] << std::endl;
    std::cout << "outputIndexes: " << outputIndexes[0] << std::endl;

    PiNet::OpParameterUnion OpParameterUnion = ConvOpT->parameter;
    switch (OpParameterUnion.type) {
    case OpParameter_Conv: {
        auto ConvOpParameterUnion = OpParameterUnion.AsConv();
        auto k = ConvOpParameterUnion->kernelX;
        std::cout << "ConvOpParameterUnion, k: " << k << std::endl;
        break;
    }
    case OpParameter_Pool: {
        auto PoolOpParameterUnion = OpParameterUnion.AsPool();
        auto k = PoolOpParameterUnion->padX;
        std::cout << "PoolOpParameterUnion, k: " << k << std::endl;
        break;
    }
    default:
        break;
    }
}


bool isFileExists_stat(const char* filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

bool isFileExists_ifstream(const char* filename) {
    std::ifstream f(filename);
    return f.good();
}

bool isFileExists_fopen(const char* filename) {
    if (FILE* file = fopen(filename, "r")) {
        fclose(file);
        return true;
    }
    else {
        return false;
    }
}

bool isFileExists_access(const char* filename)
{
    if (_access(filename, 0) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int creagame(const char* filename, bool isCreateFile) {
    if (isFileExists_access(filename)) {
        return 1;
    }
    if (!isCreateFile)return 2;
    using namespace Companyage;
    //CreateGun()
    flatbuffers::FlatBufferBuilder builder(1024);
    
    GunT* gu = new GunT();
    for (int i = 0; i < 560000; i++) {
        gu->serializedDependencyHash.emplace_back("xxgdfgfdggggggggggggggggrewwwwwwwrrrrrtttttttttttttttthdrgsefweawfeasfesfsdvsdfv");
        gu->serializedGuid.emplace_back("xxxdd xxgdfgfdggggggggggggggggrewwwwwwwrrrrrtttttttttttttttthdrgsefweawfeasfesfsdvsdfv");
        
        IntArrayT* b =  new IntArrayT();
        if (i % 3 == 0) {
            b->values.emplace_back(1);// = { 1,2,3 };
            b->values.emplace_back(2);
            b->values.emplace_back(3);
            b->values.emplace_back(4);
            
            b->values.emplace_back(5);
        }
        else if(i%2==0) {
            //b->values = { 1,2,3,4,5,6,7,8,9,1,0,2,2,2 };
            b->values.emplace_back(9);
            b->values.emplace_back(10);
        }
        else {
            b->values = {  };
        }
        
       
        gu->serializedDenpendencies.emplace_back(b);
        
    }
    auto bsdf = CreateGun(builder, gu);
    builder.Finish(bsdf);

    // This must be called after `Finish()`.
    uint8_t* buf = builder.GetBufferPointer();
    int size = builder.GetSize();  // Returns the size of the buffer that
    //`GetBufferPointer()` points to.
    // 文件不存在才会创建测试文件
    std::ofstream output(filename, std::ofstream::binary);
    output.write((const char*)buf, size);
    output.close();
    return 0;
    
    
    

}

void _DLLExport readgunserializedGuid(const char* filename, int itemCount, char** stringArray)
{
    using namespace Companyage;
    std::ifstream infile;
    infile.open(filename, std::ios::binary | std::ios::in);
    infile.seekg(0, std::ios::end);
    std::streampos length = infile.tellg();
    infile.seekg(0, std::ios::beg);
    char* buffer_pointer = new char[length];
    infile.read(buffer_pointer, length);
    infile.close();

    auto net = GetGun(buffer_pointer);
    //net->serializedDependencyHash()->GetAsString();
    int Guidsize = net->serializedGuid()->size();
    char** bbb = new char* [Guidsize];
    
    for (int i = 0; i < (std::min)(itemCount, Guidsize); i++) {
        bbb[i] = const_cast<char*>(net->serializedGuid()->Get(i)->c_str());
        stringArray[i] = _strdup(bbb[i]);
    }   
    // 释放内存
    delete buffer_pointer;
    return;
}

void _DLLExport readgunserializedGuidSize(const char* filename, int* outGuidSize)
{
    using namespace Companyage;
    
    const char* buffer_pointer = readFlatBuffers(filename);;
    
    auto net = GetGun(buffer_pointer);
    //net->serializedDependencyHash()->GetAsString();
    int Guidsize = net->serializedGuid()->size();
    *outGuidSize = Guidsize;
    //return void _DLLExport();
}

void _DLLExport readgunserializedDependencyHash(const char* filename, int itemCount, char** stringArray)
{
    using namespace Companyage;
    const char*  tempchar = readFlatBuffers(filename);
    auto net = GetGun(tempchar);



    int Guidsize = net->serializedDependencyHash()->size();
    char** bbb = new char* [Guidsize];

    for (int i = 0; i < (std::min)(itemCount, Guidsize); i++) {
        bbb[i] = const_cast<char*>(net->serializedDependencyHash()->Get(i)->c_str());
        stringArray[i] = _strdup(bbb[i]);
    }


    // 释放内存
    delete tempchar;

    //return void _DLLExport();
}

void _DLLExport readgunserializedDependencyHashSize(const char* filename, int* outGuidSize)
{
    //readFlatBuffers(filename);
    using namespace Companyage;
    const char* tempchar = readFlatBuffers(filename);
    auto net = GetGun(tempchar);
    //net->serializedDependencyHash()->GetAsString();
    int Guidsize = net->serializedDependencyHash()->size();
    *outGuidSize = Guidsize;

    // 释放内存
    delete tempchar;
}

void _DLLExport readgunserializedDenpendencies(const char* filename, int itemCount, int itemCount2, int** stringArray, MyDelegate callbackfun, MyDelegate_error delegatefun)
{
    using namespace Companyage;
    const char* tempchar = readFlatBuffers(filename);
    auto net = GetGun(tempchar);
    
    int Guidsize = net->serializedDenpendencies()->size();
    
    for (int i = 0; i < (std::min)(itemCount, Guidsize); i++) {
        auto vlueas = net->serializedDenpendencies()->Get(i)->values();
        if (vlueas) {
            
            memcpy(stringArray[i], vlueas->data(), vlueas->size()*sizeof(int32_t));


            //for (int j = 0; j < vlueas->size(); j++) {
              //  stringArray[i][j] =  vlueas->Get(j);
           // }
            
        }
    }

    // 释放内存
     delete tempchar;
     tempchar = nullptr;

    
}

void _DLLExport readgunserializedDenpendenciesSize(const char* filename, int* outGuidSize)
{
    using namespace Companyage;
    const char* tempchar = readFlatBuffers(filename);
    auto net = GetGun(tempchar);
    //net->serializedDependencyHash()->GetAsString();
    int Guidsize = net->serializedDenpendencies()->size();
    *outGuidSize = Guidsize;

    // 释放内存
    delete tempchar;

    
}

int _DLLExport readgunserializedDenpendenciesIntArraySize(const char* filename, int GuidSizes, void* outGuid)
{
    //return;
    using namespace Companyage;
    const char* tempchar = readFlatBuffers(filename);
    auto net = GetGun(tempchar);
    //net->serializedDependencyHash()->GetAsString();
    int Guidsize = net->serializedDenpendencies()->size();
    //int* bbb = new int [Guidsize];
    int* outGuidSize = static_cast<int*>(outGuid);
    for (int i = 0; i < (std::min)(GuidSizes, Guidsize); i++) {

        try {
            ::flatbuffers::Vector<int32_t>* ssd = const_cast<::flatbuffers::Vector<int32_t> *>(net->serializedDenpendencies()->Get(i)->values());

            if (ssd) {
                int aa = ssd->size();
                outGuidSize[i] = aa;
            }
            else {
                outGuidSize[i] = 0;
                continue;
                // return i;
            }
        }
        catch (std::exception &e) {
            //errormes = const_cast<char*>(e.what());


        }
       
    }
   
    
    // 释放内存
    delete tempchar;
    return 0;
}

const char* readFlatBuffers(const char* filename)
{
    std::ifstream infile;
    infile.open(filename, std::ios::binary | std::ios::in);
    infile.seekg(0, std::ios::end);
    std::streampos length = infile.tellg();
    infile.seekg(0, std::ios::beg);
    char* buffer_pointer = new char[length];
    infile.read(buffer_pointer, length);
    infile.close();
    return buffer_pointer;
    // return nullptr;
}
//相加
int _DLLExport Add(int x, int y,char* bbbb, char* bbc[])
{
    std::vector<std::string> bbb;
    bbb.push_back("xxx");
    std::string bbdfdfbc[10];
    
   // test(bbbb, bbb);

	return x + y;
}
//取较大的值
int _DLLExport Max(int x, int y)
{
	return (x >= y) ? x : y;
}
int _DLLExport GenerateItems( int* itemCount, double* itemsFound)
{
    //itemsFound[0][0] = 10;
    *itemCount = 10;
    *itemsFound = 2.0;
    //auto items = new std::vector<double>();
    /*double* items = *itemsFound;
    for (int i = 0; i < *itemCount; i++) {
        items->push_back((double)i);
        if (items[i] == 2.0) {
            std::string s[1] = { "GenerateItems" };
            test("fdsfse", s);
        }
    }*/
    //*itemsFound = items->data();
    
    //std::string s[1] = {"GenerateItems"};
    //test(bb[0],s );
    return 0;
}
int _DLLExport GenerateItems1(int itemCount, const char** stringArray)
{
    //(*test).push_back("xxx");
    std::string giuid[] = { "x" };

    test(stringArray[0], giuid);
    return 0;
}

int _DLLExport CreateFlatBuffersFileTest(const char* filename, bool isCreateFile)
{
    return creagame(filename, isCreateFile);
}

