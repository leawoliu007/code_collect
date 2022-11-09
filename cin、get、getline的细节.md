# cin、get、getline 的细节

学 C++的时候，这几个输入函数弄的有点迷糊；这里做个小结，为了自己复习，也希望对后来者能有所帮助，如果有差错的地方还请各位多多指教（本文所有程序均通过 VC 6.0 运行）转载请保留作者信息；
1、cin
1、cin.get()
2、cin.getline()
3、getline()
4、gets()
5、getchar()

**_1、cin>> _**

用法 1：最基本，也是最常用的用法，输入一个数字：

#include <iostream>
using namespace std;
main ()
{
int a,b;
cin>>a>>b;
cout<<a+b<<endl;
}

输入：2[回车]3[回车]
输出：5

用法 2：接受一个字符串，遇“空格”、“TAB”、“回车”都结束

#include <iostream>
using namespace std;
main ()
{
char a[20];
cin>>a;
cout<<a<<endl;
}

输入：jkljkljkl
输出：jkljkljkl

输入：jkljkl jkljkl //遇空格结束
输出：jkljkl

**_2、cin.get()_**

用法 1： cin.get(字符变量名)可以用来接收字符

#include <iostream>
using namespace std;
main ()
{
char ch;
ch=cin.get(); //或者 cin.get(ch);
cout<<ch<<endl;
}

输入：jljkljkl
输出：j

用法 2：cin.get(字符数组名,接收字符数目)用来接收一行字符串,可以接收空格

#include <iostream>
using namespace std;
main ()
{
char a[20];
cin.get(a,20);
cout<<a<<endl;
}

输入：jkl jkl jkl
输出：jkl jkl jkl

输入：abcdeabcdeabcdeabcdeabcde （输入 25 个字符）
输出：abcdeabcdeabcdeabcd （接收 19 个字符+1 个'\0'）

用法 3：cin.get(无参数)没有参数主要是用于舍弃输入流中的不需要的字符,或者舍弃回车,弥补 cin.get(字符数组名,接收字符数目)的不足.

这个我还不知道怎么用，知道的前辈请赐教；

**_3、cin.getline() // 接受一个字符串，可以接收空格并输出_**

#include <iostream>
using namespace std;
main ()
{
char m[20];
cin.getline(m,5);
cout<<m<<endl;
}

输入：jkljkljkl
输出：jklj

接受 5 个字符到 m 中，其中最后一个为'\0'，所以只看到 4 个字符输出；

如果把 5 改成 20：
输入：jkljkljkl
输出：jkljkljkl

输入：jklf fjlsjf fjsdklf
输出：jklf fjlsjf fjsdklf

//延伸：
//cin.getline()实际上有三个参数，cin.getline(接受字符串的看哦那间 m,接受个数 5,结束字符)
//当第三个参数省略时，系统默认为'\0'
//如果将例子中 cin.getline()改为 cin.getline(m,5,'a');当输入 jlkjkljkl 时输出 jklj，输入 jkaljkljkl 时，输出 jk

当用在多维数组中的时候，也可以用 cin.getline(m[i],20)之类的用法：

#include<iostream>
#include<string>
using namespace std;

main ()
{
char m[3][20];
for(int i=0;i<3;i++)
{
cout<<"\n 请输入第"<<i+1<<"个字符串："<<endl;
cin.getline(m[i],20);
}

cout<<endl;
for(int j=0;j<3;j++)
cout<<"输出 m["<<j<<"]的值:"<<m[j]<<endl;

}

请输入第 1 个字符串：
kskr1

请输入第 2 个字符串：
kskr2

请输入第 3 个字符串：
kskr3

输出 m[0]的值:kskr1
输出 m[1]的值:kskr2
输出 m[2]的值:kskr3

**_4、getline() // 接受一个字符串，可以接收空格并输出，需包含“#include<string>”_**

#include<iostream>
#include<string>
using namespace std;
main ()
{
string str;
getline(cin,str);
cout<<str<<endl;
}

输入：jkljkljkl
输出：jkljkljkl

输入：jkl jfksldfj jklsjfl
输出：jkl jfksldfj jklsjfl

和 cin.getline()类似，但是 cin.getline()属于 istream 流，而 getline()属于 string 流，是不一样的两个函数

**_5、gets() // 接受一个字符串，可以接收空格并输出，需包含“#include<string>”_**

#include<iostream>
#include<string>
using namespace std;
main ()
{
char m[20];
gets(m); //不能写成 m=gets();
cout<<m<<endl;
}

输入：jkljkljkl
输出：jkljkljkl

输入：jkl jkl jkl
输出：jkl jkl jkl

类似 cin.getline()里面的一个例子，gets()同样可以用在多维数组里面：

#include<iostream>
#include<string>
using namespace std;

main ()
{
char m[3][20];
for(int i=0;i<3;i++)
{
cout<<"\n 请输入第"<<i+1<<"个字符串："<<endl;
gets(m[i]);
}

cout<<endl;
for(int j=0;j<3;j++)
cout<<"输出 m["<<j<<"]的值:"<<m[j]<<endl;

}

请输入第 1 个字符串：
kskr1

请输入第 2 个字符串：
kskr2

请输入第 3 个字符串：
kskr3

输出 m[0]的值:kskr1
输出 m[1]的值:kskr2
输出 m[2]的值:kskr3

自我感觉 gets()和 cin.getline()的用法很类似，只不过 cin.getline()多一个参数罢了；

这里顺带说明一下，对于本文中的这个 kskr1,kskr2,kskr3 的例子，对于 cin>>也可以适用，原因是这里输入的没有空格，如果输入了空格，比如“ks kr jkl[回车]”那么 cin 就会已经接收到 3 个字符串，“ks,kr,jkl”；再如“kskr 1[回车]kskr 2[回车]”，那么则接收“kskr,1,kskr”；这不是我们所要的结果！而 cin.getline()和 gets()因为可以接收空格，所以不会产生这个错误；

**_6、getchar() //接受一个字符，需包含“#include<string>”_**

#include<iostream>
#include<string>
using namespace std;
main ()
{
char ch;
ch=getchar(); //不能写成 getchar(ch);
cout<<ch<<endl;
}

输入：jkljkljkl
输出：j

//getchar()是 C 语言的函数，C++也可以兼容，但是尽量不用或少用；
