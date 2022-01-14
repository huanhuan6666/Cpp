## 简介
* QT的头文件一般和类名一样，并且前两个字母大写
### QApplication
* 每一个QT项目**有且只有**一个`QApplication`类，有点像C语言的main函数
* 并且`exec()`函数实现让程序一直执行，等待事件的发生，如果不调用`exec()`窗口就会一闪而过
* 因此一般的框架都是
```cpp
int main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  //...
  //自己写的代码
  //...
  a.exec();
  return 0; //也可以直接return a.exec();
}
```

### QWidget
* 组件基类，并且默认是隐藏，想要显示必须`show()`出来
