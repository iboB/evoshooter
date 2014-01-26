//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
//This game and all content in this file is licensed under  
//the Attribution-Noncommercial-Share Alike 3.0 version of the Creative Commons License.
//For reference the license is given below and can also be found at http://creativecommons.org/licenses/by-nc-sa/3.0/
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
    //for (int i = 0; i < Num_Genes; ++i)
    //{
    //    for (auto& dd : d)
    //    {
    //        cout << dd.m[i].value() << "\t";
    //    }

    //    cout << endl;
    //}
    //for (auto& dd : d)
    //{
    //    cout << dd.calculateCost() << "\t";
    //}

    return 0;
}
