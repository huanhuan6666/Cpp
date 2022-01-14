//
//
#ifndef SOCKET_C___CSOCKETPROTOCOL_H
#define SOCKET_C___CSOCKETPROTOCOL_H
#include <iostream>
using namespace std;
#define IN
#define OUT

class CSocketProtocol { //定义接口即抽象类，里面最好只有成员函数
public:
    virtual ~CSocketProtocol()
    {
        ;
    }

    virtual int SocketInit() = 0;

    virtual int SocketSend(IN char *buf, int buflen) = 0;

    virtual int SocketRev(IN char *buf, IN int *buflen) = 0;

    virtual int SocketDestory() = 0;
};

class CSckFactoryImp1 : public CSocketProtocol
{
private:
    int m_len;
    char *m_p;
public:
    ~CSckFactoryImp1()
    {
        ;
    }
    int SocketInit() override {
        cout << "CSckFactoryImp1 Init!" << endl;
        m_p = NULL;
        m_len = 0;
        return 0;
    }

    int SocketSend(char *buf, int buflen) override {
        if(buf == NULL || buflen <= 0)
            return -1;
        cout << "CSckFactoryImp1 Send!" << endl;
        m_len = buflen;
        m_p = new char[m_len + 1];
        strncpy(m_p, buf, buflen);
        return 0;
    }

    int SocketRev(char *buf, int *buflen) override {
        if(buf == NULL || buflen == NULL)
            return -1;
        cout << "CSckFactoryImp1 Recv!" << endl;
        strncpy(buf, m_p, m_len);
        *buflen = m_len;
        return 0;
    }

    int SocketDestory() override {
        cout << "CSckFactoryImp1 Destory!" << endl;
        if(m_p != NULL)
            delete[] m_p;
        m_len = 0;
        return 0;
    }
};

class CSckFactoryImp2 : public CSocketProtocol
{
private:
    int m_len;
    char *m_p;
public:
    ~CSckFactoryImp2()
    {
        ;
    }
    int SocketInit() override {
        cout << "CSckFactoryImp2 Init!" << endl;
        m_p = NULL;
        m_len = 0;
        return 0;
    }

    int SocketSend(char *buf, int buflen) override {
        if(buf == NULL || buflen <= 0)
            return -1;
        cout << "CSckFactoryImp2 Send!" << endl;
        m_len = buflen;
        m_p = new char[m_len + 1];
        strncpy(m_p, buf, buflen);
        return 0;
    }

    int SocketRev(char *buf, int *buflen) override {
        if(buf == NULL || buflen == NULL)
            return -1;
        cout << "CSckFactoryImp2 Recv!" << endl;
        strncpy(buf, m_p, m_len);
        *buflen = m_len;
        return 0;
    }

    int SocketDestory() override {
        cout << "CSckFactoryImp2 Destory!" << endl;
        if(m_p != NULL)
            delete[] m_p;
        m_len = 0;
        return 0;
    }
};

#endif //SOCKET_C___CSOCKETPROTOCOL_H
