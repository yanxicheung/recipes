#include "OSS.h"
#include <iostream>
#include <unistd.h>

using namespace std;

extern void DemoEntry1(int state, int eventid, void *msg, int msgLen, void* data);
extern void DemoEntry2(int state, int eventid, void *msg, int msgLen, void* data);

enum ThreadType
{
    E_DemoEntry1,
    E_DemoEntry2
};

const ThreadConfig userConfigs[] =
{
    {E_DemoEntry1, DemoEntry1, 100, "entry1"},
    {E_DemoEntry2, DemoEntry2, 100, "entry2"}
};

int main(int argc, char **argv)
{
    cout<<"Hello cpa demo"<<endl;
    OSS_Init();
    OSS_UserEntryRegist((ThreadConfig*)userConfigs, sizeof(userConfigs)/ sizeof(ThreadConfig));
    while (1)
    {
        sleep(1);
    }
    return 0;
}
