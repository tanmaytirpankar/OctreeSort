//
// Created by tanmay on 24/6/18.
//

#ifndef OCTREESORT_RADIXSORT_H
#define OCTREESORT_RADIXSORT_H
#include "Points.h"
#include <vector>
#include <iostream>
#include <omp.h>
#include <time.h>
#include <fstream>
#include <string>
#include <chrono>
#include <math.h>
#include <algorithm>
using namespace std;
template <class T>
class RadixSort {
    //Primary and Secondary buffers for the list of points.
    vector <Points<T>> arr,arr1;
    //num_threads -> Number of threads to execute each loop with.
    //word_size -> number of bits to use from each co-ordinate of a point for bucketing purpose.
    //actual_word_size -> The number of bits used for bucketing = word_size*3;
    //level -> The max depth that the algorithm can go to after which the bits are to bucket are over and function returns.
    int num_threads, word_size, actual_word_size, level;
    //Number of points to sort.
    unsigned int num_elements;
public:
    RadixSort(unsigned int n, int num, int user_word_size)
    {
        num_elements=n;
        this->word_size=user_word_size;
        actual_word_size=user_word_size*3;
        this->num_threads=num;

        //Section of code if input is to be taken from a .csv file.
        /*ifstream in;
        string line;
        string filename = "exampleOutput5.csv";
        in.open(filename);
        arr=vector<Points<T>>(n);
        for (int i = 0; i < n; i++) {
            getline( in, line,',');
            x=stoi(line);
            getline( in, line,',');
            y=stoi(line);
            getline( in, line,'\n');
            z=stoi(line);
            arr[i]=Points<T>(x,y,z);
        }
        in.close();*/
        arr=vector<Points<T>>(num_elements);
        arr1=vector<Points<T>>(num_elements);
        srand(2);
        omp_set_num_threads(num_threads);
        unsigned int x,y,z;
        //Section where random numbers are assigned to points.
#pragma omp parallel for
        for (int i = 0; i < num_elements; i++) {
//            cout<<"Point "<<i<<":"<<endl;
//            cin>>x>>y>>z;
            x=rand();
            y=rand();
            z=rand();
            arr[i]=arr1[i]=Points<T>(x,y,z);
        }
        level = (sizeof(arr[0].getX())*8)/word_size;
        if ((sizeof(arr[0].getX())*8)%word_size!=0)
            level++;
//        print();
//        cout<<endl;
//        print1();
//        cout<<endl;
    }
    //This function prints all points of list and their transformed interleaved bit versions in binary form.
    void print()
    {
        for (int i = 0; i < num_elements; i++)
        {
            int num_length = sizeof(arr[0].getX())*8;
            T x=arr[i].getX();
            T y=arr[i].getY();
            T z=arr[i].getZ();
            T a=arr[i].getA();
            T b=arr[i].getB();
            T c=arr[i].getC();
            cout<<"("<<arr[i].getX()<<","<<arr[i].getY()<<","<<arr[i].getZ()<<"),";
            for (int j = 0; j < num_length; j++) {
                x=arr[i].getX();
                x=x>>num_length-j-1;
                cout<<x%2;
            }
            cout<<",";
            for (int j = 0; j < num_length; j++) {
                y=arr[i].getY();
                y=y>>num_length-j-1;
                cout<<y%2;
            }
            cout<<",";
            for (int j = 0; j < num_length; j++) {
                z=arr[i].getZ();
                z=z>>num_length-j-1;
                cout<<z%2;
            }
            cout<<"\t";
            for (int j = 0; j < num_length; j++) {
                a=arr[i].getA();
                a=a>>num_length-j-1;
                cout<<a%2;
            }
            cout<<",";
            for (int j = 0; j < num_length; j++) {
                b=arr[i].getB();
                b=b>>num_length-j-1;
                cout<<b%2;
            }
            cout<<",";
            for (int j = 0; j < num_length; j++) {
                c=arr[i].getC();
                c=c>>num_length-j-1;
                cout<<c%2;
            }
            cout<<endl;
        }
    }
    //This function prints all points in (x,y,z) form.
    void print1()
    {
        for (int i = 0; i < num_elements; i++) {
            cout << "(" << arr1[i].getX() << "," << arr1[i].getY() << "," << arr1[i].getZ() << "),";
        }
    }
    //This function is used to pass on to std:sort function to compare two <Points.h> objects.
    static bool compare1(Points<T> p1,Points<T> p2)
    {
        return p1 < p2;
    }
    //The recursive Sort function taking input as index of first and last element of the list to sort and level number.
    void Sort(int first, int last, int lvls)
    {
        unsigned int buck_num_elements = last-first+1;
        //Switching to sort function if level or number of elements threshold is crossed.
        if(buck_num_elements<=20 || lvls>=level-1)
        {
            if (lvls % 2 == 1) {
                sort(&arr1[first], &arr1[last + 1], compare1);
                for (int i = first; i <=last ; i++) {
                    arr[i] = arr1[i];
                }

            }
            else {
                sort(&arr[first], &arr[last + 1], compare1);
                for (int i = first; i <=last ; i++) {
                    arr1[i] = arr[i];
                }
            }
            return ;
        }
        //Use this section to test solely Radix Sort without the std:sort function.
//        cout<<lvls<<","<<omp_get_thread_num()<<","<<omp_get_level()<<","<<omp_get_num_threads()<<endl;
//        if(buck_num_elements<=1 || lvls>=level-1)
//        {
//            if(buck_num_elements==1) {
//                if (lvls % 2 == 1)
//                    arr[first] = arr1[first];
//                else
//                    arr1[first] = arr[first];
//            }
//            return ;
//        }
        //Boolean variable to determine if current level is even or odd.
        bool isodd;
        if(lvls % 2 == 1)
            isodd = true;
        else
            isodd = false;
        //Calculates the amount by which to right shift the co-ordinates to get required bits on LSB side.
        int shift=(sizeof(arr[0].getX())*8)-(lvls+1)*word_size;
        if(shift < 0)
            shift=0;
//        int data_type_size = sizeof(arr[0].getX());
        //Calculating number of buckets depending on total bits for bucketing = 2^actual_word_size. Can be done by shifting 1.
        unsigned int buckets = (1 << actual_word_size);
        //count -> Matrix of num_threads*buckets which contains the count of elements in particular bucket for count sort.
        vector<vector<unsigned int>> count(num_threads);
        //position -> Matrix of num_threads*buckets which determines where a particular thread will write an element to.
        vector<vector<unsigned int>> position(num_threads);
        for (int l = 0; l < num_threads; l++) {
            count[l] = vector<unsigned int>(buckets);
            position[l] = vector<unsigned int>(buckets);
        }
        //position1 -> An array to determine where the buckets start in the array to decide sections for next level.
        unsigned int position1[buckets];
        chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();

#pragma omp parallel
        for (unsigned int j = 0; j < buckets; j++) {
            position1[j]=0;
            count[omp_get_thread_num()][j]=0;
            position[omp_get_thread_num()][j]=0;
        }

        chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>( t2 - t1 ).count();

        //cout <<"The time taken for initialization is "<<duration <<" microseconds"<<endl;

        t1 = chrono::high_resolution_clock::now();


//        cout<<"Remainders: "<<","<<remainder<<endl;
//        cout<<"Flags: "<<flag1<<","<<flag2<<","<<flag3<<endl;
//        cout<<"first: "<<first<<" last: "<<last<<" sizeof(count): "<<count.size()<<" sizeof(count[0])"<<count[0].size()<<endl;
//        cout<<"                                                                    ";
        //This section determines the bucket a point will go into and increments the corresponding count value.
#pragma omp parallel for
        for (int i = first; i <= last; i++)
        {
            T a,b,c,x;
            int shift1;
            x=0;
            if(isodd) {
                a = arr1[i].getX();
                b = arr1[i].getY();
                c = arr1[i].getZ();
            }
            else {
                a = arr[i].getX();
                b = arr[i].getY();
                c = arr[i].getZ();
            }
            //This loop generates the bucketing number to determine the bucket for a point.
            for (int j = word_size-1; j >= 0; j--) {
                shift1 = shift+j;
                x = x << 1;
                x = x | ((a >> shift1) & 1);
                x = x << 1;
                x = x | ((b >> shift1) & 1);
                x = x << 1;
                x = x | ((c >> shift1) & 1);
            }
            count[omp_get_thread_num()][x]++;
//            x=x>>((level-1)*word_size);
//            if(remainder!=0)
//                x=x>>remainder-1;
//            cout<<x<<"\t";

        }
//        cout<<endl;
        //cout<<endl<<"Shift is "<<shift<<endl;
        t2 = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::microseconds>( t2 - t1 ).count();

        //cout<<"The time taken for calculating counts is "<<duration <<" microseconds"<<endl;
          //Displays values of count matrix.
//        cout<<"Values in count:"<<endl;
//        for(int i = 0; i < num_threads; i++){
//            for(int j = 0; j < buckets; j++){
//                cout<<count[i][j]<<" ";
//            }
//            cout<<endl;
//
//        }
        t1 = chrono::high_resolution_clock::now();
        //Prefix Sum Scan on count matrix to determine positions of each bucket for each thread.
        position1[0]=position[0][0]=first;
        for (int buc = 0; buc < buckets; buc++) {
            for (int tid = 1; tid < num_threads; tid++) {
                if(tid==0&&buc==0)
                {}
                else
                {
                    position[tid][buc]=position[tid-1][buc]+count[tid-1][buc];
                }
            }
            if(buc<buckets-1)
                position1[buc+1]=position[0][buc+1]=position[num_threads-1][buc]+count[num_threads-1][buc];
        }
        //Displays values in position matrix.
//        cout<<"Positions in position at depth "<<lvls<<" is:"<<endl;
//        for(int i = 0; i < num_threads; i++){
//            for(int j = 0; j < buckets; j++){
//                cout<<position[i][j]<<" ";
//            }
//            cout<<endl;
//        }
        //This section is used to shift and sort the points depending on and position matrix. Shifting dont between the 2 buffers depending on level.
#pragma omp parallel for
        for (int i = first; i <= last; i++) {
            T a,b,c,x;
            int shift1;
            x=0;
            if(isodd) {
                a = arr1[i].getX();
                b = arr1[i].getY();
                c = arr1[i].getZ();
            }
            else {
                a = arr[i].getX();
                b = arr[i].getY();
                c = arr[i].getZ();
            }
            //Section for determining the bucketing number again to check which position to shift this number to based on position matrix.
            for (int j = word_size - 1; j >= 0; j--) {
                shift1 = shift+j;
                x = x << 1;
                x = x | ((a >> shift1) & 1);
                x = x << 1;
                x = x | ((b >> shift1) & 1);
                x = x << 1;
                x = x | ((c >> shift1) & 1);
            }
            if(isodd)
                arr[position[omp_get_thread_num()][x]]=arr1[i];
            else
                arr1[position[omp_get_thread_num()][x]]=arr[i];
            position[omp_get_thread_num()][x]++;
        }
        //        temp.clear();
//        temp.shrink_to_fit();
        t2 = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::microseconds>( t2 - t1 ).count();

        //cout<<"The time taken for final sorting is "<<duration <<" microseconds"<<endl;

        //cout<<"Recursion number "<<level<<endl;
        //print();

//                if(lvls==0)
//                    return;

//        if(isodd)
//            print();
//        else
//            print1();

        //Section to invoke Sort functions recursively for each bucket.
#pragma omp parallel for
        for (int i = 0; i < buckets; i++)  {
            int begin=position1[i];
            int ending;
            if(i!=buckets-1)
                ending=position1[i+1]-1;
            else
                ending=last;
            Sort(begin,ending,lvls+1);
        }
    }
    //This function is the head of the recursive function.
    void sorting()
    {
        int lvls=0;
        //vector<Points<T>> arr1(n);
        Sort(0,num_elements-1,lvls);
//        print();
        if(check())
            cout<<"List1 is sorted."<<endl;
        else
            cout<<"List1 is not sorted.";
        if(check1())
            cout<<"List2 is sorted."<<endl;
        else
            cout<<"List2 is not sorted.";
    }
    //Compares the interleaved numbers a,b,c of 2 points to determine the greater one. Only used for checking if list is sorted correctly.
    bool compare(Points<T> num1,Points<T> num2)
    {
        if(num1.getA()<num2.getA())
            return false;
        else if(num1.getA()>num2.getA())
            return true;
        else if(num1.getB()<num2.getB())
            return false;
        else if(num1.getB()>num2.getB())
            return true;
        else if(num1.getC()>num2.getC())
            return true;
        else
            return false;
    }
    //This function is used to check whether primary buffer has been sorted correctly by using the above compare function.
    bool check()
    {
        for (int i = 0; i < num_elements-1; i++) {
            if(compare(arr[i],arr[i+1])) {
                cout<<endl<<arr[i].getA()<<"\t"<<arr[i].getB()<<"\t"<<arr[i].getC()<<endl;
                cout<<arr1[i].getA()<<"\t"<<arr1[i].getB()<<"\t"<<arr1[i].getC()<<endl;
                cout<<arr[i+1].getA()<<"\t"<<arr[i+1].getB()<<"\t"<<arr[i+1].getC()<<endl;
                cout<<arr1[i+1].getA()<<"\t"<<arr1[i+1].getB()<<"\t"<<arr1[i+1].getC()<<endl;
                return false;
            }
        }
        return true;
    }
    //This function is used to check whether the secondary buffer has been sorted correctly by using above compare function.
    bool check1()
    {
        for (int i = 0; i < num_elements-1; i++) {
            if(compare(arr1[i],arr1[i+1]))
                return false;
        }
        return true;
    }

};


#endif //OCTREESORT_RADIXSORT_H
