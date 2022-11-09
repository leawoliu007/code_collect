# 关于 const char*和 char*、const char** 和 char** 赋值问

## 根据 ANSI C 标准的赋值约束条件：

```
1. 两个操作数都是指向有限定符或无限定符的相容类型的指针。

2. 左边指针所指向的类型必须具有右边指针所指向类型的全部限定符。
```

## 一、const char*和 char*

const char\*的类型是：“指向一个具有 const 限定符的 char 类型的指针”。（不能修改其值）

char\*的类型是：“指向一个 char 类型的指针”。

因此 const char*和 char*都是指向 char 类型的指针，只不过 const char\*指向的 char 类型是 const 的。

因此对于代码：

```
char* src;
const char* dest ;
dest = src;
```

这样赋值是正确的，因为:

- 操作数指向的都是 char 类型，因此是相容的
- 左操作数具有有操作数所指向类型的全部限定符（右操作数没有限定符），同时自己有限定符（const）

如果反过来赋值就违反了赋值的约束条件：src 指向的对象的值可以修改，而 dest 指向的对象的值不可修改

如果让 src 去指向 dest 所指向的那个不可修改的对象，如果合法，岂不是变得可修改了？

src = dest; // 这样赋值，左操作数指向的类型没有右操作数指向类型的 const 限定符，不符合约束条件 2

2. const char** 和 char**

   const char\*\*的类型是：“指向一个有 const 限定符的 char 类型的指针的指针”。

   char\*\*的类型是：“指向一个 char 类型的指针的指针”。

   对于 const char** 和 char**来说，二者都是没有限定符的指针类型，但是它们指向的类型不一样，前者指向 char*， 而后者指向 const char*，因此它们不相容，所以 char**类型的操作数不能赋值给 const char**类型的操作数。

   即对于下列代码，编译器会报错：

   ```c++
   char** src;
   const char** dest;
   dest = src;
   // error: invalid conversion from `char**' to `const char\*\*'
   ```

   ***

### const

1. 限定符声明变量只能被读

   ```
   const int i=5;
   int j=0;
   ...
   i=j; //非法，导致编译错误
   j=i; //合法
   ```

2. 必须初始化

   ```
   const int i=5; //合法
   const int j; //非法，导致编译错误
   ```

3. 在另一连接文件中引用 const 常量

   ```
   extern const int i; //合法
   extern const int j=10; //非法，常量不可以被再次赋值
   ```

4. 便于进行类型检查
   用 const 方法可以使编译器对处理内容有更多了解。

   ```
   #define I=10
   const long &i=10; /_dapingguo 提醒：由于编译器的优化，使得在 const long i=10; 时 i 不被分配内存，而是已 10 直接代入 以后的引用中，以致在以后的代码中没有错误，为达到说教效 果，特别地用&i 明确地给出了 i 的内存分配。不过一旦你关闭所 有优化措施，即使 const long i=10;也会引起后面的编译错误。_/
   char h=I; //没有错
   char h=i; //编译警告，可能由于数的截短带来错误赋值。
   ```

5. 可以避免不必要的内存分配

   ```c++
   #define STRING "abcdefghijklmn\n"
   const char string[]="abcdefghijklm\n";
   ...
   printf(STRING); //为 STRING 分配了第一次内存
   printf(string); //为 string 一次分配了内存，以后不再分配
   ...
   printf(STRING); //为 STRING 分配了第二次内存
   printf(string);
   ...
   ```

   由于 const 定义常量从汇编的角度来看，只是给出了对应的内存地址，
   而不是象#define 一样给出的是立即数，所以，const 定义的常量在
   程序运行过程中只有一份拷贝，而#define 定义的常量在内存中有
   若干个拷贝。

6. 可以通过函数对常量进行初始化

   ```
   int value();
   const int i=value();
   dapingguo 说：假定对 ROM 编写程序时，由于目标代码的不可改写，本语句将会无效，不过可以变通一下：
   const int &i=value();
   ```

   只要令 i 的地址处于 ROM 之外，即可实现：i 通过函数初始化，而其值有不会被修改。

7. 是不是 const 的常量值一定不可以被修改呢？
   观察以下一段代码：

   ```c
   const int i=0;
   int _p=(int_)&i;
   p=100;
   ```

   通过强制类型转换，将地址赋给变量，再作修改即可以改变 const 常量值。

8. 请分清数值常量和指针常量，以下声明颇为玩味：

   ```c++
   int ii=0;
   const int i=0; //i 是常量，i 的值不会被修改
   const int _p1i=&i; //指针 p1i 所指内容是常量，可以不初始化
   int _ const p2i=ⅈⅈ //指针 p2i 是常量，所指内容可修改
   const int * const p3i=&i; //指针 p3i 是常量，所指内容也是常量
   p1i= ii //合法
   *p2i=100; //合法
   ```

---

关于 C++中的 const 关键字的用法非常灵活，而使用 const 将大大改善程序的健壮性，参考了康建东兄的 const 使用详解一文，对其中进行了一些补充，写下了本文。

1. ```
   const常量，如const int max = 100;
   ```

   优点：const 常量有数据类型，而宏常量没有数据类型。编译器可以对前者进行类型安全检查，而对后者只进行字符替换，没有类型安全检查，并且在字符替换时可能会产生意料不到的错误（边际效应）

2. const 修饰类的数据成员。如：
   ```c++
   class A
   {
   const int size;
    }
   ```
   const 数据成员只在某个对象生存期内是常量，而对于整个类而言却是可变的。因为类可以创建多个对象，不同的对象其 const 数据成员的值可以不同。所以不能在类声明中初始化 const 数据成员，因为类的对象未被创建时，编译器不知道 const 数据成员的值是什么。如

```c++
class A
{
const int size = 100; //错误
int array[size]; //错误，未知的 size
}
```

const 数据成员的初始化只能在类的构造函数的初始化表中进行。要想建立在整个类中都恒定的常量，应该用类中的枚举常量来实现。如

```c++
class A
{…
enum {size1=100, size2 = 200 };
int array1[size1];
int array2[size2];
}
```

枚举常量不会占用对象的存储空间，他们在编译时被全部求值。但是枚举常量的隐含数据类型是整数，其最大值有限，且不能表示浮点数。

3. const 修饰指针的情况，见下式：

```c++
int b = 500;
const int* a = & [1]
int const*a = & [2]
int* const a = & [3]
const int* const a = & [4]
```

如果你能区分出上述四种情况，那么，恭喜你，你已经迈出了可喜的一步。不知道，也没关系，我们可以参考《Effective c++》Item21 上的做法，如果 const 位于星号的左侧，则 const 就是用来修饰指针所指向的变量，即指针指向为常量；如果 const 位于星号的右侧，const 就是修饰指针本身，即指针本身是常量。因此，[1]和[2]的情况相同，都是指针所指向的内容为常量（const 放在变量声明符的位置无关），这种情况下不允许对内容进行更改操作，如不能\*a = 3 ；[3]为指针本身是常量，而指针所指向的内容不是常量，这种情况下不能对指针本身进行更改操作，如 a++是错误的；[4]为指针本身和指向的内容均为常量。

### 4. const 的初始化

先看一下 const 变量初始化的情况

1. 非指针 const 常量初始化的情况：A b;
   const A a = b;
2. 指针 const 常量初始化的情况：
   A* d = new A();
   const A* c = d;
   或者：const A\* c = new A();
   3）引用 const 常量初始化的情况：
   A f;
   const A& e = f; // 这样作 e 只能访问声明为 const 的函数，而不能访问一

般的成员函数；

```c++
[思考1]： 以下的这种赋值方法正确吗？
const A* c=new A();
A* e = c;
[思考2]： 以下的这种赋值方法正确吗？
A* const c = new A();
A* b = c;
```

5. 另外 const 的一些强大的功能在于它在函数声明中的应用。在一个函数声明中，const 可以修饰函数的返回值，或某个参数；对于成员函数，还可以修饰是整个函数。有如下几种情况，以下会逐渐的说明用法：
   ```c++
   A& operator=(const A& a);
   void fun0(const A\* a );
   void fun1( ) const; // fun1( ) 为类成员函数
   const A fun2( );
   ```

1） 修饰参数的 const，如 void fun0(const A* a ); void fun1(const A& a);
调用函数的时候，用相应的变量初始化 const 常量，则在函数体中，按照 const 所修饰的部分进行常量化，如形参为 const A* a，则不能对传递进来的指针的内容进行改变，保护了原指针所指向的内容；如形参为 const A& a，则不能对传递进来的引用对象进行改变，保护了原对象的属性。
[注意]：参数 const 通常用于参数为指针或引用的情况，且只能修饰输入参数;若输入参数采用“值传递”方式，由于函数将自动产生临时变量用于复制该参数，该参数本就不需要保护，所以不用 const 修饰。

[总结]对于非内部数据类型的输入参数，因该将“值传递”的方式改为“const 引用传递”，目的是为了提高效率。例如，将 void Func(A a)改为 void Func(const A &a)
对于内部数据类型的输入参数，不要将“值传递”的方式改为“const 引用传递”。否则既达不到提高效率的目的，又降低了函数的可理解性。例如 void Func(int x)不应该改为 void Func(const int &x)

2） 修饰返回值的 const，如 const A fun2( ); const A* fun3( );
这样声明了返回值后，const 按照"修饰原则"进行修饰，起到相应的保护作用。const Rational operator*(const Rational& lhs, const Rational& rhs)
{
return Rational(lhs.numerator() _ rhs.numerator(),
lhs.denominator() _ rhs.denominator());
}

返回值用 const 修饰可以防止允许这样的操作发生:Rational a,b;
Radional c;
(a\*b) = c;

一般用 const 修饰返回值为对象本身（非引用和指针）的情况多用于二目操作符重载函数并产生新对象的时候。 [总结]

1. 一般情况下，函数的返回值为某个对象时，如果将其声明为 const 时，多用于操作符的重载。通常，不建议用 const 修饰函数的返回值类型为某个对象或对某个对象引用的情况。原因如下：如果返回值为某个对象为 const（const A test = A 实例）或某个对象的引用为 const（const A& test = A 实例） ，则返回值具有 const 属性，则返回实例只能访问类 A 中的公有（保护）数据成员和 const 成员函数，并且不允许对其进行赋值操作，这在一般情况下很少用到。
2. 如果给采用“指针传递”方式的函数返回值加 const 修饰，那么函数返回值（即指针）的内容不能被修改，该返回值只能被赋给加 const 修饰的同类型指针。如：
   const char \* GetString(void);

如下语句将出现编译错误：

char \*str=GetString();

正确的用法是：

const char \*str=GetString();

3. 函数返回值采用“引用传递”的场合不多，这种方式一般只出现在类的赙值函数中，目的是为了实现链式表达。如：

```c++
class A
{…
A &operate = (const A &other); //负值函数
}
A a,b,c; //a,b,c 为 A 的对象
…
a=b=c; //正常
(a=b)=c; //不正常，但是合法若负值函数的返回值加 const 修饰，
```

那么该返回值的内容不允许修改，上例中 a=b=c 依然正确。(a=b)=c 就不正确了。
[思考 3]： 这样定义赋值操作符重载函数可以吗？
const A& operator=(const A& a);

6. ```
   类成员函数中const的使用
   ```

   一般放在函数体后，形如：void fun() const;
   任何不会修改数据成员的函数都因该声明为 const 类型。如果在编写 const 成员函数时，不慎修改了数据成员，或者调用了其他非 const 成员函数，编译器将报错，这大大提高了程序的健壮性。如：

```c++
class Stack
{
public:
void Push(int elem);
int Pop(void);
int GetCount(void) const;   //const 成员函数
private:
int m_num;
int m_data[100];
};

int Stack::GetCount(void) const

{
++m_num; //编译错误，企图修改数据成员 m_num
Pop(); //编译错误，企图调用非 const 函数
Return m_num;
}
```

7.  使用 const 的一些建议

1 要大胆的使用 const，这将给你带来无尽的益处，但前提是你必须搞清楚原委；
2 要避免最一般的赋值操作错误，如将 const 变量赋值，具体可见思考题；
3 在参数中使用 const 应该使用引用或指针，而不是一般的对象实例，原因同上；
4 const 在成员函数中的三种用法（参数、返回值、函数）要很好的使用；
5 不要轻易的将函数的返回值类型定为 const;
6 除了重载操作符外一般不要将返回值类型定为对某个对象的 const 引用;

[思考题答案]
1 这种方法不正确，因为声明指针的目的是为了对其指向的内容进行改变，而声明的指针 e 指向的是一个常量，所以不正确；
2 这种方法正确，因为声明指针所指向的内容可变；
3 这种做法不正确；
在 const A::operator=(const A& a)中，参数列表中的 const 的用法正确，而当这样连续赋值的时侯，问题就出现了：
A a,b,c:
(a=b)=c;
因为 a.operator=(b)的返回值是对 a 的 const 引用，不能再将 c 赋值给 const 常量。

C 函数形参里面为何要用:const char \*inputString
保护实参指针指向的数据不被意外改写。

```c++
const char *inputString;//定义指向常量的指针
```

指针本身地址值可以增减，也可以给指针更换指向的常量，但是指向的内容有常量性质，指向的内容不能被更改。即：

```c++
inputString++;//这是可以的。
(\*inputString)++;//这是禁止的。
```

强制类型转换的实质是告诉编译器,"这可行, 这行的通... 至于程序会不会出错, 编译器则不管, 因为你都说可行了."

两个操作数都是指向有限定符或无限定符的相容类型的指针，左边指针所指向的类型必须具有右边指针所指向类型的全部限定符。

正是这个条件，使得函数调用中实参 char *能够与形参 const char *匹配（在 C 标准库中，所有的字符串处理函数就是这样的）。

函数那部分的时候，有一个很重要的概念是区别：值传递、指针传递、引用传值(好像是这三种说法)。

我觉得要理解这部分知识点，首先应该知道不同种类的变量在内存中是如何分配存储的，它们的生命周期多长等这些问题，然后在理解哪三种情况就好理解了。函数的参数一般都

是在 stack 栈上分配的，所以它的生命周期就在它所属的函数内，函数执行完毕的时候，它的内存将被回收。

如果我们想在函数内对实际参数进行操作(不是对形式参数的副本)的话，一般会使用引用，即声明函数的形式参数为引用类型，比如 char _ fun(char _ &p)，这样实参和形参为同一个变量，我们在函数中操作形参 p 就等于直接在操作实参变量。我看 C++语法书的时候，书上说这样用还有一个好处是，在调用函数的时候，不用再为形式参数分配内存了，这样执行效率会高一点儿。

下面是函数形参为指针的几种情况：

```c++
#include <iostream>
using namespace std;

char* func1(char *p);
void func2(char _p);
void func3(char _ &p);

char s1[]="原来的";
char s2[]="指向我了吗";

int main()
{
char \*ptr=s1;
cout<<ptr<<endl;
ptr=func1(ptr); //返回值改变 ptr 使它指向另一个地址
//func2(ptr); //ptr 的指向没有改变，func2 函数中改变的只是它的副本(一个局部变量)
//func3(ptr); //改变了 ptr 的指向，func3 函数的形式参数为引用类型，实参和形参是同一个变量
cout<<ptr<<endl;
return 0;
}

char* func1(char *p)
{
p=s2;
return p;
}
void func2(char _p)
{
p=s2;
}
void func3(char _ &p)
{
p=s2;
}
```
