/*
Во всех задачах из следующего списка следует написать структуру данных, обрабатывающую команды push* и pop*.
Формат входных данных.
В первой строке количество команд n. n ≤ 1000000.
Каждая команда задаётся как 2 целых числа: a b.
a = 1 - push front
a = 2 - pop front
a = 3 - push back
a = 4 - pop back
Команды добавления элемента 1 и 3 заданы с неотрицательным параметром b.
Для очереди используются команды 2 и 3. Для дека используются все четыре команды.
Если дана команда pop*, то число b - ожидаемое значение. Если команда pop вызвана для пустой структуры данных, то ожидается “-1”.
Формат выходных данных.
Требуется напечатать YES - если все ожидаемые значения совпали. Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.

*/
/*
3_3. Реализовать очередь с помощью двух стеков. Использовать стек, реализованный с помощью динамического буфера.
*/
// части реализации структуры "стек" взяты из лекции (с небольшой доработкой) 
#include "stdafx.h"


#include <iostream>
//копипаста семинара
struct Operation {
	int Code = 0;
	int Value = 0;
};

class Stack {
public:
	Stack(size_t _bufferSize = 64);
	virtual ~Stack();
	// Добавление и извлечение элемента из стека.
	void Push(int a);
	int Pop();
	//Переворачивание переданного стека, записывая результат в этот стек.
	void CopyWithInverse(Stack* const source);
	// Проверка на пустоту.
	bool IsEmpty() const { return top == -1; }
private:
	int* buffer;
	size_t bufferSize;
	int top; // Индекс верхнего элемента.
};

Stack::Stack(size_t _bufferSize) :
	bufferSize(_bufferSize),
	top(-1)
{
	buffer = new int[bufferSize]; // Создаем буфер.
}
Stack::~Stack()
{
	delete[] buffer; // Удаляем буфер.
}
// Добавление элемента.
void Stack::Push(int a)
{
	++top;
	buffer[top] = a;
}
// Извлечение элемента.
int Stack::Pop()
{
	if (top > -1) {
		top--;
		return buffer[top + 1];
	}
	else {
		return -1;
	}
}


//Переворачивание переданного стека, записывая результат в этот стек.
void Stack::CopyWithInverse(Stack* const source)
{
	int _bufferForCopy(0);
	for (int i = source->top; i >= 0; i--) {
		_bufferForCopy = source->Pop();
		this->Push(_bufferForCopy);
	}
}

class myQueue
{
public:
	myQueue(size_t _bufferSize);
	myQueue(const myQueue& other) = delete;
	~myQueue();

	myQueue& operator=(const myQueue& other) = delete;
	void Push(int value);
	int Pop();
private:
	size_t bufferSize;
	Stack *StackForPop,
		*StackForPush;
};

myQueue::myQueue(size_t _bufferSize) :
	bufferSize(_bufferSize)
{
	StackForPop = new Stack(_bufferSize);
	StackForPush = new Stack(_bufferSize);
}
void myQueue::Push(int value) {
	if (!(StackForPop->IsEmpty())) {
		StackForPush->CopyWithInverse(StackForPop);
	}
	StackForPush->Push(value);
}
int myQueue::Pop() {
	if (!(StackForPush->IsEmpty())) {
		StackForPop->CopyWithInverse(StackForPush);
	}
	return StackForPop->Pop();
}
myQueue::~myQueue()
{
	delete StackForPop;
	delete StackForPush;
}


bool do_operation(myQueue& queue, Operation op)
{
	switch (op.Code) {
	case 3:
		queue.Push(op.Value);
		return true;
	case 2: {
		const int result = queue.Pop();
		return result == op.Value;
	}
	}

	return false;
}

int main()
{
	int n = 0;
	std::cin >> n;

	myQueue queue(32);
	for (int i = 0; i < n; i++) {
		Operation op;
		std::cin >> op.Code >> op.Value;
		if (!do_operation(queue, op)) {
			std::cout << "NO";
			return 0;
		}
	}

	std::cout << "YES";

	return 0;
}
