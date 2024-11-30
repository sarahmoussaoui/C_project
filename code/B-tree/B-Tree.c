#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX 3
#define MIN 1

struct btreeNode
{
    int val[MAX + 1], count;
    struct btreeNode *link[MAX + 1];
};

struct btreeNode *root;

/* creating new node */
struct btreeNode *createNode(int val, struct btreeNode *child)
{
    struct btreeNode *newNode = (struct btreeNode *)malloc(sizeof(struct btreeNode));
    newNode->val[1] = val;
    newNode->count = 1;
    newNode->link[0] = root;
    newNode->link[1] = child;
    return newNode;
}

/* Places the value in appropriate position */
void addValToNode(int val, int pos, struct btreeNode *node, struct btreeNode *child)
{
    int j = node->count;
    while (j > pos)
    {
        node->val[j + 1] = node->val[j];
        node->link[j + 1] = node->link[j];
        j--;
    }
    node->val[j + 1] = val;
    node->link[j + 1] = child;
    node->count++;
}

/* split the node */
void splitNode(int val, int *pval, int pos, struct btreeNode *node, struct btreeNode *child, struct btreeNode **newNode)
{
    int median, j;

    if (pos > MIN)
        median = MIN + 1;
    else
        median = MIN;

    *newNode = (struct btreeNode *)malloc(sizeof(struct btreeNode));
    j = median + 1;
    while (j <= MAX)
    {
        (*newNode)->val[j - median] = node->val[j];
        (*newNode)->link[j - median] = node->link[j];
        j++;
    }
    node->count = median;
    (*newNode)->count = MAX - median;

    if (pos <= MIN)
    {
        addValToNode(val, pos, node, child);
    }
    else
    {
        addValToNode(val, pos - median, *newNode, child);
    }
    *pval = node->val[node->count];
    (*newNode)->link[0] = node->link[node->count];
    node->count--;
}

/* sets the value val in the node */
int setValueInNode(int val, int *pval, struct btreeNode *node, struct btreeNode **child)
{
    int pos;
    if (!node)
    {
        *pval = val;
        *child = NULL;
        return 1;
    }

    if (val < node->val[1])
    {
        pos = 0;
    }
    else
    {
        for (pos = node->count;
             (val < node->val[pos] && pos > 1); pos--)
            ;
        if (val == node->val[pos])
        {
            printf("Les duplications des valeurs ne sont pas autorisé\n");
            return 0;
        }
    }
    if (setValueInNode(val, pval, node->link[pos], child))
    {
        if (node->count < MAX)
        {
            addValToNode(*pval, pos, node, *child);
        }
        else
        {
            splitNode(*pval, pval, pos, node, *child, child);
            return 1;
        }
    }
    return 0;
}

/* insert val in B-Tree */
void insertion(int val)
{
    int flag, i;
    struct btreeNode *child;

    flag = setValueInNode(val, &i, root, &child);
    if (flag)
        root = createNode(i, child);
}

/* copy successor for the value to be deleted */
void copySuccessor(struct btreeNode *myNode, int pos)
{
    struct btreeNode *dummy;
    dummy = myNode->link[pos];

    for (; dummy->link[0] != NULL;)
        dummy = dummy->link[0];
    myNode->val[pos] = dummy->val[1];
}

/* removes the value from the given node and rearrange values */
void removeVal(struct btreeNode *myNode, int pos)
{
    int i = pos + 1;
    while (i <= myNode->count)
    {
        myNode->val[i - 1] = myNode->val[i];
        myNode->link[i - 1] = myNode->link[i];
        i++;
    }
    myNode->count--;
}

/* shifts value from parent to right child */
void doRightShift(struct btreeNode *myNode, int pos)
{
    struct btreeNode *x = myNode->link[pos];
    int j = x->count;

    while (j > 0)
    {
        x->val[j + 1] = x->val[j];
        x->link[j + 1] = x->link[j];
    }
    x->val[1] = myNode->val[pos];
    x->link[1] = x->link[0];
    x->count++;

    x = myNode->link[pos - 1];
    myNode->val[pos] = x->val[x->count];
    myNode->link[pos] = x->link[x->count];
    x->count--;
}

/* shifts value from parent to left child */
void doLeftShift(struct btreeNode *myNode, int pos)
{
    int j = 1;
    struct btreeNode *x = myNode->link[pos - 1];

    x->count++;
    x->val[x->count] = myNode->val[pos];
    x->link[x->count] = myNode->link[pos]->link[0];

    x = myNode->link[pos];
    myNode->val[pos] = x->val[1];
    x->link[0] = x->link[1];
    x->count--;

    while (j <= x->count)
    {
        x->val[j] = x->val[j + 1];
        x->link[j] = x->link[j + 1];
        j++;
    }
}

/* merge nodes */
void mergeNodes(struct btreeNode *myNode, int pos)
{
    int j = 1;
    struct btreeNode *x1 = myNode->link[pos], *x2 = myNode->link[pos - 1];

    x2->count++;
    x2->val[x2->count] = myNode->val[pos];
    x2->link[x2->count] = myNode->link[0];

    while (j <= x1->count)
    {
        x2->count++;
        x2->val[x2->count] = x1->val[j];
        x2->link[x2->count] = x1->link[j];
        j++;
    }

    j = pos;
    while (j < myNode->count)
    {
        myNode->val[j] = myNode->val[j + 1];
        myNode->link[j] = myNode->link[j + 1];
        j++;
    }
    myNode->count--;
    free(x1);
}

/* adjusts the given node */
void adjustNode(struct btreeNode *myNode, int pos)
{
    if (!pos)
    {
        if (myNode->link[1]->count > MIN)
        {
            doLeftShift(myNode, 1);
        }
        else
        {
            mergeNodes(myNode, 1);
        }
    }
    else
    {
        if (myNode->count != pos)
        {
            if (myNode->link[pos - 1]->count > MIN)
            {
                doRightShift(myNode, pos);
            }
            else
            {
                if (myNode->link[pos + 1]->count > MIN)
                {
                    doLeftShift(myNode, pos + 1);
                }
                else
                {
                    mergeNodes(myNode, pos);
                }
            }
        }
        else
        {
            if (myNode->link[pos - 1]->count > MIN)
                doRightShift(myNode, pos);
            else
                mergeNodes(myNode, pos);
        }
    }
}

/* delete val from the node */
int delValFromNode(int val, struct btreeNode *myNode)
{
    int pos, flag = 0;
    if (myNode)
    {
        if (val < myNode->val[1])
        {
            pos = 0;
            flag = 0;
        }
        else
        {
            for (pos = myNode->count;
                 (val < myNode->val[pos] && pos > 1); pos--)
                ;
            if (val == myNode->val[pos])
            {
                flag = 1;
            }
            else
            {
                flag = 0;
            }
        }
        if (flag)
        {
            if (myNode->link[pos - 1])
            {
                copySuccessor(myNode, pos);
                flag = delValFromNode(myNode->val[pos], myNode->link[pos]);
                if (flag == 0)
                {
                    printf("Given data is not present in B-Tree\n");
                }
            }
            else
            {
                removeVal(myNode, pos);
            }
        }
        else
        {
            flag = delValFromNode(val, myNode->link[pos]);
        }
        if (myNode->link[pos])
        {
            if (myNode->link[pos]->count < MIN)
                adjustNode(myNode, pos);
        }
    }
    return flag;
}

/* delete val from B-tree */
void deletion(int val, struct btreeNode *myNode)
{
    struct btreeNode *tmp;
    if (!delValFromNode(val, myNode))
    {
        printf("La valeur entrée n'existe pas dans l'arbre\n");
        return;
    }
    else
    {
        if (myNode->count == 0)
        {
            tmp = myNode;
            myNode = myNode->link[0];
            free(tmp);
        }
    }
    root = myNode;
}

/* search val in B-Tree */
void searching(int val, int *pos, struct btreeNode *myNode)
{
    if (!myNode)
    {
        printf("La valeur entrée elle n'est pas presente dans l'arbre\n");
        return;
    }

    if (val < myNode->val[1])
    {
        *pos = 0;
    }
    else
    {
        for (*pos = myNode->count;
             (val < myNode->val[*pos] && *pos > 1); (*pos)--)
            ;
        if (val == myNode->val[*pos])
        {
            printf("La valeur entrée est trouvé avec succées dans l'arbre\n");
            return;
        }
    }
    searching(val, pos, myNode->link[*pos]);
}

/* B-Tree Traversal */
void traversal(struct btreeNode *myNode)
{
    int i;
    if (myNode)
    {
        for (i = 0; i < myNode->count; i++)
        {
            traversal(myNode->link[i]);
            printf("%d ", myNode->val[i + 1]);
        }
        traversal(myNode->link[i]);
    }
}

void displayTree(struct btreeNode *myNode, int level)
{
    if (myNode)
    {
        for (int i = myNode->count; i > 0; i--)
        {
            displayTree(myNode->link[i], level + 1);
            for (int j = 0; j < level; j++)
            {
                printf("    "); // Indentation pour chaque niveau
            }
            printf("%d\n", myNode->val[i]);
        }
        displayTree(myNode->link[0], level + 1);
    }
}

//----------Fonction pour vérifier si un nombre est déjà dans le tableau
int estUnique(int *tab, int taille, int nombre)
{
    for (int i = 0; i < taille; i++)
    {
        if (tab[i] == nombre)
        {
            return 0; // Non unique
        }
    }
    return 1; // Unique
}
/*------Fonction pour generer un fichier qui contient des nombres aleatoires-----*/
void genererNombresAleatoiresOptimise(char *nomFichier, int taille, int borneMin, int borneMax)
{
    if (borneMax - borneMin + 1 < taille)
    {
        printf("Erreur : La plage de nombres est trop petite pour générer %d nombres uniques.\n", taille);
        return;
    }

    int *nombres = malloc((borneMax - borneMin + 1) * sizeof(int));
    if (nombres == NULL)
    {
        printf("Erreur : Allocation de mémoire échouée.\n");
        return;
    }

    // Remplir le tableau avec tous les nombres possibles
    for (int i = 0; i < (borneMax - borneMin + 1); i++)
    {
        nombres[i] = borneMin + i;
    }

    // Mélanger les nombres
    for (int i = 0; i < (borneMax - borneMin + 1); i++)
    {
        int j = rand() % (borneMax - borneMin + 1);
        int temp = nombres[i];
        nombres[i] = nombres[j];
        nombres[j] = temp;
    }

    // Écrire les `taille` premiers nombres dans le fichier
    FILE *fichier = fopen(nomFichier, "w");
    if (fichier == NULL)
    {
        printf("Erreur : Impossible d'ouvrir le fichier.\n");
        free(nombres);
        return;
    }

    for (int i = 0; i < taille; i++)
    {
        fprintf(fichier, "%d\n", nombres[i]);
    }

    fclose(fichier);
    free(nombres);
}

int main()
{
    root = NULL;
    const char *nomFichier = "nombres_aleatoires.txt";
    printf("Veuillez entrer la taille (le nombre de chiffre que vous souhaitez avoir dans l'arbre) \n");
    int t;
    scanf("%d", &t);
    int taille = t;   // Générer 1 million de nombres
    int borneMin = 1; // Valeur minimale
    int borneMax = t; // Valeur maximale
    genererNombresAleatoiresOptimise("nombres_aleatoires.txt", taille, borneMin, borneMax);

    // Creation de l'arbre
    FILE *fichier = fopen(nomFichier, "r");
    int nombre;
    clock_t start, end;
    if (!fichier)
    {
        perror("Erreur lors de l'ouverture du fichier");
        return 0;
    }
    start = clock();
    while (fscanf(fichier, "%d", &nombre) == 1)
    {
        insertion(nombre); // Affiche chaque nombre lu
        // Tu peux ici ajouter du code pour utiliser les nombres selon tes besoins
    }
    end = clock();
    int val;
    int opt = 0;

    printf("\nAffichage du B-Tree d'ordre 4 créer : ");
    traversal(root);
    printf("\nle temps de creation est %f sec \n", ((double)(end - start)) / CLOCKS_PER_SEC);
    printf("\n**************Debut du programme************\n");
    int n = 0;
    printf("\n**************Menu************\n");
    printf("1.Insertion \t 2.Suppression \n3.Recherche \t 4.Affichage \n5.Quitter le programme\n");
    printf("Veuillez entrer le chiffre correspond a une opération : ");
    scanf("%d", &n);
    while (n != 5)
    {
        switch (n)
        {
        case 1:
            printf("\nVeuillez entrez le nombre que vous souhaiter inserer : ");
            scanf("%d", &val);
            start = clock();
            insertion(val);
            end = clock();
            printf("Le B-tree aprés insertion : ");
            traversal(root);
            printf("\nle temps d'execution de cette insertion est %f sec \n", ((double)(end - start)) / CLOCKS_PER_SEC);
            break;
        case 2:
            printf("Veuillez entrez le nombre que vous souhaiter supprimer : ");
            scanf("%d", &val);
            start = clock();
            deletion(val, root);
            end = clock();
            printf("Le B-tree aprés suppression : ");
            traversal(root);
            printf("\nle temps d'execution de cette suppression est %f sec \n", ((double)(end - start)) / CLOCKS_PER_SEC);
            break;
        case 3:
            printf("Veuillez entrez le nombre que vous souhaiter rechercher : ");
            scanf("%d", &val);
            start = clock();
            searching(val, &opt, root);
            end = clock();
            printf("\nle temps d'execution de cette recherche est %f sec \n", ((double)(end - start)) / CLOCKS_PER_SEC);
            break;
        case 4:
            printf("Affichage du B-tree : ");
            start = clock();
            traversal(root);
            end = clock();
            printf("\nle temps d'execution de l'affichage est %f sec \n", ((double)(end - start)) / CLOCKS_PER_SEC);
            break;
        default:
            printf("Le chiffre est éronner \n ");
            break;
        }
        printf("\n**************Menu************\n");
        printf("1.Insertion \t 2.Suppression \n3.Recherche \t 4.Affichage \n5.Quitter le programme\n");
        printf("Veuillez entrer le chiffre correspond a une opération : ");
        scanf("%d", &n);
    }

    return 0;
}