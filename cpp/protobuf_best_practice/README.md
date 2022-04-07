# protocol buffer

本文仅介绍`C++`下使用`protobuf`的方法，基于`proto3`。



## 安装：

介绍安装`protocol buffer`的编译器:`protoc`。

（1）下载 `protobuf`安装包:

```bash
git clone https://github.com/protocolbuffers/protobuf.git
```

（2）安装依赖:

```bash
sudo apt-get install autoconf automake libtool curl make g++ unzip libffi-dev -y
```

（3）安装`protoc`:

```bash
cd protobuf/
./autogen.sh
./configure
make
sudo make install
sudo ldconfig  // 刷新共享库，很重要的一步
```


（4）测试一下是否安装成功 :

```bash
 protoc --version
```



## 编译：

介绍基于`.proto`文件运行`protocol buffer`编译器`protoc`，生成对应的源文件，运行的命令如下所示：

```bash
protoc --proto_path=IMPORT_PATH --cpp_out=DST_DIR --java_out=DST_DIR --python_out=DST_DIR path/to/file.proto
```

`IMPORT_PATH`声明了一个`.proto`文件所在的具体目录。如果忽略该值，则使用当前目录。如果有多个目录则可以 对`--proto_path` 写多次，它们将会顺序的被访问并执行导入。`-I=IMPORT_PATH`是它的简化形式。



举个例子,文件名 `addressbook.proto`：

```protobuf
syntax = "proto3";
package tutorial;

message Person {
    string name = 1;
    int32 id = 2;
    string email = 3;

    enum PhoneType {
        MOBILE = 0;
        HOME = 1;
        WORK = 2;
    }

    message PhoneNumber {
        string number = 1;
        PhoneType type = 2;
    }

    repeated PhoneNumber phone = 4;
}

message AddressBook {
    repeated Person person = 1;
}
```

`proto`文件所在当前目录下，输入：

```
protoc --cpp_out=. addressbook.proto
```

得到头文件和源文件：

![demo](./pic/demo.png)



## 语法:



### 关键字：

oneof

optional

默认是optional

对于普通成员变量（required和optional）提供`has`_方法判断变量值是否被设置；提供clear_方法清除设置的变量值。 

对于repeated变量，提供了其它一些特殊的方法：

- _size方法：返回repeated field’s
- 通过下脚标访问其中的数组成员组
- 通过下脚标返回其中的成员的mutable_的方法
- add_方法：增加一个成员。

如果proto结构体的变量是基础变量，比如`int`、`string`、`枚举`等等，那么set的时候直接调用`set_xxx`即可。

如果变量是**自定义类型(message)**，那么C++的生成代码中，就没有`set_xxx`函数名，取而代之的是三个函数名：

- **set_allocated_xxx()**
- **release_xxx()**
- **mutable_xxx()**

使用`set_allocated_xxx()`来设置变量的时候，变量不能是普通栈内存数据，必须是手动new出来的指针，至于何时delete，就不需要调用者关心了，protobuf内部会自动delete掉通过`set_allocated_xxx()`设置的内存；

`release_xxx()`是用来取消之前由`set_allocated_xxx()`设置的数据，调用`release_xxx()`以后，protobuf内部就不会自动去delete这个数据了；

`mutable_xxx()`是用来返回之前`set_allocated_xxx()`设置的数据的const指针，方便你临时读取这个数据的同时，protobuf内部继续自动管理(delete)该数据。



## 实践：





## refrence:

1. [Protobuf使用手册](https://www.cnblogs.com/langqi250/p/7283702.html )
2. [Protobuf语言指南](https://www.cnblogs.com/dkblog/archive/2012/03/27/2419010.html)
3. [protobuf C++生成代码中使用自己定义类型参数时的set问题](https://www.iteye.com/blog/aigo-2300297 )
4. [C++序列化工具最佳实践](https://www.jianshu.com/p/50065ff82be3)
5. [ProtoBuf3 C++使用篇](https://www.cnblogs.com/DswCnblog/p/6700660.html)
6. [Google protobuf使用技巧和经验](https://www.cnblogs.com/zgwu/p/10403939.html)
7. [protobuf如何用proto文件导出C++代码文件及链接protobuf静态库问题](https://blog.csdn.net/cs_polebear/article/details/44057793)
8. [Protobuf 安装及使用](https://www.jianshu.com/p/00be93ed230c)
9. [protobuf的介绍、安装与使用](https://blog.csdn.net/junaozun/article/details/90675118)