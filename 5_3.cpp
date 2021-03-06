// Во всех задачах данного раздела необходимо реализовать и использовать сортировку слиянием. Общее время работы алгоритма O(n log n).
/*Закраска прямой 1.
На числовой прямой окрасили N отрезков. Известны координаты левого и правого концов каждого отрезка (Li и Ri). 
Найти длину окрашенной части числовой прямой.
*/

#include "stdafx.h"

#include<iostream>
#include<cstring>

template <class T, class Cmp>
void MergeSort(T Arr[], size_t SizeArr)
{
	Cmp Compare;
	size_t LengthSection = 1,
		NumBetweenPointer = 0,
		NumSection = 0,
		NumFirstElForCompare = 0,
		NumSecondElForCompare = 0;
	T Buffer;
	while (LengthSection  < SizeArr)
	{
		NumBetweenPointer = LengthSection;
		LengthSection *= 2;
		NumSection = SizeArr / LengthSection;
		if (SizeArr % LengthSection <= 1) {
			NumSection--;//теперь NumSection - индекс
		}
		for (size_t i = 0; i <= NumSection; i++)
		{
			NumFirstElForCompare = i * LengthSection;
			NumSecondElForCompare = NumFirstElForCompare + NumBetweenPointer;

			while (NumFirstElForCompare != NumSecondElForCompare)
			{
				if (Compare(Arr[NumFirstElForCompare], Arr[NumSecondElForCompare]))//если первый< второго 
				{
					Buffer = Arr[NumSecondElForCompare];
					if (((NumSecondElForCompare + 1) < (i + 1) * LengthSection) && (NumSecondElForCompare + 1 < SizeArr))
					{
						NumSecondElForCompare++;
					}
					memmove(&Arr[NumFirstElForCompare + 1], &Arr[NumFirstElForCompare], (NumBetweenPointer - 1) * sizeof(T));
					Arr[NumFirstElForCompare] = Buffer;
				}
				NumFirstElForCompare++;
			}
		}
	}
}


struct straightLine
{
	long int begin;
	long int end;
};

class cmpLine {
public:
	bool operator() (const straightLine& line1, const straightLine& line2) const
	{
		return line1.begin > line2.begin;
	}
};

long int countLay(struct straightLine Arr[], size_t n)
{
	MergeSort<struct straightLine, cmpLine>(Arr, n);
	size_t NumBegLine = 0;
	long int LengthLay = 0;
	for (size_t i = 1; i < n; i++) {
		if (Arr[NumBegLine].end < Arr[i].end)
		{
			if (Arr[NumBegLine].end <= Arr[i].begin)
			{
				LengthLay += Arr[NumBegLine].end - Arr[NumBegLine].begin;
			}
			else {
				LengthLay += Arr[i].begin - Arr[NumBegLine].begin;
			}
			NumBegLine = i;
		}
	}
	LengthLay += Arr[NumBegLine].end - Arr[NumBegLine].begin;
	return LengthLay;
}

struct straightLine* InputData(size_t* sizeArr) {
	struct straightLine* inputArr = nullptr;
	size_t sizeofArr = 0;
	std::cin >> sizeofArr;
	inputArr = new struct straightLine[sizeofArr];
	for (size_t i = 0; i < sizeofArr; i++) {
		std::cin >> inputArr[i].begin;
		std::cin >> inputArr[i].end;
	}
	*sizeArr = sizeofArr;
	return inputArr;
}

int main()
{
	size_t n = 0;
	struct straightLine* Arr = nullptr;
	Arr = InputData(&n);
	std::cout << countLay(Arr, n);
    return 0;
}

