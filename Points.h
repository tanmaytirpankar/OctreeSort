//
// Created by tanmay on 24/6/18.
//

#ifndef OCTREESORT_POINTS_H
#define OCTREESORT_POINTS_H

#include <iostream>
using namespace std;
template <class T>
class Points{
    //x,y,z -> Coordinates of point.
    //a,b,c -> interleaved numbers by mixing x,y,z bits.
    T x,y,z,a,b,c;
public:
    Points()
    {
        this->x=0;
        this->y=0;
        this->z=0;
    }
    //This constructor is used to generate the interleaved numbers. Interleaved numbers a,b,c are only used for checking final list.
    Points(T x,T y, T z)
    {
        this->x=x;
        this->y=y;
        this->z=z;
        a=b=c=0;
        int x1,y1,z1;
        int arr1[96];
        for (int i = 0; i < 32; ++i) {
            x1=x;
            y1=y;
            z1=z;
            x1=x1>>31-i;
            y1=y1>>31-i;
            z1=z1>>31-i;
            x1=x1&1;
            y1=y1&1;
            z1=z1&1;
            arr1[3*i]=x1;
            arr1[(3*i)+1]=y1;
            arr1[(3*i)+2]=z1;
//            cout<<arr1[3*i]<<arr1[(3*i)+1]<<arr1[(3*i)+2];
        }
        for (int k = 0; k < 32; k++) {
            a=a+(arr1[k]<<31-k);
            b=b+(arr1[k+32]<<31-k);
            c=c+(arr1[k+64]<<31-k);
//            cout<<endl<<"Iteration "<<k<<endl;
//            cout<<a<<","<<b<<","<<c;
        }
    }
    T getX()
    {
        return x;
    }
    T getY()
    {
        return y;
    }
    T getZ()
    {
        return z;
    }
    T getA()
    {
        return a;
    }
    T getB()
    {
        return b;
    }
    T getC()
    {
        return c;
    }
    //Overloaded < operator to work on Points object for std:sort.
    bool operator <(Points<T> p2)
    {
        T a1 = 0, b1 = 0, c1 = 0, a2 = 0, b2 = 0, c2 = 0;
        for (int i = 31; i >= 22; i--) {
            a1 = a1 | ((x >> i) & 1);
            a1 = a1 << 1;
            a1 = a1 | ((y >> i) & 1);
            a1 = a1 << 1;
            a1 = a1 | ((z >> i) & 1);
            a1 = a1 << 1;
            a2 = a2 | ((p2.x >> i) & 1);
            a2 = a2 << 1;
            a2 = a2 | ((p2.y >> i) & 1);
            a2 = a2 << 1;
            a2 = a2 | ((p2.z >> i) & 1);
            a2 = a2 << 1;
        }
        a1 = a1 | ((x >> 21) & 1);
        a1 = a1 << 1;
        a1 = a1 | ((y >> 21) & 1);
        b1 = b1 | ((z >> 21) & 1);
        b1 = b1 << 1;
        a2 = a2 | ((p2.x >> 21) & 1);
        a2 = a2 << 1;
        a2 = a2 | ((p2.y >> 21) & 1);
        b2 = b2 | ((p2.z >> 21) & 1);
        b2 = b2 << 1;
        for (int i = 20; i >= 11; i--) {
            b1 = b1 | ((x >> i) & 1);
            b1 = b1 << 1;
            b1 = b1 | ((y >> i) & 1);
            b1 = b1 << 1;
            b1 = b1 | ((z >> i) & 1);
            b1 = b1 << 1;
            b2 = b2 | ((p2.x >> i) & 1);
            b2 = b2 << 1;
            b2 = b2 | ((p2.y >> i) & 1);
            b2 = b2 << 1;
            b2 = b2 | ((p2.z >> i) & 1);
            b2 = b2 << 1;
        }
        b1 = b1 | ((x >> 10) & 1);
        c1 = c1 | ((y >> 10) & 1);
        c1 = c1 << 1;
        c1 = c1 | ((z >> 10) & 1);
        b2 = b2 | ((p2.x >> 10) & 1);
        c2 = c2 | ((p2.y >> 10) & 1);
        c2 = c2 << 1;
        c2 = c2 | ((p2.z >> 10) & 1);
        for (int i = 9; i >= 0; i--) {
            c1 = c1 << 1;
            c1 = c1 | ((x >> i) & 1);
            c1 = c1 << 1;
            c1 = c1 | ((y >> i) & 1);
            c1 = c1 << 1;
            c1 = c1 | ((z >> i) & 1);
            c2 = c2 << 1;
            c2 = c2 | ((p2.x >> i) & 1);
            c2 = c2 << 1;
            c2 = c2 | ((p2.y >> i) & 1);
            c2 = c2 << 1;
            c2 = c2 | ((p2.z >> i) & 1);
        }
        if(a1<a2)
            return true;
        else if(a1>a2)
            return false;
        else if(b1<b2)
            return true;
        else if(b1>b2)
            return false;
        else if(c1<c2)
            return true;
        else
            return false;
    }
};
#endif //OCTREESORT_POINTS_H
