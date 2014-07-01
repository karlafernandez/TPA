#include <iostream>
#include <fstream>
#include <cstdlib>
#include <math.h>

using namespace std;

class Node
{
public:
	int key;
	int degree;
	Node* parent;
	Node* child;
	Node* sibling;

	Node(int key_ = 0)
	{
		key = key_;
		parent = NULL;
		child = NULL;
		sibling = NULL;
		degree = 0;
	}

	static void print(Node* node)
	{
		cout << "key = " << node->key << endl;
		cout << "degree = " << node->degree << endl;
	}
};

class BinomialHeap
{
private:
	ofstream file;
	int nNodes;

public:

	Node *H;

	BinomialHeap()
	{
		H = NULL;
	}

	Node* makeHeap()
	{
		Node* H = NULL;
		return H;
		// funcion delete
		Node* del;
		del = NULL;
		return del;
	}

	Node* minimum(Node *H = 0)
	{
		if(H == 0) H = this->H;
		Node* y = NULL;
		Node* x = H;
		int min = 10000;

		while(x != NULL)
		{
			if(x->key < min)
			{
				min = x->key;
				y = x;
			}
			x = x->sibling;
		}
		return y;
	}

	Node* minimum(Node*& leftH, Node *H = 0)
	{
		if(H == 0) H = this->H;
		Node* y = NULL;
		Node* x = H;
		int min = 10000;

		Node* prev_x = NULL;
		while(x != NULL)
		{
			if(x->key < min)
			{
				min = x->key;
				y = x;
				leftH = prev_x;
			}
			prev_x = x;
			x = x->sibling;
		}
		return y;
	}

	void extractMin()
	{
		Node *prev_x = new Node();
		Node *x = minimum(prev_x, H);
		Node *x_child = x->child;
		if(prev_x != NULL)
			prev_x->sibling = NULL;
		Node *next_x = x->sibling;

		Node *H_p = makeHeap();

		Node* tmp = x_child;
		Node* prev_tmp;
		Node* last_tmp;

		int i = 0;
		while(tmp != NULL)
		{
			tmp->parent = NULL;
			if(i == 0)
			{
				prev_tmp = tmp;
				last_tmp = tmp;
				tmp = tmp->sibling;
			}
			else {
				if(tmp->sibling == NULL)
				{
					last_tmp = tmp;
					x_child->sibling = NULL;
					tmp->sibling = prev_tmp;
					break;
				}
				last_tmp = tmp;
				tmp = tmp->sibling;
				last_tmp->sibling = prev_tmp;
				prev_tmp = last_tmp;
			}
			i++;
		}
		H_p = last_tmp;

		x_child->sibling = next_x;

		if(prev_x == NULL)
			H = NULL;

		delete(x);


		H = union_h(H, H_p);

		
	}

	void decreaseKey(Node *x, int k)
	{
		if( k > x->key)
			return;
		x ->key = k;
		Node *y = x;
		Node *z = y->parent;
		while ( z != NULL && y->key < z->key)
		{
			int tempKey; //intercambio de llaves
			tempKey = y->key;
			y->key=z->key;
			z->key = tempKey;
			y = z;
			z = z->parent;
		}
	}



	void merge(Node *&H1, Node *&H2)
	{

		if(H1 == NULL)
		{
			H1 = H2;
			return;
		}
		if(H2 == NULL)
			return;

		Node*a = H1;
		Node*b = H2;

		if(a->degree <= b->degree)
			H1 = a;
		else
			H1 = b;

		if(H1 == b)
			b = a;
		a = H1;

		while(b != NULL)
		{
			if(a->sibling == NULL)
			{
				a->sibling = b;
				return;
			}
			else{
				if(a->sibling->degree < b->degree)
				{
					a = a->sibling;
				}
				else
				{
					Node*c = b->sibling;
					b->sibling = a->sibling;
					a->sibling = b;
					a = a->sibling;
					b = c;
				}
			}
		}
	}

	void link(Node *y, Node *z)
	{
		y->parent = z;
		y->sibling = z->child;
		z->child = y;
		z->degree = z->degree + 1;
	}

	Node* union_h(Node *H, Node *H2)
	{
		merge(H, H2); //H = merge(H1, H2);

		if (H == NULL)
			return H;

		Node* prev_x = NULL;
		Node* x = H;
		Node* next_x = x->sibling;

		while(next_x != NULL)
		{
			if((x->degree != next_x->degree) || 
				(next_x->sibling != NULL && next_x->sibling->degree == x->degree))
			{
				prev_x = x;
				x = next_x;
			}
			else
			{
				if(x->key <= next_x->key)
				{
					x->sibling = next_x->sibling;
					link(next_x, x);
				}
				else
				{
					if(prev_x == NULL)
					{
						H = next_x;
					}
					else
					{
						prev_x->sibling = next_x;
					}
					link(x, next_x);
					x = next_x;
				}
			}
			next_x = x->sibling;
		}
		return H;
	}

	Node* insert(int key)
	{
		Node *H_p = makeHeap();
		Node *node = new Node(key);
		H_p = node;
		H = union_h(H, H_p);

		return H;
	}

	void deleteH (Node* x)
	{
		decreaseKey(x,-99999);
		cout<<"-------------------------------- "<<endl;
	
		print(0);
		cout<<"-------------------------------- "<<endl;
	
		extractMin();
	}

	void printTabs(int n)
	{
		int i = 0;
		while(i < n)
		{
			cout << "\t";
			i++;
		}
	}

	void printNode(Node* node, int depth, int nodeid, int toFile = 0)
	{
		printTabs(depth);
		cout << "Depth # " << depth << " Node # "<< nodeid <<endl;
		if(toFile){
			if(depth == 0) 
			{
				if(node->sibling != NULL) file << node->key << " -> " << node->sibling->key << endl;
			}
			else file << node->parent->key << " -> " << node->key << endl;
		}
		else
		{ 
			if(depth == 0)
			{ 
				if(node->sibling != NULL) cout << node->key << " -> " << node->sibling->key << endl;
			}
			else
			{
				printTabs(depth);
				cout << node->parent->key << " -> " << node->key << endl;
			}
		}
		printTabs(depth); 
		cout << "  key = " << node->key << endl;
		printTabs(depth);
		cout << "  degree = " << node->degree << endl;
	}

	void print(int toFile, Node* H = 0)
	{
		if(H == 0) H = this->H;
		if(toFile){
			file.open("out.dot");
			file << "digraph G {" << endl;
		}
		nNodes = 0;

		printSiblings(H, 0, toFile);

		if(toFile) 
		{
			file << "}" << endl;
			file.close();
		}
	}

	void printSiblings(Node* tempSibling, int depth = 0, int toFile = 0)
	{
		while(tempSibling != NULL)
		{
			printNode(tempSibling, depth, nNodes, toFile);
			nNodes++;
			printChild(tempSibling->child, depth, toFile);
			tempSibling = tempSibling->sibling;
		}
	}

	void printChild(Node* tempChild, int depth = 0, int toFile = 0)
	{
		while(tempChild != NULL)
		{	
			depth++;
			printNode(tempChild, depth, nNodes, toFile);
			nNodes++;
			printSiblings(tempChild->sibling, depth, toFile);
			tempChild = tempChild->child;			
		}
	}

};

int main()
{

	BinomialHeap B;

	for(int i = 0; i < 1000; ++i)
	{
		cout << i << endl;
		B.insert(i);
	}

	B.print(1);

	/*
	cout<<"-------------------------------- "<<endl;
	//Node::print(B.minimum());

	cout<<"-------------------------------- "<<endl;
	B.extractMin();
	cout<<"-------------------------------- "<<endl;
	B.print(0);
	cout<<"-------------------------------- "<<endl;
	
	//B.decreaseKey(B.H->sibling->child, -9999);
	//B.print(0);
	cout<<"-------------------------------- "<<endl;

	B.deleteH(B.H->sibling->child);

	B.print(0);
	cout<<"-------------------------------- "<<endl;
	*/
	
	return 0;
}