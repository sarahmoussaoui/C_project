#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>


#define MAX_CLAUSES 100  // Maximum number of clauses
#define MAX_VARIABLES 20 // Maximum number of variables

// Clause structure
typedef struct {
    int literals[3]; // 3 literals for 3 SAT
} Clause;

Clause formula[MAX_CLAUSES]; // formula is composed of clauses
int num_clauses, num_variables; // number of clauses and variables

// Function to check if a clause is satisfied by the current affectation
bool verifier_satisfaction_clause(Clause clause, int affectation[]) {
    for (int i = 0; i < 3; i++) {
        int literal = clause.literals[i];
        int variable = abs(literal); // to know which variable is it , exemple if -4 then its X4
        bool value = affectation[variable - 1]; // because index starts at 0
        if ((literal > 0 && value) || (literal < 0 && !value)) { // exemple: if x and x true then true, if non x and x false then true
            return true; // Clause is satisfied
        }
    }
    return false; // Clause is not satisfied
}

// Function to check if the formula is satisfied
bool verifier_satisfaction_formule(int affectation[]) {
    for (int i = 0; i < num_clauses; i++) {
        if (!verifier_satisfaction_clause(formula[i], affectation)) {
            return false;
        }
    }
    return true;
}

// DFS function to find a satisfying affectation
bool dfs(int index, int affectation[]) {
    if (index == num_variables) {
        // All variables have been affected, check if the formula is satisfied
        return verifier_satisfaction_formule(affectation);
    }

    // Try affecting 0 (false) to the current variable
    affectation[index] = 0;
    if (dfs(index + 1, affectation)) {
        return true;
    }

    // Try affecting 1 (true) to the current variable
    affectation[index] = 1;
    if (dfs(index + 1, affectation)) {
        return true;
    }

    // No solution found
    return false;
}

int main() {
    // Input number of variables and clauses
    printf("Nombre de variables? : ");
    scanf("%d", &num_variables);
    printf("Nombre de clauses? : ");
    scanf("%d", &num_clauses);

    // Input clauses
    printf("Faites entrer chaque clause (>0 pour variable X  ou <0 si negation de X):\n"); // si x1+x2 on met 1 2
    for (int i = 0; i < num_clauses; i++) {
        for (int j = 0; j < 3; j++) {
            scanf("%d", &formula[i].literals[j]);
        }
    }

    // tableau pour l'affectation initilise a 0
    int affectation[MAX_VARIABLES] = {0};

    // Mesure du temps d'exécution de la verification
    clock_t start = clock();
    
    // Trouver une affectation possible
    if (dfs(0, affectation)) {
        printf("Affectation trouvée:\n");
        for (int i = 0; i < num_variables; i++) {
            printf("x%d = %d\n", i + 1, affectation[i]);
        }
    } else {
        printf("Aucune affectation satisable existe.\n");
    }
    clock_t end = clock();
    double execution_time = (double)(end - start) / CLOCKS_PER_SEC;

    FILE *file = fopen("temps_exec_verification.txt", "a");
    if (file) {
        fprintf(file, "Execution time for %d variables and %d clauses: %f seconds\n", num_variables, num_clauses, execution_time);
        fclose(file);
    } else {
        printf("Erreur d'ouverture des fichiers de résultats.\n");
    }
    return 0;
}
