#include "graph.h"
#include "probabilities.h"
#include "reading.h"
#include "misc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isInLinkedList(const LinkedList* LL, const char* s){ // Fonction renvoyant 1 s'il existe un maillon de nom s dans la liste chainee en argument, 0 sinon
	Node* current = LL->first;
	while(current!=NULL){
		if(strcmp(current->name, s)==0) return 1;
		current = current->next;
	}
	return 0;
}

int maxTab(const double* Probas, const int nbPages){ // Fonction retournant l'indice de la page ayant le meilleur PageRank, afin de faire le classement.
	int indexMax = 0;
	for(int i=0; i<=nbPages; i++){
		if(Probas[i]>Probas[indexMax]) indexMax = i;
	}
	return indexMax;
}


double absoluteValue(const double d){ // Fonction renvoyant la valeur abosulue d'un reel
	if(d < 0) return -d;
	return d;
}

void marginError(const double* Probas, const double* AwaitedProbas, double* MarginError, const int dim){ // Fonction calculant les marges d'erreurs de chaque resultat et les mettants dans le tableau MarginError
	for(int i=0; i<dim; i++){
		MarginError[i] = absoluteValue((Probas[i]-AwaitedProbas[i])/AwaitedProbas[i]);
	}
}	