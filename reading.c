#include "reading.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define V_MAX 200000
#define SP "Super Noeud"

int getLines(const char fileName[100], char* file_lines[V_MAX]){ //Cette fonction permet d'ouvrir et lire un fichier texte, et de placer chaque ligne de ce ficher dans un tableau de chaine de caracteres. De plus elle renvoie le nombre de lignes
	FILE * file;
	file = fopen(fileName, "r");
	if(file == NULL) return 0;
	int i = 0;
	while((fgets(file_lines[i], V_MAX, file) != NULL) && (i++ < V_MAX)); // recupere la ligne dans file_line[i] et increment le compteur de ligne
	fclose(file);
	return i;
}


int matrixDimension(const char fileName[100]){ //fonction recuperant la dimension de la matrice afin d'effecteur les allocations dynamiques de la matrice
	FILE * mat;
	mat = fopen(fileName, "r");
	if(mat==NULL) return 0;
	int dim = 0;
	int curr = fgetc(mat);
	
	while(curr!=10){ // 10 = code ascii du retour a la ligne remplissage de la premiere ligne de la matrice + recuperation de la dimension etant donnée que la matrice est carrée (- super noeud donc -1)
		if(curr==48 || curr==49) dim+=1; // 48 = 0 en ascii , 49 = 1 en ascii
		curr = fgetc(mat);
	}
	fclose(mat);
	return dim+1; // Rajout du super noeud
}

void readMatrix(int** M, const char fileName[100], const int dim, char* awaitedProbas){ // Fonction remplissant la matrice contenue dans le fichier et apportant le super neoud en 0, et le fait que toutes les pages pointent sur elles meme
	FILE * mat;
	mat = fopen(fileName, "r");
	if(mat==NULL) return;
	int curr;
	int c;
	for(int l=1; l<dim; l++){
		c = 1;
		while(c<dim){
			curr = fgetc(mat);
			if(curr==48) M[l][c++] = 0;
			else if(curr==49) M[l][c++] = 1;
		}
	}
	
	for(int i=0; i<dim; i++){ // Ajout du super noeud et pointage sur soit meme
		for(int j=0; j<dim; j++){
			if(i==0 && j==0) M[i][j]=0; // Le super noeud ne pointe pas sur lui meme
			else if(i==j) M[i][j]=1; // Toutes les pages pointent sur elle memes
			else if(i==0 && j!=0) M[i][j]=1; // Premiere ligne de la matrice (sauf 0,0) egale a 1 car le sp pointe sur tout le monde
			else if(j==0 && i!=0) M[i][j]=1; // Premiere colonne de la matrice (sauf 0,0) egale a 1 car tout le monde pointe sur le super noeud
		}
	}
	fgets(awaitedProbas, 50000, mat); //Descente vers la ligne contenant les probas (ces lignes sont vides)
	fgets(awaitedProbas, 50000, mat);
	
	fgets(awaitedProbas, 50000, mat); //Contient les probas
	fclose(mat);
}

double* getProbasMatrix(char* awaitedProbasStr, const int dim){ // Fonction renvoyer un tableau contenant les probabilités attendues pour la matrice associée
	double* AwaitedProbas = malloc(dim*sizeof(double));
	if(AwaitedProbas == NULL) return NULL;
	char* awaitedProbasStrBis[dim];
	awaitedProbasStrBis[0] = strtok(awaitedProbasStr, ", "); 
	for(int i=1; i<dim; i++){
		awaitedProbasStrBis[i] = strtok(NULL, ", "); // Separation de toutes les probas de la ligne dans un tableau de chaine de caracteres
	}
	for(int i=0; i<dim; i++){
		AwaitedProbas[i] = strtod(awaitedProbasStrBis[i], NULL); // Placement des probabilites sous forme de double dans le tableau final (conversion)
	}
	return AwaitedProbas;
}

int* getNeighboursMatrix(int** M, const int dim){ // Fonction renvoyant un tableau conteant le nombre de voisins tel que nbNeighbourMatrix[i] renvoie le nombre de voisins sortants de la ligne i, sans super noeud
	int* nbNeighboursMatrix = malloc(dim*sizeof(int));
	if(nbNeighboursMatrix == NULL) return NULL;
	for(int i=0;i<dim;i++){
		nbNeighboursMatrix[i]=0; // Initialisation de tous les nombres de voisins a 0
	}
	for(int i=0;i<dim;i++){
		for(int j=0;j<dim;j++){
			if(M[i][j]==1) nbNeighboursMatrix[i]+=1; // Si j est detecté voisin sortant du i, le nombre de voisins de i est incrementé de 1
		}
	}
	for(int i=1; i<dim; i++) nbNeighboursMatrix[i]-=1; // Correction du fait que le super ait ete pris en compte dans le comptage du nombre de voisins
	
	return nbNeighboursMatrix;
}