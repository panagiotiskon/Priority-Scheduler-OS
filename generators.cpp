#include "generators.hpp"
#include "process.hpp"

using namespace std;

struct ComparePriority
{
    bool operator()(Process *p1, Process *p2)
    {
        return p2->get_priority() < p1->get_priority(); // return "true" if "p1" is ordered before "p2"
    }
};

int *exponential_generator(float median, int num)
{ // use exponential distribution
    float *arr = new float[num];
    int *res = new int[num];
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    exponential_distribution<double> distribution(median);
    for (int i = 0; i < num; i++)
    {
        double number = distribution(generator);
        arr[i] = round(number * 100.0) / 100.0; // 2 decimal places
        res[i] = arr[i] * 10;
        if (res[i] == 0)
            res[i] += 1;
    }
    return res;
}

int *uniform_generator(int num)
{ // use uniform distribution
    int *arr = new int[num];
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_int_distribution<int> distribution(1, 7);
    for (int i = 0; i < num; i++)
    {
        arr[i] = distribution(generator);
    }
    return arr;
}

int random_generator(int max)
{ // returns random number from[0, max-1]

    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_int_distribution<int> distribution(0, max - 1);
    return distribution(generator);
}

bool propability(int k)
{
    return (rand() % 100) < k;
}