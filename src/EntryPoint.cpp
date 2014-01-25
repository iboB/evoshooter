//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// application entry point
#include "EvoShooter.pch.h"
#include "Application.h"
#include "MonsterDNA.h"

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

    //MonsterDNA d[4];
    //d[0].randomize();
    //
    //d[1] = d[0];
    //for (int i = 0; i < 10; ++i)
    //d[1].mutate();

    //d[2] = d[1];
    //for (int i = 0; i < 10; ++i)
    //d[2].mutate();

    //d[3] = d[2];
    //for (int i = 0; i < 10; ++i)
    //d[3].mutate();


    ////srand(10);
    //for (int i = 0; i < MonsterDNA::Num_Genes; ++i)
    //{
    //    for (auto& dd : d)
    //    {
    //        cout << dd.m[i].value() << "\t";
    //    }

    //    cout << endl;
    //}

    return 0;
}
