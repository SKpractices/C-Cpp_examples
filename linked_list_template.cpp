
#include <iostream> 
#include <string>
using namespace std; 




void print_array(int* arr,int length); 

struct Node
{
	int value; 
	Node *next_node; 
}; 

int main(int argc, char *argv[])
{	
	Node *root, *node1, *node2, *conductor;

	root = new Node; 
	node1 = new Node; 
	node2 = new Node; 
	conductor = new Node; 
	root->value = 12; 
	root->next_node = node1;

	node1->value = 24; 
	node1->next_node = node2;

	node2->value = 36; 
	node2->next_node = 0;

	conductor = root; 
	
	while(conductor->next_node!=0)
	{
		cout<<conductor->value<<endl; 
		conductor = conductor->next_node; 
	}

	return 0; 
} 


