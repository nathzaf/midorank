#ifndef PROBABILITIES_H
#define PROBABILITIES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double* initProbas(const int nbPages);

double probas(LinkedList** G, const int s, int nbPages, const double* CurrentProbas, const int* nbNeighbours);

void refreshProbas(LinkedList** G, const int nbPages, double* CurrentProbas, double* RefreshedProbas, const int* nbNeighbours);

double* initProbasMat(const int dim);

double probaMat(int** M, int dim, int s, double* Probas, int *nbNeighbours);

void refreshProbasMat(int ** M, const int dim, double* CurrentProbas, double* RefreshedProbas, int* nbNeighbours);

void checkProbas(const int nbPages, const double* Probas);

#endif