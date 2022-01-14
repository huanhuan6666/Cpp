//
//
#ifndef SOCKET_C___CENCDESPROTOCOL_H
#define SOCKET_C___CENCDESPROTOCOL_H
#include <iostream>
#include <cstring>

#define SIZE 4096
using namespace std;

class CEncDesProtocol {
public:
    virtual ~CEncDesProtocol() //虚析构函数要写上，函数体为空就行
    {

    }
    virtual int DesEnc(
            unsigned char *pInData,
            int nInDataLen,
            unsigned char *pOutData,
            int *pOutDataLen) = 0;

    virtual int DesEnc_raw(
            unsigned char *pInData,
            int nInDataLen,
            unsigned char *pOutData,
            int *pOutDataLen) = 0;

    virtual int DesDec(
            unsigned char *pInData,
            int nInDataLen,
            unsigned char *pOutData,
            int *pOutDataLen) = 0;

    virtual int DesDec_raw(
            unsigned char *pInData,
            int nInDataLen,
            unsigned char *pOutData,
            int *pOutDataLen) = 0;

    int encode_file(const char *fileInName, const char *fileOutName);

    int decode_file(const char *fileInName, const char *fileOutName);
};

#endif //SOCKET_C___CENCDESPROTOCOL_H
