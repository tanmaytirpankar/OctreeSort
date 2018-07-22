#include <iostream>
#include "RadixSort.h"
#include <fstream>
#include <string>
#include <ctime>
#include <cmath>
#include <chrono>
using namespace std;
int main() {
    unsigned int n;
    int num,buck,user_word_size;
    cout<<"Enter the number of points: ";
    cin>>n;
    cout<<"Number of points "<<n<<endl;
    cout<<"Enter the number of threads: ";
    cin>>num;
    cout << "Enter the word size (preferably 1 to 5): ";
    cin >> user_word_size;
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