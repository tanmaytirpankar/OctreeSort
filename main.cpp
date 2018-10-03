#include <iostream>
#include "RadixSort.h"
#include <fstream>
#include <string>
#include <ctime>
#include <cmath>
#include <chrono>
#include <getopt.h>
using namespace std;
int main(int argc, char **argv) {
    unsigned int n;
    int num,user_word_size,max_levels;
    int c;
    //Loop used to assign parameters based on parameter name. Used for python scripts.
    while ((c = getopt (argc, argv, "p:t:w:l:")) != -1)
        switch (c)
        {
            case 'p':
                n = atoi(optarg);
                break;
            case 't':
                num = atoi(optarg);
                break;
            case 'w':
                user_word_size = atoi(optarg);
                break;
            case 'l':
                max_levels = atoi(optarg);
                break;
            default:
                abort ();
        }
    cout<<"Number of points "<< n << " Number of threads " <<num<< " Word size " <<user_word_size<< " Levels "<<max_levels<<endl;

    omp_set_max_active_levels(max_levels);


//    cout<<omp_get_thread_limit()<<endl;
    //cout << omp_get_max_threads();
    omp_set_nested(1);
    RadixSort<unsigned int> *obj=new RadixSort<unsigned int>(n, num, user_word_size);
//    obj->print();
//    cout<<endl;
//    obj->print1();
//    cout<<endl;
    chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
    obj->sorting();
//    obj->print();
//    cout<<endl;
//    obj->print1();
    chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>( t2 - t1 ).count();
    cout <<endl <<"The time taken is "<<duration <<" microseconds"<<endl;
    return 0;
}