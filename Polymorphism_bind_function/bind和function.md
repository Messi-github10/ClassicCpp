# bind

## 绑定函数

```c++
#include <iostream>
#include <functional>
using namespace std;

int add(int x, int y){
    cout << "int add(int x, int y)" << endl;
    return x + y;
}

int multiply(int x, int y, int z){
    cout << "int multiply(int x, int y, int z)" << endl;
    return x * y * z;
}

class Example{
public:
    int add(int x, int y){
        cout << "int Example::add(int, int)" << endl;
        return x + y;
    }
};

void test(){
    auto f = bind(add, 10, 20);
    cout << "f() = " << f() << endl;
    
    cout << endl;
    auto f2 = bind(&multiply, 3, 4, 5);
    cout << "f2() = " << f2() << endl;

    cout << endl;
    Example ex;
    auto f3 = bind(Example::add, &ex, 11, 22);
    cout << "f3() = " << f3() << endl;

    // 占位符
    using namespace std::placeholders;
    auto f4 = bind(add, 100, _3);
    cout << "f4(200) = " << f4(200, 300, 400) << endl;

}

int main(int argc, char *argv[]){
    test();
    printf("&main = %p\n", &main);
    printf("main = %p\n", main);
    return 0;
}
```

<span style=color:red;background:yellow;font-size:20px>注意：std::bind既可以绑定普通函数，也可以绑定成员函数，但是如果是成员函数，要注意取地址和一个隐藏的`this`指针。</span>

## 函数指针

> 定义函数指针的方式

### 方式一：直接定义

```c++
void test1(){
    // 定义：返回值 (*指针名)(参数列表)
    // 这种方式定义的是一个变量
    int (*funcPtr)(int, int);
    funcPtr = add;
}
```

### 方式二：定义函数指针类型

```c++
void test2(){
    // 定义函数指针类型
    // 这种方式定义的是函数指针"类型"
    typedef int (*pFunc)(int, int);
    pFunc f = add;
}
```

### 方式三：C++11使用using

```c++
void test3(){
    // C++11使用using（using定义类型别名，与typedef相似）
    using pFunc = int (*)(int, int);
    pFunc f = add;
}
```

### 方式四：现代C++方法（`function`类）

```c++
void test4(){
    // 现代C++方法
    // function是函数对象类，其表示的是返回值类型是函数指针的类
    function<int(int, int)> funcObj = add;

    // 支持Lambda
    funcObj = [](int x, int y)
    {
        return x * y;
    };
}
```

### 方式五：自动推导为函数指针（`auto`）

```c++
void test5(){
    // 自动推导为函数指针
    auto funcPtr = add;
    auto lambdaPtr = [](int x)
    {
        return x * 2;
    };
}
```

### 方式六：对于类成员函数指针

```c++
class Calculator{
public:
    int multiply(int a, float b){
        return a * b;
    }
};

void test6(){
    int (Calculator::*mFuncPtr)(int, float) = &Calculator::multiply;
    Calculator calc;
    (calc.*mFuncPtr)(3, 1.5f);
}
```

## 回调函数

回调函数是一种通过**函数指针**或**函数对象**（如C++的`std::function`）实现的机制，允许将一个函数作为参数传递给另一个函数，并在特定事件或条件触发时被调用。

### 回调函数的实现方式

回调函数的实现依赖于**函数指针**或者**函数对象**.

回调函数的使用步骤：注册回调函数 + 执行回调函数

```c++
void test()
{
    //1、定义函数指针类型
    typedef int(*pFunc)();//函数指针(C语言中的写法)
    using pFunc3 = int(*)();//函数指针(C++中的写法)

    //2、回调函数可以使用函数指针进行调用
    //3、思想：回调函数可以被延迟调用，但是需要借助函数指针
    pFunc f = func1;//注册回调函数
    //....
    cout << "===================" << endl;
    cout << "f() = " << f() << endl;//执行回调函数

    f = &func2;
    cout << "f() = " << f() << endl;

    /* f = add;//error */
}
```

## 占位符

```c++
#include <iostream>
#include <functional>

using std::cout;
using std::endl;
using std::bind;

void func(int x1, int x2, int x3, const int &x4, int x5)
{
    cout << "x1 = " << x1 << endl
         << "x2 = " << x2 << endl
         << "x3 = " << x3 << endl
         << "x4 = " << x4 << endl
         << "x5 = " << x5 << endl;
}

void test(){
    int number = 111;
    using namespace std::placeholders;
    auto f = bind(&func, 10, _1, _2, std::cref(number), number);
    number = 8888;
    f(20, 30, 400, 50000, 1);
}

int main(int argc, char *argv[]){
    test();
    return 0;
}
```

使用占位符需要写`using namespace std::placeholders;`。

占位符整体代表的是函数形参的位置，占位符中的数字代表的是函数的实参位置。

函数在传参的时候，参数的个数一定要大于等于占位符中的数字的最大值。

bind默认采用的是值传递。

如果想使用引用传递，需要使用引用的包装器：

* `std::ref = reference`
* `std::cref = const reference`

多余的参数起到占位的作用，用完之后就地丢掉。

## 引用折叠

![image-20250630211543057](C:\Users\lione\AppData\Roaming\Typora\typora-user-images\image-20250630211543057.png)

```c++
template< class F, class... Args >
/*unspecified*/ bind( F&& f, Args&&... args );

&& &&------>&&
&  &&------>&
&& &------>&
& &------->&
```

总结：这就解释了为什么`bind`的参数列表为右值引用符。在模板中，如果右值引用遇到右值引用，最终会折叠为右值引用，只要有左值引用参与，最终的结果都会折叠为左值引用。

## 绑定数据成员

![image-20250630211815461](C:\Users\lione\AppData\Roaming\Typora\typora-user-images\image-20250630211815461.png)

为什么可以这样绑定数据成员并且像函数调用一样返回呢？

`int _data`经过`bind`适配之后，会变成：

```c++
struct __bind_data_member {
    Example* obj;
    int operator()() { return obj->_data; } // 转换为无参函数
};
```

`int _data`将被视为无参返回`int`的函数。

当我们进行`cout << ... <<endl;`，会先进行`std::function` 的 `operator()`，返回`int`类型的`obj->_data`。

## `function`类

![image-20250630202617465](C:\Users\lione\AppData\Roaming\Typora\typora-user-images\image-20250630202617465.png)

```c++
//add函数的形态：int(int, int)
//返回值f的形态：int()
function<int()> f = bind(add, 10, 20);
cout << "f() = " << f() << endl;
```

传递进去的函数指针形态是：`int(int, int)`，由于我们已经传递了两个参数10和20，则返回给f的形态就是：`int()`。

```c++
    function<int()> f = bind(add, 10, 20);
    cout << "f() = " << f() << endl;

    cout << endl;
    //multiply的函数类型:int(int, int, int)
    //f2的形态:int()
    /* function<int()> f2 = bind(&multiply, 3, 4, 5); */
    /* cout << "f2() = " << f2() << endl; */
    f = bind(&multiply, 3, 4, 5);
    cout << "f() = " << f() << endl;

    cout << endl;
    Example ex;
    //成员函数add:int(Example *, int, int)
    //f3的形态：int()
    /* function<int()> f3 = bind(&Example::add, &ex, 11, 22); */
    /* cout << "f3() = " << f3() << endl; */
    f = bind(&Example::add, &ex, 11, 22);
    cout << "f() = " << f() << endl;
```

上面的这段代码只是为了解释：如果f、f2、f3的返回值类型是相同的。

```c++
	//占位符
    //add函数的形态：int(int, int)
    //f4的形态：int(int)
    function<int(int)> f4 = bind(add, 100, std::placeholders::_1);
    cout << "f4(200) = " << f4(200) << endl;

    using namespace std::placeholders;
    //add的形态：int(int, int)
    function<int(int, int)> f5 = bind(add, _1, _2);

    //func的形态：void(int, string)
    function<void(int, string)> f6 = bind(func, 100, _2);
```

当我们使用了占位符后，传递进去的函数指针形态是：`int(int, int)`，由于我们只传递了参数10，则返回给f的形态就是：`int(int)`。

```c++
#include <iostream>
#include <functional>
#include <string>

using std::cout;
using std::endl;
using std::bind;
using std::function;
using std::string;

class Example
{
public:
    int add(int x, int y)
    {
        cout << "int Example::add(int, int)" << endl;
        return x + y;
    }

    /* int _data() */
    /* { */
    /*     return 1234; */
    /* } */

    int _data = 1234;//C++11的写法，在声明的时候初始化
};

void test()
{
    Example ex;
    function<int()> f = bind(&Example::add, &ex, 11, 22);
    cout << "f() = " << f() << endl;

    cout << endl;
    function<int()> f2 = bind(&Example::_data, &ex);
    cout << "f2() = " << f2() << endl;
}

int main(int argc, char *argv[])
{
    test();
    return 0;
}
```

当遇到类成员函数的时候，我们在使用`bind`的时候必须注意：传入的函数指针需要带作用域符和取地址符，以及隐藏的`this`指针。但是如果是静态成员函数的话，是不需要传入`this`指针的。

## Lambda的误用

```c++
#include <iostream>
#include <functional>
#include <vector>
#include <string>

using std::cout;
using std::endl;
using std::function;
using std::vector;
using std::string;

vector<function<void(const string &)>> vec;

void test()
{
    int num = 100;
    string name("wangdao");

    vec.push_back([&num, &name](const string &value){//error
    /* vec.push_back([num, name](const string &value){//ok */
                  cout << "num = " << num << endl;
                  cout << "name = " << name << endl;
                  cout << "value = " << value << endl;
                  });
}

void test2()
{
    for(auto &elem : vec)
    {
        elem("hello");
    }
}

int main(int argc, char *argv[])
{
    test();
    test2();
    return 0;
}
```

从上述代码中，我们注意到在`test()`中，我们使用`vec.push_back`插入一个函数指针，这个函数指针我们直接使用Lambda表达式写在里面，我们使用引用捕获局部变量，而这将会在`test2()`的遍历中出错，原因在在于，当我们在`test2()`遍历中，局部变量`num`和`name`已经销毁了，而你引用绑定了一个已经销毁的局部变量就会发生未定义行为。所以在使用上，一般不推荐在Lambda中引用捕获局部变量。

## bind与function结合使用

接下来我们会展现一段代码，这段代码的效果与前面所学的多态的效果是一致的。

但在看这段代码之前，我们先回顾一下之前多态的代码。

```c++
#include <math.h>
#include <iostream>

using std::cout;
using std::endl;

//抽象类
class Figure
{
public:
    //声明了两个纯虚函数
    virtual
    void display() const = 0;//打印图形的名字

    virtual 
    double area() const = 0;//计算图形的面积
};

class Rectangle
: public Figure
{
public:
    Rectangle(double len = 0, double width = 0)
    : _len(len)
    , _width(width)
    {
        cout << "Rectangle(double = 0, double = 0)" << endl;
    }

    void display() const override
    {
        cout << "Rectangle";
    }

    double area() const override
    {
        return _len * _width;
    }

    ~Rectangle()
    {
        cout << "~Rectangle()" << endl;
    }

private:
    double _len;
    double _width;

};

//Circle c1 = 10;//int ===>Circle
class Circle
: public Figure
{
public:
    explicit
    Circle(double radius = 0)
    : _radius(radius)
    {
        cout << "Circle(double = 0)" << endl;
    }

    void display() const override
    {
        cout << "Circle";
    }

    double area() const override
    {
        return 3.14 * _radius * _radius;
    }

    ~Circle()
    {
        cout << "~Circle()" << endl;
    }
private:
    double _radius;
};

class Triangle
: public Figure
{
public:
    Triangle(double a = 0, double b = 0, double c = 0)
    : _a(a)
    , _b(b)
    , _c(c)
    {
        cout << "Triangle(double = 0, double = 0, double = 0)" << endl;
    }

    void display() const override
    {
        cout << "Triangle" ;
    }

    double area() const override
    {
        double tmp = (_a + _b + _c)/2;
        return sqrt(tmp * (tmp - _a) * (tmp - _b) * (tmp -_c));
    }

    ~Triangle()
    {
        cout <<"~Triangle()" << endl;
    }
private:
    double _a;
    double _b;
    double _c;
};

void func(const Figure &ref)
{
    ref.display();
    cout << "'s 的面积 " << ref.area() << endl;
}

int main(int argc, char **argv)
{
    Rectangle rec(10, 20);
    Circle cir(10);
    Triangle tri(3, 4, 5);

    func(rec);
    func(cir);
    func(tri);

    return 0;
}

```

接下来的代码，展示的是`bind`和`function`相结合达到多态的效果。

```c++
#include <iostream>
#include <functional>
#include <math.h>
using namespace std;

class Figure{
public:
    // 定义函数指针类型
    using DisplayCallback = function<void()>;
    using AreaCallback = function<double()>;

    DisplayCallback _displayCallback;
    AreaCallback _areaCallback;

    // 给数据成员设置值
    void setDisplayCallback(DisplayCallback &&cb){
        // 传递给setDisplayCallback的是右值，但是cd可不是右值
        _displayCallback = std::move(cb);
    }

    void setAreaCallback(AreaCallback &&cb){
        _areaCallback = std::move(cb);
    }

    void handleDisplayCallback() const{
        if(_displayCallback){
            _displayCallback();
        }
    }

    double handleAreaCallback() const{
        if(_areaCallback){
            return _areaCallback();
        }else{
            return 0.0;
        }
    }
};

class Rectangle
{
public:
    Rectangle(double len = 0, double width = 0)
    : _len(len)
    , _width(width)
    {
        cout << "Rectangle(double = 0, double = 0)" << endl;
    }

    void display(int x) const 
    {
        cout << "Rectangle";
    }

    double area() const 
    {
        return _len * _width;
    }

    ~Rectangle()
    {
        cout << "~Rectangle()" << endl;
    }

private:
    double _len;
    double _width;

};

//Circle c1 = 10;//int ===>Circle
class Circle
{
public:
    explicit
    Circle(double radius = 0)
    : _radius(radius)
    {
        cout << "Circle(double = 0)" << endl;
    }

    void print() const
    {
        cout << "Circle";
    }

    double printArea() const 
    {
        return 3.14 * _radius * _radius;
    }

    ~Circle()
    {
        cout << "~Circle()" << endl;
    }
private:
    double _radius;
};

class Triangle
{
public:
    Triangle(double a = 0, double b = 0, double c = 0)
    : _a(a)
    , _b(b)
    , _c(c)
    {
        cout << "Triangle(double = 0, double = 0, double = 0)" << endl;
    }

    void show() const 
    {
        cout << "Triangle" ;
    }

    double showArea() const 
    {
        double tmp = (_a + _b + _c)/2;
        return sqrt(tmp * (tmp - _a) * (tmp - _b) * (tmp -_c));
    }

    ~Triangle()
    {
        cout <<"~Triangle()" << endl;
    }
private:
    double _a;
    double _b;
    double _c;
};

void func(const Figure &ref){
    ref.handleDisplayCallback();
    cout << "'s 的面积 " << ref.handleAreaCallback() << endl;
}

int main(int agrc, char *argv[]){

    Rectangle rec(10, 20);
    Circle cir(10);
    Triangle tri(3, 4, 5);

    Figure fig;

    fig.setDisplayCallback(bind(&Rectangle::display, &rec, 100));
    fig.setAreaCallback(bind(&Rectangle::area, &rec));
    func(fig);

    fig.setDisplayCallback(bind(&Circle::print, &cir));
    fig.setAreaCallback(bind(&Circle::printArea, &cir));
    func(fig);

    fig.setDisplayCallback(bind(&Triangle::show, &tri));
    fig.setAreaCallback(bind(&Triangle::showArea, &tri));
    func(fig);

    return 0;
}
```

![image-20250630211400988](C:\Users\lione\AppData\Roaming\Typora\typora-user-images\image-20250630211400988.png)

![image-20250630211409371](C:\Users\lione\AppData\Roaming\Typora\typora-user-images\image-20250630211409371.png)

![image-20250630211429013](C:\Users\lione\AppData\Roaming\Typora\typora-user-images\image-20250630211429013.png)

![image-20250630211435819](C:\Users\lione\AppData\Roaming\Typora\typora-user-images\image-20250630211435819.png)

|    **传统多态（虚函数）**    |          **`bind`+`function` 模拟多态**           |                       **对应关系说明**                       |
| :--------------------------: | :-----------------------------------------------: | :----------------------------------------------------------: |
|     **基类（抽象接口）**     |               `std::function` 类型                |         定义统一的调用接口（如 `DisplayCallback`）。         |
| **虚函数（如 `display()`）** |    被绑定的成员函数（如 `Rectangle::display`）    |               具体实现由不同类的成员函数提供。               |
| **派生类（如 `Rectangle`）** |        普通类（如 `Rectangle`、`Circle`）         |      包含实际要调用的成员函数，但**无需继承统一基类**。      |
|      **运行时多态绑定**      |          `std::bind` 绑定对象和成员函数           | 将对象实例和成员函数绑定到 `std::function`，实现运行时动态调用。 |
| **基类指针/引用调用虚函数**  | `std::function` 的调用（如 `_displayCallback()`） |        通过统一的 `std::function` 接口调用具体行为。         |