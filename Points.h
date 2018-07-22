//
// Created by tanmay on 24/6/18.
//

#ifndef OCTREESORT_POINTS_H
#define OCTREESORT_POINTS_H

#include <iostream>
using namespace std;
template <class T>
class Points{
    T x,y,z,a,b,c;
public:
    Points()
    {
        this->x=0;
        this->y=0;
        this->z=0;
    }
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
};
#endif //OCTREESORT_POINTS_H
