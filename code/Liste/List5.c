#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Node {
    int data;
    struct Node* prev;
    struct Node* next;
} Node;

Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

void insert(Node** head, int data) {
    Node* newNode = createNode(data);
    if (*head == NULL) {
        *head = newNode;
        return;
    }
    Node* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
    newNode->prev = temp;
}

void fillListWithRandomElements(Node** head, int count) {
    srand(time(NULL));
    for (int i = 0; i < count; i++) {
        int randomValue = rand() % 100;
        insert(head, randomValue);
    }
}

void sortList(Node** head) {
    if (*head == NULL) return;
    Node* i = (*head)->next;
    while (i != NULL) {
        int key = i->data;
        Node* j = i->prev;
        while (j != NULL && j->data > key) {
            j->next->data = j->data;
            j = j->prev;
        }
        if (j == NULL) {
            (*head)->data = key;
        } else {
            j->next->data = key;
        }
        i = i->next;
    }
}

void deleteNode(Node** head, int key) {
    if (*head == NULL) {
        printf("La liste est vide.\n");
        return;
    }
    
    Node* temp = *head;
    while (temp != NULL && temp->data != key) {
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("L'élément %d n'existe pas dans la liste.\n", key);
        return;
    }

    if (temp->prev == NULL) {
        *head = temp->next;
    } else {
        temp->prev->next = temp->next;
    }

    if (temp->next != NULL) {
        temp->next->prev = temp->prev;
    }

    free(temp);
    printf("L'élément %d a été supprimé de la liste.\n", key);
}

void displayList(Node* head) {
    Node* temp = head;
    printf("Liste: ");
    while (temp != NULL) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

void search(Node* head, int key) {
    Node* temp = head;
    int count = 0;

    while (temp != NULL) {
        if (temp->data == key) {
            count++;
        }
        temp = temp->next;
    }

    if (count > 0) {
        printf("L'élément %d apparaît %d fois dans la liste.\n", key, count);
    } else {
        printf("L'élément %d n'existe pas dans la liste.\n", key);
    }
}

int getChoice() {
    int choice;
    printf("\nMenu:\n");
    printf("1. Insérer un élément\n");
    printf("2. Supprimer un élément\n");
    printf("3. Trier la liste\n");
    printf("4. Rechercher un élément\n");
    printf("5. Afficher la liste\n");
    printf("6. Quitter\n");
    printf("Entrez votre choix: ");
    
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        return -1;
    }
    
    return choice;
}

int getIntegerInput(const char* prompt) {
    int value;
    printf("%s", prompt);
    while (scanf("%d", &value) != 1) {
        while (getchar() != '\n');
        printf("Entrée invalide. %s", prompt);
    }
    return value;
}

int main() {
    Node* head = NULL;
    clock_t start, end;

    int numberOfElements = getIntegerInput("Entrez le nombre d'éléments aléatoires à ajouter à la liste: ");

    start = clock();
    fillListWithRandomElements(&head, numberOfElements);
    end = clock();
    double creationTime = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Liste initiale avec %d éléments aléatoires:\n", numberOfElements);
    //displayList(head);
    printf("Temps de création et remplissage de la liste: %.6f secondes\n", creationTime);

    int choice, value;

    while (1) {
        choice = getChoice();

        if (choice == -1) {
            printf("Entrée invalide. Veuillez entrer un nombre entre 1 et 6.\n");
            continue;
        }

        switch (choice) {
            case 1:
                displayList(head);
                value = getIntegerInput("Entrez la valeur à insérer: ");
                start = clock();
                insert(&head, value);
                end = clock();
                printf("Liste après insertion:\n");
                displayList(head);
                printf("Temps d'insertion: %.6f secondes\n", (double)(end - start) / CLOCKS_PER_SEC);
                break;
                
            case 2:
                displayList(head);
                value = getIntegerInput("Entrez la valeur à supprimer: ");
                start = clock();
                deleteNode(&head, value);
                end = clock();
                displayList(head);
                printf("Temps de suppression: %.6f secondes\n", (double)(end - start) / CLOCKS_PER_SEC);
                break;

            case 3:
                displayList(head);
                start = clock();
                sortList(&head);
                end = clock();
                displayList(head);
                printf("Temps de tri: %.6f secondes\n", (double)(end - start) / CLOCKS_PER_SEC);
                break;

            case 4:
                value = getIntegerInput("Entrez la valeur à rechercher: ");
                start = clock();
                search(head, value);
                end = clock();
                printf("Temps de recherche: %.6f secondes\n", (double)(end - start) / CLOCKS_PER_SEC);
                break;

            case 5:
                displayList(head);
                break;

            case 6:
                printf("Fin du programme.\n");
                return 0;

            default:
                printf("Choix invalide. Veuillez entrer un nombre entre 1 et 6.\n");
                break;
        }
    }
}
