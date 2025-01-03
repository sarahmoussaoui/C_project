#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <psapi.h> // pour mesurer l'utilisation de la mémoire
#pragma comment(lib, "psapi.lib") // Lier la bibliothèque psapi.lib

// Fonction pour afficher la mémoire utilisée
void afficher_memoire_utilisee(FILE *file, int n, int m) {
    PROCESS_MEMORY_COUNTERS memCounters;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &memCounters, sizeof(memCounters))) {
        fprintf(file, "Mémoire utilisée pour %d variables et %d clauses: %ld bytes\n", n, m, memCounters.WorkingSetSize);
    } else {
        fprintf(file, "Erreur pour obtenir les informations mémoire\n");
    }
}

// Fonction pour générer des clauses aléatoires
void generer_clauses(int **clauses, int m, int n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < 3; j++) {
            int var = rand() % n + 1;
            clauses[i][j] = (rand() % 2 == 0) ? var : -var;
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
int DFS_3SAT(int **clauses, int m, int n, int index, int affectation[]) {
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
void ecrire_clauses_dans_fichier(int **clauses, int m, const char *nom_fichier) {
    FILE *fichier = fopen(nom_fichier, "w");
    if (fichier == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier %s\n", nom_fichier);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < m; i++) {
        fprintf(fichier, "Clause %d : ", i + 1);
        for (int j = 0; j < 3; j++) {
            fprintf(fichier, "%d ", clauses[i][j]);
        }
        fprintf(fichier, "\n");
    }

    fclose(fichier);
}

int main() {
    int n, m; // Nombre de variables et de clauses
    int **clauses; // Allocation dynamique des clauses
    int *affectation; // Allocation dynamique de l'affectation
   
    srand(time(NULL));
    printf("Nombre de variables (n) : ");
    scanf("%d", &n);
    printf("Nombre de clauses (m) : ");
    scanf("%d", &m);

    // Allocation dynamique
    clauses = (int **)malloc(m * sizeof(int *));
    for (int i = 0; i < m; i++) {
        clauses[i] = (int *)malloc(3 * sizeof(int));
    }
    affectation = (int *)malloc(n * sizeof(int));

    // Génération des clauses aléatoires
    generer_clauses(clauses, m, n);

    // Écrire les clauses dans un fichier texte
    ecrire_clauses_dans_fichier(clauses, m, "clauses.txt");

    // Mesure du temps d'exécution
    clock_t start = clock();
    int resultat = DFS_3SAT(clauses, m, n, 0, affectation);
    clock_t end = clock();
    double execution_time = (double)(end - start) / CLOCKS_PER_SEC;

    // Résultats
    if (resultat) {
        printf("Affectation satisfiable!\n");
        for (int i = 0; i < n; i++) {
            printf("%d ", affectation[i] ? i + 1 : -(i + 1));
        }
    } else {
        printf("Affectation non satisfiable.\n");
    }

    FILE *file = fopen("results.txt", "a");
    FILE *file2 = fopen("memoire_results.txt", "a");
    if (file && file2) {
        fprintf(file, "Execution time for %d variables and %d clauses: %f seconds\n", n, m, execution_time);
        afficher_memoire_utilisee(file2, n, m);
        fclose(file);
        fclose(file2);
    } else {
        printf("Erreur d'ouverture des fichiers de résultats.\n");
    }

    // Libération de la mémoire
    for (int i = 0; i < m; i++) {
        free(clauses[i]);
    }
    free(clauses);
    free(affectation);

    return 0;
}
