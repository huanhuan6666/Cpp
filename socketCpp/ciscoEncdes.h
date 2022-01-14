//
//
#include "CEncDesProtocol.h"
#ifndef SOCKET_C___CISCOENCDES_H
#define SOCKET_C___CISCOENCDES_H


class ciscoEncdes : public CEncDesProtocol{
public:
    int DesEnc(unsigned char *pInData, int nInDataLen, unsigned char *pOutData, int *pOutDataLen) override;

    int DesEnc_raw(unsigned char *pInData, int nInDataLen, unsigned char *pOutData, int *pOutDataLen) override;

    int DesDec(unsigned char *pInData, int nInDataLen, unsigned char *pOutData, int *pOutDataLen) override;

    int DesDec_raw(unsigned char *pInData, int nInDataLen, unsigned char *pOutData, int *pOutDataLen) override;
};


#endif //SOCKET_C___CISCOENCDES_H
