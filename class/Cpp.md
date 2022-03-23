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

### 对象的构造和析构
#### 构造函数分类
* 无参数构造函数
* 带参数构造函数
* 拷贝构造函数
* 如果不显示定义构造函数，C++编译器会自己提供**默认**无参构造函数和**默认**的拷贝构造函数
  * 默认无参构造函数：函数体为**空**
  * 默认拷贝构造函数：对成员变量简单值拷贝，即**浅拷贝**
**无参数构造函数和带参数构造函数**
```cpp
class Test
{
	int m_a;
	int m_b;
public:
	Test()
	{
		m_a = 0;
		m_b = 0;
		std::cout << "无参数的构造函数\n";
	}
	Test(int a)
	{
		m_a = a;
		m_b = 0;
		std::cout << "带一个参数的构造函数\n";
	}
	Test(int a, int b)
	{
		m_a = a;
		m_b = b;
		std::cout << "带参数的构造函数\n";
	}
	Test(const Test& x) //必须写成引用，const可加可不加，最好是常引用防止实参被修改
	{
    m_a = x.m_a;
    m_b = x.m_b;
		std::cout << "拷贝构造函数\n";
	}
};
int main()
{
  //调用无参数构造函数
	Test a1;
  //!!!下面这个这样写，不是想象中的调用默认构造函数，C++编译器认为这是个函数声明！！！
  	Test a1();
  //调用带参数构造函数的三种方法：
  //1.括号法
	Test a2(1, 2);
  //2.直接调用法，涉及到匿名对象
	Test a3 = Test(3, 4);
  //3.赋值法，(1, 2)是个逗号表达式值为2，最终调用的是带一个参数的构造函数
	Test a4 = (1, 2);
	return 0;
}
```
> Test a1();这玩意C++认为是个函数声明，Test a1才是想要的创建对象，就是说C++多少沾点😅

带参数构造函数的第二种方法，还有个`=`难道不会再次涉及到一次赋值构造吗？
答案是**不会**，方法2**只调用一次**带参数的构造函数就完成了a3的**构造和初始化**。这是现代C++的特性，对于匿名对象的直接转换。

* 区分赋值和初始化：
```cpp
Test b;
Test a = Test(3, 4); //定义处通过带参数的构造函数初始化
a = b; //这是对象的赋值
```
**拷贝构造函数**
* 为什么可以访问引用参数的私有数据？
因为访问控制是针对类来讲而不是针对对象，拷贝构造函数在类的内部，当然可以访问它的`private`数据。
* 初始化时
```cpp
Tset(const Test &p)//参数一般为const引用
{
 ...
}
Test a1(1, 2); //带参数的构造函数

Test a2 = a1; // 初始化法，定义a2用拷贝构造函数初始化
Test a3(a1); // 括号法
```
* 函数形参
```cpp
int func(Test p)
{
  cout << p.getx(); //形参会在func结束时调用析构函数析构掉
}
int main()
{
  Test a1(1, 2);
  func(a1); //函数调用时会调用拷贝构造函数，初始化形参变量
}
```
如果函数参数是个**引用**的话，就**不用拷贝**了，形参就是对象的别名。

* 函数返回值
```cpp
Test func()
{
  Test a(1, 2);
  return a;
}
```
函数返回对象时，做法如下：
a调用拷贝构造函数构造一个**匿名对象**，随后a调用析构函数析构
也就是函数实际上返回的是一个**匿名对象**。

之后**分情况**：
```cpp
Test b = func(); //这时不再调用拷贝构造函数去构造b，而是直接将匿名对象转换成b，因此它不会被析构而是有了名字b

Test c;
c = func(); //这是赋值操作，不涉及构造函数，匿名对象在赋值c之后会被析构掉
```
> 关于匿名对象：
> ```cpp
> Test(1, 2);
> ```
> 这个匿名对象，执行这一行结束，将调用构造函数和析构函数，也就是这句话一结束，匿名对象就被析构了，**生命周期就只有这一行**，因为没人接它：要么初始化别人转正，要么赋值给别人后被析构。
> 比如：
> ```cpp
> class A
> {
> 	A(int a, int b, int c)
> 	{
> 		m_a = a;
> 		m_b = b;
> 		m_c = c;
>	}
>	A(int a, int b)
>	{
>		m_a = a;
>		m_b = b;
>		A(a, b, 100); //这句话就是生成个匿名对象，而且声明周期就这一行，也就是说m_c没有被赋值
>	}
> }
> int main()
> {
> 	A(1, 2);
> 	cout << c;
> }
> ```
> 输出c不是10，而是个乱码

#### 深拷贝和浅拷贝
默认拷贝构造函数实现的**就是浅拷贝**，只能简单的将值复制。
如果对象属性有**指针**并且指向**堆区**的一块内存，那么浅拷贝后两个对象的那个指针属性将指向堆区同一块内存，这就是浅拷贝，如果一个对象释放掉这个内存的话另一个对象的**指针就悬挂了**。

我们想要的拷贝是两个对象各自指向**自己的**内存，并且内存里的内容相等，这就是深拷贝。
**深拷贝只能手动编写实现**，分配了新的内存空间。显然深拷贝之后，指针变量的值是不一样的，毕竟指向了不同的内存，只是内存里存的东西是一样的。

这里还是要注意初始化和赋值的区别：
```cpp
Test a = b; //初始化，调用拷贝构造函数
Test c;
c = b; //赋值，使用C++提供的默认等号操作
```
等号操作符 C++默认**也是浅拷贝**，因此要想让赋值也是深拷贝，必须**显式重载等号操作符**

#### 构造函数初始化列表
* 初始化列表出现动机：
1.如果类中包含另一个类，即对象成员，如果该对象成员的类**没有**默认构造函数而定义了带参数的构造函数，那用参数初始化对象成员只能在**初始化列表**上实现，放在构造函数内会报错

2.类中的**const成员**和**引用**类型成员，由于必须在**声明时就初始化**，而且初始化之后就**不能再次赋值**了。如果直接在声明的时候就写好值也可以但是太死板，能不能实现那种传个参数然后让const或者引用变量成为我们传入的值？这就是初始化列表的意义，我们没办法**在函数体中赋值**，所以我们干脆就不进入函数体了，直接在初始化列表完成初始化，完美解决问题。
```cpp
class A
{
private:
	Test m_a;
	Test m_b;
	int m_b;
public:
	A(): m_b(2), m_a(1)
	{;}
	
	A()       //会报错
	{m_a(1);}
}
```
初始化的顺序是和定义顺序一致，于初始化列表的顺序**无关**。执行完初始化列表后，再执行函数体。

* **建议使用**初始化列表
在进入构造函数体**之前**，先扫描初始化列表，并且初始化列表是**直接带着初始值**创建每个成员的，函数体还得先创建成员一次，再在函数体内部赋值一次，就是说非常的低效了属于是😕

也就是说，初始化列表**更高效**，并且所有的构造函数都能使用初始化列表，非常的好用🤭


#### 构造函数和析构函数的调用顺序
对象里组合了其他对象，先调用其他对象的构造函数，最后调用自己的。**析构顺序和构造顺序相反**。

### 什么时候需要自己编写构造函数
我们知道一旦自己编写了构造函数，那么编译器提供的那些默认构造函数，默认拷贝构造函数和默认赋值构造函数(Special Member Function)就失效了，当然也能用`= default`的关键字保留默认构造函数。

这里我们就探讨到底什么时候需要自己编写构造函数：当默认构造函数满足不了的时候，**典型的就是**我们的**指针成员变量**需要**动态分配内存空间**的时候。

* **原则**：Rule of 3 (C++ 98)
如果显式定义了拷贝构造函数，拷贝赋值操作符或者析构函数，那么这三个要么**全部**自定义，要么全不写。因为一旦写这些东西就说明你正在**手动控制一些资源**，必须完完整整从出生到死亡安排地明明白白！

### 转移构造函数和转移赋值操作符(move)
也就是C++11提供的move机制，避免了无意义的多次拷贝。
```cpp
class Widget{
Widget (Widget&& rhs);                // move constructor
Widget& operator = (Widget&& rhs);    // move assignment operator
}
```
所谓的move操作将在下面条件都满足时发生：
* 类中没有拷贝构造函数、拷贝赋值操作符、析构函数的显式声明
* 结合之前的原则，也就是**全部使用**编译器提供的**默认**函数。当然不代表我们就不能自己写构造函数了，这可以通过`=default`关键字做到：
```cpp
Widget(Widget&&) = default;
Widget& operator=(Widget&&) = default;        // support moving
Widget(const Widget&) = default;
Widget& operator=(const Widget&) = default;   // support copying
```

### 动态分配内存
C++和C的区别：
* C中是`malloc`和`free`，是函数
* C++中是`new`和`delete`，和`sizeof`一样是操作符，效率更高
* **更重要的**，`new`和`delete`创建和销毁**对象**时，可以**自动调用**构造函数和析构函数，而`malloc`和`free`**无法做到**这一点。因此可以`Test *p = new Test(10)`这样带参数构造。
```cpp
//分配一块内存
int *p = (int *)malloc(sizeof(int));
free(p);
int *p1 = new int;
delete p1;

//分配数组
int *p = (int *)malloc(sizeof(int)*10);
free(p);
int *p1 = new int[10];
delete []p1;  //注意区别，释放数组必须要delete[]
```

### 静态成员变量和成员函数
* 静态成员变量，表示该类的所有对象，共享这一个静态变量
* 静态成员函数，**只能访问类中的静态成员变量**
为什么？
```cpp
class A
{
private:
	int m_a;
	int m_b;
	static int m_c;
public:
	static void func()
	{
		cout << m_a; //报错
		cout << m_c; //只能访问静态成员
	}	
}
int A::m_c = 0; //静态数据成员的初始化
int main()
{
	A::func();
	A a;
	a.func();
}
```
* 因为只有**静态成员**才允许通过类名限定来访问:`A::func()`，`A::m_c`。而如果用`A::func()`访问的静态函数中引用了普通成员，类A下有很多对象，将**无法确定到底引用的是哪个对象**，而该类静态成员变量确定只有一个，因此可以访问。

### C++面向对象模型
* 数据成员和成员函数分开存储
* 数据成员
	* 静态成员：存储在全局数据区
 	* 非静态成员： 存在栈区，布局和struct相似
* 成员函数：存储在**代码区**中
	* 普通函数：调用时隐式传入**对象的this指针** ，来确定访问哪个对象
	* 静态函数：调用时不会传入this，因此无法访问普通变量，只能访问静态成员变量
 	* 虚函数

* this指针
```cpp
void getadd() const
{
	m_x = 1; // 报错
	cout << m_x;
}
```
用const修饰**成员函数**表示函数体禁止修改`this`指针所指向的对象，这是因为原本的`this`编译成`Test char* const this`，而用`const`修饰函数体后，`this`指针编译成了`const Test char* const this`。
**通过this指针返回对象自身的引用**
```cpp
//add是一个成员函数
Test& add(Test &t2)
{
	this->m_a += t2.m_a; //this->可写可不写，毕竟在类的内部
	this->m_b += t2.m_b;
	return *this;
}
```
我们知道不能返回局部对象的引用，因为函数一结束引用的内存就被销毁掉了，这里返回的`*this`在函数结束后可不会被销毁，它是安全的内存。

### 友元函数和友元类
一般我们都把类中的数据成员设置成`private`，这确实安全。但是访问数据只能用`getaddr()`这种接口函数，实在太傻，由于访问控制导致使用起来反而不如结构体那么来的直接痛快。
友元函数和友元类就是解决这个问题的，在类的内部**声明**友元函数/友元类，那么该友元函数/友元类就可以像结构体那样，直接访问私有数据成员。比如：
```cpp
class A
{
friend class B; //B是A的友元类
private:
	int m_a;
	int m_b;
public:
	friend void func(const A&x, const A&y); //func是A的友元函数
	friend void func1(A *p1, A *p2); // func1也是A的友元函数
}
void func(const A&x, const A&y)
{
	cout << x.m_a << y.m_b; //直接访问A的私有数据成员
}
void func1(A *p1, A *p2)
{
	cout << p1->m_a < p2->m_b; //用指针也可以直接访问A的嘶有数据成员
}

class B
{
private:
	A a;
public:
	void func()
	{
		cout << a.m_a << a.m_b; //由于B是A的友元类，因此即使在类A外部也可以直接访问A的私有数据
	}
}
```
从上面例子可以看出，B是A的友元类。它的用法就是B里面组合了一个A对象，然后A将B声明为友元类，之后就可以通过B来直接修改A了。
A基本就像一个辅助类，类似于E-R图中的**弱实体**，是为了让B去用它的。比如圆和点的关系，圆类中组合一个点作为圆心，圆心把圆设置成友元类方便其操作，也是一种**辅助作用**。

### 运算符重载
用来给自定义数据类似一种简单的操作方法，运算符重载的**本质就是函数**，只不过**函数名**形式为`operator?`，其中`?`就是一个运算符，比如`+,-,++,--,>,<,==`等等。
事实上重载操作符后：
```cpp
Test operator+(Test &a, Test &b)
{
	...
}
Test t1, t2;
Test t3 = operator+(t1, t2);
Test t4 = t1 + t2;
```
其中`operator+(t1, t2)`和`t1 + t2`是**完全一样**的。
#### 一般写法
成员函数比全局函数**少一个左操作数参数**而已，就是默认传入的this指针。
* 作为全局函数(一般**定义成类的友元函数**)：
	* 二元运算符：+ - * / 
	```cpp
	class Test
	{
	friend Test operator+(Test &a, Test &b); //将全局函数声明为友元函数
	friend Test& operator++(Test &a);
	friend Test operator++(Test &a, int);
	}
	
	Test operator+(Test &a, Test &b); //a b分别为左操作数和右操作数
	```
	* 一元运算符： ++ --
	```cpp
	Test& operator++(Test &a) //前置++，返回自身
	{
		a.m_a++;
		a.m_b++;
		return a;
	}
	Test operator++(Test &a, int) //后置++，int是个占位参数
	{
		Test tmp(a.m_a+1, a.m_b+1);
		return tmp; //返回一个匿名对象
	}
	```
* 作为成员函数：用this指针隐式当作**左操作数**
	* 二元运算符：+ - * /  
	```cpp
	Test operator+(Test &b) //左操作数被隐含了，编译器自动用this指针填充
	{
		Test tmp(m_a + b.m_a, m_b + b.m_b);
		return tmp; //返回了个匿名对象
	}
	```
	* 一元运算符： ++ -- 后置操作的**int占位语法**
	```cpp
	Test& operator++() //前置++，返回自身
	{
		this->m_a++;
		m_b++;
		return *this;
	}
	Test operator++(int) //后置++，int是个占位参数
	{
		Test tmp(m_a+1, this->m_b+1);
		return tmp; //返回一个匿名对象
	}
	```
* 这个后置操作的占位int，我感觉这种语法也是迫不得已，因为重载函数不能通过返回值区分，如果单纯一个返回自身一个返回匿名对象的话编译都通不过，为了区分前置后置来了个占位int，记住就好emmm估计还是为了兼容C吧
* 其实就是写函数，重点在于返回值的判断，也就是**是否需要当左值**。如果像+ - * /这样返回一个结果只用当右值，那么直接返回匿名对象即可，返回值为`Test`; 而像前置++ --这样返回值**需要当左值**，**函数要当左值就返回引用**；而如果是关系运算符比如`> < >= <= ==`那么返回值就是`bool`类型，简单判断一下即可。
* 到底用友元函数实现还是成员函数实现？
  最好还是用成员函数实现吧，友元还破坏了封装性hhh。捏麻麻声明和定义分开写我都受不了，写代码能不能有点人性？
#### 只能用友元函数重载 >> <<
* 什么时候用友元函数实现？
  **无法修改左操作数类**的时候，比如输入输出符号`<< >>`，`cout << a`中`cout`是左操作数，如果非得用成员函数重载的话必须得修改类`ostream`的代码，这很难hhh，所以这个时候只能友元函数了。
```cpp
ostream& operator<<(ostream &out, const Test &a) //左操作数右操作数都不能少
{
	out << "m_a is" << a.m_a << "m_b is" << a.m_b;
	return out;
}
istream& operator>>(istream &in, Test &a)
{
	cout << "please input the a:" << endl;
	in >> a.m_a;
	cout << "please input the b:" << endl;
	in >> a.m_b;
	return in;
}
Test a(1, 2);
std::cout << a << "asdads";
```
最后返回引用支持**链式编程**，hh毕竟`cout`还要**当左值**嘛，肯定必须得返回引用喽。
* 还有比如运算符的左右操作数类型不同
```cpp
Test a(1, 2);
a + 27; //可以正确调用成员函数
27 + a; //无法调用，因为左操作数不是类
```
这时候只能友元函数实现一个
```cpp
Test operaotr+(int a, const Test &a)
```

#### 对赋值=操作符的重载
=操作符的结合顺序是**从右向左**的
和拷贝构造函数那里遇到的问题一样，即深浅拷贝的问题。因此一般如果要手动实现拷贝构造函数的话，=操作符也需要被重载。
而且这个如果浅拷贝的话更严重，不光是同一块内存被一个对象释放掉的指针悬挂问题，而且由于c原本分配了堆区内存的话，一浅拷贝内存就泄漏了。
```cpp
Test a(1, 2), c(3, 4);
c = a;
```
=操作符的重载**只能用成员函数实现**：
```cpp
Test& operator=(const Test &a)
{
	if(&a == this) return *this; //防止自身赋值
	if(m_p != NULL)
		delete [] m_p;  //释放原本的内存防止泄漏
	m_p = new char[30];
	strcpy(m_p, a.m_p);
	return *this; //返回自身用于链式编程
}
```
* ！！！别忘了自身赋值直接返回！！！
其实这里就算`Test operator=(Test a)`不使用任何引用**也可以完成链式赋值**，这里使用引用更多是出于**效率问题**。

更深入的：如果`Test operator=(Test &a)`这样的话，`t1 = t2 = t3`会**报错**：这是因为链式赋值**从右到左**执行，先执行完`t2 = t3`后返回一个匿名对象是**右值**，而函数参数是**非常量引用**，**非常量引用的初始值必须是左值**，因此类型不匹配。

最简单的做法就是参数设置成**常引用**`const Tset& a`，因为**常引用兼容左值和右值**，因此常引用做函数参数一方面是为了避免修改参数本身，更深层的是为了**兼容左值和右值**。
【参考文章】：[深入理解C++重载赋值操作符](https://www.cnblogs.com/dabaopku/p/4849377.html)

#### 函数参数不要滥用常引用！
上文提到的operator=操作符的重载，我们说明了常引用的优势，但是**切记**在其他地方**慎重使用**常引用，我们使用常引用参数的时候是直接通过参数**访问私有数据本身**，然而常引用参数却无法访问非const函数！！！，这就非常鸡肋了，比如：
```cpp
class Test
{
	int a;
	int b;
public:
	Test(int i, int j) {
		a = i;
		b = j;
	}
	int geta() {
		return a;
	}
	int getb() {
		return b;
	}
	bool operator<(const Test& t2)  //这是个常引用参数！
	{
		return a < t2.geta();   //试图通过const对象访问非const参数，报错！！！
	}
};
int main()
{
	Test t1(1, 2), t2(3, 4);
	if (t1 < t2)
	{
		cout << "hahha" << endl;
	}
	return 0;
}
```
看似没问题，编译可以通过但是一运行就宕，原因就在于：
```cpp
bool operator<(const Test& t2)  //这是个常引用参数！
{
	return a < t2.geta();   //试图通过const对象访问非const参数，报错！！！
}
```
因此**不要滥用常引用做参数**！！！这里简单写`Test &`普通引用都没问题，**而且效率是一样的！**。
```cpp
bool operator<(Test& t2)  //普通引用能完成就别再添加限制了！
{
	return a < t2.geta();  
}
```
还是那句话，普通引用就能完成的事情，非得加个const限制人家，多少沾点那啥🤭

#### 对[]的重载
[]是个二元运算符！像数组那样`A[1] = 10`，显然需要当左值，因此返回引用是必须的。
```cpp
int& operator[](int &i)
{
	return this->array[i];
}
```
至于对()的重载就不写了，这玩意不会真有人用吧，写法和生成匿名对象的写法一样还容易混淆，谁知道是重载的()还是带参数的构造函数

#### 关系操作符
重载一个重载一片，clion自动生成的也是这样
```cpp
bool operator>(const Test &a)
{
	if(this->m_a > a.m_a || this->m_b > a.m_b)
		return true;
	return false;
}
bool operator<=(const Test &a)
{
	return !((*this) > a); //>已经被重载了直接用
}
```
#### 为什么不要重载&&, ||
&& 和 || 都有**短路原则**，可以省略某个式子的计算。
而操作符重载是通过函数实现的，操作数作为函数的参数，**函数参数都会被求值**，因此无法实现短路。

### 仿函数，λ表达式和函数指针
这三种东西很像，它们的之间也有练习，都可以**统一到**`std::function<>`中，即C++中一切可调用类型的统称。

关于这部分的详细描述见[CS106L](http://web.stanford.edu/class/cs106l/)的lecture9.

```cpp
std::function<void(int, double)> func;
```
其中void是返回值，(int, double)是参数列表，`std::function<>`是C++中所有和函数相关的**祖师爷**，剩下什么函数指针什么λ表达式都给我往后稍稍，是为了方便使用的亲民版本。
```cpp
void functionPointer (int arg) {
    int num = 0;
    num += arg;
}
int num = 0;
auto lambda = [&num] (int arg) { num += arg; };
lambda(5); // num = 5;
std::function<void(int)> func = lambda;
std::function<void(int)> func = functionPointer;
```
只要返回值和参数列表一致，那么就可以赋值。

* STL的<algorithm>
当然提到什么仿函数λ表达式还有函数指针，就不得不提STL的算法库了，
```cpp
std::count_if(nums.begin(), nums.end(), isBetween);
```
isBetween可以是个λ表达式，可以是函数指针也可函数对象。这部分在CS106L的那个课件里也有讲，在STL的笔记里也有。
	
* 关于cpp类型安全的思考，见CS106L的lecture14，打开新世界了属于是hhhh
	
## 继承和派生
> 面向对象四个特点：抽象，封装，继承和多态。之前的内容讲了抽象和封装，还有什么jb操作符重载都往后稍稍，在多态面前没有任何牌面。接下来的**继承和多态才是面向对象的精华**，是面向对象方法论的核心部分。
### 继承的访问控制
三种，`public, private, protected`
```cpp
class Parent
{
private:
...
public:
...
protected:
...
}
class Child : public/protected/private Parent
{

}
```
需要明确的是，子类确实继承了父类的**所有成员**（基类的构造函数、析构函数和赋值操作符重载函数除外），还有自己新定义的成员，只是父类的`private`属性**无法被直接访问**。比如：
```cpp
//C++中，派生类不能直接访问基类的私有成员。
class A
{	
	int x,y;
public:
	void f();
	void g() { ... x ，y ... }
};
class B: public A
{	
	int z;
public:
	void h() 
	{	... x，y ...  //Error，x、y为基类的私有成员。
		f();  //OK
		g();  //OK，通过函数g访问基类的私有成员x和y。
	}
}; 

```
父类的`private`属性和函数在子类中不能被直接访问，如果想要访问只能使用父类提供的`public`接口。
而父类的`public`和`protected`**属性和函数**可以在子类中被直接访问。

### 类型/赋值兼容性原则
**`public`派生**得到的子类，满足以下兼容规则：
* 子类对象可以直接**当作父类对象使用**，子类对象可以直接**赋值**给父类对象
* 父类指针或者引用可以直接**指向或者引用**子类对象 -------->**实现多态的基础**
```cpp
class Parent
{
}
class Child : public Parent
{
}
int main()
{
	Child c;
	Parent p = c; //直接用子类对象初始化和赋值父类对象
	Parent* p1 = &c; //父类指针或者引用指向子类对象
	Parent &p2 = c;
}
```
### 继承中的对象模型
之前我们已经知道了C++的类模型中，数据成员和成员函数分开存储，成员函数在代码区通过`this`指针与具体对象绑定，数据成员的内存分配和结构体类似，一般就分配在栈区。类的静态数据成员只有一个存储在静态区。
继承就是子类成员+父类成员。
![image](https://user-images.githubusercontent.com/55400137/149291455-3e79bd99-0f2d-43db-9e20-c32f4e89823d.png)
![image](https://user-images.githubusercontent.com/55400137/149291527-ad64a790-1e98-4cac-a345-9ff4be23e6dc.png)
子类继承了父类的数据成员，加上自己的数据成员形成更大的结构体而已。
这就涉及到一个问题，子类该如何初始化这些继承来的数据成员，即构造和析构

### 继承中的构造和析构
* 原则：
  	子类构造时，先调用父类的构造函数初始化继承来的数据成员，然后调用自己的构造函数初始化自己新增的成员；
  	析构时，先调用自己的析构函数，再调用父类的析构函数。
还是利用成员初始化列表，像组合那样。
```cpp
class Child : public Parent
{
	Child(int a, b, c): Parent(a, b), m_c(c)
	{}
}
```
只是简单赋值的话初始化列表还是方便，函数体都不用写。

**遵循先构造的后析构**

### 既有继承又有组合的构造和析构
* 原则：
  	构造时，先构造父类，再构造组合对象，最后构造自己；
  	析构时，先析构自己，再析构组合对象，最后析构父类

还是用初始化列表
```cpp
class Child : public Parent
{
	Test A;
	Obj B;
	Child(int a, b, c): Parent(a, b), A(a, b), B(c), m_c(c)
	{}
}
```
别忘了构造的顺序和初始化列表的顺序无关，和**定义**的顺序一致。
**同样遵循先构造的后析构**

### 子类和父类有同名数据成员/成员函数
注意内存模型，子类还是继承了父类的**所有数据成员**，只不过默认访问的子类的成员，要想访问父类需要类型限定`Parent::func(), Parent::m_a`。

![image](https://user-images.githubusercontent.com/55400137/149300267-93033ca9-9a1a-4855-a81e-3f9002cf958d.png)

![image](https://user-images.githubusercontent.com/55400137/149300266-0a5a74a8-71e8-4f8c-ba8e-f9cd99d4531c.png)

**注意！**：
	
对基类而言，派生类成员标识符的作用域是**嵌套在**基类作用域中的。因此，如果子类中重定义了基类的**同名函数**，**不构成重载！因为重载是在同一个作用域下才有的概念**，因此：**没办法通过函数参数来区分函数是来自父类或者子类**。
	
只要子类定义了重名函数，那么父类的函数就被`hidden`隐藏了，因为在派生类的作用域内**根本感受不到**父类该名字函数的**存在**，就相当于`{int i;}`内定义了一个i，那么就完全感受不到大括号外的i了，如：
```cpp
class A //基类
{		
	int x,y;
public:
	void f();
	void g();
};
class B: public A
{	
	int z;
public:
	void f(int); //注意不是重载A的f！在派生类作用域完全感受不到A::f的存在，因为被hidden了
	void h() 
	{	f(1);  //OK
		f();  //Error
		A::f();  //OK 想要访问A的f只能类限定
	}
};
```
	
也可以在派生类中使用using声明把基类中某个的函数名对派生类开放：
```cpp
class B: public A
{	
	int z;
public:
	using A::f; //在这里让派生类
	void f(int); 
	void h() 
	{	f(1);  //OK
		f();  //OK，等价于A::f();
	}
};

```
### 多继承-->虚继承
基本被摒弃了，像Java就没有多继承的概念了，取而代之的是接口的概念，即C++里的抽象类。
多继承可能出现二义性的问题，比如继承的多个父类有相同的爷爷类，如果用普通的方法继承访问爷爷类会出现**二义性**：
![image](https://user-images.githubusercontent.com/55400137/149302627-ab00d69b-3c77-402f-aa72-c6dfc1b988e4.png)

访问`c.B`时不知道访问哪个，因此为了解决这种问题，引入**虚继承**：

![image](https://user-images.githubusercontent.com/55400137/149302939-6da33571-507c-4df2-ae96-0a486874bcf1.png)

即使用`virtual`关键字后，编译器对`B`的构造函数**只调用一次**，从而避免了二义性。
## 多态
封装突破了C语言函数的概念，继承实现了代码的简单复用
多态可以满足**开闭原则**，是**设计模式**的基础，是**框架**的基础。

### 使用方法
**多态成立**三个条件：
* 要有继承
* 虚函数重写override
* **父类**指针/引用指向子类对象。
这是面向对象的精髓所在。

而C语言指针的最大意义就是**间接赋值**：
* 定义形参和实参
* 实参取地址给形参建立关联
* 在函数中通过* p间接修改实参的值

### 多态原理
#### 静态绑定和动态绑定
* 静态绑定在编译器**编译阶段**就已经**确定了**要调用哪个函数，已经确定了**函数的入口地址**，比如**重载函数**，写好代码之后**编译阶段**就可以根据参数列表来确定具体要调用的函数是哪一个。
* 而动态绑定则是只能在**运行**的时候根据代码的动作来确定调用哪个函数，比如`if`和`switch`语句就需要根据**运行时**的条件来判断最终调用哪个函数。

而C++也是静态绑定型的语言，需要经过编译处理，如果：
```cpp
class Parent
{
public:
	void func()   //不写virtual关键字！
	{
		cout << "parent!" << endl;
	}
}
class Chlild : public Parent
{
public:
	void func()
	{
		cout << "Child!" << endl;
	}
}
int main()
{
	Child c;
	Parent *p = &c;
	p->func();
}
```
由于没有写`virtual`关键字，因此编译阶段**只会执行静态绑定**，这时编译器的操作是：`p`指针是个`Parent`指针，我**不清楚**`p`在运行时具体指向的到底是`Parent`还是`Child`，所以我要静态绑定，**一律绑定到父类的`func()`**，因此最后输出`parent!`。这就是静态绑定的特性。

#### 虚函数表相关(vtbl&vptr)
实现多态的理论基础是**函数指针**做函数参数，捏麻麻看来我在C的回调函数那里的猜测是对的🤭。因为vtbl里面就存了一堆函数指针，而这些函数指针都是固定类型的，因为重写**被重写的函数在父类子类中函数类型完全一致**，这就相当于抽象出了**固定不变**的东西，我们只需要按照函数的类型给出不同的实现，然后用指针指过去就OK了，这不就是妥妥的多态嘛。

即虚函数表和指向虚函数表的指针。
多态/动态绑定的一种常见实现方式：
* 当类声明是`virtual`虚函数时，**编译器**会给类创建一个虚函数表，里面存放该类**所有虚函数的入口地址**，其实就是一堆**函数指针**。
* 该对象的内存空间会多出一个指向虚函数表的指针vptr，这个指针一般是类的第一个成员。因此一般sizeof会发现**多出4个字节**，就是那个vptr。
* 通过指针调用函数时，运行时编译器根据指针指向的对象内存，取其vptr找到vptl，然后在表中找对应函数指针调用即可。因为编译器最多只能将代码`p->func()`编译成：
  `(*(*(vtbl *)p))(p)`的形式，`p`是对象的指针，先强制类型转换成`vtbl*`再取地址得到虚函数表的地址，之后再取地址得到虚函数表**首元素的地址**，即第一个函数指针，传入`p`相当于`this`指针，就实现了调用。
  也就是说编译器最后还得看**运行**的时候`p`到底指向的谁，这就是动态绑定。

> 虚函数表是和类相关的，只要在类中**声明**了虚函数就会创建虚函数表，类的函数**只有代码区的一份**，每个对象都有各自的数据，但它们共用类的一份代码。

### 虚析构函数
多态的核心就在于**父类**指针或者引用**指向子类**对象，这样通过父类指针调用虚函数就会有多态的性质，非常方便。
于是产生一个问题，既然我们用的是父类指针，那么析构的时候可以正常析构子类吗，即可以析构掉父类和子类的全部内存而不会**产生内存泄漏**，**很不幸，不可以**。
```cpp
Child c;
Parent *p = &c;
delete p;
```
**只会**调用`~Parent()`即父类的析构函数，因为这是**静态绑定**，`delete`会自动调用析构函数，但是编译器看到的`p`仅仅是个`Parent`类的指针，因此静态绑定到`~Parent()`函数。而**不会表现出多态性质**。

虚析构函数就是为了解决这个问题：为了可以**通过父类指针完整析构子类**。

使用方法很简单，只需要在**父类**的虚构函数前加上`virtual`关键字即可。
```cpp
class Parent
{
public:
	virtual ~Parent()   //写virtual关键字，这是个虚析构函数
	{
		cout << "~Parent!" << endl;
	}
}
class Chlild : public Parent
{
public:
	~Chlild()
	{
		cout << "~Child!" << endl;
	}
}
int main()
{
	Child c;
	Parent* p = &c;
	delete p; // 通过父类指针正确地析构掉了子类，而不会产生内存泄漏，这就是虚析构函数的作用
}
```
构造时先调用`Parent()`再调用`Child()`，析构时先调用`~Child()`再调用`~Parent()`.

#### 父类必须写虚析构函数！
如果尝试用多态的话，即全部使用父类指针进行操作，那么父类**必须写虚析构函数**，就算父类是个抽象类，没有任何数据成员也得写！因为`delete p;`全靠虚析构函数才能**正确调用子类的析构函数**，比如接口类：
```cpp
class interface //接口类
{
public:
	virtual ~interface() //虚析构函数就算空也必须写！
	{
		;
	}
	virtual void func()1 = 0;
	virtual void func()2 = 0;
	virtual void func()3 = 0;
}
```
### 多态相关问题
* 构造函数中调用虚函数能够实现多态吗？
```cpp
class Parent
{
private:
	int a;
public:
	Parent(int i)
	{
		a = i;
		print();   //构造函数中调用虚函数
	}
	virtual void print()
	{
		cout << "我是父类";
	}
}
class Child : Parent
{
private:
	int b;
public:
	Child(int i) : Parent(5)  //先调用父类构造函数时, vptr指向父类虚函数表输出我是父类
	{
		b = i; 
		print();  //父类构造函数完毕后vptr指向子类虚函数表，输出我是子类
	}
	virtual void print() //虚函数重写
	{
		cout << "我是子类";
	}
}
int main()
{
	Child c1; //定义一个子类对象
}
```
`Child c1;`先调用Parent的构造函数，这里的print究竟是输出"我是父类"还是输出"我是子类"？如果输出`我是子类`则说明构造函数中调用虚函数也能发生多态，输出"我是父类"则说明不能。

经过实验，最后**没有发生多态**！这是因为在创建c1时要初始化c1的vptr指针，初始化过程是**分步**的：先执行父类构造函数时，c1的vptr指针指向的**父类的虚函数表**；而当**父类**构造函数运行**完毕**后，c1的vptr才会指向子类的虚函数表。

### 不要在数组上使用多态！
这是个**铁律**，就好像不要返回临时对象的地址和引用一样！因为父类指针和子类**指针的步长大概率不同**，一般子类会添加一些私有数据导致子类指针的步长更长，而如果：
```cpp
class Parent
{
	virtual void func() {}
};
class Child: Parent
{
	virtual void func() {} //虚函数重写
};
int main()
{
	Child array[10];
	Parent *p = array;  //父类指针指向子类对象，构成多态
	for(int i = 0; i < 10; i++)
	{
		p[i].func();  //试图用父类指针采用多态的方式调用子类重写的func
	}
}
```
问题出在`p[i]`上，因为p是个父类指针，`p[i]`会解析成`*(p + sizeof(Parent)*i)`，即使用的是**父类指针的步长**，因此地址完全不对了，不是我们以为的`array[i]`。

这种情况就别卖弄了，直接`array[i].func()`完事，不会真有人觉得多态好用吧🤤又不是搭框架。

什么时候子类指针步长和父类步长一样？子类没有添加任何新的数据成员的时候，显然。

### 重载&重写&重定义概念辨析
* 重载必须在**同一个类**中进行，子类无法重载父类的函数，父类的同名函数将被子类覆盖。重载是**静态绑定**的，在编译阶段就确定了。
* 重写必须发生在父类和子类之间，并且重写的函数在父类子类中**类型完全一致**，是通过`virtual`关键字标注的。
* 如果没有`virtual`关键字，那么相当于重定义，这时子类名称会覆盖掉父类，通过**类名限定**可以访问父类的函数。

## 纯虚函数和抽象类
就相当于Java中的**接口**，也就是说抽象类中的纯虚函数没有给出实现，每个子类都要定义**自己的实现版本**。

纯虚函数是个父类中特殊的虚函数，子类**必须重写**纯虚函数；含有纯虚函数的类叫抽象类，抽象类不能建立对象，因此就不能作为函数参数和返回值类型。

但是纯虚函数的指针和引用可以，不然连多态都没法用了，可以说抽象类就是为了多态而生的，让子类实现纯虚函数然后父类指针指向子类。。。
```cpp
class Parent //抽象类
{
public:
	virtual void func() = 0;  //纯虚函数
}
```

### 抽象类与多继承
之前讲过多继承的二义性，同一个名字有两种可能的选择，因为两个父类继承自同一个爷爷类，导致爷爷类的数据有两套，子类访问时就不知道访问哪个了，非常麻烦。因此引入了虚继承让爷爷类的数据**只产生一套**，防止二义性。

还有另一种场景虚继承也没法解决，那就是两个父类里面出现**同名的数据成员**，这个二义性死都没办法解决，难用的要死就是说。。。

实际上多继承在工程中早就**被抛弃**了，JAVA里根本就没有多继承的概念，这种鸡肋功能维护起来麻烦的要死，取而代之的就是**接口**，可惜C++没有接口，但是可以**用抽象类模拟接口**。

即我们写的抽象类中，只写一堆纯虚函数的原型，**没有任何数据成员**，这就和接口的行为一致了。
```cpp
class interface
{
public: //一堆纯虚函数
	virtual void func1() = 0;
	virtual void func2() = 0;
	virtual void func3() = 0;
}
```
由于接口类里面**没有定义**，所以如果我们多继承接口，即使函数类型出现冲突，也不会出现二义性，毕竟接口里面**什么定义都没有**还怎么二义？

### C语言多态——利用函数指针
这部分在C语言的那个笔记里，函数指针真是人类伟大的发明就是说，C语言也能面向对象了是吧，什么叫抽象啊🤭

## 函数模板和类模板(泛型)
就是STL的本质嘛，这部分说用处也大得很，用的时候稍微看看吧，之后再补。> 2.18补

* 函数模板
函数的参数类型和返回值类型，导致函数只能对于某组**固定类型**的数据完成操作并返回固定类型，这样**局限性**太大。那么有没有一种机制，**让数据类型参数化**，从而减少大量工作近似的代码。这就是**函数模板**，也就是所谓**泛型编程**。

函数模板的**动机**就是为了让函数**从确定数据类型中解放出来**，把精力完全集中在**业务逻辑**上。

* 类模板
类模板产生的动机和函数模板类似，如果几个类的功能相同，**只是数据类型不同**，那么就可以定义类模板。

类模板在表示如**链表**，**图**等结构时非常好用，因为它们都有所谓**结点**的概念，**结点中的数据类型**很可能需要变来变去，但是各种操作的逻辑不发生变化，通过类模板可以很好的实现。

### 函数模板
#### 泛型格式
**定义**函数模板时，开头先写上`template`告诉C++编译器我要**开始泛型**了，不要认不得给我乱报错。后面**紧跟**`<typename T>`即**参数化数据类型T**。
```cpp
template <typename T>  //这里的参数化数据类型只有一个T，可以有多个
void swap(T &a, T &b)
{
	T c;
	c = a;
	a = b;
	b = a;
}
```
**调用**函数模板时，有两种方式，很少用自动类型推导，给我老老实实写`<>`**显式调用**函数模板！
```cpp
int a = 1;
int b = 2;
swap<int>(a, b);  //显式类型调用
swap(a, b);       //自动类型推导
```
一般写函数模板的时候，先写一个普通的函数，然后开头加上`template<typename T..>`（或者`template<class T>`）把函数体里面的变量类型换成参数化的T就完事了。

#### 函数模板&函数重载
重要的话题，如果出现下面这种场景：
```cpp
template <typename T>  //函数模板 等价于template<class T>
void swap(T &a, T &b)
{
	T c;
	c = a;
	a = b;
	b = a;
}
void swap(int a, char b) //重载的普通函数
{
	cout << "重载函数";
}
int main
{
	int a;
	char b;
	swap(a, b); //调用普通函数
	swap(b, a);  //还是调用普通函数
	swap(a, a);  //调用函数模板
}
```
函数模板可以像普通函数一样**被重载**，显然两个swap函数构成了重载，因为**参数类型不一样**。
> 函数重载说实话也有够鸡肋的，如果重载的函数体功能相差较大那还好，但是如果像为了交换不同类型的数字而写了几个几乎一模一样的重载函数，那可真是有点太傻逼了，纯纯的复制粘贴，函数模板就解决了这个问题。

函数模板和普通函数的本质区别：在调用函数模板时，将进行**严格类型匹配**，**不会进行类型转换**。而普通函数可能进行**隐式类型转换**，比如int和char之间转换。

如果函数模板被重载，那么**调用规则**如下：
* C++编译器**优先考虑**普通函数
* 如果函数模板可以产生一个**更好的匹配**，那么选择模板
* 可以通过空模板实参列表的语法限定编译器**只**调用模板函数


#### 函数模板实现机制剖析
其实是g++编译器会**解析**源码中对于函数模板的调用，分析源码出现的调用的数据类型，来生成那**几种只有数据类型不同的函数**，这将在编译过程中**生成的汇编文件**中看到。

也就是说，是C++**编译器帮程序员完成了重复功能函数的编写**，更具体的讲：编译器会对函数模板进行**两次编译**，在**声明**的地方对模板代码本身进行编译；在**调用**的地方对参数替换后的代码进行编译。

所谓：编译器根据函数模板的调用状况生成几个模板函数。


### 类模板
#### 泛型格式
同样定义类之前写上：
```cpp
template<class T>
class test
{
protected:
	T m_a;  //参数化数据类型T
	static int m_s; //类模板中的静态成员
public:
	A(T a) {m_a = a; }
	T get_a() { return m_a; }
}

int main()
{
	test<int> A(100); //类模板必须显式指定<类型>
}
```
类模板必须显式指定<>中的类型，并且**注意**：类模板是**抽象的**，**无法用类模板来定义对象**，这点看来好似抽象类；只有给出<>中的类型后**才算实例化了一个类**，也就是所谓**模板类**，才可以定义对象。

但是，可以不给出`<>`也能够用类模板的**指针或者引用**，一般都用模板指针/引用做**函数参数**。

因此，**类模板的静态成员仅属于实例化后的类**，这也是很好理解的，毕竟类模板什么都不是，就是个抽象的概念，给出数据类型后才算确定了类：
```cpp
test<int> a, b; //a, b共享一个m_s静态成员，即类test<int>类静态成员
test<int> c, d; //c, d共享一个m_s静态成员，即类test<double>类静态成员
```
还有类模板做函数参数时也是：
```cpp
int func(test<int> a)  //如果不给出<int>编译器根本不知道该如何给a分配内存
{
	cout << a.get_a();
}
```
把`test<int>`看成一个**整体的类名**，只是这个类名比较奇怪而已。

#### 继承中的类模板
比如让一个类模板派生出子类：
```cpp
class Child: public test  //报错，因为test不是具体类
{
}
class Chila: public test<int>
{
private:
	int m_b;
public:
	Child(int a, int b): test<int>(a)   //由于test类中写了带参数构造函数屏蔽了默认构造函数，
					    //子类必须启用成员初始化列表显式调用test的带参数构造函数
	{
		m_b = a;
	}
}
```
子类实例化时需要首先**实例化一个父类对象**，但是类模板根本就没法确定一个对象的大小，因为数据类型都不确定。

还是那句话，类模板是个抽象类，只有实例化后才能用，把`test<int>`看作一个类名就行。

当然也能搞一搞**从模板类派生模板类**这种编程游戏。：
```cpp
template <class T>
class C: public test<T> //T可以传给父模板类，这样也是个具体类
{
private:
	T m_n;
public:
	C(T n, T a) : test<T>(a) //启用成员初始化列表
	{
		m_n = n;
	}
}
```

#### 类模板中的操作符重载
* 所有函数都定义在类模板的内部，包括友元函数

这种方法最保险，最不容易出现问题，后面我们会讲函数定义写在类模板外部的写法。
```cpp
template<class T>
class Complex
{
	friend ostream& operator<<(ostream& out, const Complex& c) //把友元函数定义也写在类内部，写在外面运行时会报错
	{
		out <<  c.m_a << "+" << c.m_b << "i" << endl;
		return out;
	}
private:
	T m_a;
	T m_b;
public:
	Complex(T a, T b)
	{
		m_a = a;
		m_b = b;
	}
	Complex operator+(const Complex &c)
	{
		Complex res(m_a+c.m_a, m_b+c.m_b);
		return res;
	}
}
```
显然重载<<运算符需要通过**友元**函数，但是这里我们需要把**友元函数的定义也写在类的内部**，否则就会报错。这和普通类不一样，之前的例子我们都是直接把友元函数作为全局函数写在**类外的**，但是即便定义在类内部也**不是类成员**，而是**全局函数**。所谓类成员必须有类**成员限定符**`Complex::`的.

* 所有函数定义都写在类模板的外部，但是在**同一个源文件**中
这就不可避免要用**类限定符**了，类里面只有函数声明，定义全在类外，但是仍处于同一个cpp里，代码如下：
```cpp
class Complex
{
	friend ostream& operator<< <T>(ostream& out, const Complex& c) //友元函数的声明必须在函数名后加上<T>
private:
	T m_a;
	T m_b;
public:
	Complex(T a, T b);
	Complex operator+(const Complex &c);
	void print();
}
//实现在类外，需要类限定和template关键字
template<class T> //构造函数
Complex<T>::Complex(T a, T b)
{
	m_a = a;
	m_b = b;
}

template<class T> //普通成员函数
void Complex<T>::print()
{
	cout << "test" << endl;
}

template<class T> //普通的成员函数，只不过是个操作符重载
Complex<T> Complex<T>::operator+(const Complex& c) //参数的<T>可写可不写
{
	Complex<T> res(m_a + c2.m_a, m_b + c2.m_b); //这个是成员函数，相当于类的内部，因此这一行的<T>可写可不写
	return res;
}
```

接下来就是重量级了，凭什么友元函数写在外面就报错？比如：
```cpp
template<class T>
ostream& operator<<(ostream& out, const Complex<T>& c)
{
	out << "m_a:" << c.m_a << " m_b:" << c.m_b << endl;
	return out;
}
```
> 别给我写什么Complex<T>::类限定，友元函数是**全局函数**ok？根本**不是类的成员**！！！
这样编译可以过，但是运行时就是会宕掉。这其实是由于模板函数的二次编译导致的，具体原因不再细究了，解决办法为：在声明函数名后加上`<T>`
```cpp
friend ostream& operator<< <T>(ostream& out, const Complex& c);
```
非常丑，比那个重载后自增的`operator++(int)`还丑，说实话这确实是C++编译器做的不好的一个，但没办法就是这么定的。

哈哈说不定还是为了限制滥用友元的良苦用心呢，还是老老实实在**类内部写函数定义**吧，就没这么多屁事了。除了左移右移的重载，其他的都算**滥用友元**！

* 所有函数定义写在类外，并且.h和.cpp分开
这是最麻烦的，假如你写了个**模板类**，并且声明和定义写在了不同的.h和.cpp中，如下：
```cpp
//test.c中
#include "Complex.h"
int main()
{
	...//测试
}
```
会发现还是编译可以通过，但是一运行就宕，这还是两次编译惹的祸。这样只能在测试文件里包
```cpp
#include "Complex.cpp"
```
非常奇怪，因为正常情况都是包头文件的，但是不这么包就是找不到定义，就是会宕。
	
* **总结时刻**
想写类模板，就老老实实写在一个**头文件**.h里，更好的就是把函数定义全写在类的内部，这样最方便，别人用起来直接包头就OK了，事实上STL源码就算这么干的，没有说把声明和实现拆开。

#### 在类模板中调用T的成员函数
这就有点奇巧淫计了，但是有些时候我们不得不这样做，如果类型T的一些数据是private的话只能**事先得知**T的获取attr的成员函数名字，才能完成模板类的编写，这是没办法的事情。
	
上面的例子我们都很简单，模板类没有出现非得用**未知的**T的成员函数的情况，但有些时候我们不得不这么做。

我们**需要确保**的是：在类模板通过T使用其成员函数，**函数名必须对应**，否则就会出现编译可以通过，但是一运行**产生实例类**后找不到那个成员函数的位置了。下面是个例子：

```cpp
class Vaccine
{
private:
	string m_name;
	int m_price;
	int m_number;
	int m_bound;
public:
	Vaccine(const string& name, int price, int number, int bound);
	string GetName();
	int GetPrice();
	int GetNumber();
	int GetBound();
	int GetCost();
};

class Mask
{
private:
	string m_name;
	int m_price;
	int m_number;
public:
	Mask(const string& name, int price, int number);
	string GetName();
	int GetPrice();
	int GetNumber();
	int GetCost();
};
```
写了两个类，下面写模板：
```cpp
template<class T>
class CombinedOrder
{
public:
	vector<T> m_order; //所有订单
	CombinedOrder() { ; };
	bool AddOrder(T& order) 
	{
		string t_name = order.GetName();
		int size = m_order.size();
		int i = 0;
		for (i = 0; i < size; i++)
		{
			if (t_name == m_order[i].GetName()) //类别一致
				return m_order[i].add_order(order.GetNumber());
		}
		if (i == size) //没有这个订单则新加
			m_order.push_back(order);
	}
	int GetTotalCost()
	{
		int res = 0, size = m_order.size();
		int i = 0;
		for (i = 0; i < size; i++)
		{
			res += m_order[i].GetNumber() * m_order[i].GetPrice();
		}
		return res;
	}
	void print_all()
	{
		cout << "order's info:" << endl;
		int size = m_order.size();
		for (int i = 0; i < size; i++)
			m_order[i].print_info();
	}
	T& operator[](int index)
	{
		return m_order[index];
	}

};
```

可以看到我们不得不在类模板里面使用`GetName(), GetNumber(), GetPrice()`这样的函数，虽然原则上来讲我们应该是不知道T将来是什么，但是**在这个场景下**，我们只是做了一个仅仅**只适用于**有这些方法的类的模板，这里也能看出之前那两个类的编写，煞费苦心的让它们**都有**`GetName(), GetNumber(), GetPrice()`这些函数是吧😅。
					
这可能有点违背模板的初衷？但实际上这个例子还是体现了模板的强大，你看他不就让Mask和Vaccine这两个类可以用一个统一模板CombinedOrder表示么🤭属于是阉割版了，有点为了模板而模板说实话。
					 
> 但是从这个例子我们可以对类模板有更深的认识🤭。。。

### 再谈类模板的分文件编写
有名言，摘自(CS 106L的lecture7)[http://web.stanford.edu/class/cs106l/]：
> Templates don’t emit code until instantiated.
	
> 也就是说模板在实例化之前不产生任何代码
	
那么就会出现典中典的**找不到实现**的错误：
```cpp
//vector.h 类模板头文件
template <typename T>
class vector<T> {
    T at(int i);
};
//vector.cpp 类模板实现文件
#include "vector.h"
template <typename T>
void vector<T>::at(int i) {
    // oops
}
//main.cpp 使用模板
#include "vector.h"
vector<int> a;
a.at(5);
```
还是那句话：模板在被**实例化之前**不产生任何代码，因此编译main.cpp时产生了调用vector的代码，但是编译vector.cpp时因为是个**模板类**，没有生成代码，而且main.cpp包的是vector.h文件，vector.cpp啥玩意没有生成，自然找不到实现了。
> 不像普通类按照这种编写方式就可以生成实现代码，然后顺利通过编译。
	
如何解决？在vector.h文件里面包含`#include "vector.cpp"`!!!，这非常奇怪，头文件咋还包含上源文件了捏🤭没办法谁让你main.cpp调用的是vector.h？不是模板不emit代码么，我直接把实现都包到.h头文件里，管你emit不emit？🤭
	
> 这进一步说明了，对于模板类的编写，实现和声明放在一起是多么的方便hhhh
	
事实上CS106L的原话如下：
	
> Templates don’t emit code until instantiated, so #include the .cpp file in the .h file, not the other way around!
	
**只推荐**在头文件中包含源文件的做法，呃这么推荐当然是必要的，不过确实包起来也不麻烦，就是重载友元的时候很阴间，干脆以后还是分文件写，需要重载<<的时候直接在.h文件里写得了🤭我真天菜。
	
	
## C++的类型转换
类型转换：Type Caet
C风格的类型转换非常简单粗暴，直接**强制类型转换**：
```cpp
TYPE a = (TYPE)b; //
```
C++粒度更细：
* static_cast	静态类型转换。如int转换成char
* reinterpreter_cast  重新解释类型 
* dynamic_cast	命名上理解是动态类型转换。如子类和父类之间的多态类型转换。
* const_cast	字面上理解就是去const属性，不能转换基础类型
用法格式如下`TYPE a = static_cast<TYPE>b;`

这玩意细分太麻烦了。。。最好还是避免类型转换吧（

## 异常处理
### 异常处理的基本思想
**传统**的异常处理我们一般都是通过**函数返回值**来处理的，比如出现`NULL`返回值设置成`-1`等等，这是基于函数调用模型的栈结构机制，即先进后出，**依次访问**，是**无法进行跳跃的**。
而C++的异常处理机制不再借助原本的栈结构，因为错误处理的特征就是遇到错误就想要**直接转到若干级之上**重新尝试，这样被调用函数就可以*专注于业务的实现而将异常处理抛给上层函数*。如下图，

![image](https://user-images.githubusercontent.com/55400137/149437663-9c72c59e-989b-418e-a04e-4c905b9a875b.png)

也就是说异常超脱于函数机制，可以**跨越函数**，是一种**程序控制机制**，与函数机制独立和互补。

### C++异常处理的实现
* 就是`try-catch`语句，把可能产生异常的代码段放到`try`语句块内，可以**手工**`throw`出自定义的异常类型。
* 异常在`catch`依次匹配，是严格按照**类型匹配**的，匹配不到就会`abort()`掉，比如：
```cpp
try
{
	throw 'H'; //抛出一个char类型的异常
}
catch (int e)
{
	cout << "出现异常"
}
```
这样在运行的时候会直接`abort()`宕掉，这也是异常抛出`throw`和函数返回值`return`的一个**根本性的区别**，**异常完全按照类型匹配**，而`return`可以隐式类型转换。
* 我们没有办法为每种类型的异常都写一个`catch`语句，于是乎有默认类型的`catch`，就是三个点`catch(...)`
```cpp
try
{
	throw 'H'; //抛出一个char类型的异常
}
catch (int e)
{
	cout << "出现异常" << endl;
}
catch(...)
{
	cout << "其他异常" << endl;
}
```
这样运行时异常会被`...`接住，输出`其他异常`，而不会宕掉。

### 栈解旋unwinding
名字很高大上，但是实际上概念很简单，就是说`try`语句块中如果抛出了异常，那么期间创建的对象会**自动调用析构函数**析构掉，这不废话么😅，进程结束一个是异常结束`throw`一个是`return`正常结束，都结束了还保留那块内存干嘛？不提供栈解旋那才叫日怪。

### 异常接口声明
又是一个说唱功能🤤就是说你可以在定义函数的时候声明可能抛出的异常类型：
```cpp
int func() throw (int, char*, char)
{

}
```
有点像python的函数注释，不过函数注释彻彻底底就是个注释，这玩意写上的话如果抛出的异常类型不在声明里面就会报错hhh，我的评价是：不如不写。

## C++输入/输出流以及文件操作
输入输出还是分三种：
* 标准输入输出：键盘和屏幕，即标准IO
* 文件输入输出，即文件IO
* 内存指定空间的输入输出，即字符串输入输出，简称串IO
C++提供了输入输出iostream类库，它的继承关系如下：

![image](https://user-images.githubusercontent.com/55400137/149446086-7e80eea1-6d41-4144-abd0-9c773a9a1c51.png)

可以看到iostream多继承了istream和ostream类，因此有多继承的二义性问题，看源码也可以发现istream和ostream对于ios是**虚继承**。
而我们常用的cin和cout分别是类istream和ostream的**对象**。类istream重载了操作符`<<`，ostream重载了操作符`>>`，对于普通类型完成了重载，对于自定义类型还是只能通过友元函数重载，见前文。

之前学的什么fopen, fwrite都是基于函数库的文件IO，是C风格的。C++基于类库的才是正儿八经的stream流概念。

而最常用的cout,cin其实是ostream,istream**类的对象**，还有个错误流cerr也是ostream类的对象。下面依次介绍：
	
### cin标准输入流对象
首先是典中典的用法：
```cpp
int a;
float b;
cin >> a >> b; //直接接受键盘数据到a和b
char buf[1024]; 
string buf1;
cin >> buf1;   //cin接受string不能接受空格
cin >> buf;   //用cin输入char数组也不能接受空格！
```
那么该如何接受字符串的空格呢？
几个API函数，即cin对象的成员函数：
```cpp
cin.get() //一次只能读取一个字符
cin.get(一个参数) //读一个字符，支持链式
cin.getline()
cin.read() //二进制方式读文件
cin.ignore() //忽略缓冲区的几个字符
cin.peek() //只窥探一下缓冲区的下一个字符而不使用
cin.putback() //将刚刚读到的字符吐回缓冲区
```
比如：
```cpp
char ch;
while ((ch = cin.get()) != EOF)
	cout << ch;
```
这样会不断接受缓冲区的字符，直到读到EOF，EOF可以用`ctrl+z`或者`ctrl+c`来模拟。
	
当然get有几个重载：
```cpp
char a, b, c;
cin.get(a);
cin.get(b);
cin.get(c);
cin.get(a).get(b).get(c); //支持链式编程
```
这样和`getchar()`非常像了，但是高级的是可以**链式编程**。
	
> 如果缓冲区内容不够(没有输入或者输入太少)的话，这个程序就**阻塞**到了cin那里了，进程就被挂起了hhh直到数据就绪

* 接受带空格的**字符数组表示**的字符串：
```cpp
char buf[1024];
cin.getline(buf, 1024); //内存打包
```

* 接受带空格的string：
```cpp
#include<string> //需要用到string库中的getline函数
string str; 
getline(cin, str);	
```
这里的getline是string库中的，不是cin的成员函数。

### cout标准输出流对象
还是一堆API罢了:
```cpp
cout.flush() //刷新缓冲区
cout.put()   //输出一个字符
cout.write() //二进制方式写文件
cout.width()
cout.fill()
cout.setf(标记)
```
重要的是cout的**格式化输出**，比如输出宽度，十六进制输出，保留两位小数啥的。。
```cpp
#include <iomanip> //cout控制符需要包这个头文件
int main()
{
	cout << hex << a << endl; //以十六进制形式输出整数a
	const char* pt = "China"; //pt指向字符串"China"
	cout << setfill('*') << setw(10) << pt << endl; //指定域宽,输出字符串,空白处以'*'填充
}
```
还是printf好用点。。

### 文件流IO
需要包头文件：
```cpp
#include <iostream>
#include <fstream>  //fstream是iostream的子类
```
#### 读写文本文件
* 写文件
输入输出和fprint什么的类似，输出流是写文件的，输入流是读文件的：
```cpp
ofstream fout; //文件输出流
fout.open("test.txt");
ofstream fout("test.txt"); //构造函数直接将输出流fout关联到test.txt文件上
ofstream fout("test.txt", ios::out); //输出流默认为ios::out
fout << "往文件里写一些东西..." << endl;  //endl不写下一句就会写到同一行
fout << "再写一句" << endl;
fout.close(); //别忘了关不然根本写不到文件上除非程序终止！
```
之前提到的各种什么width(), fill()输出控制符都可以用

* 读文件
用个输入流代表文件：
```cpp
ifstream fin; //文件输入流
fin.open("test.txt");
ifstream fin("test.txt");  //直接读test.txt
ifstream fin("test.txt", ios::in);  //输入流默认为ios::in
string str;
char ch;
getline(fin, str); //从fin中带空格读一行
while((ch = fin.get()) != EOF) //一个个字符读整个文件直到结束
	cout << ch;
//while(fin.cin(ch)) 也一样
fin.close(); //别忘了关！
```
关于打开文件的各种方式的参数设置如下：

![image](https://user-images.githubusercontent.com/55400137/154791573-ec9b9e9f-757c-4f51-990c-5a5626005cec.png)

我们用起来其实也不必区分ifstream还是ofstream，统一用`fstream`，然后指定打开方式即可，就像`fopen()`那样。

可以用“位或”运算符“|”对输入输出方式进行**组合**
```cpp
ios::in | ios:: noreplace  //打开一个输入文件，若文件不存在则返回打开失败的信息
ios::app | ios::nocreate  //打开一个输出文件，在文件尾接着写数据，若文件不存在，则返回打开失败的信息
ios::out l ios::noreplace  //打开一个新文件作为输出文件，如果文件已存在则返回打开失败的信息
ios::in l ios::out I ios::binary  //打开一个二进制文件，可读可写
```	
如果文件打开失败，`open`返回0，因此打开后可以做异常处理：
```cpp
 if(outfile.open("f1.bat", ios::app) ==0)
        cout <<"open error";
```
#### 二进制读写文件及其原地修改
* 用二进制方式读写文件的二进制数据：write, read
由于默认是以ASCII方式打开文件的，如果要以二进制方式则：
```cpp
struct T
{
	int a = 0;
	char name[10] = "adasd";
};
outfile.open("f1.bat", ios::out | ios::binary);
T b;
outfile.write((char *)&b, sizeof(b)); //用write写入二进制数据
infile.open("f1.bat", ios::in | ios::binary);
T c;
infile.read((char *)&c, sizeof(c)); //用read读入二进制数据到c中
```
对应的也有移动文件指针的函数，
```cpp
seekp(); //移动输出指针
tellp();
seekg(); //移动输入指针
tellg();
```
一般通过移动指针定位写的都是二进制文件，写一个结构体数组进去，可以一块块read然后找到符合的那个**下标**index，然后`seekp(index*sizeof(st));`来移动到数组指定的位置**修改文件**。
> 这个之所以能够**原地修改**是因为结构体**大小已经确定**了！才能这么原地改不出问题。

#### 如何修改文本文件
而如果ASCII文件想要**修改特定的一行**，或者在文件**中间加一行**，**只能重写**，读源文件并且创建一个新的临时文件，**读一行写一行**，在写的过程中实现修改，最后将临时文件**改名覆盖源文件**，这是最可靠的做法，虽然很笨但没办法。

但是尴尬的是fstream根本就不带文件名这个属性，还得用`<cstdio>`里的`rename()`函数修改，这个函数直接操作文件，**不涉及任何流**。
> 貌似我测试的时候rename根本没法覆盖，如果冲突的话就rename失败了，所以还是先remove再rename吧。
代码如下，是将源文件中的"HELLO"行替换成"GOODBYE"：
```cpp
#include <iostream>
#include <fstream>
#include<cstdio>

using namespace std;

int main()
{
	string strReplace = "HELLO";
	string strNew = "GOODBYE";
	ifstream filein("filein.txt"); //File to read from
	ofstream tempout("fileout.txt"); //Temporary file
	if (!filein || !tempout)
	{
		cout << "Error opening files!" << endl;
		return 1;
	}

	string strTemp;
	while (filein >> strTemp)
	{
		if (strTemp == strReplace)
			strTemp = strNew;
		strTemp += "\n";
		tempout << strTemp;
	}
	filein.close();
	tempout.close();
	const char* oldname = "fileout.txt";
	const char* newname = "filein.txt";
	if (remove(newname) != 0)
	{
		cout << "remove old file error!";
		exit(1);
	}
	if (rename(oldname, newname) != 0)
	{
		cout << "rename temp file error!";
		exit(1);
	}
	return 0;
}
```
更普遍的，将文件中"cactus"替换成另一个单词""oranges
```cpp
string search_string = "cactus";
string replace_string = "oranges";
string inbuf;
fstream input_file("demo.txt", ios::in);
ofstream output_file("result.txt");
while (!input_file.eof())
{
	getline(input_file, inbuf);

	int spot = inbuf.find(search_string); //找cactus的起始位置下标
	if(spot >= 0)
	{
		 string tmpstring = inbuf.substr(0,spot);
		 tmpstring += replace_string;
		 tmpstring += inbuf.substr(spot+search_string.length(), inbuf.length());
		 inbuf = tmpstring;
	}

	output_file << inbuf << endl;
}
```
