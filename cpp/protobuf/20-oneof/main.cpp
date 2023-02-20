#include <iostream>
#include "movie.pb.h"
#include <assert.h>
using namespace std;
using namespace movie;

void createMovieInfoReq(HelloMessages& msg)
{
	MovieInfoRequest* req = msg.mutable_movie_info_request();
	req->set_name("daniel");
}

void movieInfoReqStringCodec(const HelloMessages& msg)
{
	string data;
	msg.SerializeToString(&data);

	HelloMessages msg1;
	msg1.ParseFromString(data);
	cout << msg1.DebugString() << endl;
	assert(msg1.WorldMessage_case() == HelloMessages::kMovieInfoRequest);
	assert(msg1.has_movie_info_request() == true);
	auto& req = msg1.movie_info_request();
	cout<<"name===> "<<req.name()<<endl;
}

///////////////////////////////////////////////////////
void fillBasicInfo(BasicInfo* basicInfo)
{
	basicInfo->set_movie_type(MovieType::science);
	basicInfo->add_directors("Daniel");
	basicInfo->add_directors("Jack");
}

void fillScore(Score* score, int value, const string& comment)
{
	score->set_value(value);
	score->set_comment(comment);
}

void createMovieInfoRsp(HelloMessages& msg)
{
	MovieInfoResponse* rsp = msg.mutable_movie_info_response();
	rsp->set_name("daniel");
	fillBasicInfo(rsp->mutable_basic_info());
	fillScore(rsp->add_score_items(), 100, "Good");
	fillScore(rsp->add_score_items(), 99, "Perfect");
}

void movieInfoRspStringCodec(const HelloMessages& msg)
{
	string data;
	msg.SerializeToString(&data);

	HelloMessages msg1;
	msg1.ParseFromString(data);
	cout << msg1.DebugString() << endl;
	assert(msg1.WorldMessage_case() == HelloMessages::kMovieInfoResponse);
	assert(msg1.has_movie_info_response() == true);
	auto& rsp = msg1.movie_info_response();

	assert(rsp.has_basic_info() == true);
	assert(rsp.basic_info().movie_type() == MovieType::science);
	assert(rsp.basic_info().directors_size() == 2);
	assert(rsp.basic_info().directors(0) == "Daniel");
	assert(rsp.basic_info().directors(1) == "Jack");
	assert(rsp.score_items_size() == 2);
	assert(rsp.score_items(0).comment() == "Good");
	assert(rsp.score_items(1).value() == 99);
}


int main()
{
	HelloMessages msg;
	createMovieInfoReq(msg);
	movieInfoReqStringCodec(msg);

	HelloMessages msg1;
	createMovieInfoRsp(msg1);
	movieInfoRspStringCodec(msg1);
	return 0;
}