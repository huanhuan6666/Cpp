# STL学习

### 类模板复习
先手写个类模板压压惊，比如实现个简易vector
```cpp
template <class T>
class MyVector
{
private:
  int m_size;
  T *m_array; //array才是真正的容器，因此是T类型!
public:
  MyVector(int size = 0) //构造函数
  {
    m_size = size;
    m_array = new T[size];
  }
  MyVector(const MyVector& obj)  //拷贝构造函数，注意深拷贝
  {
    delete[] m_array;
    m_array = new T[obj.m_size];
    m_size = obj.m_size;
    int i = 0;
    for(; i<m_size; i++)
      m_array[i] = obj.m_array[i];
  }
  ~MyVector() //析构函数
	{
		cout << "vector 析构" << endl;
		delete[] m_array;
	}
public:
  T& operator[](int index) //重载[]，注意返回的是T&
  {
    return m_array[index];
  }
  MyVector& operator=(const MyVector& my)  //重载=，还是注意深拷贝的问题
	{
		delete[] m_array;
    m_size = my.m_size;
		m_array = new T[my.m_size];
		int i = 0;
		for (i = 0; i < m_array; i++)
			m_array[i] = my.m_array[i];
    return *this;
	}
}
```
还是那个问题，这个数组模板内部的拷贝构造和=操作符，最后都**落到了元素本身**的=操作，因此还是典中典的：如果类中出现了**指针数据成员**，那么必须提供相应的**深拷贝函数**，并且**重载=操作符**。

还要注意的一点就是，我们看到类模板也重载了<<操作符，但是最后还是**落到了元素本身**的<<操作，因此自定义类的话如果想用模板也必须重载<<操作符。

也就是说：**使用模板的类千万注意深拷贝问题(拷贝构造函数和重载=操作符)，以及重载<<等问题**

## STL理论基础
* STL从广义上讲分为三类：
	* algorithm（算法）
	* container（容器）
	* iterator（迭代器）

容器和算法通过迭代器可以进行无缝地连接。
* C++标准的STL的头文件有：<algorithm>、<deque>、<functional>、<iterator>、<vector>、<list>、<map>、<memory>、<numeric>、<queue>、<set>、<stack> 和<utility>
	* 其中容器主要为：<vector>,<list>,<deque>,<set>,<map>,<stack> 和<queue>
	* 迭代器主要有：<utility>,<iterator>和<memory>
	* 算法主要有：<algorithm>，<numeric>和<functional>

### 算法和方法的区别
真正意义的算法是凌驾于模板的**全局函数**，类想要使用这种算法必须老老实实**传入自己的迭代器**；而很多时候类自己有成员函数也可以实现某些功能，也就是**所谓方法**，概念要区分清楚！
> 算法是全局函数，方法是成员函数。

举个例子：
```cpp
#include<vector>
#include<iostream>
using namespace std;
int main()
{
	vector<int> v1;  //容器
	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);
	for (vector<int>::iterator it = v1.begin(); it != v1.end(); it++) //迭代器
		cout << *it << ' ';
	int num = count(v1.begin(), v1.end(), 3); //算法，是全局函数
	cout << v1.size(); //方法，是一个成员函数
}
```
* `v1`就是个**容器**，`vector<int>::iterator it`就是个**迭代器**，它的行为和**指针类似**，当然更方便的写法是用`auto`：
```cpp
for(auto it = v1.begin(); ...)
```
现在遍历时根本不用迭代器，而是像py那样：
```cpp
for(auto i : v1)
	cout << i << ' ';
```
* `count()`就是个**算法**，可以看到容器和算法**通过迭代器**进行沟通。

### 关键
那么关键的地方来了：向STL容器中放入对象(比如push_back)是把对象**拷贝了一份**，因此千万注意**深浅拷贝**的问题！！！
> char* p = NULL; free(p);及free一个NULL指针**不会发生任何事情**，我们真正担心的是一块**内存空间被二次释放**(运行时报错)，而不是指针。就算每次free(p);之后都将p置为NULL也无法避免内存被二次释放，这才是浅拷贝真正的问题所在，所以深拷贝开辟两份内存。
	

## 容器
* 容器大体分为：
	* 序列式容器（Sequence containers）
	每个元素都有固定位置－－取决于插入时机和地点，和元素值无关。比如vector、deque、list
	* 关联式容器（Associated containers）
	元素位置取决于特定的排序准则，和插入顺序无关。比如set、multiset、map、multimap
* 具体分类如下：
| 数据结构                 | 描述                                                         | 实现头文件 |
| ------------------------ | ------------------------------------------------------------ | ---------- |
| 向量(vector)             | 连续存储的元素                                               | <vector>   |
| 列表(list)               | 由节点组成的双向链表，每个结点包含着一个元素                 | <list>     |
| 双队列(deque)            | 连续存储的指向不同元素的指针所组成的数组                     | <deque>    |
| 集合(set)                | 由节点组成的红黑树，每个节点都包含着一个元素，节点之间以某种作用于元素对的谓词排列，没有两个不同的元素能够拥有相同的次序 | <set>      |
| 多重集合(multiset)       | 允许存在两个次序相等的元素的集合                             | <set>      |
| 栈(stack)                | 后进先出的值的排列                                           | <stack>    |
| 队列(queue)              | 先进先出的执的排列                                           | <queue>    |
| 优先队列(priority_queue) | 元素的次序是由作用于所存储的值对上的某种谓词决定的的一种队列 | <queue>    |
| 映射(map)                | 由{键，值}对组成的集合，以某种作用于键对上的谓词排列         | <map>      |
| 多重映射(multimap)       | 允许键对有相等的次序的映射                                   | <map>      |

### STL的string
string的伟大之处在于，其所有成员函数都**兼容了**C风格的`char *`即**字符数组**。

#### 操作
下面只展示一**部分**成员函数，来简单展示一下：
* 转换成char *
将string类转换成char *指针：
```cpp
string a = "asd";
printf("%s ", a.c_str());
```
`c_str()`返回一个`const char *`指针，即C风格的字符串。
* 拷贝到char *内存空间：
```cpp
int copy(char *s, int n, int pos=0) const;  //将string的从pos开始的n个字符拷贝到s指针处
```
需要注意n要小于s的大小，否则会**越界**；而且只管拷贝n个字符，**不给你写\0**！
```cpp
char s[10] = {0}; //全为\0
string str = "asdasd";
str.copy(s, 6); //默认pos=0
```
* 构造
```cpp
//拷贝构造函数：
string(const string &str);	//构造一个与str一样的string。如string s1(s2)。
string(const char *s);    //用字符串s初始化
string(int n,char c);    //用n个字符c初始化
```
* 赋值
```cpp
string &operator=(const string &s);//把字符串s赋给当前的字符串
string &assign(const char *s); //把字符串s赋给当前的字符串
```
* 连接
```cpp
string &operator+=(const string &s);  //把字符串s连接到当前字符串结尾
string &operator+=(const char *s);//把字符串s连接到当前字符串结尾
```
* 比较
```cpp
int compare(const string &s) const;  //与字符串s比较
int compare(const char *s) const;   //与字符串s比较
```
* 字串
```cpp
string substr(int pos=0, int n=npos) const;    //返回由pos开始的n个字符组成的子字符串
```
* 查找替换/区间删除和插入
```cpp
//find, replace, insert, erase之类的。。
```
	
#### 算法
转换大小写的算法``：`transform`。因为是算法，所以使用时需要老老实实传入迭代器。
```cpp
string s2 = "AAAbbb";
transform(s2.begin(), s2.end(), toupper); //全部转换成大写

string s3 = "AAAbbb";
transform(s3.begin(), s3.end(), tolower); //转换成小写
```
	
