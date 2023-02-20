#include "addressbook.pb.h"
#include <iostream>
#include <string>
#include <fstream>
#include <assert.h>
using namespace std;
using namespace daniel::tutorial;

void createAddressBook(AddressBook& book)
{
	Person* p1 = book.add_person();
	// 自己定义的复杂嵌套消息不能够通过简单的set_来赋值，可采取set_allocated和mutable_两种方式
	BasicInfo* pBasic = p1->mutable_basicinfo();
	pBasic->set_name("daniel");
	pBasic->set_id(151040023);
	pBasic->set_email("daniel@163.com");

	Person_PhoneNumber* phone1 = p1->add_phone();
	phone1->set_type(Person_PhoneType::Person_PhoneType_MOBILE);
	phone1->set_number("17654387654");

	Person_PhoneNumber* phone2 = p1->add_phone();
	phone2->set_type(Person_PhoneType::Person_PhoneType_HOME);
	phone2->set_number("0512-87655432");

	// 调试打印;
	cout << book.DebugString() << endl;
}

void createAddressBook1(AddressBook& book)
{
	Person* p1 = book.add_person();
	BasicInfo* pBasic = p1->mutable_basicinfo();
	pBasic->set_name("zpp");
	pBasic->set_id(11041531);
	pBasic->set_email("zpp@163.com");

	Person_PhoneNumber phone1;
	phone1.set_number("17654387675");
	phone1.set_type(Person_PhoneType::Person_PhoneType_MOBILE);

	Person_PhoneNumber phone2;
	phone2.set_number("0513-87655431");
	phone2.set_type(Person_PhoneType::Person_PhoneType_HOME);

	vector<Person_PhoneNumber> phones;
	phones.push_back(phone1);
	phones.push_back(phone2);
	p1->mutable_phone()->CopyFrom({ phones.begin(), phones.end() });

	// 调试打印;
	cout << book.DebugString() << endl;
}


void fileCodec(const AddressBook& book)
{
	cout << "================fileCodec================" << endl;
	// 序列化到文件
	fstream output("addressBook", ios::out | ios::trunc | ios::binary);
	book.SerializePartialToOstream(&output);
	output.close();

	// 从文件读取二进制流,反序列化
	daniel::tutorial::AddressBook  book1;
	fstream input("addressBook", ios::in | ios::binary);
	book1.ParseFromIstream(&input);
	for (int i = 0; i < book1.person_size(); ++i)
	{
		const Person& person = book1.person(i);
		assert(person.has_basicinfo() == true);
		const BasicInfo& basicInfo = person.basicinfo();
		cout << "id " << basicInfo.id() << endl;
		cout << "email " << basicInfo.email() << endl;
		cout << "name " << basicInfo.name() << endl;
		for (int j = 0; j < person.phone_size(); ++j)
		{
			const Person_PhoneNumber& phone = person.phone(j);
			cout << "type " << phone.type() << " number " << phone.number() << endl;
		}
		cout << endl;
	}
}

void stringCodec(const AddressBook& book)
{
	cout << "================stringCodec================" << endl;
	// 序列化到string,这里string当作一个容器来使用,存储二进制字节流
	string data;
	book.SerializeToString(&data);

	// 从string读取二进制流,反序列化
	daniel::tutorial::AddressBook  book1;
	book1.ParseFromString(data);
	for (int i = 0; i < book1.person_size(); ++i)
	{
		const Person& person = book1.person(i);
		assert(person.has_basicinfo() == true);
		const BasicInfo& basicInfo = person.basicinfo();
		cout << "id " << basicInfo.id() << endl;
		cout << "email " << basicInfo.email() << endl;
		cout << "name " << basicInfo.name() << endl;
		for (int j = 0; j < person.phone_size(); ++j)
		{
			const Person_PhoneNumber& phone = person.phone(j);
			cout << "type " << phone.type() << " number " << phone.number() << endl;
		}
		cout << endl;
	}
}

int main()
{
	daniel::tutorial::AddressBook  book;
	createAddressBook(book);
	fileCodec(book);
	stringCodec(book);

	book.Clear();
	createAddressBook1(book);
	fileCodec(book);
	stringCodec(book);
	return 0;
}