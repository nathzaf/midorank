#include "graph.h"
#include "probabilities.h"
#include "misc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double* initProbas(const int nbPages){ // Fonction initalisant les probabilites du graphe d'un wiki (1/n pour tous les sommets et 0 pour le super neoud situé en 0)
	double* Probas = malloc((nbPages+1)*sizeof(double));
	if(Probas==NULL) return NULL;
	Probas[0]=0;
	for(int i=1; i<=nbPages; i++) Probas[i]=1/((double)nbPages);
	return Probas;
}

double probas(LinkedList** G, const int s, const int nbPages, const double* CurrentProbas, const int* nbNeighbours){ //Fonction calculant la probabilité rafraichit d'un sommet 
	const double epsilon = 1/((double)nbPages*10);
	double prob=0;
	for(int u=0; u<=nbPages; u++){
		if((isInLinkedList(G[u], G[s]->first->name))==1){ // Entrée dans la somme sur u, predecesseurs de s : u est predecesseur de s si s est present dans la liste chainne Adj[u]
			
			if(u==0 && s==0) continue; // Car le super noeud ne pointe pas sur lui meme
			
			if(u==0) prob += (CurrentProbas[u])/((double)nbPages); // Cas ou u est super noeud
			
			else if(s==0) prob += epsilon*CurrentProbas[u]; // Cas ou s est super noeud
			
			else prob += ((1.0-epsilon)*CurrentProbas[u])/(nbNeighbours[u]); // Dernier cas
		}
	}
	return prob;
}

void refreshProbas(LinkedList** G, const int nbPages, double* CurrentProbas, double* RefreshedProbas, const int* nbNeighbours){ //Fonction actualisant les probas de tous les sommets dans un tableau rereshed afin de ne pas biaiser les resultats
	for(int s=0; s<=nbPages; s++){
		RefreshedProbas[s] = probas(G, s, nbPages, CurrentProbas, nbNeighbours);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

double* initProbasMat(const int dim){ // Fonction initialisant les probabilites du graphe de matrices (1/n pour tous les sommets et 0 pour le super noeud situé en 0)
	double* ProbaMatrix = malloc(dim*sizeof(double));
	if(ProbaMatrix==NULL) return NULL;
	ProbaMatrix[0]=0.0;
	for(int i=1;i<dim;i++) ProbaMatrix[i]=1/((double)dim-1.0);
	return ProbaMatrix;
}

double probaMat(int** M,int dim, int s, double* Probas, int *nbNeighbours){ //Fonction calculant la probabilité rafraichit d'un sommet version matrice
	const double epsilon = 1.0/(10.0*((double)dim-1));
	double proba=0;
	for(int u=0; u<dim; u++){
		if(M[u][s]==1){
		
			if(u==0) proba += Probas[u]/((double)dim-1.0); //cas ou u est super noeud

			else if(s==0) proba += epsilon*Probas[u]; // cas ou s est super noeud
			
			else proba += ((1-epsilon)*Probas[u])/((double)nbNeighbours[u]); // dernier cas
		}
	}
	return proba;
}

void refreshProbasMat(int ** M, const int dim, double* Probas, double* RefreshedProbas, int* nbNeighbours){ //Fonction actualisant les probas de tous les sommets dans un tableau rereshed afin de ne pas biaiser les resultats version matrice
	for(int s=0; s<dim; s++){
		RefreshedProbas[s] = probaMat(M, dim, s, Probas, nbNeighbours);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void checkProbas(const int nbPages, const double* Probas){ // Fonction permettant d'afficher la somme des probas
	double sum = 0;
	for(int i=0; i<=nbPages; i++) sum+=Probas[i];
	printf("Sum of all probabilites : %lf\n", sum);
}