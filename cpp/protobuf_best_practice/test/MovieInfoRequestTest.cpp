#include "gtest/gtest.h"
#include "movie.pb.h"
#include <string>
#include <iostream>
using namespace std;
using namespace movie;

struct MovieInfoRequestTest:testing::Test
{
protected:
    void serialize(string &str);
    void deserialize(const string &str);
protected:
    string str;
};

void MovieInfoRequestTest::serialize(string &str)
{
    Message msg;
    MovieInfoRequest * req = msg.mutable_movie_info_request();

    req->set_name("Judy");
    ASSERT_TRUE(req->DebugString() == R"(name: "Judy"
)");

    ASSERT_TRUE(msg.Messages_case() == Message::kMovieInfoRequest);

    ASSERT_TRUE(msg.DebugString() == R"(movie_info_request {
  name: "Judy"
}
)");

    msg.SerializeToString(&str);
    ASSERT_TRUE(str == msg.SerializeAsString());
}

void MovieInfoRequestTest::deserialize(const string &str)
{
    Message msg;
    msg.ParseFromString(str);
    ASSERT_TRUE(msg.DebugString() == R"(movie_info_request {
  name: "Judy"
}
)");
    MovieInfoRequest req = msg.movie_info_request();
    ASSERT_TRUE(req.name() == "Judy");
}

TEST_F(MovieInfoRequestTest,movie_info_request_test)
{
    serialize(str);
    deserialize(str);
}
