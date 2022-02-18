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


