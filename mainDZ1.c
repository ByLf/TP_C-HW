/*
Задача A-4. Задача о простых сомножителях
Time limit:	14 s
Memory limit:	64 M

Составить программу разложения положительного целого числа на простые сомножители и единицу.

Программа считывает входные данные со стандартного ввода, и печатает результат в стандартный вывод.

Верными входными данными считается только ровно одно положительное число, не превосходящее 2^63 - 1, возможно с предшествующими или последующими пробельными символами.

Хотя единица не входит в каноническое разложение, но в ответе первым элементом всегда необходимо выводить единицу.
Считать, что разложение самой единицы состоит только из единицы.

Процедура разложения числа должна быть оформлена в виде отдельной функции, которой на вход подается целое число.
Функция должна возвращать указатель на массив целых чисел, содержащий разложение введенного числа на простые сомножители.
Последний элемент этого массива должен быть нулевым.

Программа должна уметь обрабатывать ошибки - такие как неверные входные данные(не число, отрицательное число), ошибки выделения памяти и т.п.
В случае возникновения ошибки нужно вывести сообщение "[error]" (без кавычек) и завершать выполнение программы.
*/
#include <stdio.h>
#include "stdlib.h"
#include <string.h>
#include <math.h>

#define MINSIZEOFARRAY 3
unsigned long long* DecompositionNumber(unsigned long long Number){
  if (Number == 0){
    printf("[error]\n");
    return NULL;
   }
  unsigned long long _endOfCalculation = (unsigned long long)roundl(sqrt(Number));
  unsigned long long *result = (unsigned long long*)calloc( MINSIZEOFARRAY, sizeof(unsigned long long) );
  if (result == NULL){
    printf("[error]\n");
    return NULL;
  }
  size_t _NumUsedElem = 1;
  size_t _SizeOfArr = MINSIZEOFARRAY;// лучший вариант - простое число -> массив будет (1, число, 0)
  result[0] = 1;
  unsigned long long i = 2;
  while(i <= _endOfCalculation){
    if (Number%i != 0){
      i++;
    }else{
      result[_NumUsedElem++] = i;
      Number /= i;
      if (_NumUsedElem == _SizeOfArr){
        _SizeOfArr *= 2;
        result = (unsigned long long*)realloc(result, _SizeOfArr*sizeof(unsigned long long));
        if (result == NULL){
          printf("[error]\n");
          return NULL;
        }
      }
      _endOfCalculation = (unsigned long long)roundl(sqrt(Number));
    }
  }
  if( Number > 1){
    result[_NumUsedElem++] = Number;
  }
  if (_NumUsedElem == _SizeOfArr){
    _SizeOfArr++;
    result = (unsigned long long*)realloc(result, _SizeOfArr*sizeof(unsigned long long));
    if ((result = NULL)){
      printf("[error]\n");
      return NULL;
    }
  }else{
    result = (unsigned long long*)realloc(result, (_NumUsedElem + 1)*sizeof(unsigned long long));
    if (result == NULL){
      printf("[error]\n");
      return NULL;
    }
  }
  result[_NumUsedElem] = 0;
  return result;
}

#define SIZEBUFSTR 4 // диапазон int - 63к -> гарантированно будет обработано все до 10000
unsigned long long TransformStrToNumber( const char* Str){
  if ( Str == NULL) {
    printf("[error]\n");
    return 0;
  }
  size_t ProcessingStr = strlen(Str);
  if (ProcessingStr == 0){
    printf("[error]\n");
    return 0;
  }
  size_t BeginProcessing = 0;
  int buffer = 0;
  char BufStr[SIZEBUFSTR+1] = { 0 };
  char* PointerForCheck = NULL;
  char* StrForCompare = "0123456789";
  unsigned long long Number = 0;
  unsigned long long degree = 1;
  while ((Str[BeginProcessing] == ' ') && (ProcessingStr > BeginProcessing)){
    BeginProcessing++;
  }
  while((Str[ProcessingStr] == ' ')&&(ProcessingStr > BeginProcessing)){
    ProcessingStr--;
  }
  ProcessingStr -= BeginProcessing;
  if (BeginProcessing > 0) { memmove((void*)Str, Str+BeginProcessing, ProcessingStr); }
  BeginProcessing = 0;
  while (ProcessingStr >  0) {
    for( size_t i = 0; i < SIZEBUFSTR; i++){
      BufStr[i] = 0;
    }
    if ( ProcessingStr > SIZEBUFSTR ){
      ProcessingStr = ProcessingStr - SIZEBUFSTR;
      strncpy(BufStr, &Str[ProcessingStr], SIZEBUFSTR);
    }else {
      strncpy(BufStr, &Str[0], ProcessingStr);
      ProcessingStr = 0;
    }
    BufStr[SIZEBUFSTR] = 0;
    for(size_t i = 0; i < SIZEBUFSTR; i++){
      PointerForCheck = NULL;
      PointerForCheck = strchr( StrForCompare, BufStr[i]);
      if (PointerForCheck == NULL) {
        printf("[error]\n");
        return 0;
       }
    }
    buffer = atoi(BufStr);
    if(buffer < 0){
      printf("[error]\n");
      return 0;
    }
    Number += buffer * degree;
    degree *= (unsigned long long)pow(10,(SIZEBUFSTR));
  }
  if (Number == 0) { printf("[error]\n"); }
  return Number;
}

#define MAXSIZESTR 80
unsigned long long InputNumber()
{
  unsigned long long Number = 0;
  short int CheckForReading = 0;
  char* readingStr = (char*)calloc(MAXSIZESTR, sizeof(char));
  if (readingStr == NULL) {
      printf("[error]\n");
      return 0;
  }
  CheckForReading = scanf("%s\n", readingStr);
  if (CheckForReading == 0) {
    printf("[error]\n");
    return 0;
  }
  Number = TransformStrToNumber(readingStr);
  free(readingStr);
  return Number;
}

void OutputArrayLongLong( const unsigned long long Array[]) {
  size_t i = 0;
  short int CheckForWriting = 0;
  while(Array[i] != 0){
    CheckForWriting = printf("%llu ", Array[i]);
    if (CheckForWriting == 0) { break; }
    i++;
  }
}

int main() {
  unsigned long long Number = 0;
  unsigned long long* array = NULL;
  Number = InputNumber();
  if (Number != 0) { array = DecompositionNumber(Number); }
  if (array != NULL) { OutputArrayLongLong(array); }
  free(array);
  return 0;
}
