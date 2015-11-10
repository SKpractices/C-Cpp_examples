//Program to get duplicate number in an array

#include<iostream>

using namespace std;

int main()
{
    //Get the array. 
    int a[] = {10,20,30,5,10,70,100,120,130,120,23,56,6754,3,5,65,7,7,5,4,23,4,5,6};
    int len,count =0;
    
    //Get the length of array.
    len = sizeof(a)/sizeof(*a);
    
    for(int i=0;i<len;i++)
    {
        for(int j=i+1;j<len;j++)
        {
            if(a[i]==a[j])
            {
                count+=1;
                cout<<count<<"\t";
                cout<<"duplicate number found."<<"\n";
            }
        }
    }
    
    return 0;
}
