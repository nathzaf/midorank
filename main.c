#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "probabilities.h"
#include "reading.h"
#include "misc.h"

#define V_MAX 200000
#define SP "Super Noeud"


int main(int argc, char* argv[]){
	if(strcmp(argv[1],"-n")==0){ //Entrée dans le mode n
	
		char* file_lines[V_MAX]; // Creation d'un tableau de chaines de caracteres qui va contenir toutes les lignes du fichier wiki
		int nbPages;
	
		for(int i=0; i<V_MAX; i++){ 
			file_lines[i] = malloc(V_MAX*sizeof(char));
			if(file_lines[i]==NULL) return 1;	
		}
		
		nbPages = getLines(argv[2], file_lines); // Execution de la fonction recuperant les lignes du fichier texte
		
		printf("\nThere are %d pages ", nbPages);
	
		int* nbNeighbours = malloc((nbPages+1)*sizeof(int)); // Initialisation d'un tableau tel que nbNeighbours[i] donne le nombre de voisins sortants de la page numero i

		LinkedList** Adj = createGraph(nbPages, file_lines, nbNeighbours); // Creation du graphe en lui meme. Le graphe est un tableau de liste chainées tel que Adj[i] est la liste chaine de la i eme ligne du fichier txt (et Adj[0] le super noeud)
		
		for(int i=0; i<V_MAX; i++) free(file_lines[i]);

		redLinks(Adj, nbPages, nbNeighbours); // Suppression de tous les liens rouges + correction du nombre de voisins sortants

		double* Probas = initProbas(nbPages); // Initialisation d'un tableau de reels qui contiendra toutes les probabilités tel que Probas[i] est le score MidoRank de la page Adj[i]
		double* ProbasRefreshed = malloc((nbPages+1)*sizeof(double)); // Initialisation d'un second tableau de reels qui contiendra les probas actualisé sur un rafraichissement, afin de ne pas biaiser les calculs
	
 		int k=0;
		for(k=0; k<24; k++){ // Rafraichissement des probabilités pour un certain nombre de fois, en suivant l'algorithme simplifié de PageRank
			refreshProbas(Adj, nbPages, Probas, ProbasRefreshed, nbNeighbours);
			for(int l=0; l<=nbPages; l++) Probas[l] = ProbasRefreshed[l];
		}
		free(ProbasRefreshed);
		checkProbas(nbPages, Probas); // Fonction affichant la somme des probabilites
		printf("\n");
		int indexMax;
		int nbRanking = (int)strtof(argv[3], NULL);
		for(int i=0; i<nbRanking; i++){ // Le classement des meilleurs pages se fait par recuperation de l'indice du meilleur score MidoRank, son affichage et enfin la mise de son score a 0 afin de récupérer le 2eme meilleur, et ainsi de suite
			indexMax = maxTab(Probas, nbPages); 
			printf("Page number %d : %s (MidoRank of %f)\n", i+1, Adj[indexMax]->first->name, Probas[indexMax]);
			Probas[indexMax]=0.0;
		}
		printf("\n");
		
		free(nbNeighbours);
		free(Adj);
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	
	} else if(strcmp(argv[1], "-t")==0){ // Entree dans le mode -t
	
		char* fileName[6]={"alea4-6.txt", "alea4-12.txt", "alea10-40.txt", "alea50-1000.txt", "alea100-5000.txt", "alea1000-10000.txt"};
		int fn=0;
		for(fn=0; fn<6; fn++){
		
			int dim;
			dim=matrixDimension(fileName[fn]); // Recuperation de la dimension de la matrice afin de faire les allocations dynamiques
			int **M=malloc(dim*sizeof(int*));
			for(int i=0;i<dim;i++){
				M[i]=malloc(dim*sizeof(int));
			}

			char awaitedProbasStr[50000]; // Creation d'une chaine de caractere contenant la ligne où les probabilités attendues sont écrites.
			readMatrix(M, fileName[fn], dim, awaitedProbasStr); // Fonction lisant une matrice dans un fichier et la mettant de M, en recuperant awaitedProbas
		
			double* AwaitedProbas = getProbasMatrix(awaitedProbasStr, dim);
			double* ProbaMatrix = initProbasMat(dim); // Initialisation de la matrice conteant les probabilités
			int* nbNeighboursMatrix = getNeighboursMatrix(M, dim);
			double* ProbRefreshedMatrix = malloc((dim)*sizeof(double)); // Initialisation de la matrice de probas rafraichies (temporaires)
		
			int k;
			int nbIterations = (int)strtof(argv[2], NULL);
			for(k=0; k<nbIterations; k++){
				refreshProbasMat(M, dim, ProbaMatrix, ProbRefreshedMatrix, nbNeighboursMatrix);
				for(int l=0; l<=dim; l++) {
 					ProbaMatrix[l] = ProbRefreshedMatrix[l];
				}
			}
			double MarginError[dim];
	
			marginError(ProbaMatrix, AwaitedProbas, MarginError, dim); // Affichage des probabilites obtenues

			printf("\nFor %d iteration on the file %s, the probabilites are : \n\n", k, fileName[fn]);
			//Affichages des probabilites calculées, avec celles attendues, ainsi que la marge d'erreur avec la probabilités attendues
			double average = 0;
			if(dim>=20) printf("As there is too much probabilities to display, only the margin error max and average will be. \n");
			for(int i=0; i<dim; i++){
				if(dim<20) printf("Line %d : %lf with an awaited probabilite of %lf - The margin of error is %lf percent\n", i, ProbaMatrix[i], AwaitedProbas[i], MarginError[i]*100); 
				average += MarginError[i];
			}
			printf("\n");
			checkProbas(dim-1, ProbaMatrix);
		
			printf("\nThe maximum of margin error is %lf percent, with an average of %lf percent.\n", MarginError[maxTab(MarginError, dim-1)]*100, (average*100)/dim);
			printf("\n--------------------------------------------------------------\n");
		
			for(int i=0; i<dim; i++) free(M[i]);
			free(M);
			free(AwaitedProbas);
			free(ProbaMatrix);
			free(nbNeighboursMatrix);
			free(ProbRefreshedMatrix);
		}
		
	} else printf("\nError : please enter -n or -t.\n\n");
	
	return 0;

}