// 1115201900266
// PANAGIOTIS KONTOEIDIS

#include <iostream>
#include "scheduler.hpp"

using namespace std;

int n, sn, k1, la, ll, lb;

int main()
{

    cout << "how many processes? " << endl;
    cin >> n;

    cout << "how many semaphores?" << endl;
    cin >> sn;

    cout << "enter a value between 0-100 for the propability" << endl;
    cin >> k1;

    cout << "enter lamda for arrival of processes" << endl;
    cin >> la;

    cout << "enter lamda for lifetime of processes" << endl;
    cin >> ll;

    cout << "enter lamda for critical section of processes" << endl;
    cin >> lb;

    Preemptive_Priority(n, sn, k1, la, lb, ll);
    init();
    Priority_Inheritance(n, sn, k1, la, lb, ll);

    cout << "+-----------------------------+" << endl;
    cout << "------Check .txt files --------" << endl;
    cout << "+-----------------------------+" << endl;

    return 0;
}
