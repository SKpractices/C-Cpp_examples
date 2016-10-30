//Code for bubble sort, 



#include <iostream> 
#include <string>
using namespace std; 


void sort_seq(int* arr_ptr, int start_idx, int end_idx)
{
	int temp; 
	for (int i = start_idx ; i < end_idx ; i++) 
	{
		if(*(arr_ptr+i) > *(arr_ptr +i+1))
		{
			temp = *(arr_ptr + i); 
			*(arr_ptr + i)
			 = *(arr_ptr + i + 1); 
			*(arr_ptr + i + 1) = temp; 
		}
	}

};

int main(int argc, char *argv[])

{	

	int input_sequence[argc]; 

	for (int i=1 ; i<argc ; i++) 
	{
		input_sequence[i] = atoi(argv[i]);
	}

	for (int j = 0 ; j<argc-1 ; j++)
	{
		sort_seq(input_sequence,0,argc-1-j);
	}

	for (int i=0 ; i<argc-1 ; i++) 
	{
		cout<<"Sequence after sorting : "; 
		cout<<input_sequence[i]<<" "<<endl;
	}


	return 0; 
} 


