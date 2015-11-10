//Program to reverse the words in string. 

#include<iostream>
#include<string>
using namespace std;

int main()
{
    //Get the string. 
    char a[] = "Let's have fun.";
    int len,count =0;
    char b[len];

    
    //Get the length of array.
    len = sizeof(a)/sizeof(*a);
    cout<<len<<"\n";
    for(int i=0;i<len;i++)
    {
        
            b[i]=a[i];
            if(a[i]==' ')
            {
                cout<<b;
                cout<<" ";
            }
            }
    
    
    return 0;
}
