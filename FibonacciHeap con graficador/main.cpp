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
		fib[0] = 1;
		fib[1] = 2;
		for(int i = 2; i < n; ++i)
		{
			fib[i] = fib[i - 1] + fib[i - 2];
		}
	}

	int	upperBound(int n)
	{
		//float golden_ratio = 1.61803;
		//int upper = ceil(log((float)n)/log(golden_ratio));
		//cout << "upper = " << upper << endl;
		//cout << "log2(n) = " << log2(n) << endl;
		if(n <= 0) return 0;
		int upper = (int)(ceil(log2(n)));
		return upper;

		//return fib[ (int)(ceil(log2(n))) + 1];
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

	void insert(Node*& x, int notn = 0)
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
			if(!notn) n = n + 1;
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

	void consolidate(int cheap = 0)
	{
		int i, D_n = upperBound(n - 1) + 1;

		cout << "D_n = " << D_n << endl;
		cout << "n = " << n << endl;
		Node** A = new Node*[D_n];

		for(i = 0; i < D_n; ++i)
			A[i] = NULL;

		Node* w = min;
		Node* x, *y;
		Node* tmp;
		i = 0;

		int nheads;
		if(!cheap) nheads = n;
		else nheads = D_n;

		while(i < n)
		{
			x = w;
			int d = x->degree;

			cout << "CABEZA = w->key = " << w->key << endl;

			while(A[d] != NULL)
			{
				y = A[d];
				if(x->key < y->key)
				{
					cout << "****** 1er if" << endl;
					cout << "--------------- link -------------" << endl;
					cout << "y->key = " << y->key << endl;
					cout << "x->key = " << x->key << endl;
					cout << "parent = " << x->key << endl;
					cout << "----------------------------------" << endl;

					link(y, x);
					w = x;
					x = x;
					cout << "NUEVO = w->key = " << w->key << endl;
				}
				else {
					cout << "****** 2do if" << endl;
					cout << "--------------- link -------------" << endl;
					cout << "y->key = " << y->key << endl;
					cout << "x->key = " << x->key << endl;
					cout << "parent = " << y->key << endl;
					cout << "----------------------------------" << endl;

					//   0, 3
					link(x, y);
					w = y;
					x = y;
					cout << "NUEVO = w->key = " << w->key << endl;
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
				insert(A[i], 1);
			}
		}
		cout << endl;
	}

	Node* extractMin()
	{
		cout << "------------- extractMin ------------" << endl;
		cout << "n = " << n << endl;

		Node* z = min;
		if(z != NULL)
		{
			cout << "z->key = " << z->key << endl;
			Node* x = z->child;
			int i = 0;
			while(i < z->degree)
			{
				//insert(x, 1);
				x->parent = NULL;
				x = x->right;
				cout << "x->right->key = " << x->right->key << endl;

				i++;
			}

			n = n - 1;
			cout << "n = " << n << endl;
			if(z == z->right)
			{
				
				cout << "Solo raiz" << endl;
				root = z->child;
				cout << "Nueva raiz = " << root->key << endl;

				cout << "root->right->key = " << root->right->key << endl;
				cout << "root->right->right->key = " << root->right->right->key << endl;
				cout << "root->child->key = " << root->child->key << endl;
				
				min = NULL;
			}
			else{
				if(z->child != NULL)
				{
					z->right->left = x->left;
					z->left->right = z->child;
					z->child->left = z->left;
					x->left->right = z->right;	
				}
				else
				{
					z->right->left = z->left;
					z->left->right = z->right;
				}

				min = z->left;
				root = z->left;

				cout << "NUEVO MIN = " << min->key << endl;
				cout << "consolidate ---> " << endl;
				consolidate();
			}
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
		x->right->left = x->left;
		x->left->right = x->right;
		if(y->child == x) y->child = x->right;
		y->degree = y->degree - 1;
		delete y;
		///////////////////////
		x->left = min->left;
		x->right = min; 
		min->left->right = x;
		min->left = x;
		x->parent = NULL;
		x->mark = false;
	}

	void deleteH(Node* x){
		decreaseKey(x, -99999);
		extractMin();
	}

	void cascadingCut(Node* y)
	{
		Node* z = y->parent;
		if(z != NULL)
		{
			if(y->mark == false)
				y->mark = true;
			else
			{
				cut(y,z);
				cascadingCut(z);
			}
		}
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
				if(node->right != NULL)
				{
					file << node->key << " -> " << node->right->key << endl;	
					file << node->right->key << " -> " << node->key << endl;
				} 
			}
			else {
				file << node->parent->key << " -> " << node->key << endl;
				file << node->key << " -> " << node->parent->key << endl;
				if(node->right != NULL)
				{
					file << node->key << " -> " << node->right->key << endl;	
					//file << node->right->key << " -> " << node->key << endl;
				}
			}
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
	FibonacciHeap F(2*n);

	for(int i = 0; i < n; ++i)
	{
		F.insert(i);
	}

	//F.print(1);


	//F.print(0);

	F.consolidate();

	F.print(1);


	//F.print(0);

	cout<<"-------------------------------- "<<endl;
	//Node::print(F.minimum());

	//
	cout<<"-------------------------------- "<<endl;
	//F.extractMin();
	
	//F.print(1);


	//system("PAUSE");
	
	return 0;
}
