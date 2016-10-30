//Code for bubble sort, 

#include <iostream> 
#include <string>
using namespace std; 

void print_array(int* arr,int length); 
void sort_arr(int arr[], int length); 

int main(int argc, char *argv[])
{	
	int sequence[argc-1];
	for (int i=1; i<argc ; i++)
	{
		sequence[i-1] = atoi(argv[i]);
	}

	sort_arr(sequence,argc-1);
	return 0; 
} 

void sort_arr(int arr[], int length)
{
	int temp; 
	for (int i=1 ; i<length ; i++) 
	{
		int j = i; 
		while(j>0 && arr[j-1]>arr[j])
		{
			temp = arr[j]; 
			arr[j] = arr[j-1]; 
			arr[j-1] = temp; 

			j = j-1; 
		}
	}

	print_array(arr,length); 
}


void print_array(int* arr, int length)
{
	for (int i=0; i<length; i++)
	{
		cout<<*(arr+i)<<" "; 
	}
}
