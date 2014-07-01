#include <iostream>
#include <fstream>
#include <math.h> 

using namespace std;

class Node
{
public:
	int key;
	int degree;
	bool mark;
	Node* parent;
	Node* child;

	Node* left;
	Node* right;

	Node(int key_ = 0)
	{
		key = key_;
		degree = 0;
		mark = false;
		parent = NULL;
		child = NULL;
	}

	static void print(Node* node)
	{
		cout << "key = " << node->key << endl;
		cout << "degree = " << node->degree << endl;
	}
};

class FibonacciHeap
{

private:
	ofstream file;
	int nNodes;

public:
	Node* root;
	Node* min;
	int n;

	int* fib;

	FibonacciHeap(int n_max = 1)
	{
		root = NULL;
		min = NULL;
		n = 0;
		fib = new int[n_max];
		fillFibonacci(n_max, fib);
	}

	int fillFibonacci(int n, int*& fib)
	{
		fib[0] = 0;
		fib[1] = 1;
		for(int i = 2; i < n; ++i)
		{
			fib[i] = fib[i - 1] + fib[i - 2];
		}
	}

	int	upperBound(int n)
	{
		float golden_ratio = 1.61803;
		int upper = floor(log((float)n)/log(golden_ratio));
		cout << "upper = " << upper << endl;
		return upper;

		//return fib[ (int)(ceil(log2(n))) + 1];
	}

	void fibSerie()
	{

	}

	FibonacciHeap* makeHeap()
	{
		FibonacciHeap* H = NULL;
		return H;
	}

	Node* minimum()
	{
		return min;
	}

	void insert(int key)
	{
		Node* x = new Node(key);
		if(min == NULL)
		{
			root = x;
			min = x;
			n = 1;
		}
		else
		{
			if(n == 1)
			{
				min->left = x;
				min->right = x;
				x->left = min;
				x->right = min;
			} else {
				x->left = min->left;
				x->right = min;
				min->left->right = x;
				min->left = x;
			}

			if(x->key < min->key)
			{
				min = x;
			}
			n = n + 1;
		}
	}

	void insert(Node*& x)
	{
		if(min == NULL)
		{
			root = x;
			root->right = root;
			root->left = root;
			min = x;
		}
		else
		{
			if(n == 1)
			{
				min->left = x;
				min->right = x;
				x->left = min;
				x->right = min;
			} else {
				x->left = min->left;
				x->right = min;
				min->left->right = x;
				min->left = x;
			}

			if(x->key < min->key)
			{
				min = x;
			}
			n = n + 1;
		}
	}

	void link(Node*& y, Node*& x)
	{

		y->left->right = y->right;
		y->right->left = y->left;

		y->left = NULL;
		y->right = NULL;

		if(x->child == NULL)
		{
			x->child = y;
		} else {
			int i = 1;
			Node* last = x->child;
			while(i < x->degree) last = last->right, i++;
			x->child->left = y;
			y->right = x->child;
			y->left = last;
			last->right = y;
			x->child = y;
		}
		

		y->parent = x;

		x->degree ++;
		y->mark = false;
	}

	void printArray(Node**A, int n)
	{
		cout << "A = [";
		for(int i = 0; i < n; i++)
		{
			if(A[i] == NULL) cout << " " << ", ";
			else cout << A[i]->key << ", ";
			
		}
		cout << "]";
		cout << endl;
	}

	void consolidate()
	{
		int i, D_n = n;

		cout << "D_n = " << D_n << endl;
		Node** A = new Node*[D_n];

		for(i = 0; i < D_n; ++i)
			A[i] = NULL;

		Node* w = min;
		Node* x, *y;
		Node* tmp;
		i = 0;

		while(i < D_n)
		{
			x = w;
			int d = x->degree;

			while(A[d] != NULL)
			{
				y = A[d];
				if(x->key < y->key)
				{
			
					link(y, x);
					w = x;
					x = x;
				}
				else {

					//   0, 3
					link(x, y);
					w = y;
					x = y;
				}

				A[d] = NULL;
				d = d + 1;
			}

			A[d] = w;
			
			w = w->right;

			i++;
			//printArray(A, D_n);
		}

		min = NULL;

		for(i = 0; i < D_n; ++i)
		{
			if(A[i] != NULL)
			{				
				insert(A[i]);
			}
		}
	}

	Node* extractMin()
	{
		if(n == 1)
		{
			root = NULL;
			min = NULL;
			n = n - 1;
			return NULL;
		}

		Node* z = min;
		if(z != NULL)
		{
			Node* x = z->child;
			while(x != NULL)
			{
				insert(x);
				x->parent = NULL;
				x = x->right;
			}
			
			z->left->right = z->right;
			z->right->left = z->left;

			if(z == z->right)
				min = NULL;
			else{
				min = z->right;
				root = min;
			}

			consolidate();
			n = n - 1;
		}
		return z;
	}



	void union_h(FibonacciHeap* H1, FibonacciHeap* H2)
	{
		FibonacciHeap* H = makeHeap();
		H->min = H1->min;

		H->root->right = H2->root;
		H2->root->left = H->root;

		if((H1->min == NULL) || (H2->min != NULL && H2->min->key < H1->min->key))
		{
			H->min = H2->min;
		}
		H->n = H1->n + H2->n;
	}

	void decreaseKey(Node* x, int k)
	{
		if(k > x->key)
			return;
		x->key = k;
		Node* y = x->parent;
		if(y != NULL && x->key < y->key)
		{
			cut(x, y);
			cascadingCut(y);
		}
		if(x->key < min->key)
			min = x;
	}


	void cut(Node* x, Node* y)
	{
		/*if (x == x->right)
			y->child = NULL;
			(x->left)->right = x->right;
			(x->right)->left = x->left;
		if (x == y->child)
			y->child = x->right;
			y->degree = y->degree - 1;
			x->right = x;
			x->left = x;

			y->left->right = y->right;
			y->right->left = y->left;
			y->left = NULL;
			y->right = NULL;

			//(H1->left)->right = x; //del pseudo cormen?
			//x->right = H1;
			//x->left = H1->left;
			//H1->left = x;

			x->parent = NULL;
			x->mark = false;        */
	}

	void cascadingCut(Node* y)
	{/*
		Node* y = A[d];
		int z;
		Node* x;
		Node* w = root;
		i = 0;
				link(y, x);
				A[d] = NULL;
				d = d + 1;
		
			A[d] = x;
			w = w->right;
			i++;
			printArray(A, D_n);
		Node* z = min;
		z = y->parent;
		if (z = NULL)
			if ( y.mark == false) 
				y.mark = true;
			else (
				//cut(H, y, z);
		//cascadingCut(H, z);      */
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
				if(node->right != NULL) file << node->key << " -> " << node->right->key << endl;
			}
			else file << node->parent->key << " -> " << node->key << endl;
		}
		else
		{ 
			if(depth == 0)
			{ 
				if(node->right != NULL) cout << node->key << " -> " << node->right->key << endl;
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

	void print(int toFile, FibonacciHeap* fh = 0)
	{
		if(fh == 0) fh = this;
		if(toFile){
			file.open("out.dot");
			file << "digraph G {" << endl;
		}

		if(fh->n == 0)
		{
			cout << "Empty" << endl;
		}
		else
		{
			nNodes = 0;
			int maxT = n;
			cout << "D_n = " << maxT << endl << endl;


			printHeader(fh->root, 0, toFile, maxT);

			if(toFile) 
			{
				file << "}" << endl;
				file.close();
			}
		}
	}

	void printHeader(Node* tempRight, int depth = 0, int toFile = 0, int maxT = 0)
	{
		int i = 0;
		Node* tmp = tempRight;
		while(i < maxT)
		{
			//cout<< "printHeader"<< endl;
			printNode(tempRight, depth, nNodes, toFile);
			nNodes++;
			printRight(tempRight, depth + 1, toFile);

			tempRight = tempRight->right;
			i++;
			if(tempRight == tmp) break;
		}
	}

	void printRight(Node* tempRight, int depth = 0, int toFile = 0, int maxT = 0)
	{
		
		if(tempRight == NULL) return;
		Node* rightChild = tempRight->child;

		int i = 0;
		while(i < tempRight->degree)
		{
			//cout<< "printRight"<< endl;
			printNode(rightChild, depth, nNodes, toFile);
			nNodes++;
			
			printChild(rightChild->child, depth, toFile);

			rightChild = rightChild->right;
			i++;
		}
	}

	void printChild(Node* tempChild, int depth = 0, int toFile = 0)
	{
		while(tempChild != NULL)
		{	
			depth++;
			printNode(tempChild, depth, nNodes, toFile);
			nNodes++;
			//cout<< "printChild"<< endl;
			printRight(tempChild->right, depth, toFile);
			tempChild = tempChild->child;			
		}
	}

};


int main()
{

	int n = 10;
	FibonacciHeap F(n);

	for(int i = 0; i < 1000; ++i)
	{
		F.insert(i);
	}

	//F.print(1);


	//F.print(0);




	//F.print(0);

	cout<<"-------------------------------- "<<endl;
	//Node::print(F.minimum());

	F.consolidate();
	cout<<"-------------------------------- "<<endl;
	//F.extractMin();
	F.print(1);


	//system("PAUSE");
	
	return 0;
}
