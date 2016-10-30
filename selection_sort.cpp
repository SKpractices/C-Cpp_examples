//Code for selection sort, 

#include <iostream> 
#include <string>
using namespace std; 

void print_array(int* arr,int length); 
int find_min(int arr[],int idx, int length); 


int main(int argc, char *argv[])
{	
	int sequence[argc-1]; 
	int op_sequence[argc-1]; 

	for(int i=1; i<argc; i++)
	{
		sequence[i-1] = atoi(argv[i]);
	}

	for (int j=0; j<argc-1; j++)
	{
		op_sequence[j] = find_min(sequence,j,argc-1); 
	}

	print_array(op_sequence,argc-1); 
	return 0; 
} 

int find_min(int arr[],int idx, int length)  
{
	int i=length-1;
	int temp;
	
	while(i!=idx)
	{
		if(arr[i]<arr[i-1])
		{
			temp = arr[i-1];
			arr[i-1] = arr[i];
			arr[i] = temp; 
		}

		i = i-1; 
	}

	return arr[idx]; 

}

void print_array(int* arr, int length)
{
	for (int i=0; i<length; i++)
	{
		cout<<*(arr+i)<<" "; 
	}
}
