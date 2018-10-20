/*
Задача B-1. Калькулятор для числовых множеств
Time limit:	14 s
Memory limit:	64 M

Разработайте программу-калькулятор, умеющую вычислять арифметические выражения над множествами натуральных чисел. Входные данные (выражение с описанием множеств и производимых над ними операций) подаются на стандартный поток ввода программы,
 результат вычислений должен подаваться на стандартный поток вывода.

Поддерживаемые операции: 'U' - объединение, '^' - пересечение, '\' - разность множеств, '()' - задание приоритета вычислений.
Множества задаются в виде [ el1, el2, ... ], где элементами являются обычные натуральные числа.
Множества могут быть пустыми.
Между операциями, множествами и элементами множеств может быть любое количество пробелов.

При вычислениях должны учитываться приоритеты операций (в том числе, заданные вручную при помощи круглых скобочек).

Результирующее множество должно выводиться без пробелов, в отсортированном порядке.
В случае, если введенное выражение содержит ошибку, необходимо вывести в поток стандартного вывода сообщение "[error]" и завершить выполнение программы.
*/
#include <stdlib.h>
#include <stdio.h>
#include "string.h"


struct Set{
  int* el;
  char operationAfterSet;
  size_t countEl;
  size_t priorityOperation;
};
void SetElIntoZero( struct Set *reviewedSet){
  reviewedSet -> el = NULL;
  reviewedSet -> operationAfterSet = 0;
  reviewedSet -> countEl = 0;
  reviewedSet -> priorityOperation = 0;
}

void DeleteSet( struct Set* deletedSet){
  free(deletedSet->el);
  free(deletedSet);
}

//сортировка merge без рекурсии
void SortSet(struct Set *const SortedSet){
  if ( SortedSet != NULL ) {
    size_t LengthSection = 1,
           CountEl = SortedSet->countEl,
           NumSection = 0,
           NumBetweenPointer  = 0;
    int BufferForSwap = 0,
        *FirstElForCompare = NULL,
        *SecondElForCompare = NULL;
    while( LengthSection < CountEl ){
      NumBetweenPointer = LengthSection;
      LengthSection *= 2;
      NumSection = CountEl / LengthSection;
      if (CountEl % LengthSection <= 1){
        NumSection--;
      }
      for (size_t i = 0; i <= NumSection; i++){
        FirstElForCompare = &(SortedSet->el[i * LengthSection]);
        if (i != NumSection){
          SecondElForCompare = FirstElForCompare + (NumBetweenPointer);
        }else {
          SecondElForCompare = FirstElForCompare + (CountEl - NumSection * LengthSection) / 2;
        }

        while (FirstElForCompare != SecondElForCompare){
          if (*FirstElForCompare < *SecondElForCompare) {
            NumBetweenPointer--;
          }else{
            BufferForSwap = *SecondElForCompare;
            if ( (SecondElForCompare + 1) != &(SortedSet->el[i * LengthSection])) {
              SecondElForCompare++;
            }
            memmove(FirstElForCompare+1, FirstElForCompare, NumBetweenPointer-1);
            *FirstElForCompare = BufferForSwap;
          }
          FirstElForCompare++;
        }
      }
    }
  }else{
    printf("[error]\n");
  }
}

struct Set* UnionSet(struct Set *const Set1, struct Set *const Set2){
  if ((Set1 == NULL) || (Set2 == NULL)){
    printf("[error]\n");
    return NULL;
  }
  struct Set* ResultSet = (struct Set*)malloc(sizeof(struct Set));
  if (ResultSet == NULL){
    printf("[error]\n");
    return NULL;
  }
  SetElIntoZero(ResultSet);
  ResultSet->countEl = Set1->countEl + Set2->countEl;
  ResultSet->el = (int*)malloc(ResultSet->countEl * sizeof(int));
  if (ResultSet->el == NULL){
    free(ResultSet);
    printf("[error]\n");
    return NULL;
  }
  size_t NumElOfSet1 = 0,
         NumElOfSet2 = 0,
         NumElResSet = ResultSet->countEl;
  for ( size_t i = 0; i < ResultSet->countEl; i++){
    if (NumElOfSet1 < Set1->countEl){
      if (NumElOfSet2 < Set2->countEl){
        if (Set1->el[NumElOfSet1] < Set2->el[NumElOfSet2]){
          ResultSet->el[i] = Set1->el[NumElOfSet1];
          NumElOfSet1++;
        }else{
          if (Set1->el[NumElOfSet1] > Set2->el[NumElOfSet2]){
            ResultSet->el[i] = Set2->el[NumElOfSet2];
            NumElOfSet2++;
          }else{
            (NumElResSet)--;
            ResultSet->el[i] = Set1->el[NumElOfSet1];
            NumElOfSet1++;
            NumElOfSet2++;
          }
        }
      }else{
        ResultSet->el[i] = Set1->el[NumElOfSet1];
        NumElOfSet1++;
      }
    }else{
      ResultSet->el[i] = Set2->el[NumElOfSet2];
      NumElOfSet2++;
    }
  }
  ResultSet->el = (int*)realloc(ResultSet->el, NumElResSet);//
  ResultSet->countEl = NumElResSet;
  if ( Set1->operationAfterSet == 'U'){
    ResultSet->operationAfterSet = Set2->operationAfterSet;
    ResultSet->priorityOperation = Set2->priorityOperation;
  }else{
    ResultSet->operationAfterSet = Set1->operationAfterSet;
    ResultSet->priorityOperation = Set1->priorityOperation;
  }
  *Set1 = *ResultSet;
  free(ResultSet);
  deletedSet(Set2);
  return Set1;
}

struct Set* IntersectionSet(struct Set *const Set1, struct Set *const Set2){
  if ((Set1 == NULL) || (Set2 == NULL)){
    printf("[error]\n");
    return NULL;
  }
  struct Set* ResultSet = (struct Set*)malloc(sizeof(struct Set));
  if (ResultSet == NULL){
    printf("[error]\n");
    return NULL;
  }
  SetElIntoZero(ResultSet);
  ResultSet->countEl = Set1->countEl;
  ResultSet->el = (int*)malloc(ResultSet->countEl * sizeof(int));
  if (ResultSet->el == NULL){
    free(ResultSet);
    printf("[error]\n");
    return NULL;
  }
  size_t NumElOfSet2 = Set2->countEl,
         NumResElem = 0;
  for ( size_t i = Set1->countEl; i > 0; i--){
    while ((Set1->el[i] < Set2->el[NumElOfSet2]) && (NumElOfSet2 > 0)) {
      NumElOfSet2--;
    }
    if (Set1->el[i] == Set2->el[NumElOfSet2]){
      ResultSet->el[NumResElem++] = Set1->el[i];
    }
  }
  ResultSet->el = (int*)realloc(ResultSet->el, NumResElem * sizeof(int));
  ResultSet->countEl = NumResElem;

  if ( Set1->operationAfterSet == '^'){
    ResultSet->operationAfterSet = Set2->operationAfterSet;
    ResultSet->priorityOperation = Set2->priorityOperation;
  }else{
    ResultSet->operationAfterSet = Set1->operationAfterSet;
    ResultSet->priorityOperation = Set1->priorityOperation;
  }
  *Set1 = *ResultSet;
  free(ResultSet);
  deletedSet(Set2);
  return Set1;
}

struct Set* DifferenceSet(struct Set *const subtractSet, struct Set *const dividerSet){
  if ((subtractSet == NULL) || (dividerSet == NULL)){
    printf("[error]\n");
    return NULL;
  }
  struct Set* ResultSet = (struct Set*)malloc(sizeof(struct Set));
  if (ResultSet == NULL){
    printf("[error]\n");
    return NULL;
  }
  SetElIntoZero(ResultSet);
  if ( subtractSet->operationAfterSet == '\\'){
    ResultSet->operationAfterSet = dividerSet->operationAfterSet;
    ResultSet->priorityOperation = dividerSet->priorityOperation;
  }else{
    printf("[error]\n");
    return NULL;
  }
  ResultSet->countEl = subtractSet->countEl;
  ResultSet->el = (int*)malloc(ResultSet->countEl * sizeof(int));
  if (ResultSet->el == NULL){
    free(ResultSet);
    printf("[error]\n");
    return NULL;
  }
  size_t NumElOfDivSet = dividerSet->countEl,
         NumResElem = 0;
  for ( size_t i = subtractSet->countEl; i > 0; i--){
    while ((subtractSet->el[i] < dividerSet->el[NumElOfDivSet]) && (NumElOfDivSet > 0)) {
      NumElOfDivSet--;
    }
    if (subtractSet->el[i] != dividerSet->el[NumElOfDivSet]){
      ResultSet->el[NumResElem++] = subtractSet->el[i];
    }
  }
  ResultSet->el = (int*)realloc(ResultSet->el, NumResElem * sizeof(int));
  ResultSet->countEl = NumResElem;
  *subtractSet = *ResultSet;
  free(ResultSet);
  deletedSet(dividerSet);
  return subtractSet;
}

struct Set* addSet(struct Set* Sets, size_t* NumSet, size_t* SizeArrSet){
  if (*NumSet < *SizeArrSet){
    (*NumSet)++;
    SetElIntoZero(&Sets[*NumSet]);
    return Sets;
  }else{
    struct Set* ResultSet = NULL;
    (*SizeArrSet)++;
    ResultSet = (struct Set*)realloc(Sets, ((*SizeArrSet) + 1)*sizeof(struct Set));
    if ( ResultSet == NULL){
      free(Sets);
      printf("[error]\n");
      return NULL;
    }
    return ResultSet;
  }
}

struct Set* ReadSets(const char* Str){
  if (Str == NULL){
    printf("[error]\n");
    return NULL;
  }
  struct Set* Sets = (struct Set*)malloc(sizeof(struct Set) * 2);//скорее всего должно быть минимум 2 множества
    if (Sets == NULL){
      printf("[error]\n");
      return NULL;
    }
  size_t NumSet = 0,
         SizeArrSet = 1,
         SizeArrElOfSet = 0,
         NumElOfSet = 0,
         CheckBracket = 0,
         priorityOperation = 0;
  while ( *Str != 0 ){
      switch (*Str) {
        case ',':
          if ( CheckBracket == 0){
            for(size_t i = 0; i < NumSet; i++){
              deletedSet(&Sets[i]);
            }
            printf("[error]\n");
            return NULL;
          }
        case ' ':
          Str++;
          break;
        case '[':
            if(CheckBracket == 0){
              CheckBracket = 1;
              Sets = addSet(Sets, &NumSet, &SizeArrSet);
              Sets[NumSet].priorityOperation = priorityOperation;
              SizeArrElOfSet = 3;
              NumElOfSet = 0;
              Sets[NumSet].el = (int*)malloc(sizeof(int) * SizeArrElOfSet);//в примере было по 3 числа
              if (Sets[NumSet].el == NULL){
                for(size_t i = 0; i < NumSet; i++){
                  deletedSet(&Sets[i]);
                }
                printf("[error]\n");
                return NULL;
              }
            }else{
              for(size_t i = 0; i < NumSet; i++){
                deletedSet(&Sets[i]);
              }
              printf("[error]\n");
              return NULL;
            }
            Str++;
            break;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '0':
          if (CheckBracket == 1){
            Sets[NumSet].el[NumElOfSet] =  strol(Str, Str, 10);
            NumElOfSet++;
            if (SizeArrElOfSet == NumElOfSet){
              SizeArrElOfSet *=2;
              Sets[NumSet].el = (int*)realloc(Sets[NumSet].el, SizeArrElOfSet);//
            }
          }else{
            for(size_t i = 0; i < NumSet; i++){
              deletedSet(&Sets[i]);
            }
            printf("[error]\n");
            return NULL;
          }
          break;
        case ']':
              if (CheckBracket == 1){
                CheckBracket = 0;
                Sets[NumSet].countEl = NumElOfSet;
                Sets[NumSet].el = (int*)realloc(Sets[NumElOfSet].el, sizeof(int) * NumElOfSet);//делать проверку на неудачу не нужно, т.к. идет укорачивание выделенной памяти
                SortSet(Sets);
              }else{
                for(size_t i = 0; i < NumSet; i++){
                  deletedSet(&Sets[i]);
                }
                printf("[error]\n");
                return NULL;
              }
              Str++;
              break;
        case 'U':
          if ((CheckBracket == 0)&&(Sets[NumSet].operationAfterSet == 0)){
            Sets[NumSet].operationAfterSet = 'U';
          }else{
            for(size_t i = 0; i < NumSet; i++){
              deletedSet(&Sets[i]);
            }
            printf("[error]\n");
            return NULL;
          }
          Str++;
          break;
        case '^':
          if ((CheckBracket == 0)&&(Sets[NumSet].operationAfterSet == 0)){
            Sets[NumSet].operationAfterSet = '^';
          }else{
            for(size_t i = 0; i < NumSet; i++){
              deletedSet(&Sets[i]);
            }
            printf("[error]\n");
            return NULL;
          }
          Str++;
          break;
        case '\\':
            if ((CheckBracket == 0)&&(Sets[NumSet].operationAfterSet == 0)){
              Sets[NumSet].operationAfterSet = '\\';
            }else{
              for(size_t i = 0; i < NumSet; i++){
                deletedSet(&Sets[i]);
              }
              printf("[error]\n");
              return NULL;
            }
            Str++;
            break;
        case '(':
          if (CheckBracket == 0){
            priorityOperation++;
          }else{
            for(size_t i = 0; i < NumSet; i++){
              deletedSet(&Sets[i]);
            }
            printf("[error]\n");
            return NULL;
          }
          Str++;
          break;
        case ')':
          if ((priorityOperation > 0)&&(CheckBracket == 0)){
            priorityOperation--;
          }else{
            for(size_t i = 0; i < NumSet; i++){
              deletedSet(&Sets[i]);
            }
            printf("[error]\n");
            return NULL;
          }
          Str++;
          break;
        case 0:
          if (priorityOperation != 0){
            for(size_t i = 0; i < NumSet; i++){
              deletedSet(&Sets[i]);
            }
            printf("[error]\n");
            return NULL;
          }
          Sets[NumSet].operationAfterSet = 0;
          Sets = (struct Set*)realloc(Sets, sizeof(struct Set) * NumSet);
          break;
        default:
          for(size_t i = 0; i < NumSet; i++){
            deletedSet(&Sets[i]);
          }
          printf("[error]\n");
          return NULL;
      }
  }
  return Sets;
}

struct Set* RunOperation(struct Set* Sets){
  size_t NumSet = 0;
  size_t NumSetWithMaxpriority = 0;
  while(Sets->operationAfterSet != 0){
    while(Sets[NumSet].operationAfterSet != 0){
      if (Sets[NumSet].priorityOperation > Sets[NumSetWithMaxpriority].priorityOperation){
        NumSetWithMaxpriority = NumSet;
      }
    }
    if (Sets[NumSetWithMaxpriority].operationAfterSet != 0){
        if (Sets[NumSetWithMaxpriority].priorityOperation == Sets[NumSetWithMaxpriority + 1].priorityOperation){
          switch (Sets[NumSetWithMaxpriority].operationAfterSet) {
            case 'U':
              UnionSet(&Sets[NumSetWithMaxpriority], &Sets[NumSetWithMaxpriority + 1]);
              break;
            case '^':
              IntersectionSet(&Sets[NumSetWithMaxpriority], &Sets[NumSetWithMaxpriority + 1]);
              break;
            case '\\':
              DifferenceSet(&Sets[NumSetWithMaxpriority], &Sets[NumSetWithMaxpriority + 1]);
          }
        }
    }else{
      Sets[NumSetWithMaxpriority].priorityOperation--;
    }
  }
  return Sets;
}

void OutputSet(struct Set *const Set){
  printf("[");
  for(size_t i = 0; i < Set->countEl - 1; i++){
    printf("%d,", Set->el[i]);
  }
  printf("%d]\n", Set->el[Set->countEl - 1]);
}

int main(int argc, char const *argv[]) {
  struct Set *Sets = NULL;
  char *Str = (char*)malloc(sizeof(char)*80);
  scanf("%s\n", Str);
  Sets = ReadSets(Str);
  free(Str);
  if (Sets != NULL){
    Sets = RunOperation(Sets);
  }
  if (Sets != NULL){
    OutputSet(Sets);
  }
  return 0;
}
