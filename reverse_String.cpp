//Program to reverse the words in string. 

//Program to reverse the words in string. 

#include<iostream>
#include<string>
using namespace std;

int main()
{
    //Get the string. 
    char a[] = "Let's have funferf fvrvrvrv.";
    int len;
    
    len = sizeof(a)/sizeof(*a);
    
    for(int i = len-1 ; i>=0 ; i--)
    {
        if(a[i]==' ')
        {
            int l=1;
            while(!((a[i+l]=' ') || (a[i+1]='\0')))
            {
                cout<<a[i+l];
                l+=1;
            }
            cout<<' ';
        }
    }
    return 0;
}

