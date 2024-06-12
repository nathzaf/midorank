#include "graph.h"
#include "probabilities.h"
#include "reading.h"
#include "misc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define V_MAX 200000
#define SP "Super Noeud"

Node* createNode(const char *s){ // Fonction creeant un maillon du liste chainée de nom dans le parametres et pointant sur NULL
	Node *m=malloc(sizeof(Node));
	if(m==NULL) return NULL;
	m->name = malloc((strlen(s)+1)*sizeof(char)); 
	if(m->name==NULL) return NULL;
	strcpy(m->name, s);
	m->next = NULL;
	return m;
}

LinkedList* createLinkedList(char* line, int* nbNeighbours, const int pos){ // Fonction creant la liste chainée d'une ligne d'un fichier txt au format ...|...|... et remplissant le tableau nbNeigbours pour la liste chainée donné en pos
	LinkedList* lc = malloc(sizeof(LinkedList));
	if(lc==NULL) return NULL;
	int nbN = 0;
	char* word = strtok(line, "|\n"); //separation des |
	char* first_word = malloc(50*sizeof(char));
	strcpy(first_word, word);
	Node* current = createNode(word);
	lc->first = current;
	while(word!=NULL){
		word = strtok(NULL, "|\n");
		if(word==NULL) break;
		if(isInLinkedList(lc, word)==1) continue; //cas ou une page a ajouter est déjà existant, elle doit donc être ignorée
		current->next = createNode(word);
		current = current->next;
		if(strcmp(word, first_word)==0) continue; //cas ou la page pointe sur elle meme de base, ce qui doit etre ignoré
		nbN++;
	}
	current->next = createNode(lc->first->name); //La page doit pointer sur elle meme afin de rendre le graphe fortement connexe
	nbN++;
	current = current->next;
	current->next = createNode(SP); //Pour qu'il pointe sur le super noeud, ce qui rend le graphe fortement connexe. (Lors d'un appel de creeMaillon le next est mis a NULL)
	nbNeighbours[pos] = nbN;
	return lc;
}	

LinkedList* superNoeud(LinkedList** G, const int nbLines){ //Fonction creant la liste chainee du super noeud, en le faisant pointer sur toutes les pages, afin de rendre le graphe fortement connexe
	LinkedList* superNoeud = malloc(sizeof(LinkedList));
	if(superNoeud == NULL) return NULL;
	Node* current = createNode(SP);
	superNoeud->first = current;
	for(int i=1; i<=nbLines; i++){
		current->next = createNode(G[i]->first->name);
		current = current->next;
	}
	return superNoeud;
}
	
	
	
LinkedList** createGraph(const int nbLines, char* file_lines[V_MAX], int* nbNeighbours){ //Fonction créant le graphe en lui même, en recuperer le nombre de voisins sortant pour chaque liste chainees
	LinkedList** Adj = malloc((nbLines+1)*sizeof(LinkedList*)); //+1 car super noeud sera en 0
	if(Adj == NULL) return NULL;
	nbNeighbours[0]=nbLines; // Le super noeud pointant sur tout le monde a donc le nombre de pages en voisins
	for(int i=1; i<=nbLines; i++){
		Adj[i] = createLinkedList(file_lines[i-1], nbNeighbours, i); // Le super noeud étant placé en 0, il y a un decalage entre liste chainée et ligne du fichier texte
 	} 

	Adj[0] = superNoeud(Adj, nbLines);
	return Adj;
}

void redLinks(LinkedList** G, const int nbPages, int* nbNeighbours){ //Fonction supprimant les liens rouges en verifiant pour chaque maillon des liste chainees si il existe dans le graphe, en apportant la correction sur le nombre de voisins sortants
	int total = 0;
	for(int i=1; i<=nbPages; i++){
		Node* curr = G[i]->first; 
		Node* temp = NULL;
		while(curr->next != NULL){
			if(isInLinkedList(G[0], curr->next->name) == 0){ //Verifie donc l'existance du maillon dans le graphe, car si ce maillon est pointé par le super noeud alors ce maillon existe
				total+=1;
				temp = curr->next;
				curr->next = temp->next;
				free(temp);
				nbNeighbours[i] -= 1;
			} else curr = curr->next;
		}
	}
	printf("and a total of %d red links \n\n", total);
}
