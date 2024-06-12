#ifndef MISC_H
#define MISC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isInLinkedList(const LinkedList* LL, const char* s);

int maxTab(const double* Probas, const int nbPages);

double absoluteValue(const double d);

void marginError(const double* Probas, const double* AwaitedProbas, double* MarginError, const int dim);

#endif