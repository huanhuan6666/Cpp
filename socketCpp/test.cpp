#include <iostream>
#include <cstring>

#include "CSocketProtocol.h"
#include "ciscoEncdes.h"
//C++的框架类

class MainOP {
private:
    CSocketProtocol *sp; //用组合代替继承
    CEncDesProtocol *ep;
public:
    MainOP() {
        sp = NULL;
        ep = NULL;
    }

    MainOP(CSocketProtocol *&sp, CEncDesProtocol *&ep) { //注入法
        this->sp = sp;
        this->ep = ep;
    }

    ~MainOP() {
        if (sp != NULL)
            delete sp;
        if (ep != NULL)
            delete ep;
    }

    void setSp(CSocketProtocol *&sp) {
        this->sp = sp;
    }

    void setEp(CEncDesProtocol *&ep) {
        this->ep = ep;
    }

    int TestSp()
    {
        int ret = 0;
        char outbuf[4096] = {0};
        int outlen = 20;
        char inbuf[4096] = {0};
        int inlen = 0;

        ret = sp->SocketInit();
        if (ret != 0) goto END;
        strcpy(outbuf, "asdasdasd");

        ret = sp->SocketSend(outbuf, outlen);
        if (ret != 0) goto END;

        ret = sp->SocketRev(inbuf, &inlen);
        if (ret != 0) goto END;

        ret = sp->SocketDestory();
        if (ret != 0) goto END;

        END:
        return ret;
    }

};

//C语言风格的框架函数，用main函数调用
int testSocket(CSocketProtocol *sp) //注意是父类指针
{
    int ret = 0;
    if (sp == NULL) {
        ret = -1;
        return ret;
    }
    char outbuf[4096] = {0};
    int outlen = 20;
    char inbuf[4096] = {0};
    int inlen = 0;

    ret = sp->SocketInit();
    if (ret != 0) goto END;
    strcpy(outbuf, "asdasdasd");

    ret = sp->SocketSend(outbuf, outlen);
    if (ret != 0) goto END;

    ret = sp->SocketRev(inbuf, &inlen);
    if (ret != 0) goto END;

    ret = sp->SocketDestory();
    if (ret != 0) goto END;

    END:
    return ret;
}

int main() {
    CSocketProtocol *sp = NULL;
    int ret = 0;
    sp = new CSckFactoryImp1; //父类指向子类
    //ret = testSocket(sp);
    delete sp;
    sp = new CSckFactoryImp2; //指向另一个实现类
    //ret = testSocket(sp); //测试框架不变
    //delete sp;

    CEncDesProtocol* ep = new ciscoEncdes;
    //框架类方法
    MainOP OP(sp, ep);
    OP.TestSp();

    return 0;
}
