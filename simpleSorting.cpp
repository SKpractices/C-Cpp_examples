/*
Simple code to sort array of numbers. This will contain all major sorting algorithms implementation with c++.
Methode 1 - Simple get the max and store it. 
*/


#include <iostream>

using namespace std;

    // declare the functions
    //int getArray(int num_ele);
    int max(int a[]);
    int *stripArray(int a[]);
    int main()
    {
    int a[7] = {10,2,10,30,50,12,14};
    cout<<sizeof(a)/sizeof(*a)<<"\n";
    int *b;
    b=  stripArray(a);
    cout<<b[3];



        //a=getArray(4);
    //cout<<a;
    return 0;
    }

    // function to get the maximum of an array.
    int max(int a[])
    {
    for(int i=0; i<7 ; i++)
    {
        if(a[i]>a[i+1])
        {
            a[i+1] = a[i];
        }
    }

    return a[6];
    }

    int *stripArray(int a[])
    {
        int len = (sizeof(a)/sizeof(*a))-1;
        int b[len-1];
        for(int i=0;i<len-1;i++)
        {
            b[i] = a[i];
        }
        return b;
    }
/*
int getArray(int num_ele)
{
    int a[num_ele];
    for(int i=0; i<=num_ele; i++)
    {
        count<<"Enter Number : ";
        cin>>a[i];
    }

    return a;
}

*/
