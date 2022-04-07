#include "gtest/gtest.h"
#include "movie.pb.h"
#include <string>
#include <iostream>
#include "base/BaseTypes.h"
using namespace std;
using namespace movie;

struct MovieInfoResponseTest:testing::Test
{
protected:
    void serialize(string &str);
    void deserialize(const string &str);
    void fillBaseInfo(BasicInfo * basicInfo);
    void fillScore(Score *score,WORD32 value,const string&comment);
protected:
    string str;
};

void MovieInfoResponseTest::fillBaseInfo(BasicInfo* basicInfo)
{
    basicInfo->set_movie_type(MovieType::science);
    basicInfo->add_directors("Daniel");
    basicInfo->add_directors("Jack");
}

void MovieInfoResponseTest::fillScore(Score* score, WORD32 value,const string& comment)
{
    score->set_value(value);
    score->set_comment(comment);
}

TEST_F(MovieInfoResponseTest,movie_info_response_test)
{
    serialize(str);
    deserialize(str);
}

void MovieInfoResponseTest::serialize(string& str)
{
    Message msg;

    auto rsp = msg.mutable_movie_info_response();
    rsp->set_name("Judy");
    fillBaseInfo(rsp->mutable_basic_info());
    fillScore(rsp->add_score_items(),100,"Good");
    fillScore(rsp->add_score_items(),99,"Perfect");

    msg.SerializeToString(&str);
    ASSERT_TRUE(str == msg.SerializeAsString());
    cout<<msg.DebugString()<<endl;
}

void MovieInfoResponseTest::deserialize(const string& str)
{
    Message msg;
    msg.ParseFromString(str);
    ASSERT_TRUE(msg.has_movie_info_response() == true);

    auto rsp = msg.mutable_movie_info_response();
    ASSERT_TRUE(rsp->has_basic_info() == true);
    ASSERT_TRUE(rsp->basic_info().movie_type() == MovieType::science);
    ASSERT_TRUE(rsp->basic_info().directors_size() == 2);
    ASSERT_TRUE(rsp->basic_info().directors(0) == "Daniel");
    ASSERT_TRUE(rsp->basic_info().directors(1) == "Jack");

    ASSERT_TRUE(rsp->score_items_size() == 2);
    ASSERT_TRUE(rsp->score_items(0).comment() == "Good");
    ASSERT_TRUE(rsp->score_items(1).value() == 99);
    ASSERT_TRUE(msg.Messages_case() == movie::Message::kMovieInfoResponse);
}


