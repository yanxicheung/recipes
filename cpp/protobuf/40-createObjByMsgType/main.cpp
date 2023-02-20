#include "addressbook.pb.h"
#include <iostream>
#include <string>
#include <fstream>
#include <assert.h>
using namespace std;
using namespace daniel::tutorial;
using namespace google::protobuf;

void testInvariant()
{
	typedef daniel::tutorial::AddressBook T;  // 每个proto msg都继承自 google::protobuf::Message

	std::string type_name = T::descriptor()->full_name();
	cout << type_name << endl;

	const Descriptor* descriptor = DescriptorPool::generated_pool()->FindMessageTypeByName(type_name);
	assert(descriptor == T::descriptor());
	cout << "FindMessageTypeByName() = " << descriptor << endl;
	cout << "T::descriptor()         = " << T::descriptor() << endl;
	cout << endl;

	const Message* prototype = MessageFactory::generated_factory()->GetPrototype(descriptor);
	assert(prototype == &T::default_instance());
	cout << "GetPrototype()        = " << prototype << endl;
	cout << "T::default_instance() = " << &T::default_instance() << endl;
	cout << endl;

	T* new_obj = dynamic_cast<T*>(prototype->New());
	assert(new_obj != NULL);
	assert(new_obj != prototype);
	assert(typeid(*new_obj) == typeid(T::default_instance()));
	cout << "prototype->New() = " << new_obj << endl;
	cout << endl;
	delete new_obj;
}

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

Message* createMessage(const std::string& typeName)
{
	Message* message = NULL;
	const Descriptor* descriptor = DescriptorPool::generated_pool()->FindMessageTypeByName(typeName);
	if (descriptor)
	{
		const Message* prototype = MessageFactory::generated_factory()->GetPrototype(descriptor);
		if (prototype)
		{
			message = prototype->New();
		}
	}
	return message;
}

void createObjByTypeName(const string& name, const void* buffer, int len)
{
	Message* msg = createMessage(name);
	msg->ParseFromArray(buffer, len);
	daniel::tutorial::AddressBook* pBook = dynamic_cast<daniel::tutorial::AddressBook*>(msg);

	for (int i = 0; i < pBook->person_size(); ++i)
	{
		const Person& person = pBook->person(i);
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
	delete msg;
}

int main()
{
	testInvariant();
	daniel::tutorial::AddressBook book;
	createAddressBook(book);
	string typeName = book.GetTypeName();
	cout << "msg type name:====>" << typeName << endl;

	string data;
	book.SerializeToString(&data);
	createObjByTypeName(typeName, data.c_str(), data.size());
	return 0;
}