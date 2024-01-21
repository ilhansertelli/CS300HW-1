#include <time.h>
#include <stdlib.h>
#include "RandGen.h"
#include <cmath>

#include <iostream>
using namespace std;

int RandGen::ourInitialized = 0;

void RandGen::SetSeed(int seed)
{
    if (0 == ourInitialized)
    {   ourInitialized = 1;
        srand(seed);
    }
}


RandGen::RandGen()
{
    if (0 == ourInitialized)
    {   ourInitialized = 1;


        time_t now;
        time (&now);
        srand(int((sin(double(now))*1000000)));//G�khan

    }
}

int RandGen::RandInt(int max)
{
    return int(RandReal() * max);
}

int RandGen::RandInt(int low, int max)
{
    return low + RandInt(max-low+1);
}

double RandGen::RandReal()
{
    return rand() / (double(RAND_MAX) + 1);
}

double RandGen::RandReal(double low, double high)
{
    double width = fabs(high-low);
    double thelow = low < high ? low : high;
    return RandReal()*width + thelow;
}