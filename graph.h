#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define V_MAX 200000
#define SP "Super Noeud"

struct node{
	char* name; // nom de la page
	struct node* next;
};

typedef struct node Node;

struct linkedlist{
	Node *first; // une liste chainée est définie par son premier element 
};

typedef struct linkedlist LinkedList;

Node* createNode(const char *s);

LinkedList* createLinkedList(char* line, int* nbNeighbours, const int pos);

LinkedList* superNoeud(LinkedList** G, const int nbPages);

LinkedList** createGraph(const int nbPages, char* file_lines[V_MAX], int* nbNeighbours);

void redLinks(LinkedList** G, const int nbPages, int* nbNeighbours);

#endif