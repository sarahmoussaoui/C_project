#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


// Clause structure
typedef struct {
    int literals[3]; // 3 literals for 3 SAT
} Clause;

Clause *formula = NULL; // Dynamically allocated formula (composed of clauses)
int num_clauses, num_variables; // Number of clauses and variables

// Function to check if a clause is satisfied by the current affectation
bool verifier_satisfaction_clause(Clause clause, int affectation[]) {
    for (int i = 0; i < 3; i++) {
        int literal = clause.literals[i];
        int variable = abs(literal); // to know which variable it is, e.g., if -4, then it's X4
        bool value = affectation[variable - 1]; // because index starts at 0
        if ((literal > 0 && value) || (literal < 0 && !value)) { // e.g., if x and x=true, then true; if non x and x=false, then true
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
int DFS_3SAT(Clause *clauses, int m, int n, int index, int affectation[]) {
    if (index == n) {
        // Check if all clauses are satisfied with the current affectation
        return verifier_satisfaction_formule(affectation);
    }

    // Try affecting 0 (false) to the current variable
    affectation[index] = 0;
    if (DFS_3SAT(clauses, m, n, index + 1, affectation)) {
        return 1; // Found a satisfying affectation
    }

    // Try affecting 1 (true) to the current variable
    affectation[index] = 1;
    if (DFS_3SAT(clauses, m, n, index + 1, affectation)) {
        return 1; // Found a satisfying affectation
    }

    return 0; // No solution found
}

int main() {
    // Input number of variables and clauses
    printf("Nombre de variables? : ");
    scanf("%d", &num_variables);
    printf("Nombre de clauses? : ");
    scanf("%d", &num_clauses);

    // Dynamically allocate memory for formula and affectation array
    formula = (Clause *)malloc(num_clauses * sizeof(Clause)); // Allocate space for clauses
    if (formula == NULL) {
        perror("Erreur d'allocation mémoire pour la formule"); // Handle allocation failure
        return EXIT_FAILURE;
    }

    int *affectation = (int *)calloc(num_variables, sizeof(int)); // Allocate space for affectation
    if (affectation == NULL) {
        perror("Erreur d'allocation mémoire pour l'affectation"); // Handle allocation failure
        free(formula);
        return EXIT_FAILURE;
    }

    // Input clauses
    printf("Faites entrer chaque clause (>0 pour variable X ou <0 si negation de X ):\n"); // e.g., if x1+x2, input 1 2
    for (int i = 0; i < num_clauses; i++) {
        for (int j = 0; j < 3; j++) {
            scanf("%d", &formula[i].literals[j]); // Read literals for each clause
        }
    }


    // Find a satisfying assignment using DFS
    int resultat = DFS_3SAT(formula, num_clauses, num_variables, 0, affectation);

    if (resultat) {
        printf("Affectation trouvée:\n");
        for (int i = 0; i < num_variables; i++) {
            printf("x%d = %d\n", i + 1, affectation[i]); // Output each variable's value
        }
    } else {
        printf("Aucune affectation satisable existe.\n"); // No satisfying assignment
    }


    // Free allocated memory
    free(formula); 
    free(affectation); 

    return 0;
}
