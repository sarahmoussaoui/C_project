#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define BILLION 1000000000.0

typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
} Node;


Node* creerNoeud(int data) {
    Node* noeud = (Node*)malloc(sizeof(Node));
    noeud->data = data;
    noeud->left =NULL;
	 noeud->right = NULL;
    return noeud;
}


Node* ajoute(Node* racine, int data) {
    if (racine == NULL) {
        return creerNoeud(data);
    }
    if (data < racine->data) {
        racine->left = ajoute(racine->left, data);
    } else if (data > racine->data) {
        racine->right = ajoute(racine->right, data);
    }
    return racine;
}



Node* rechercher(Node* racine, int data) {
    if (racine == NULL || racine->data == data) {
        return racine;
    }
    if (data < racine->data) {
        return rechercher(racine->left, data);
    } else {
        return rechercher(racine->right, data);
    }
}


Node* noeudMinimum(Node* racine) {
    Node* actuel = racine;
    while (actuel->left != NULL) {
        actuel = actuel->left;
    }
    return actuel;
}


Node* supprimer(Node* racine, int data) {
    if (racine == NULL) {
        return racine;
    }
    if (data < racine->data) {
        racine->left = supprimer(racine->left, data);
    } else if (data > racine->data) {
        racine->right = supprimer(racine->right, data);
    } else {
        if (racine->left == NULL) {
            Node* temp = racine->right;
            free(racine);
            return temp;
        } else if (racine->right == NULL) {
            Node* temp = racine->left;
            free(racine);
            return temp;
        }
        Node* temp = noeudMinimum(racine->right);
        racine->data = temp->data;
        racine->right = supprimer(racine->right, temp->data);
    }
    return racine;
}


void afficherParcoursInfixe(Node* racine) {
    if (racine != NULL) {
        afficherParcoursInfixe(racine->left);
        printf("%d ", racine->data);
        afficherParcoursInfixe(racine->right);
    }
}

int main() {
    Node* racine = NULL;
    int choix, element,nbr;
    // printf("Donnez nomber des elements de l'arbre :");
    // scanf("%d",&nbr);
    // printf("\n");
    // for(int i=1;i<=nbr;i++){
    // 	printf("Nomber %d d'arbre :",i);
    // 	scanf("%d",&element);
    //      racine=ajoute(racine,element);
	// }
    clock_t start = clock();
    for(int i=0;i<100000000;i++){
        racine=ajoute(racine,rand());
    }
    clock_t end = clock();
                double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
     printf("%f nanoSec", time_taken);
     printf("Afficher arbre apres insertion :");
     afficherParcoursInfixe(racine);
    while (1) {
        printf("\n\nMenu :\n");
        printf("1. Inserer un element\n");
        printf("2. Rechercher un element\n");
        printf("3. Supprimer un element\n");
        printf("4. Afficher l'arbre\n");
        printf("5. Quitter\n");
        printf("Entrez votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
             case 1:
                printf("Entrez l'element a inserer : ");
                scanf("%d", &element);
                 start = clock();
                racine = ajoute(racine, element);
                 end = clock();
                 time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
                printf("%f Sec", time_taken);

                printf("\n");
                break;
            case 2:
                printf("Entrez l'element a rechercher : ");
                scanf("%d", &element);
                 start = clock();
                Node* resultat = rechercher(racine, element);
                if (resultat != NULL) {
                    printf("Element %d trouve dans l'arbre.\n", element);
                } else {
                    printf("Element %d non trouve dans l'arbre.\n", element);
                }
                 end = clock();
                 time_taken = (((double)(end - start)) / CLOCKS_PER_SEC)*1e9;
                printf("%f nSec", time_taken);
                break;
            case 3:
                printf("Entrez l'element a supprimer : ");
                scanf("%d", &element);
                 start = clock();
                racine = supprimer(racine, element);

                printf("Element %d supprime .\n", element);
                 end = clock();
                 time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
                printf("%f Sec", time_taken);
                break;
            case 4:
                printf("ParcoursInfixe : ");
                afficherParcoursInfixe(racine);
                printf("\n");
                break;
            case 5:
                printf("Quitter le programme.\n");
                exit(0);
            default:
                printf("Choix invalide. Veuillez reessayer.\n");
        }
    }
    return 0;
}
