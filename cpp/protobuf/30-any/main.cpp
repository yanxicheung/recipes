#include <iostream>
#include "com.pb.h"
#include "msg.pb.h"
using namespace std;

void encode(string& data)
{
	cout << "=========any encode=========" << endl;
	com::TestMsg  testMsg;
	com::Head* pHead = testMsg.mutable_head();
	pHead->set_uri("/usr/system/basicInfo");
	pHead->set_token("353738389494");
	pHead->set_err("Get logdir failed");
	pHead->set_len(4556);
	pHead->set_serial(12345);
	pHead->set_context(788999);

	msg::BasicInfo info;
	info.set_id(1234);
	info.set_name("daniel");
	info.set_email("daniel@163.com");

	google::protobuf::Any* pBody = testMsg.mutable_body();
	pBody->PackFrom(info);

	testMsg.SerializeToString(&data);
	cout << "ByteSize===>" << testMsg.ByteSize() << endl;
	cout << "data.size()===>" << data.size() << endl;
}

void decodeFromString(const string& data)
{
	cout << "=========ParseFromString decode=========" << endl;
	com::TestMsg testMsg;
	testMsg.ParseFromString(data);
	const com::Head& head = testMsg.head();
	cout << "url===> " << head.uri() << endl;
	cout << "token===> " << head.token() << endl;
	cout << "err===> " << head.err() << endl;
	cout << "len===> " << head.len() << endl;
	cout << "serial===> " << head.serial() << endl;

	auto& body = testMsg.body();

	if (!body.Is<msg::BoardInfo>())
	{
		cout << "body is not msg::BoardInfo" << endl;
	}

	if (body.Is<msg::BasicInfo>())
	{
		msg::BasicInfo info;
		cout << "body is msg::BasicInfo" << endl;
		body.UnpackTo(&info);
		cout << info.name() << endl;
		cout << info.id() << endl;
		cout << info.email() << endl;
	}
}


void decodeFromBuffer(const void* data, int size)
{
	cout << "=========decodeFromBuffer decode=========" << endl;
	com::TestMsg testMsg;
	testMsg.ParseFromArray(data,size);
	const com::Head& head = testMsg.head();
	cout << "url===> " << head.uri() << endl;
	cout << "token===> " << head.token() << endl;
	cout << "err===> " << head.err() << endl;
	cout << "len===> " << head.len() << endl;
	cout << "serial===> " << head.serial() << endl;

	auto& body = testMsg.body();

	if (!body.Is<msg::BoardInfo>())
	{
		cout << "body is not msg::BoardInfo" << endl;
	}

	if (body.Is<msg::BasicInfo>())
	{
		msg::BasicInfo info;
		cout << "body is msg::BasicInfo" << endl;
		body.UnpackTo(&info);
		cout << info.name() << endl;
		cout << info.id() << endl;
		cout << info.email() << endl;
	}
}

int main()
{
	string data;
	encode(data);
	decodeFromString(data);
	decodeFromBuffer(data.c_str(), data.size());
	return 0;
}