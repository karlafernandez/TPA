// FibonacciHeap.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

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

	FibonacciHeap()
	{
		root = NULL;
		min = NULL;
		n = 0;
	}

	int	maxTrees(int n)
	{
		if(n == 0) return -1;
		if(n == 1) return 1;
		float golden_ratio = 1.61803;
		return floor(log((float)n)/log(golden_ratio));
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

	void link(Node* y, Node* x)
	{
		y->left->right = y->right;
		y->right->left = y->left;
		y->left = NULL;
		y->right = NULL;
		x->child = y;
		y->parent = x;
	}

	void printArray(Node**A, int n)
	{
		cout << "A = " << endl;
		for(int i = 0; i < n; i++)
		{
			if(A[i] == NULL) cout << " " << ", ";
			else cout << A[i]->key << ", ";
			
		}
		cout << endl;
	}

	void consolidate()
	{
		int i, D_n = maxTrees(n);
		Node** A = new Node*[D_n];

		for(i = 0; i < D_n; ++i)
			A[i] = NULL;

		Node* x;
		Node* w = root;
		i = 0;
		while(i < (D_n + 1))
		{
			x = w;
			int d = x->degree;
			while(A[d] != NULL)
			{
				Node* y = A[d];
				if(x->key > y->key)
				{
					int tmp = x->key;
					x->key = y->key;
					y->key = tmp;
				}
				link(y, x);
				A[d] = NULL;
				d = d + 1;
			}
			A[d] = x;
			w = w->right;
			i++;
			printArray(A, D_n);
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
	}

	void cascadingCut(Node* y)
	{
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
			int maxT = maxTrees(fh->n);
			cout << "D_n = " << maxTrees(fh->n) << endl << endl;

			printRight(fh->root, 0, toFile, maxT);

			if(toFile) 
			{
				file << "}" << endl;
				file.close();
			}
		}
	}

	void printRight(Node* tempRight, int depth = 0, int toFile = 0, int maxT = 0)
	{
		int i = 0;
		while(depth == 0 && i < maxT)
		{
			printNode(tempRight, depth, nNodes, toFile);
			nNodes++;
			printChild(tempRight->child, depth, toFile);

			tempRight = tempRight->right;
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
			printRight(tempChild->right, depth, toFile);
			tempChild = tempChild->child;			
		}
	}
};


int main()
{

	FibonacciHeap F;
	F.insert(0);
	F.insert(1);
	F.insert(2);
	F.insert(3);

	//F.print(0);

	cout<<"-------------------------------- "<<endl;
	//Node::print(F.minimum());

	cout<<"-------------------------------- "<<endl;

	cout << F.maxTrees(0) << endl;
	cout << F.maxTrees(1) << endl;
	cout << F.maxTrees(2) << endl;
	cout << F.maxTrees(3) << endl;
	cout<<"-------------------------------- "<<endl;
	
	F.extractMin();
	F.print(1);

	system("PAUSE");
	
	return 0;
}