#ifndef SORTING_H
#define SORTING_H
#include "Vehicle.h"

//key: 1=ID, 2=Capacity, 3=type, 4=plate
void insertionSort(Vehicle q[],int T,int key);
void mergeSort(Vehicle q[],int T,int key);
int binarysearch(Vehicle q[],int T,const string& id);
#endif
