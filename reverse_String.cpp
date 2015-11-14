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
    char[] getword(char []);
    
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

char [] getword(char a[])
{
    int len = sizeof(a)/sizeof(*a);
    for(int i=0; i<len ; i++ )
    {
        if(a[i]==' ')
        {
            char b[len-i],c[i];
            for(int k=i; k<len ; k++)
            {
                b[k-i] = a[k]  
            }
            
            cout<<getword(b)<<
        }
        
    }
    }
