/*
Simple code to sort array of numbers. This will contain all major sorting algorithms implementation with c++.
Methode 1 - Simple get the max and store it. 
*/

#include <iostream>

using namespace std;

    // declare the functions
    //int getArray(int num_ele);
    //int max(int a[]);

    int main()
    {
    int a[7] = {10,2,10,30,50,12,14};
    cout<<"Size of given array is :- ";
    int len = sizeof(a)/sizeof(*a);
    cout<<len<<"\n";
    
    for(int i=len ; i>=1; i--)
    {
        for(int j = 0; j<i ; j++)
        {
            if(a[j]>a[j+1])
            {
                int temp;
                temp = a[j+1];
                a[j+1] = a[j];
                a[j] = temp;
            }
        }
        cout<<a[i-1];
        cout<<"\n";
    }

    return 0;
    }

/*
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










