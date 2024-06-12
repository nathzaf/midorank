#ifndef READING_H
#define READING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define V_MAX 200000
#define SP "Super Noeud"

int getLines(const char fileName[100], char* resultat[V_MAX]);

int matrixDimension(const char fileName[100]);

void readMatrix(int** M, const char fileName[100], const int dim, char* awaitedProbas);

double* getProbasMatrix(char* awaitedProbasStr, const int dim);

int* getNeighboursMatrix(int** M, const int dim);

#endif