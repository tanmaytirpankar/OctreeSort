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
    vector <Points<T>> arr,arr1;
    int num_threads, word_size, level;
    unsigned int num_elements;
public:
    RadixSort(unsigned int n, int num, int user_word_size)
    {
        num_elements=n;
        this->word_size=user_word_size*3;
        this->num_threads=num;

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
#pragma omp parallel for
        for (int i = 0; i < num_elements; i++) {
//            cout<<"Point "<<i<<":"<<endl;
//            cin>>x>>y>>z;
            x=rand();
            y=rand();
            z=rand();
            arr[i]=arr1[i]=Points<T>(x,y,z);
        }
        level = (sizeof(arr[0].getX())*24)/word_size;
        if ((sizeof(arr[0].getX())*24)%word_size!=0)
            level++;
//        print();
//        cout<<endl;
//        print1();
//        cout<<endl;
    }
    void print()
    {
        for (int i = 0; i < num_elements; i++)
        {
            int num_length = sizeof(arr[0].getX())*8;
            int x=arr[i].getX();
            int y=arr[i].getY();
            int z=arr[i].getZ();
            int a=arr[i].getA();
            int b=arr[i].getB();
            int c=arr[i].getC();
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
    void print1()
    {
        for (int i = 0; i < num_elements; i++) {
            cout << "(" << arr1[i].getX() << "," << arr1[i].getY() << "," << arr1[i].getZ() << "),";
        }
    }
    void Sort(int first, int last, int lvls)
    {
        unsigned int buck_num_elements = last-first+1;
        //Gap for switching to std:sort from radix sort code
        if(buck_num_elements<=1 || lvls>=level-1)
        {
            if(buck_num_elements==1) {
                if (lvls % 2 == 1)
                    arr[first] = arr1[first];
                else
                    arr1[first] = arr[first];
            }
            return ;
        }
        int remainder,shift;
        int word_section_start_bit = lvls * word_size;
        int data_type_size = sizeof(arr[0].getX());
        unsigned int buckets = int(pow(2,word_size));
        vector<vector<unsigned int>> count(num_threads);
        vector<vector<unsigned int>> position(num_threads);
        for (int l = 0; l < num_threads; l++) {
            count[l] = vector<unsigned int>(buckets);
            position[l] = vector<unsigned int>(buckets);
        }
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

        cout <<"The time taken for initialization is "<<duration <<" microseconds"<<endl;

        t1 = chrono::high_resolution_clock::now();
        bool flag1= false,flag2= false,flag3= false;

        if(word_section_start_bit < data_type_size*8) {
            remainder = (data_type_size*8)%word_size;
            shift = (data_type_size*8)-(lvls+1)*word_size;
            if(data_type_size*8 - word_section_start_bit<word_size)
                flag1=true;
        }
        else if(word_section_start_bit < data_type_size*16)
        {
            remainder = (data_type_size*16)%word_size;
            shift = (data_type_size*16)-((lvls+1)*word_size);
            if(data_type_size*16 - word_section_start_bit<word_size)
                flag2=true;
        }
        else if(word_section_start_bit < data_type_size*24)
        {
            remainder = (data_type_size*24)%word_size;
            shift = (data_type_size*24)-((lvls+1)*word_size);
            if(data_type_size*24 - word_section_start_bit<word_size)
                flag3=true;
        }
//        cout<<"Remainders: "<<remainder<<","<<remainder<<","<<remainder<<endl;
//        cout<<"Flags: "<<flag1<<","<<flag2<<","<<flag3<<endl;
        int mask=int(pow(2,word_size)-1);
//        cout<<"first: "<<first<<" last: "<<last<<" sizeof(count): "<<count.size()<<" sizeof(count[0])"<<count[0].size()<<endl;
//        cout<<"                                                                    ";


////Original determine the bucket section.
//#pragma omp parallel for
//        for (int i = first; i <= last; i++) {
//            T a,b,c,x;
//            //temp[i-first]=arr[i];
//            if(lvls%2==1) {
//                a = arr1[i].getA();
//                b = arr1[i].getB();
//                c = arr1[i].getC();
//            }
//            else {
//                a = arr[i].getA();
//                b = arr[i].getB();
//                c = arr[i].getC();
//            }
//            if(word_section_start_bit < data_type_size*8) {
//                if(flag1)
//                {
//                    T y=b;
//                    x=a;
//                    x=x<<(word_size-remainder);
//                    x=x&mask;
//                    y=y>>data_type_size*8-word_size+remainder;
//                    y=y&(int(pow(2,(word_size-remainder))))-1;
//                    x=x|y;
//                }
//                else
//                {
//                    x=a;
//                    x=x>>shift;
//                }
//            }
//            else if(word_section_start_bit < data_type_size*16)
//            {
//                if(flag2)
//                {
//                    T y=c;
//                    x=b;
//                    x=x<<(word_size-remainder);
//                    x=x&mask;
//                    y=y>>data_type_size*8-word_size+remainder;
//                    y=y&(int(pow(2,(word_size-remainder))))-1;
//                    x=x|y;
//                }
//                else
//                {
//                    x=b;
//                    x=x>>shift;
//                }
//            }
//            else if(word_section_start_bit < data_type_size*24)
//            {
//                if(flag3)
//                {
//                    x=c;
//                    x=x&(int(pow(2,(remainder))))-1;
//                }
//                else
//                {
//                    x=c;
//                    x=x>>shift;
//                }
//            }
////            x=x>>((level-1)*word_size);
////            if(remainder!=0)
////                x=x>>remainder-1;
//            x=x&mask;
////            cout<<x<<"\t";
//        }
#pragma omp parallel for
        for (int i = first; i <= last; i++)
        {
            T a,b,c;
            //temp[i-first]=arr[i];
            if(lvls%2==1) {
                a = arr1[i].getA();
                b = arr1[i].getB();
                c = arr1[i].getC();
            }
            else {
                a = arr[i].getA();
                b = arr[i].getB();
                c = arr[i].getC();
            }
            if(word_section_start_bit < data_type_size*8) {
                if(flag1)
                {
                    a=a<<(word_size-remainder);
                    a=a&mask;
                    b=b>>data_type_size*8-word_size+remainder;
                    b=b&(int(pow(2,(word_size-remainder))))-1;
                    a=a|b;
                }
                else
                {
                    a=a>>shift;
                    a=a&mask;
                }
                count[omp_get_thread_num()][a]++;
            }
            else if(word_section_start_bit < data_type_size*16)
            {
                if(flag2)
                {
                    b=b<<(word_size-remainder);
                    b=b&mask;
                    c=c>>data_type_size*8-word_size+remainder;
                    c=c&(int(pow(2,(word_size-remainder))))-1;
                    b=b|c;
                }
                else
                {
                    b=b>>shift;
                    b=b&mask;
                }
                count[omp_get_thread_num()][b]++;
            }
            else if(word_section_start_bit < data_type_size*24)
            {
                if(flag3)
                {
                    c=c&(int(pow(2,(remainder))))-1;
                }
                else
                {
                    c=c>>shift;
                    c=c&mask;
                }
                count[omp_get_thread_num()][c]++;
            }
//            x=x>>((level-1)*word_size);
//            if(remainder!=0)
//                x=x>>remainder-1;
//            cout<<x<<"\t";

        }
//        cout<<endl;
        //cout<<endl<<"Shift is "<<shift<<endl;
        t2 = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::microseconds>( t2 - t1 ).count();

        cout<<"The time taken for calculating counts is "<<duration <<" microseconds"<<endl;
//        cout<<"Values in count:"<<endl;
//        for(int i = 0; i < num_threads; i++){
//            for(int j = 0; j < buckets; j++){
//                cout<<count[i][j]<<" ";
//            }
//            cout<<endl;
//
//        }
        t1 = chrono::high_resolution_clock::now();
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

//        cout<<"Positions in position at depth "<<lvls<<" is:"<<endl;
//        for(int i = 0; i < num_threads; i++){
//            for(int j = 0; j < buckets; j++){
//                cout<<position[i][j]<<" ";
//            }
//            cout<<endl;
//        }
#pragma omp parallel for
        for (int i = first; i <= last; i++) {
            T a,b,c,x;
            //temp[i-first]=arr[i];
            if(lvls%2==1) {
                a = arr1[i].getA();
                b = arr1[i].getB();
                c = arr1[i].getC();
            }
            else {
                a = arr[i].getA();
                b = arr[i].getB();
                c = arr[i].getC();
            }
            if(word_section_start_bit < data_type_size*8) {
                if(flag1)
                {
                    T y=b;
                    x=a;
                    x=x<<(word_size-remainder);
                    x=x&mask;
                    y=y>>data_type_size*8-word_size+remainder;
                    y=y&(int(pow(2,(word_size-remainder))))-1;
                    x=x|y;
                }
                else
                {
                    x=a;
                    x=x>>shift;
                }
            }
            else if(word_section_start_bit < data_type_size*16)
            {
                if(flag2)
                {
                    T y=c;
                    x=b;
                    x=x<<(word_size-remainder);
                    x=x&mask;
                    y=y>>data_type_size*8-word_size+remainder;
                    y=y&(int(pow(2,(word_size-remainder))))-1;
                    x=x|y;
                }
                else
                {
                    x=b;
                    x=x>>shift;
                }
            }
            else if(word_section_start_bit < data_type_size*24)
            {
                if(flag3)
                {
                    x=c;
                    x=x&(int(pow(2,(remainder))))-1;
                }
                else
                {
                    x=c;
                    x=x>>shift;
                }
            }
//            x=x>>((level-1)*word_size);
//            if(remainder!=0)
//                x=x>>remainder-1;
            x=x&mask;
//            cout<<x<<"\t";
            if(lvls%2==1)
                arr[position[omp_get_thread_num()][x]]=arr1[i];
            else
                arr1[position[omp_get_thread_num()][x]]=arr[i];
            position[omp_get_thread_num()][x]++;
        }
        //        temp.clear();
//        temp.shrink_to_fit();
        t2 = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::microseconds>( t2 - t1 ).count();

        cout<<"The time taken for final sorting is "<<duration <<" microseconds"<<endl;

        //cout<<"Recursion number "<<level<<endl;
        //print();

//                if(lvls==0)
//                    return;

//        if(lvls%2==1)
//            print();
//        else
//            print1();
        return;
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
    void sorting()
    {
        int lvls=0;
        //vector<Points<T>> arr1(n);
        Sort(0,num_elements-1,lvls);
        //print();
        if(check())
            cout<<"List1 is sorted."<<endl;
        else
            cout<<"List1 is not sorted.";
        if(check1())
            cout<<"List2 is sorted."<<endl;
        else
            cout<<"List2 is not sorted.";
    }
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
    bool check1()
    {
        for (int i = 0; i < num_elements-1; i++) {
            if(compare(arr1[i],arr1[i+1]))
                return false;
        }
        return true;
    }
/*    void Sort(int first, int last, int level)
    {
        int num_elements=last-first+1;
        if(num_elements<=4096)
        {
            //insertionSort(first,last,level-1);
            std::sort(&arr[first],&arr[last+1]);
            return ;
        }
        if(level<1)
        {
            return ;
        }
        int buckets=int(pow(2,word_size));
        vector<vector<unsigned int>> count(num_threads);
        vector<vector<unsigned int>> position(num_threads);
        for (int l = 0; l < num_threads; ++l) {
            count[l] = vector<unsigned int>(buckets);
            position[l]=vector<unsigned int>(buckets);
        }
//        if(num_elements<=1 || level<1)
//        {
//            return ;
//        }

        int position1[buckets];
        //vector<T> temp(num_elements);
        chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
        omp_set_num_threads(num_threads);
#pragma omp parallel
        for (int j = 0; j < buckets; j++) {
            position1[j]=0;
            count[omp_get_thread_num()][j]=0;
            position[omp_get_thread_num()][j]=0;
        }
        chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>( t2 - t1 ).count();

        //cout <<"The time taken for initialization is "<<duration <<" microseconds"<<endl;

        t1 = chrono::high_resolution_clock::now();
        int remainder= (sizeof(arr[0])*8)%word_size;
        int shift=((level-1)*word_size)-(word_size-remainder);
        int mask=int(pow(2,word_size)-1);
        omp_set_num_threads(num_threads);
//        cout<<"first: "<<first<<" last: "<<last<<" sizeof(count): "<<count.size()<<" sizeof(count[0])"<<count[0].size()<<endl;
#pragma omp parallel for
        for (int i = first; i <= last; i++)
        {
            T x;
            //temp[i-first]=arr[i];
            if(level%2==1)
                x=arr1[i];
            else
                x=arr[i];
            if(level>1)
                x=x>>shift;
//            x=x>>((level-1)*word_size);
//            if(remainder!=0)
//                x=x>>remainder-1;
            x=x&mask;
            count[omp_get_thread_num()][x]++;
        }
        //cout<<endl<<"Shift is "<<shift<<endl;
        t2 = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::microseconds>( t2 - t1 ).count();

        //cout<<"The time taken for calculating counts is "<<duration <<" microseconds"<<endl;
//        cout<<"Values in count:"<<endl;
//        for(int i = 0; i < num_threads; i++){
//            for(int j = 0; j < buckets; j++){
//                cout<<count[i][j]<<" ";
//            }
//            cout<<endl;
//        }
        t1 = chrono::high_resolution_clock::now();
//        position1[0]=position[0][0]=first;
//        int prev_buc=0;
//        for (int buc = 0; buc < 8; buc++) {
//            for (int tid = 0; tid < num_threads; tid++) {
//                if (tid==0)
//                    position1[buc+1]=position[tid][buc]=prev_buc;
//                else
//                    position[tid][buc]=position[tid-1][buc]+count[tid-1][buc];
//            }
//            prev_buc=position[num_threads-1][buc]+count[num_threads-1][buc];
//        }

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

//        cout<<"Positions in position at depth "<<level<<" is:"<<endl;
//        for(int i = 0; i < num_threads; i++){
//            for(int j = 0; j < buckets; j++){
//                cout<<position[i][j]<<" ";
//            }
//            cout<<endl;
//        }


        omp_set_num_threads(num_threads);
#pragma omp parallel for
        for (int i = first; i <= last; i++) {
            T x;
            if(level%2==1)
                x = arr1[i];
            else
                x = arr[i];
            if(level>1)
                x=x>>shift;
//            x=x>>((level-1)*word_size);
//            if(remainder!=0)
//                x=x>>remainder-1;
            x=x&mask;
            if(level%2==1)
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

        omp_set_num_threads(num_threads);
#pragma omp parallel for
        for (int i = 0; i < buckets; i++)  {
            int begin=position1[i];
            int ending;
            if(i!=buckets-1)
                ending=position1[i+1]-1;
            else
                ending=last;
            Sort(begin,ending,level-1);
        }
    }
    void sorting()
    {
        int lvls=0;
        lvls=(sizeof(arr[0])*8)/word_size+1;
        //cout<<lvls;
        //vector<Points<T>> arr1(n);
        this->Sort(0,n-1,lvls);
        //print();
    }*/
};


#endif //OCTREESORT_RADIXSORT_H
