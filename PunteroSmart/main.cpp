#include <iostream>
#include <stdio.h>

#include <list>
#include <cstdlib>
#include <string.h>

using namespace std;


template <class T> 
class DirMem 
{
public:
	int referencias;
	T *direccion;

	int arraysize;

	DirMem (T *direccion_, int arraysize_ = 0) 
	{
		referencias = 1;
		direccion = direccion_;

		arraysize = arraysize_;
	}

	~DirMem()
	{
	}
};

template <class T> 
class PunteroSmart 
{

typedef typename list<DirMem<T> >::iterator Iterator;

public:
	static list<DirMem<T> > listaMemoria;

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

		DirMem <T> dir(d, arraysize_);
		listaMemoria.push_back(dir);
		//if(d != NULL)
		//	cout << *d << endl;

		dirActual = &listaMemoria.back();
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
		int i = 0;
		for (Iterator it = listaMemoria.begin(); it != listaMemoria.end(); it++){
			cout << "Elemento " << i << ", " << "Num refs = " << (*it).referencias <<" : ";
			
			if( (*it).arraysize == 0)
				cout << "(" << (*it).direccion << ", " << *((*it).direccion) << ") ";
			else {
				for (int k = 0; k < (*it).arraysize; k++){
					cout << "(" << (*it).direccion + k << ", " << *((*it).direccion + k) << "), ";
					//cout << "(" << (*it).direccion << ", " << *((*it).direccion) << ", " << (*it).referencias << "), ";
				}			
			}
			cout << endl;
			i++;
		}
		cout << endl << endl;
	}

	static void recolectar(int finalizar = 0)
	{
		if (finalizar == 1)
		{
			cout << "Ending process" << endl;
			for(Iterator it = listaMemoria.begin(); it != listaMemoria.end(); it++)
			{
				(*it).referencias = 0;
			}
		}
		
		int i = listaMemoria.size();
		Iterator it = listaMemoria.begin();

		while(i > 0 && it != listaMemoria.end()){
			i--;
			if ((*it).referencias > 0)
				continue;
			listaMemoria.erase(it++);
		}
		cout << "FIN GC" << endl;
		
	}
};



template <class T> 
list<DirMem<T> > PunteroSmart<T>::listaMemoria;


int main() {

	//int *p, *q;
	//p = new int(99);
	//q = p; // now, q points to same memory as p
	//p = new int(100);

	//int *p;
	//p = new int(99);
	//p = new int(100);


	while(1) {
		PunteroSmart<int> a;

		int n = 1000000;
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

	system("PAUSE");

	return 1;
}



//
//PunteroSmart<int> p;
//PunteroSmart<int> q;
//
//p = new int(99);
//
//q = p;
//
//p = new int(100);



/*
while(1)
	{
		PunteroSmart<int> a;
		a.asignar(new int(50));
		PunteroSmart<int>::print();

		PunteroSmart<int> b;
		b.asignar(new int(100));
		PunteroSmart<int>::print();

		a.asignar(b);
		PunteroSmart<int>::print();

		PunteroSmart<int>::recolectar();
		PunteroSmart<int>::print();


		PunteroSmart<int> p;
		p.asignar(a);
		PunteroSmart<int>::print();

		p.asignar(b);
		PunteroSmart<int>::print();

		PunteroSmart<int>::recolectar(1);
		p.print();
	}
*/