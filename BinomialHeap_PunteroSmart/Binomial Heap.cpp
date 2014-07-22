#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

template <class T>
class Node
{
public:
	T key;
	int degree;
	Node<T>* parent;
	Node<T>* child;
	Node<T>* sibling;

	Node()
	{
		parent = NULL;
		child = NULL;
		sibling = NULL;
		degree = 0;
	}

	Node(T key_)
	{
		key = key_;
		parent = NULL;
		child = NULL;
		sibling = NULL;
		degree = 0;
	}

	static void print(Node<T>* node)
	{
		cout << "key = " << node->key->referencias << endl;
		cout << "degree = " << node->degree << endl;
	}

	~Node()
	{
		//cout << "---------------- Called ~Node" << endl;
	}
};

template <class T, class K>
class BinomialHeap
{
private:
	ofstream file;
	int nNodes;

public:

	Node<T> *H;

	BinomialHeap()
	{
		H = NULL;
	}

	Node<T>* makeHeap()
	{
		Node<T>* H = NULL;
		return H;
	}

	Node<T>* minimum(Node<T> *H = 0)
	{
		if(H == 0) H = this->H;
		Node<T>* y = NULL;
		Node<T>* x = H;
		K min = 10000;

		while(x != NULL)
		{
			if(x->key->referencias < min)
			{
				min = x->key->referencias;
				y = x;
			}
			x = x->sibling;
		}
		return y;
	}

	Node<T>* minimum(Node<T>*& leftH, Node<T> *H = 0)
	{
		if(H == 0) H = this->H;
		Node<T>* y = NULL;
		Node<T>* x = H;
		K min = 10000;

		Node<T>* prev_x = NULL;
		while(x != NULL)
		{
			if(x->key->referencias < min)
			{
				min = x->key->referencias;
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
		Node<T>*prev_x = new Node<T>();
		Node<T>*x = minimum(prev_x, H);
		Node<T>*x_child = x->child;
		if(prev_x != NULL)
			prev_x->sibling = NULL;
		Node<T>*next_x = x->sibling;

		Node<T>*H_p = makeHeap();

		Node<T>* tmp = x_child;
		Node<T>* prev_tmp;
		Node<T>* last_tmp;

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

		if(i > 0) // last node
		{
			H_p = last_tmp;
			x_child->sibling = next_x;
		}

		if(prev_x == NULL)
			H = NULL;

		delete(x);

		H = union_h(H, H_p);
	}

	void decreaseKey(Node<T> *x, T k)
	{
		if( k > x->key->referencias)
			return;
		x ->key->referencias = k;
		Node<T>*y = x;
		Node<T>*z = y->parent;
		while ( z != NULL && y->key->referencias < z->key->referencias)
		{
			T tempKey; //intercambio de llaves
			tempKey = y->key->referencias;
			y->key->referencias=z->key->referencias;
			z->key->referencias = tempKey;
			y = z;
			z = z->parent;
		}
	}



	void merge(Node<T>*&H1, Node<T>*&H2)
	{

		if(H1 == NULL)
		{
			H1 = H2;
			return;
		}
		if(H2 == NULL)
			return;

		Node<T>*a = H1;
		Node<T>*b = H2;

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
					Node<T>*c = b->sibling;
					b->sibling = a->sibling;
					a->sibling = b;
					a = a->sibling;
					b = c;
				}
			}
		}
	}

	void link(Node<T>*y, Node<T>*z)
	{
		y->parent = z;
		y->sibling = z->child;
		z->child = y;
		z->degree = z->degree + 1;
	}

	Node<T>* union_h(Node<T>*H, Node<T>*H2)
	{
		merge(H, H2); //H = merge(H1, H2);

		if (H == NULL)
			return H;

		Node<T>* prev_x = NULL;
		Node<T>* x = H;
		Node<T>* next_x = x->sibling;

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
				if(x->key->referencias <= next_x->key->referencias)
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

	T insert(T key)
	{
		Node<T>*H_p = makeHeap();
		Node<T>*node = new Node<T>(key);
		H_p = node;
		H = union_h(H, H_p);

		return key;
	}

	void deleteH (Node<T>* x)
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

	void printNode(Node<T>* node, int depth, int nodeid, int toFile = 0)
	{
		printTabs(depth);
		cout << "Depth # " << depth << " Node # "<< nodeid <<endl;
		if(toFile){
			if(depth == 0) 
			{
				if(node->sibling != NULL) file << node->key->referencias << " -> " << node->sibling->key->referencias << endl;
			}
			else file << node->parent->key->referencias << " -> " << node->key->referencias << endl;
		}
		else
		{ 
			if(depth == 0)
			{ 
				if(node->sibling != NULL) cout << node->key->referencias << " -> " << node->sibling->key->referencias << endl;
			}
			else
			{
				printTabs(depth);
				cout << node->parent->key->referencias << " -> " << node->key->referencias << endl;
			}
		}
		printTabs(depth); 
		cout << "  key (num refs) = " << node->key->referencias << endl;
		printTabs(depth);
		cout << "  degree = " << node->degree << endl;

		printTabs(depth);
		if(node->key->arraysize == 0)
			cout << "(" << node->key->direccion << ", " << *(node->key->direccion) << ") ";
		else {
			for (int k = 0; k < node->key->arraysize; k++){
				cout << "(" << node->key->direccion + k << ", " << *(node->key->direccion + k) << "), ";
			}			
		}
		cout << endl;
		
	}

	void print(int toFile, Node<T>* H = 0)
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

	void printSiblings(Node<T>* tempSibling, int depth = 0, int toFile = 0)
	{
		Node<T>* tmp = tempSibling;

		while(tempSibling != NULL)
		{
			printNode(tempSibling, depth, nNodes, toFile);
			nNodes++;
			printChild(tempSibling->child, depth, toFile);
			tempSibling = tempSibling->sibling;
		}

		tempSibling = tmp;
	}

	void printChild(Node<T>* tempChild, int depth = 0, int toFile = 0)
	{
		Node<T>* tmp = tempChild;

		while(tempChild != NULL)
		{	
			depth++;
			printNode(tempChild, depth, nNodes, toFile);
			nNodes++;
			printSiblings(tempChild->sibling, depth, toFile);
			tempChild = tempChild->child;			
		}

		tempChild = tmp;
	}

	void setZeros(Node<T>* H = 0)
	{
		if(H == 0) H = this->H;
		setZeroRef_Siblings(H);
	}

	void setZeroRef_Siblings(Node<T>* tempSibling)
	{
		Node<T>* tmp = tempSibling;
		while(tempSibling != NULL)
		{
			tempSibling->key->referencias = 0;
			setZeroRef_Child(tempSibling->child);
			tempSibling = tempSibling->sibling;
		}
		tempSibling = tmp;
	}

	void setZeroRef_Child(Node<T>* tempChild)
	{
		Node<T>* tmp = tempChild;
		while(tempChild != NULL)
		{	
			tempChild->key->referencias = 0;
			setZeroRef_Siblings(tempChild->sibling);
			tempChild = tempChild->child;			
		}
		tempChild = tmp;
	}

};

////////////////////////////////////////////////

template <class T> 
class DirMem 
{
public:
	int referencias;
	T *direccion;

	int arraysize;

	DirMem()
	{
		referencias = 0;
		direccion = NULL;
		arraysize = 0;
	}

	DirMem (T *direccion_, int arraysize_ = 0) 
	{
		referencias = 1;
		direccion = direccion_;
		arraysize = arraysize_;
	}

	~DirMem()
	{
		if(arraysize > 0)
			delete[]direccion;
	}
};

template <class T> 
class PunteroSmart 
{

public:
	static BinomialHeap<DirMem<T>*, int > B;

	DirMem <T>* dirActual;

public:	

	PunteroSmart() 
	{
		dirActual = NULL;
	}

	/*T *operator[](int i)
	{
		return *(dirActual->direccion + i);
	}*/

	void asignar(T *d, int arraysize_ = 0){
		
		if (dirActual != NULL)
		{			
			cout << "dirActual = " << *(dirActual->direccion) << endl;
			dirActual->referencias --;
		}

		DirMem <T>* dir = new DirMem <T>(d, arraysize_);

		dirActual = B.insert(dir);
	}

	void asignar(PunteroSmart <T> &p)
	{
		if (dirActual != NULL)
			dirActual->referencias--;
		p.dirActual->referencias++;

		dirActual = p.dirActual;
	}

	static void print() 
	{
		cout << "PRINTING..." << endl << endl;
		B.print(0);
		cout << endl;
	}

	static void recolectar(int finalizar = 0)
	{
		if (finalizar == 1)
		{
			cout << "Ending process" << endl;
			B.setZeros();
		}
		
		while(B.H != NULL && B.H->key->referencias == 0){
			B.extractMin();
		}
		cout << "FIN GC" << endl;
	}
};

template <class T> 
BinomialHeap<DirMem<T>*, int >  PunteroSmart<T>::B;

int main()
{
	while(1) {
		PunteroSmart<int> a;

		int n = 10000000;
		a.asignar(new int[n], n);
		PunteroSmart<int>::print();

		for(int i = 0; i < n; ++i){
			*(a.dirActual->direccion + i) = i;
		}

		PunteroSmart<int>::print();

		PunteroSmart<int> b;
		b.asignar(new int(44));
		PunteroSmart<int>::print();


		a.asignar(b);
		PunteroSmart<int>::print();

		PunteroSmart<int>::recolectar();
		PunteroSmart<int>::print();

		PunteroSmart<int>::recolectar(1);

	}
	/*
	while(1)
	{
		PunteroSmart<int> a;
		a.asignar(new int(50));
		PunteroSmart<int>::print();

		PunteroSmart<int> b;
		b.asignar(new int(10));
		PunteroSmart<int>::print();

		a.asignar(b);
		PunteroSmart<int>::print();

		PunteroSmart<int>::recolectar();
		PunteroSmart<int>::print();

		cout << "--------------" << endl;
		PunteroSmart<int> p;
		p.asignar(a);
		PunteroSmart<int>::print();

		p.asignar(b);
		PunteroSmart<int>::print();

		PunteroSmart<int>::recolectar(1);
		p.print();

	}
	*/

	system("PAUSE");
	return 0;
}
