         
#include <stdio.h>
#include <string.h>

#include "hdsfuseoper.h"
#include "fuseentity.h"

using namespace std;

int main(int argc, char *argv[])
{
    //FuseOper *p = new HdsFuseOper{};

    FuseEntity fuse(new HdsFuseOper{"/share/"});

    fuse.fuseMain(argc, argv);


    return 0;
}
