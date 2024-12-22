#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <psapi.h>// pour mesurer lutilisation de la memoire
// Link the psapi.lib library
#pragma comment(lib, "psapi.lib")


#define MAX_CLAUSES 1000
#define MAX_VARIABLES 100

// Fonction pour afficher la memoire utilisee
void afficher_memoire_utilisee(FILE *file,int n, int m) {
    PROCESS_MEMORY_COUNTERS memCounters;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &memCounters, sizeof(memCounters))) {
        fprintf(file,"Mémoire utilisée pour %d variables et %d clauses: %ld bytes\n",n,m, memCounters.WorkingSetSize);
    } else {
        fprintf(file,"Erreur pour obtenir les informations mémoire\n");
    }
}



// Fonction pour générer des clauses aléatoires
void generer_clauses(int clauses[][3], int m, int n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < 3; j++) {
            int var = rand() % n + 1; // n+1 pour inclure le n 
            clauses[i][j] = (rand() % 2 == 0) ? var : -var; // si genere 0 ou 1 alors soit X soit -X
        }
    }
}

// Vérification de satisfaction d'une clause
int verifier_satisfaction_clause(int clause[], int affectation[]) {
    for (int i = 0; i < 3; i++) {
        int literal = clause[i];
        int variable = abs(literal);
        if ((literal > 0 && affectation[variable - 1]) || (literal < 0 && !affectation[variable - 1])) {
            return 1;
        }
    }
    return 0;
}

// DFS pour résoudre le problème 3SAT
int DFS_3SAT(int clauses[][3], int m, int n, int index, int affectation[]) {
    if (index == n) {
        for (int i = 0; i < m; i++) {
            if (!verifier_satisfaction_clause(clauses[i], affectation)) {
                return 0; // Clause non satisfaite
            }
        }
        return 1; // Toutes les clauses sont satisfaites
    }
    for (int valeur = 0; valeur <= 1; valeur++) {
        affectation[index] = valeur;
        if (DFS_3SAT(clauses, m, n, index + 1, affectation)) {
            return 1;
        }
    }
    return 0;
}

// Fonction pour écrire les clauses dans un fichier texte
void ecrire_clauses_dans_fichier(int clauses[][3], int m, const char *nom_fichier) {
    FILE *fichier = fopen(nom_fichier, "w"); // Ouvrir le fichier en mode écriture
    if (fichier == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier %s\n", nom_fichier);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < m; i++) {
        fprintf(fichier, "Clause %d : ", i + 1); // Écrire l'index de la clause
        for (int j = 0; j < 3; j++) {
            fprintf(fichier, "%d ", clauses[i][j]); // Écrire chaque littéral
        }
        fprintf(fichier, "\n"); // Nouvelle ligne après chaque clause
    }

    fclose(fichier); // Fermer le fichier
    // printf("Les clauses ont été écrites dans le fichier %s avec succès.\n", nom_fichier);
}

int main() {
    int n, m; // Variables et clauses
    int clauses[MAX_CLAUSES][3];
    int affectation[MAX_VARIABLES];
    
    srand(time(NULL)); // pour avoir une séquence aléatoire différente à chaque exécution.
    printf("Nombre de variables (n) : ");
    scanf("%d", &n);
    printf("Nombre de clauses (m) : ");
    scanf("%d", &m);

    // Génération des clauses aléatoires
    generer_clauses(clauses, m, n);

    // Écrire les clauses dans un fichier texte
    ecrire_clauses_dans_fichier(clauses, m, "clauses.txt"); // for debugging purposes

    // Mesure du temps d'exécution
    clock_t start = clock();
    int resultat = DFS_3SAT(clauses, m, n, 0, affectation);
    clock_t end = clock();
    double execution_time = (double)(end - start) / CLOCKS_PER_SEC;

    // Résultats
    if (resultat) {
        printf("Affectation satisfiable!\n");
        for (int i = 0; i < n; i++) {
                printf("%d ", affectation[i] ? i + 1 : -(i + 1)); // Affichage des variables
        }
    } else {
        printf("Affectation non satisfiable.\n");
    }

    FILE *file = fopen("results.txt", "a");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return 1;
    }

    FILE *file2 = fopen("memoire_results.txt", "a");
    if (file == NULL) {
        printf("Erreur d'ouverture du fichier.\n");
        return -1;
    }

    fprintf(file, "Execution time for %d variables and %d clauses: %f seconds\n", n, m, execution_time);
    afficher_memoire_utilisee(file2,n,m);
    fclose(file);
    fclose(file2);
    
    return 0;
}
