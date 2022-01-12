## 基本概念
### 类的封装
类封装了数据和数据上的操作，这点和结构体很像，C里面结构体里面套函数指针也可以实现类似的封装。
而且C++对于`struct`关键字还做了加强，在结构体里面也能写函数，也有访问控制，如：
```cpp
struct myS
{
private:
public:
  void func()
  {
    ...
  }
protected:
};
```
唯一的区别就是`struct`里默认属性的为`public`，而`class`默认属性为`private`。

`#pragma once`的作用就是**防止一个头文件被重复包含**，和
```cpp
#ifndef _MYTEACHER_H_
#define _MYTEACHER_H_
...
#endif
```
的作用是一样的
