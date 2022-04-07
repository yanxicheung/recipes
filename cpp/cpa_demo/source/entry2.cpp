#include "OSS.h"
#include <iostream>
#include "movie.pb.h"
using namespace std;
using namespace movie;

namespace
{
    void parseMoveInfoRequest(const string& str)
    {
        Message msg;
        msg.ParseFromString(str);
        MovieInfoRequest req = msg.movie_info_request();
        cout << req.name() << endl;
    }
}

void DemoEntry2(int state, int eventid, void *msg, int msgLen, void* data)
{
    if (eventid == EV_STARTUP)
    {
        cout << "DemoEntry2 Power on" << endl;
    }
    else if (eventid == Message::kMovieInfoRequest)
    {
        cout << "DemoEntry2 recv MoveInfoRequest" << endl;
        string str((const char*) msg);
        parseMoveInfoRequest(str);

        OSS_Send("entry1", 100, nullptr, 0);
    }
    else
    {
        cout << "DemoEntry2 recv unknow event " << eventid << endl;
    }
}
