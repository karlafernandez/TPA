// Binomial Heap.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cstdlib>

using namespace std;

struct nodo
{
    int n;
    int degree;
    nodo* parent;
    nodo* child;
    nodo* sibling;
};

class BinomialHeap
{
    private:
        nodo *H;
        nodo *Hr;
        int count;
    public:
        nodo* Initializeheap();
        int Binomial_link(nodo*, nodo*);
        nodo* Create_nodo(int);
		nodo* Insert(nodo*, nodo*);
		nodo* Merge(nodo*, nodo*);
        nodo* Union(nodo*, nodo*);
        nodo* Extract_Min(nodo*);
        int Display(nodo*);
        nodo* Buscar(nodo*, int);
        int Decrease_key(nodo*, int, int);
        int Delete(nodo*, int);
        BinomialHeap()
        {
            H = Initializeheap();
            Hr = Initializeheap();
            int count = 1;
        }
};

nodo* BinomialHeap::Initializeheap()
{
    nodo* np;
    np = NULL;
    return np;
}

int BinomialHeap::Binomial_link(nodo* y, nodo* z)
{
    y->parent = z;
    y->sibling = z->child;
    z->child = y;
    z->degree = z->degree ++;
}

nodo* BinomialHeap::Create_nodo(int k)
{
    nodo* p = new nodo;
    p->n = k;
    return p;
}


nodo* BinomialHeap::Insert(nodo* H, nodo* x)
{
    nodo* H1 = Initializeheap();
    x->parent = NULL;
    x->child = NULL;
    x->sibling = NULL;
    x->degree = 0;
    H1 = x;
    H = Union(H, H1);
    return H;
}

nodo* BinomialHeap::Merge(nodo* H1, nodo* H2)
{
    nodo* H = Initializeheap();
    nodo* y;
    nodo* z;
    nodo* a;
    nodo* b;
    y = H1;
    z = H2;
    if (y != NULL)
    {
        if (z != NULL)
        {
            if (y->degree <= z->degree)
                H = y;
            else if (y->degree > z->degree)
                H = z;
        }
        else
            H = y;
    }
    else
        H = z;
    while (y != NULL && z != NULL)
    {
        if (y->degree < z->degree)
        {
            y = y->sibling;
        }
        else if (y->degree == z->degree)
        {
            a = y->sibling;
            y->sibling = z;
            y = a;
        }
        else
        {
            b = z->sibling;
            z->sibling = y;
            z = b;
        }
    }
    return H;
}

nodo* BinomialHeap::Union(nodo* H1, nodo* H2)
{
    nodo *H = Initializeheap();
    H = Merge(H1, H2);
    if (H == NULL)
        return H;
    nodo* prev_x;
    nodo* next_x;
    nodo* x;
    prev_x = NULL;
    x = H;
    next_x = x->sibling;
    while (next_x != NULL)
    {
        if ((x->degree != next_x->degree) || ((next_x->sibling != NULL)
            && (next_x->sibling)->degree == x->degree))
        {
            prev_x = x;
            x = next_x;
        }
        else
	    {
            if (x->n <= next_x->n)
            {
                x->sibling = next_x->sibling;
                Binomial_link(next_x, x);
            }
            else
            {
                if (prev_x == NULL)
                    H = next_x;
                else
                    prev_x->sibling = next_x;
                Binomial_link(x, next_x);
                x = next_x;
            }
	    }
        next_x = x->sibling;
    }
    return H;
}

nodo* BinomialHeap::Extract_Min(nodo* H1)
{
    Hr = NULL;
    nodo* t = NULL;
    nodo* x = H1;
    if (x == NULL)
    {
        cout<<"No hay nodos para extraer"<<endl;
        return x;
    }
    int min = x->n;
    nodo* p = x;
    while (p->sibling != NULL)
    {
        if ((p->sibling)->n < min)
        {
            min = (p->sibling)->n;
            t = p;
            x = p->sibling;
        }
        p = p->sibling;
    }
    return x;
}


