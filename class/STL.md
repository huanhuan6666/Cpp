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
* C++标准的STL的头文件有：`<algorithm>、<deque>、<functional>、<iterator>、<vector>、<list>、<map>、<memory>、<numeric>、<queue>、<set>、<stack> 和<utility>`
	* 其中容器主要为：`<vector>,<list>,<deque>,<set>,<map>,<stack> 和<queue>`
	* 迭代器主要有：`<utility>,<iterator>和<memory>`
	* 算法主要有：`<algorithm>，<numeric>和<functional>`

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
	
## 迭代器
迭代器就如同一个指针，指出容器中的一个特定位置。记住`v1.end()`迭代器指向的是最后一个元素**之后**的那个位置。
	
分类如下：

* 只读迭代器（很少用到，写个东西要那么多限制干嘛啊这是。。。😶）
```cpp
const_iterator
const_reverse_iterator
```
就是说不能用这个迭代器修改容器的内容，一修改就报错。
	
* 按照迭代方向：
	* 正向迭代器iterator
	```cpp
	for(vector<int>::iterator it = v1.begin(); it != v1.end(); it++)
	```
	* 逆向迭代器reverse_iterator
	```cpp
	for(vector<int>::reverse_iterator it = v1.rbegin(); it != rend(); ; it++)
	```
	* 双向迭代器
	即支持it的++和--，只有list,set,multiset,map,multimap支持双向迭代器
	* 随机访问迭代器
	可++ --，也重载了[]来访问，也可以it+=5这样跨越几个访问。比如vector，deque。
	> 但是说实在vector和deque这种直接能用下标像数组那样，何必再来个迭代器套皮呢？而且迭代器限制还很多，就是说能下标就下标，不能下标再找找迭代器怎么用吧🙂


## 容器
* 容器大体分为：
	* 序列式容器（Sequence containers）
	每个元素都有固定位置－－取决于插入时机和地点，和元素值无关。比如vector、deque、list
	* 关联式容器（Associated containers）
	元素位置取决于特定的排序准则，和插入顺序无关。比如set、multiset、map、multimap
* 具体分类如下：

| 数据结构                 | 描述                                                         |实现头文件|
| ------------------------ | ------------------------------------------------------------ | ---------- |
| 向量(vector)             | 连续存储的元素                                               |<vector> |
| 列表(list)               | 由节点组成的双向链表，每个结点包含着一个元素                 |<list> |
| 双队列(deque)            | 连续存储的指向不同元素的指针所组成的数组                     |<deque>  |
| 集合(set)                | 由节点组成的红黑树，每个节点都包含着一个元素，节点之间以某种作用于元素对的谓词排列，没有两个不同的元素能够拥有相同的次序 | <set>      |
| 多重集合(multiset)       | 允许存在两个次序相等的元素的集合                             |<set>    |
| 栈(stack)                | 后进先出的值的排列                                           |<stack>  |
| 队列(queue)              | 先进先出的执的排列                                           |<queue>  |
| 优先队列(priority_queue) | 元素的次序是由作用于所存储的值对上的某种谓词决定的的一种队列 |<queue>  |
| 映射(map)                | 由{键，值}对组成的集合，以某种作用于键对上的谓词排列         |<map>    |
| 多重映射(multimap)       | 允许键对有相等的次序的映射                                   |<map>    |

### STL的string
string的伟大之处在于，其所有成员函数都**兼容了**C风格的`char *`即**字符数组**。
> string本身是STL的basic_string<>模板的一个实例。
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
* 子串
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
	
### vector容器
之前的string不是STL的内容，只不过是C++对于字符串的加强版本。
* 构造
```cpp
vector<CA> vecCA;  //用于存放CA对象的vector容器。
```
由于容器元素的存放是按**值复制的方式**进行的，所以此时CA必须提供CA的**拷贝构造函数**，以保证CA对象间拷贝正常。

* 一些操作
需要注意的是
```cpp
vector<int> v1;
v1.push_back(1);
v1.pop_back(); 
```
`pop_back()`**没有返回值**！如果想取尾部值并且弹出，只能分两步：
```cpp
int i = v1.back(); //获取尾部元素
v1.back() = 123;   //back()返回的是个左值，可以修改
v1.pop_back();     //弹出尾部元素
```
和back类似的`v1.front();` 也是返回v1中的**头部**元素的**左值**。

* 一些典中典
```cpp
v1.size(); //返回大小
v1.empty(); //是否为空
v1.clear(); //清空
v1.erase(); //清除，注意erase返回的是删除元素后迭代器指向的下一个位置
```
各种增删查改用到的时候查一查就好了。。。

比如删除v1中所有的2（原地）：
```cpp
for(auto it=v1.begin(); it!=v1.end(); )
{
	if(*it == 2)
		it = it.erase(it); //删除2并且让it指向下一个位置
	else
		it++;
}
```

### deque容器
deque是个**双端数组**，就是两头都是开口，front和back都可以push和pop；而vector是单端的，只有back才可以push和pop。
	
* 典型操作
```cpp
deque.push_back(elem);	//在容器尾部添加一个数据
deque.push_front(elem);	//在容器头部插入一个数据
deque.pop_back();    		//删除容器最后一个数据
deque.pop_front();		//删除容器第一个数据
deque.back();                  //返回尾部元素
deque.front();                 //返回头部元素
```
back和front返回的是左值，可以修改的。
	
还有什么erase，size，clear，insert都很典，用到了再查吧

### stack容器
其实vector的行为已经就是个先进后出的**栈**了，毕竟只有一端开放push和pop。但是并不是真正意义上的，**完美封装**的栈，因为它还可以front看看头部元素，甚至可以insert直接在头部插入。
	
stack就是最简单**纯粹的栈**，只有：
```cpp
stack.push(elem);   //往栈头添加元素
stack.pop();   //从栈头移除第一个元素
stack.top();	  //返回最后一个压入栈元素的左值
stack.empty();   //判断堆栈是否为空
stack.size(); 	     //返回堆栈的大小
```
	
### queue容器
和stack栈一样，这个queue也是个**简单纯粹的队列**，方法很简洁，很好体现了类的封装性：
```cpp
queue.push(elem);   //往队尾添加元素
queue.pop();   //从队头移除第一个元素
queue.back();   //返回最后一个元素
queue.front();   //返回第一个元素
queue.empty();   //判断队列是否为空
queue.size(); 	     //返回队列的大小
```
deque也能实现队列的行为，但它就是暴露的太多，不够纯粹。而queue就是：只能队尾进队首出，多余的方法一个都不给🤭

### 优先级队列priority_queue
不管怎么push进去最后pop出来的序列是按照从小到大的顺序的。其实就是push进一个元素后，优先队列**会按照优先级调整元素顺序**，说白了就是**排序**，只不过你根本不用管它**自动就给你排好序**了hhh还是挺好用的就是说。

这个库存在于`#include<queue>`库中，也就是说还是个队列嘛。

分类：最大值优先级队列（最大值先pop出来）、最小值优先级队列（最小值先pop出来），写法如下：

```cpp
priority_queue<int> p1; //默认是 最大值优先级队列 
priority_queue<int, vector<int>, less<int> > p1; //相当于这样写
priority_queue<int, vector<int>, greater<int>> p2; //最小值优先级队列
	
pq.push(item);
pq.pop();
pq.top(); 	// 获取队头元素
pq.size();
```

这里简单说明一下`priority_queue<int, vector<int>, less<int>>`，这个`less<int>`是STL提前给我们定义好的**预定义函数**，相当于一个**谓词**。
	
### list容器
是一个**双向链表**，都链表了当然不能用[]操作符了，同样它的迭代器也只能++和--，**不能跳步**比如it+=3这样，也就是所谓**不支持随机访问**。
	
双向链表有头有尾，因此方法也是这样对应的，back和front都可以push以及pop
```cpp
list.push_back(elem);	   //在容器尾部加入一个元素
list.pop_back();              //删除容器中最后一个元素
list.push_front(elem);     //在容器开头插入一个元素
list.pop_front();              //从容器开头移除第一个元素
list.front();   	//返回第一个元素
list.back();  		//返回最后一个元素
```
剩下插入删除啥的，也没啥新意，还有什么正向迭代逆向迭代，这里再写一遍删除list里的3结点吧，一回事的：
```cpp
for(list<int>::iterator it=lstInt.being(); it!=lstInt.end(); )    //小括号里不需写  ++it
{
   if(*it == 3)
        it  =  lstInt.erase(it);       //以迭代器为参数，删除元素3，并把数据删除后的下一个元素位置返回给迭代器。
   else
       ++it;
}
```
还有就是insert插入位置：
```cpp
//加入l = 1  2  3  4  5
it = l.begin();
it++;
it++;  //这时候it指向3
l.insert(it, 100);  //将100插入到3的位置
```
也就是说100会取代it指向的位置，链表变成了1 2 100 3 4 5.
	
	
### set和multiset容器
set是集合，显然和链表一样，不能随机访问，即不能[]直接访问。set内部是用**红黑树**变体实现的，红黑树是个平衡二叉树，在插入和删除这些操作上比线性的vector块不知道多少，毕竟一个是O(n)一个是O(logn)。

* set和multiset都是**自动排序**的，因此迭代器挨个输出是有大小顺序的，可以指定排序方式，默认情况下**从小到大**排序
* multiset和set的区别：set支持**唯一**键值，每个元素值只能出现一次，insert多个重复值最终**只包含一个**；而multiset中一个值可以多次出现
* set和multiset中的值都**不能直接修改**，只能删除原有然后插入新的值

#### 基本操作
```cpp
set.insert(elem);     //在容器中插入元素。
set.begin();  //返回容器中第一个数据的迭代器。
set.end();  //返回容器中最后一个数据之后的迭代器。
set.rbegin();  //返回容器中倒数第一个元素的迭代器。
set.rend();   //返回容器中倒数最后一个元素的后面的迭代器。
set.find(elem); //查找elem并返回迭代器
set.count(elem);  //返回elem的个数，set中只有0或1，multiset可以>1
```

#### 指定set排序方式
我们之前就已经挖过坑了，即优先队列也是个**有序的结构**，同样set和multiset也是有序的，可以用“谓词”`less<>, greater<>`这样的东西来指定排序方式。
	
那么`less<>, greater<>`这东西到底是什么？以及如果容器里放的是**自定义类型**该如何排序？

* `less<>, greater<>`是C++提前预定义好的**函数对象**，<>里的数据要跟set中容纳的数据类型**一致**，比如：	
```cpp
//使用stl提供的函数对象
set<int,greater<int>> setIntB;   
setIntB.insert(3);
setIntB.insert(1);
setIntB.insert(5);
setIntB.insert(2);
//此时容器setIntB就包含了按顺序的5,3,2,1元素
```
* 如果要对自定义类型排序，必须向容器提供**函数对象**，也叫**仿函数**，英文名叫functor，
默认的set是从小到大排序的，但是可以用**谓词**来让其从大到小排序。
```cpp
set<int> s1; //默认从小到大
set<int, less<int>> s2; //等价于这样写
set<int, greater<int>> s3; //从大到小排序的set
```
	
关于仿函数(函数对象)和谓词的完整解释将在算法部分提到。

#### pair二元组的用法
pair就是个二元组，有些函数返回的就是一个pair对象，当然我们马上提到的map容器里面都是些pair二元组。
	
先说说返回pair对象的函数`equal_range(item)`
```cpp
multiset<int> h;//建立一个multiset类型，变量名是h，h序列里面存的是int类型,初始h为空
while(x!=0){
	cin >> x;
	h.insert(x);//将x插入h中
}    
pair<multiset<int>::iterator , multiset<int>::iterator> MyPair = h.equal_range(22);
```
* `equal_range(item)`返回一个pair迭代器二元组，第一个迭代器指向的是第一个**>=item**的，第二个迭代器指向的是第一个**>item**的。
* 通过`pair.first`和`pair.second`来访问二元组的两个元素，比如：
```cpp
multiset<int>::iterator itBeg = pairIt.first;
multiset<int>::iterator itEnd = pairIt.second;

int nBeg = *itBeg;
int nEnd = *itEnd;
```

### map和multimap容器
map是一个**键值对**序列，即(key,value)对，其实就是一个个**pair对象**。它提供基于key的快速检索能力。
	
map中**key值唯一**，和set一样内部也是**红黑树变体**的平衡二叉树实现的，因此是**自动排序**的，所以也有仿函数指定排序方式的问题，而且也**不能指定插入位置**。
	
但是map支持[]操作，即和py的字典那样，支持`map[key] = value;`的操作，但是个更高级的字典，即内部**可以自动排序**。而multimap不支持[]操作，因为它的key可以多次出现，一对多还怎么[]访问啊

#### 一些操作
* 插入**键值对**pair
```cpp
map.insert(item);    //往容器插入元素，返回pair<iterator,bool>，即插入成功的话返回的pair.second就是true
//比如
map<int, string> mapStu;
mapStu.insert( pair<int,string>(3,"小张") ); //直接创建pair对象
mapStu.inset(make_pair(-1, “校长-1”));       //通过make_pair函数
mapStu[3] = “小刘";                          //最简单的方式，用[]访问不存在就会自动创建
```
* 通过key访问value
```cpp
string strName = mapStu[2];   //取操作或插入操作
```
* 查找
```cpp
map.find(key);  // 查找键key是否存在，若存在，返回该键的元素的迭代器；若不存在，返回map.end();
map.count(keyElem);   //返回容器中key为keyElem的对组个数。对map来说，要么是0，要么是1。对multimap来说，值可能大于1。
```
**注意**：只有当mapStu存在2这个键时才是正确的取操作，否则会**自动插入**一个实例，键为2，**值为初始化值**。

#### 键的排序方式
```cpp
map<T1, T2> map0;             //默认采用less<T1>的排序方式，即按照键升序
map<T1,T2,less<T1> >  mapA;  //该容器是按键的升序方式排列元素。未指定函数对象，默认采用less<T1>函数对象。
map<T1,T2,greater<T1>> mapB;   //该容器是按键的降序方式排列元素。
```

#### multimap的案例
一个key可以对应多个value，这不就是妥妥的**分组**，好像SQL的group by分组语句。

案例如下：
> 公司有销售部 sale （员工2名）、技术研发部 development （1人）、财务部 Financial （2人）;
> 人员信息有：姓名，年龄，电话、工资等组成;
> 请通过 multimap进行 信息的插入、保存、显示

实现如下：
```cpp
class Person
{
public:
	string name;
	int age;
	string tel;
	int sal;
}
int main()
{
	Person p1, p2, p3, p4, p5; //每个人的构造过程略
	multimap<string, Person> map2; //创建 <部门名:人> 的multimap
	map2.insert(make_pair("sale", p1));
	map2.insert(make_pair("sale", p2));
	map2.insert(make_pair("development", p3));
	map2.insert(make_pair("Financial", p4));
	map2.insert(make_pair("Financial", p5));
	
	//遍历所有键值对
	for(auto it = map2.begin(); it != map2.end(); it++) 
	{
		cout << "department: " << it->first << "name: " << it->second.name;
	}
	//按部门显示信息，这里显示销售部
	int num = map2.count("sale");
	cout << "person's count is:" << num;
	auto it = map.find("sale");
	while(it != map2.end() && num)
	{
		cout << "department: " << it->first << "name: " << it->second.name;
		it++; 
		num--;
	}
}
```

## 算法
### 算法分类
* 修改容器本身内容的算法
	* 计数算法	 `count、count_if`
	* 搜索算法	 `search、find、find_if、find_first_of、…`
	* 比较算法	 `equal、mismatch、lexicographical_compare`
* 不修改容器内容的算法
	* 删除算法	 `remove、remove_if、remove_copy、…`
	* 修改算法	 `for_each、transform`
	* 排序算法	 `sort、stable_sort、partial_sort、`

### 函数对象
重载函数调用操作符的**类**（即重载类的operator()），**其对象**常称为**函数对象**（function object），即它们是行为类似函数的对象。函数对象通过`类名(参数列表)`的方式来使用，看起来和函数的用法一模一样，如果没有上下文，完全可以看作函数来对待。
> 没什么特别的，就是类的操作符重载，只不过重载的是()函数调用操作符，稍微有点特殊罢了。因为这样看上去这个类和函数的用法非常像，所以也叫**仿函数**。
比如：
```cpp
template<class T>
class Show            //Show类重载了函数调用()操作符
{
public:
	void operator() (T &t)
	{
		cout << t << " ";	
	}
}
int main()
{
	int a = 10;
	Show<int> showelem;   //showelem就是个函数对象！
	shwoelem(a);          //和函数调用的格式一模一样
	
}
```
可以看到`shwoelem(a);`将输出10，如果不看上下文的话把他当作函数完全没问题，这就是**函数对象**，因此也叫做**仿函数**。
> 一元函数对象：函数参数1个；二元函数对象：函数参数2个；
	
* 函数对象和普通函数
上面我们定义了个类模板，重载了()操作符；下面我们定义一个函数模板，其实也能看做普通函数啦🤭
```cpp
template<class T>
void func_show(T &t)   //模板函数
{
	cout << t << " ";
}
void print_func(int &t) //普通函数
{
	cout << t << " ";
}
int main(){
	int a = 10;
	func_show<int>(a);  //调用函数模板
	print_func(a);      //调用普通函数
}
```
再看下面的例子，C++风格的函数对象与C风格的回调函数，非常的类似，可以说函数对象是对C回调函数的**面向对象拓展**：
```cpp
vector<int> v1(5, 6);
for_each(v1.begin(), v1.end(), Show<int>());   //传入匿名函数对象Show<int>()
for_each(v1.begin(), v1.end(), showelem);      //传入实名函数对象showelem
for_each(v1.begin(), v1.end(), print_func);     //C风格的传入回调函数
```
> 一般用匿名函数对象就可以，就像写lambda表达式表示匿名函数那样
如果只看调用函数这个工作的话，函数对象和普通函数没有什么区别，不管是普通调用还是回调，写法都几乎一致。
	
但是函数对象人家是个**对象**，是**类的实例**，可以突破函数的概念，能够保持调用状态信息，比如：
```cpp
template<class T>
class Show            //Show类重载了函数调用()操作符
{
private:
	int count; 
public:
	Show()
	{
		count = 0;
	}
	void operator() (T &t)
	{
		count++;  //调用次数++
		cout << t << " ";	
	}
	void printCount(){
		cout << "count is: " << count;
	}
}
int main()
{
	vector<int> v1(5, 6);
	Show<int> show_int;   //定义一个函数对象
	for_each(v1.begin(), v2.end(), show_int);  //将函数对象作为for_each参数
	show_int.printCount();    //输出记录的调用次数
}
```
可是这样最后输出的是0！？？看来还是没有理解**函数对象是个对象**的本质，`func_each()`原型如下，将一个对象做函数参数，是**值传递的**，也就是说会调用**拷贝构造函数**形成形参，count++修改的都是**形参**值！
```cpp
template <class _InIt, class _Fn>	//_InIt表示迭代器，_Fn表示函数对象/回调函数	  
	_Fn for_each(_InIt _First, _InIt _Last, _Fn _Func)			     
```
那么有什么办法呢？for_each的返回值就是形参函数对象，只需要最后**赋值**给实参就ok了，即：
```cpp
show_int = for_each(v1.begin(), v2.end(), show_int);
```
可以看到**面向对象拓展后**就是不一样，比C那种回调函数高级了不是一点，但是切记函数对象是个对象。

**明确STL算法返回的值是迭代器还是函数对象/谓词，是使用STL算法的核心所在！！！**

### 谓词
是一种特殊的函数/函数对象，即返回值为bool类型。
* 分类
	* 一元谓词： 函数参数1个，函数返回值是bool类型，可以作为一个判断式
	* 二元谓词： 函数参数2个，函数返回值是bool类型

显然谓词的概念一个逻辑上的，只不过在C语言里我们用回调函数形式写，在C++可以用函数对象的形式写。
> 也就是说，谓词可以用不同的方式表示，可以是一个回调函数，也可以是一个仿函数，只要是个返回bool类型的函数就OK

* C风格谓词举例：
```cpp
bool func(const string& s){  //C风格的一元谓词，使用起来用回调的方式
	return s.size() > 6;
}

bool func2(const string&s1, const string& s2){ //C风格的二元谓词
	return s1.size() > s2.size();
}
```

* C++风格谓词举例：函数对象
```cpp
template <typename T>
class Isdiv
{
public:
	Isdiv(const T &divisor)
		this->divisor = divisor;

	bool operator()(T &t)  //谓词
		return (t%divisor == 0);
private:
	T divisor;
};
int main()
{
	vector<int> v2;
	for (int i=10; i<33; i++)
		v2.push_back(i);
	Isdiv<int> mydiv(4);  //定义函数对象mydiv
	vector<int>::iterator it = find_if(v2.begin(), v2.end(), mydiv); //find_if(_InIt _First, _InIt _Last, _Pr _Pred)返回的是迭代器
	
	//等价于find_if(v2.begin(), v2.end(), Isdiv<int>(4))
	if (it != v2.end())
		cout << "第一个被4整除的数是：" << *it << endl;
	
}
```

* 二元谓词
一般就是用于两个元素的**比较**，比如：
```cpp
 //C风格的，模板函数，通过回调使用
template<class T>
bool my_compare(const T &t1, const T &t2)
{
	return t1 < t2;
}
sort(v1.begin(), v1.end(), my_compare<int>); 

//C++风格的，通过函数对象使用
template<class T> 
class compare_class
{
public:
	bool operator() (const T& t1, const T& t2){
		return t1 < t2;
	}
}
sort(v1.begin(), v1.end(), compare_class<int>()); //匿名函数对象
compare_class<int> my_compare;
sort(v1.begin(), v1.end(), my_compare);  //实名函数对象
```
我们发现如果写函数对象的话，还得写类写`public`非常麻烦，因此一般函数对象都用`struct`来写，这样默认所有成员都是`public`的，非常方便：
```cpp
template<class T> 
struct compare_class
{
	bool operator() (const T& t1, const T& t2){
		return t1 < t2;
	}
}			      
```			      
STL**源码**的sort如下：
```cpp
template <class _RanIt, class _Pr>  //_RanIt表示迭代器，_Pr表示谓词（当然回调函数/函数对象表示的谓词都可以
	_CONSTEXPR20 void sort(const _RanIt _First, const _RanIt _Last, _Pr _Pred) 			       
```	

### 再次理解有序容器
之前我们抛过砖，由于`set，map，priority_queue`是有序容器，如果里面放入自定义类型，那么我们必须给定一个明确的排序方式，那么该如何实现呢？这就用到了**谓词**。
	
随便找一个有序容器的源码，比如map:
```cpp
template <class _Kty, class _Pr = less<_Kty>, class _Alloc = allocator<_Kty>>
class set : public _Tree<_Tset_traits<_Kty, _Pr, _Alloc, false>> {
    // ordered red-black tree of key values, unique keys
	...
}
```
可以看到_Kty表示数据类型，_Pr就是典中典的谓词了，可以回调函数也可以函数对象，之前都讨论过，大同小异。	

比如一个自定义类Student放入set中：
```cpp
class CStudent
{
public:
	CStudent(int iID, string strName)
	{
		m_iID = iID;
		m_strName = strName;
	}
	int m_iID;		//学号
	string m_strName; 	//姓名
};
//定义函数对象形式的谓词，按照ID升序排列	
struct CmpStuFunctor
{
	bool operator() (const CStudent& s1, const CStudent& s2)
	{
		return s1.m_iID < s2.m_iID; //ID都是Stu的public属性，更方便，当然写成private的话需要getattr一下
	}
};
int main()
{
	set<CStudent, CmpStuFunctor> mtset; //myset就会按照插入Stu的ID升序排序了			   	
}					
```
#### 写法注意事项
注意在**定义时**，<>内**只**接受函数对象，**不接受回调函数**，函数对象后面**不要写括号**！
```cpp
set<CStudent, CmpStuFunctor> mtset; //OK
set<CStudent, CmpStuFunctor()> mtset;  //error！
```
而在**使用STL算法**时，函数对象和回调函数**都**可以接受，匿名函数对象后面必须**加上括号**！
```cpp
sort(b.begin(), b.end(), CmpStuFunctor); //error!
sort(b.begin(), b.end(), CmpStuFunctor()); //OK
sort(b.begin(), b.end(), less<CStudent>()); //同样别忘了()
```

经过之前的分析，我们对于`less<>, greater<>`两个谓词也可以理解机理了：其实就是个类模板，里面重载了()操作符。比如典中典：
```cpp
set<int> s1; //默认从小到大
set<int, less<int>> s2; //等价于这样写
set<int, greater<int>> s3; //从大到小排序的set
	
map<T1, T2> map0;             //默认采用less<T1>的排序方式，即按照键升序
map<T1,T2,less<T1> >  mapA;  //该容器是按键的升序方式排列元素。未指定函数对象，默认采用less<T1>函数对象。
map<T1,T2,greater<T1>> mapB;   //该容器是按键的降序方式排列元素。
```
还是注意定义时<>内的函数对象后面不要()，一个简单的实现如下：
```cpp
template<class T>
struct less
{
	bool operator() (const T& t1, const T &t2)
	{
		return t1 < t2;   //内部会让元素<比较，因此需要自己重载自定义类的<
	}
}
```
less内部让元素`<`比较，greater内部让元素`>`比较

### 预定义函数对象和函数适配器
不只有less，greater这样预定义的函数对象，STL还预定义了一些非常好用的函数对象，一般在`#include <functional>`头文件中。

大致分为：
* 算数函数对象

	* 加法：plus<Types>
	* 减法：minus<Types>	
	* 乘法：multiplies<Types>
	* 除法divides<Tpye>
	* 求余：modulus<Tpye>
	* 取反：negate<Type>

* 关系函数对象

	* 等于equal_to<Tpye>
	* 不等于not_equal_to<Type>
	* 大于 greater<Type>
	* 大于等于greater_equal<Type>
	* 小于 less<Type>
	* 小于等于less_equal<Type>

* 逻辑函数对象
	* 逻辑与 logical_and<Type>
	* 逻辑或logical_or<Type>
	* 逻辑非logical_not<Type>
	
一般这些东西都是配合STL算法结合使用的，作为STL算法的函数参数。说白了就是一堆内置函数，比如plus就是完成了+运算符这个操作，用函数模板也能实现数据类型和算法的分离，但是OO思想硬是把它做成了函数对象，当然也带了了一些其他的好处。
	
#### 函数适配器
有些时候我们想找容器中符合某种条件的元素个数有多少，用到`count_if()`算法，比如想找小于某个值的元素个数：
* 可以自定义谓词
```cpp
template<class T>
struct myCmp  //自定义谓词
{
	T m_t;
public:
	bool operator() (const T& t)
	{
		return m_t < t;
	}
}
vector<int> v1(5, 6);			       
int num = count_if(v1.begin(), v1.end(), myCmp<int>(2)); // 自定义谓词直接可以把参数写进去
```
自定义谓词可以直接把想要比较的那个值**作为参数**写到函数对象里🤭
	
但是使用预定义的函数对象我们没办法这么写，因此**函数适配器**的概念应运而生，作用就是用来特殊化或者**扩展**预定义的函数对象，比如给定某个参数这样子。

* 常用适配器是：
	* 绑定器（binder）:
	
	binder通过把二元函数对象的一个实参绑定到一个特殊的值上，将其转换成一元函数对象。C＋＋标准库提供两种预定义的binder适配器：bind1st和bind2nd，前者把值绑定到二元函数对象的第一个实参上，后者绑定在第二个实参上。
	* 取反器(negator) : 
	
	negator是一个将函数对象的值翻转的函数适配器。标准库提供两个预定义的ngeator适配器：not1翻转一元预定义函数对象的真值,而not2翻转二元谓词函数的真值。
> 捏麻就是说这东西不天天用谁tm记得住🤡，还是那句话，有个基本的了解，用到的时候再查吧

* 使用预定义函数对象+函数适配器


这样一来，使用greater加上适配器的做法如下：
```cpp
int num3 = count_if(v1.begin(), v1.end(), bind2nd(greater<int>(), 2 ) );

//取模 能被2整除的数 求奇数
int num4 = count_if(v1.begin(), v1.end(), bind2nd(modulus <int>(), 2 ) ); 
```
	
### STL设计理念
容器、算法、迭代器、函数对象以及函数适配器。

STL内部的关系如下：

1）STL的容器通过**类模板**技术，实现*数据类型和容器模型*的分离
	
2）STL的迭代器技术实现了**遍历容器的统一方法**；也为STL的算法提供了统一性

3）STL的函数对象实现了**自定义数据类型的算法运算**，比如自定义比较方式，选择条件等等
	
4）STL的函数适配器是用来辅助函数对象的
	
以算法`transform()`的运作为例：

![image](https://user-images.githubusercontent.com/55400137/154835118-2356a441-ce3c-4245-ac88-3845324a689a.png)

transform算法通过迭代器first和last指向的元算作为输入；通过result作为输出；通过函数对象来做自定义数据类型的运算。

### 算法
> 就是说STL的算法太多辣，增删查改还有排序啥的太多辣😫，这里就只看看两个遍历算法吧
	
* for_each()算法
```cpp
for_each(b, e, op);
```
遍历容器内所有元素，并且对每个元素都用传入的函数对象操作，该函数**不得修改序列中的元素**。

**返回**一个函数对象，因此可以记录函数对象的调用状态。

* transform()算法
transform是转换变换的意思。与for_each类似，遍历容器所有元素，但可对容器的元素进行修改。

有两种重载形式：
```cpp
transform(b1, e1, b2, op); //把一个容器v1中的元素经过op变换输出结果到v2容器中，可以是同一个容器
transform(b1, e1, b2, b3, op); //把两个容器v1和v2中的元素，经过变化（比如加）输出到v3容器中
```
需要注意，transform中的op**必须有返回值**，可以**追踪源码**查看到transform最终调用：
```cpp
 for (; _UFirst != _ULast; ++_UFirst, (void) ++_UDest) {
        *_UDest = _Func(*_UFirst);   //这一行出现猫腻🐱
    }
```
可以看到transform使用到`_Func`即传入的函数对象的返回值，如果没有返回值程序就会宕掉，这是transform和for_each的一个重要**区别**：

* 区别
	* for_each一般传入的函数对象可以没有返回值，并且参数一般写成引用
	* transform一般传入的函数对象的参数一般是值传递，并且要有返回值否则报错

因此for_each一般比transform速度快，但是没有transform灵活
	
## STL案例
之后再补吧。。。
