/*
Решение всех задач данного раздела предполагает использование кучи, реализованной в виде класса.
4_1.Жадина.
Вовочка ест фрукты из бабушкиной корзины.В корзине лежат фрукты разной массы.Вовочка может поднять не более K грамм.
Каждый фрукт весит не более K грамм.За раз он выбирает несколько самых тяжелых фруктов, которые может поднять одновременно, 
откусывает от каждого половину и кладет огрызки обратно в корзину.Если фрукт весит нечетное число грамм, 
он откусывает большую половину.Фрукт массы 1гр он съедает полностью.
Определить за сколько подходов Вовочка съест все фрукты в корзине.
Формат входных данных.Вначале вводится n - количество фруктов и n строк с массами фруктов.Затем K - "грузоподъемность".
Формат выходных данных.Неотрицательное число - количество подходов к корзине.
class T
*/

#include "stdafx.h"

#include <stdio.h>
#include <iostream>
using namespace  std;

#include <cstring>//для memcpy


template <class T>
class myHeap
{
public:
	myHeap();
	myHeap(size_t NumElOfArr, T ArrOfEl[]);
	myHeap(size_t NumElOfArr, size_t NumMaxEl, T ArrOfEl[]);
	~myHeap();
	void AddEl(T Element);
	T getMax();
	T getValueMax();
	T getLastEl();
	size_t getNumElOfHeap();
protected:
	void SiftDown(size_t NumOfEl);
	void SiftUp(size_t NumOfEl);
	void swap(T& El1, T& El2);
	T* Heap;
	size_t NumElOfHeap;
	size_t NumMaxElInBuf;
};

#define NumStartMaxElInBuf 8
template <class T>
myHeap<T>::myHeap()
{
	Heap = new T[NumStartMaxElInBuf];
	NumElOfHeap = 0;
	NumMaxElInBuf = NumStartMaxElInBuf;
}

template <class T>
myHeap<T>::myHeap(size_t NumElOfArr, T ArrOfEl[])
{
	Heap = ArrOfEl;
	NumElOfHeap = NumElOfArr;
	NumMaxElInBuf = NumElOfArr;
	for (int k = (NumElOfHeap / 2 - 1); k >= 0; --k) {
		SiftDown(k);
	}
}
template <class T>
myHeap<T>::myHeap(size_t NumElOfArr, size_t NumMaxEl, T ArrOfEl[])
{
	Heap = ArrOfEl;
	NumElOfHeap = NumElOfArr;
	NumMaxElInBuf = NumMaxEl;
	for (int i = (NumElOfHeap / 2 - 1); i >= 0; --i) {
		SiftDown(i);
	}
}

template <class T>
myHeap<T>::~myHeap()
{
	delete[] Heap;
}

template <class T>
void myHeap<T>::AddEl(T Element)
{
	if (NumElOfHeap == NumMaxElInBuf) {
		T* BufHeap = Heap;
		NumMaxElInBuf *= 2;
		Heap = new T[NumMaxElInBuf];
		memcpy(Heap, BufHeap, sizeof(T)*NumElOfHeap);
		delete[] BufHeap;
	}
	Heap[NumElOfHeap] = Element;
	SiftUp(NumElOfHeap++);
}

template <class T>
T myHeap<T>::getMax()
{
	swap(Heap[0], Heap[--NumElOfHeap]);
	SiftDown(0);
	return Heap[NumElOfHeap];
}

template <class T>
T myHeap<T>::getValueMax()
{
	return Heap[0];
}

template <class T>
T myHeap<T>::getLastEl()
{
	T BufEl = Heap[--NumElOfHeap];
	return BufEl;
}

template <class T>
void myHeap<T>::SiftDown(size_t NumOfEl)
{
	size_t leftChild = 2 * NumOfEl + 1;
	size_t rightChild = 2 * NumOfEl + 2;
	size_t largest = NumOfEl;
	if (leftChild < NumElOfHeap && Heap[leftChild] > Heap[largest])
		largest = leftChild;
	if (rightChild < NumElOfHeap && Heap[rightChild] > Heap[largest])
		largest = rightChild;
	if (largest != NumOfEl) {
		swap(Heap[NumOfEl], Heap[largest]);
		SiftDown(largest);
	}
}
template <class T>
void myHeap<T>::SiftUp(size_t NumOfEl)
{
	while (NumOfEl > 0) {
		int parent = (NumOfEl - 1) / 2;
		if (Heap[NumOfEl] <= Heap[parent])
			break;
		swap(Heap[NumOfEl], Heap[parent]);
		NumOfEl = parent;
	}
}

template <class T>
size_t myHeap<T>::getNumElOfHeap()
{
	return NumElOfHeap;
}

template <class T>
void myHeap<T>::swap(T& El1, T& El2)
{
	T buf = El1;
	El1 = El2;
	El2 = buf;
}

int* InputData(size_t* sizeArr) {
	int* inputArr = nullptr;
	size_t sizeofArr = 0;
	cin >> sizeofArr;
	inputArr = new int[sizeofArr];
	for (size_t i = 0; i < sizeofArr; i++) {
		cin >> inputArr[i];
	}
	*sizeArr = sizeofArr;
	return inputArr;
}

size_t CountStepForFill(size_t n, int K, int Arr[])
{
	int Step = 0,
		*buf = new int[K];
	size_t count = 0,
		NumElOfBuf = 0;
	myHeap<int> heap(n, Arr);
	while (heap.getNumElOfHeap() > 0)
	{
		Step = K;
		NumElOfBuf = 0;
		while ((Step>= heap.getValueMax()) && (heap.getNumElOfHeap() > 0))
		{
			buf[NumElOfBuf] = heap.getMax();
			Step -= buf[NumElOfBuf++];
		}
		count++;
		for (size_t i = 0; i < NumElOfBuf; i++) {
			if (buf[i] > 1) {
				heap.AddEl(buf[i] / 2);
				buf[i] = 0;
			}
			
		}
	}
	delete[] buf;
	return count;
}

int main()
{
	size_t n = 0;
	int K = 0,
		*Arr = nullptr;
	Arr = InputData(&n);
	cin >> K;
	cout << CountStepForFill(n, K, Arr);
    return 0;
}

