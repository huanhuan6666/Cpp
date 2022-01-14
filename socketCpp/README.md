## C++模型设计练习
> 对于面向抽象类编程（面向接口编程）思想的练习

### 场景
有一个信息系统的框架，要集成**第三方**的Socket通信产品和加密产品。要求能支持**多个**第三方通信产品和加密产品的入围，信息系统的框架不轻易发生变化。

### 分析
说到底根本的方法和C语言一样，第一步都是设计API，即我们到底需要完成的函数是什么样子的，然后搭建测试框架，最后实现函数体。

而所谓的面向抽象类编程就是把那些待实现的API**封装成一个抽象类**，其实就是Java里的**接口**，之后所有的具体实现都必须继承这个抽象类然后实现所有的纯虚函数。

比C语言的那套方法高级的地方在于：可以有**多种**类实现同一套接口，我们的**框架是固定的**，框架可以灵活更换不同的实现，这就达到了框架和具体实现的**解耦**，符合**开闭原则**。

* 这里的接口实现本质原理就是C++提供的**抽象类的性质**：子类必须实现接口类的所有纯虚函数。
* 通过C语言的**函数指针**也可以实现类似的接口：函数指针规定好函数原型，必须按照函数指针规定的原型来实现函数。

### 实现
看看需要几个类：通信产品socket类，还有加密解密产品类，这都是抽象类。具体的通信实现和加密解密实现类只需要继承抽象类就OK啦。
抽象类接口如下：
```cpp
class CSocketProtocol {
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
```
注意里面有个虚析构函数，是为了`delete sp`可以正确的释放子类内存，虽然例子里面全让`Destory`释放完了hhh

解密加密的接口如下：
```cpp
class CEncDesProtocol {
public:
    virtual ~CEncDesProtocol() //虚析构函数
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
```
这里还是说一下虚析构函数吧，毕竟抽象类里面我们一般都不放数据成员，因此函数体为空，但是不能不写，因为实现类对象能否析构全看这个函数是不是`virtual`，不然实现类的析构函数根本不会被调用，因此**抽象类里必须写虚析构函数！**

还有就是框架类的实现，**用组合代替继承**。

就是说例子太简单代码几乎全是clion生成的🤤
加密实现在C的那个仓库里，实在懒得写了。