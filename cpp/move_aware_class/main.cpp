#include <iostream>
#include <string>
#include <exception>
using namespace std;


#ifndef _MSC_VER
#define NOEXCEPT noexcept
#else
#define NOEXCEPT
#endif

// move aware class

class MyString
{
public:
	static size_t DCtor;
	static size_t Ctor;
	static size_t CCtor;
	static size_t CAsgn;
	static size_t MCtor;
	static size_t MAsgn;
	static size_t Dtor;
private:
	char * _data;
	size_t _len;
	void _init_data(const char* s)
	{
		_data = new char[_len + 1];
		memcpy(_data, s, _len);
		_data[_len] = '\0';
	}
public:
	MyString() :_data(NULL), _len(0)
	{
		++DCtor;
	}

	MyString(const char*p) :_len(strlen(p))
	{
		++Ctor;
		_init_data(p);
	}

	MyString(const MyString& str) :_len(str._len)
	{
		++CCtor;
		_init_data(str._data);
	}

	MyString(MyString&& str) NOEXCEPT:_data(str._data), _len(str._len){
		++MCtor;
		str._data = NULL; // 重要
		str._len = 0;
	}

	MyString& operator=(const MyString& str)
	{
		++CAsgn;
		if (this != &str)
		{
			if (_data) delete []_data;
			_len = str._len;
			_init_data(str._data); // copy
		}
		return *this;
	}

	MyString& operator=(MyString&& str) NOEXCEPT{
		++MAsgn;
		if (this != &str)
		{
			if (_data) delete[]_data;
			_len = str._len;
			str._len = 0;
			_data = str._data;
			str._data = NULL;  // 确保右值析构时内存不会被释放掉;
		}
		return *this;
	}

	virtual ~MyString()
	{
		++DCtor;
		if (_data)delete[]_data;
	}

	// for set
	bool operator<(const MyString& rhs) const
	{
		return string(this->_data) < string(rhs._data);
	}

	bool operator==(const MyString& rhs) const
	{
		return string(this->_data) == string(rhs._data);
	}

	char * get() const
	{
		return _data;
	}
};

size_t MyString::DCtor = 0;
size_t MyString::Ctor = 0;
size_t MyString::CCtor = 0;
size_t MyString::CAsgn = 0;
size_t MyString::MCtor = 0;
size_t MyString::MAsgn = 0;
size_t MyString::Dtor = 0;

void process(int &i)
{
	cout << "void process(int &i)" << endl;
}

void process(int &&i)
{
	cout << "void process(int &&i)" << endl;
}

void unperfect_forward(int &&i)
{
	cout << "void unperfect_forward(int &&i)" << endl;
	process(i);  // unperfect forward
}

void perfect_forward(int &&i)
{
	cout << "void perfect_forward(int &&i)" << endl;
	process(forward<int>(i));  // perfect forward
}


void test()
{
	int a = 0;
	process(a);
	process(1);
	process(move(a));

	unperfect_forward(2);
	unperfect_forward(move(a));

	perfect_forward(2);
}

int main(int argc, char** argv)
{
	test();
	getchar();
	return 0;
}