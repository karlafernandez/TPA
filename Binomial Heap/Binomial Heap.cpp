// Binomial Heap.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

class Node
{
public:
	int key;
	int degree;
	Node* parent;
	Node* child;
	Node* sibling;

	void print()
	{
		cout << endl << "\tkey = " << key << endl;
		cout << "\tdegree = " << degree << endl << endl;
	}

	void printChildren(Node* node, int depth)
	{
		Node* tempSibling = node->child;

		int i = 0;
		while(i < node->degree)
		{
			cout << "\tDepth # " << depth << ", Child Node # "<< i <<endl;

			i++, depth++;

			printChildren(tempSibling, depth);
			tempSibling->print();

			tempSibling = tempSibling->sibling;
		}
	}

	Node(int key_ = 0)
	{
		key = key_;
		parent = NULL;
		child = NULL;
		sibling = NULL;
		degree = 0;
	}
};

class BinomialHeap
{

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

	void print(Node* H = 0)
	{
		if(H == 0) H = this->H;

		cout<<"Printing BinomialHeap"<< endl;
		Node* temp = H;

		int i = 0;
		while(temp != NULL)
		{
			cout << "Depth # "<< 0 << ", Header Node # " << i << endl;
			temp->print();

			temp->printChildren(temp, 1);

			temp = temp->sibling;
		}
	}

};


int main()
{
	/*BinomialHeap B;
	B.insert(1);

	BinomialHeap B_temp;
	B_temp.insert(2);

	B.merge(B.H, B_temp.H);
	B.print();*/

	BinomialHeap B;
	B.insert(1);
	B.insert(2);
	B.insert(3);
	B.insert(4);
	B.insert(5);
	B.print();


	//BinomialHeap B_temp;
	//B_temp.insert(5);
	////B_temp.print(B.H);

	//B.H = B.merge(B.H, B_temp.H);

	//B.print(B.H);

	system("PAUSE");
	return 0;
}