//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// application entry point
#include "GDProto.pch.h"
#include "Application.h"

using namespace std;

int main(int argc, char* argv[])
{
    try
    {
        Application::instance().run();
    }
    catch(exception& e)
    {
        cerr << "Exception: " << e.what() << endl;
        return -1;
    }

    return 0;
}
