#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// noeud B-Tree
typedef struct NoeudBtree
{
    int *cles;
    int t;
    struct NoeudBtree **C;
    int n;
    bool feuille;
} NoeudBtree;

// Btree
typedef struct BTree
{
    NoeudBtree *root;
    int t; // degree minimum
} BTree;

// Déclarations des fonctions utilisées dans le code avant leur définition
void Deviser(NoeudBtree *noeud, int i, NoeudBtree *y);
void SupprimerValFeuille(NoeudBtree *noeud, int idx);
void SupprimerValNoeud(NoeudBtree *noeud, int idx);
void Remplir(NoeudBtree *noeud, int idx);
void emprunterPrec(NoeudBtree *noeud, int idx);
void emprunterSuiv(NoeudBtree *noeud, int idx);
void Fusionner(NoeudBtree *noeud, int idx);
int Predecesseur(NoeudBtree *noeud, int idx);
int Successeur(NoeudBtree *noeud, int idx);

// Constructeur d'un noeud de B-tree
NoeudBtree *CreerNoeudArbre(int t, bool feuille)
{
    NoeudBtree *nvNoeud = (NoeudBtree *)malloc(sizeof(NoeudBtree));
    nvNoeud->t = t;
    nvNoeud->feuille = feuille;
    nvNoeud->cles = (int *)malloc((2 * t - 1) * sizeof(int));
    nvNoeud->C = (NoeudBtree **)malloc(2 * t * sizeof(NoeudBtree *));
    nvNoeud->n = 0;
    return nvNoeud;
}

// Constructeur du B-tree
BTree *createBTree(int t)
{
    BTree *newTree = (BTree *)malloc(sizeof(BTree));
    newTree->root = NULL;
    newTree->t = t;
    return newTree;
}

// A function to AfficherArbre all noeuds in a subtree rooted with this noeud
void AfficherArbre(NoeudBtree *noeud)
{
    if (noeud == NULL)
        return;

    int i;
    for (i = 0; i < noeud->n; i++)
    {
        if (!noeud->feuille)
            AfficherArbre(noeud->C[i]);
        printf(" %d", noeud->cles[i]);
    }

    if (!noeud->feuille)
        AfficherArbre(noeud->C[i]);
}

// une fonction qui permet de faire une recherche sur la valeur et retrouner le noeud courant
NoeudBtree *chercherVal(NoeudBtree *noeud, int k)
{
    if (noeud == NULL)
        return NULL;

    int i = 0;
    while (i < noeud->n && k > noeud->cles[i])
        i++;

    if (noeud->cles[i] == k)
        return noeud;

    if (noeud->feuille)
        return NULL;

    return chercherVal(noeud->C[i], k);
}

// cette fonction permet de verifier si la valeur entrer exist dans l'arbre
void rechercher(NoeudBtree *noeud, int k)
{
    NoeudBtree *b = chercherVal(noeud, k);
    if (b == NULL)
    {
        printf("\nLa valeur %d n'existe pas \n", k);
        return;
    }
    printf("\nLa valeur %d existe\n", k);
    return;
}

// cette fonction retourne l'index de la premiere clé qui est superieur ou egale a k
int TrouverCle(NoeudBtree *noeud, int k)
{
    int idx = 0;
    while (idx < noeud->n && noeud->cles[idx] < k)
        ++idx;
    return idx;
}

// cette fonction sert a inserer une valeur dans un noeud qui n'as pas atteint ca capacité maximale
void insererNonRempli(NoeudBtree *noeud, int k)
{
    int i = noeud->n - 1;

    if (noeud->feuille)
    {
        while (i >= 0 && noeud->cles[i] > k)
        {
            noeud->cles[i + 1] = noeud->cles[i];
            i--;
        }
        noeud->cles[i + 1] = k;
        noeud->n = noeud->n + 1;
    }
    else
    {
        while (i >= 0 && noeud->cles[i] > k)
            i--;

        if (noeud->C[i + 1]->n == 2 * noeud->t - 1)
        {
            Deviser(noeud, i + 1, noeud->C[i + 1]);
            if (noeud->cles[i + 1] < k)
                i++;
        }
        insererNonRempli(noeud->C[i + 1], k);
    }
}

// La fonction qui permet de faire une devision si le noeud a atteint ca capacité maximale
void Deviser(NoeudBtree *noeud, int i, NoeudBtree *y)
{
    NoeudBtree *z = CreerNoeudArbre(y->t, y->feuille);
    z->n = noeud->t - 1;

    for (int j = 0; j < noeud->t - 1; j++)
        z->cles[j] = y->cles[j + noeud->t];

    if (!y->feuille)
    {
        for (int j = 0; j < noeud->t; j++)
            z->C[j] = y->C[j + noeud->t];
    }

    y->n = noeud->t - 1;

    for (int j = noeud->n; j >= i + 1; j--)
        noeud->C[j + 1] = noeud->C[j];

    noeud->C[i + 1] = z;

    for (int j = noeud->n - 1; j >= i; j--)
        noeud->cles[j + 1] = noeud->cles[j];

    noeud->cles[i] = y->cles[noeud->t - 1];

    noeud->n = noeud->n + 1;
}

// la fonction qui permt de supprimer la clé de l'arbre et gérer les propriétés
void supprimerCle(NoeudBtree *noeud, int k)
{
    int idx = TrouverCle(noeud, k);

    if (idx < noeud->n && noeud->cles[idx] == k)
    {
        if (noeud->feuille)
            SupprimerValFeuille(noeud, idx);
        else
            SupprimerValNoeud(noeud, idx);
    }
    else
    {
        if (noeud->feuille)
        {
            printf("la cle %d n'existe pas dans l'arbre\n", k);
            return;
        }

        bool flag = (idx == noeud->n) ? true : false;

        if (noeud->C[idx]->n < noeud->t)
            Remplir(noeud, idx);

        if (flag && idx > noeud->n)
            supprimerCle(noeud->C[idx - 1], k);
        else
            supprimerCle(noeud->C[idx], k);
    }
    return;
}

// une fonction qui permt de retirer une valeur a partir d'un noeud feuille
void SupprimerValFeuille(NoeudBtree *noeud, int idx)
{
    for (int i = idx + 1; i < noeud->n; ++i)
        noeud->cles[i - 1] = noeud->cles[i];

    noeud->n--;

    return;
}

// fonctio qui permet de retirer une valeur d'un noeud
void SupprimerValNoeud(NoeudBtree *noeud, int idx)
{
    int k = noeud->cles[idx];

    if (noeud->C[idx]->n >= noeud->t)
    {
        int pred = Predecesseur(noeud, idx);
        noeud->cles[idx] = pred;
        supprimerCle(noeud->C[idx], pred);
    }
    else if (noeud->C[idx + 1]->n >= noeud->t)
    {
        int succ = Successeur(noeud, idx);
        noeud->cles[idx] = succ;
        supprimerCle(noeud->C[idx + 1], succ);
    }
    else
    {
        Fusionner(noeud, idx);
        supprimerCle(noeud->C[idx], k);
    }
    return;
}

// Fonction qui permet de d'avoir le predecesseur d'une clé

int Predecesseur(NoeudBtree *noeud, int idx)
{
    NoeudBtree *cur = noeud->C[idx];
    while (!cur->feuille)
        cur = cur->C[cur->n];

    return cur->cles[cur->n - 1];
}

// Fonction qui permet de d'avoir le successeur d'une clé
int Successeur(NoeudBtree *noeud, int idx)
{
    NoeudBtree *cur = noeud->C[idx + 1];
    while (!cur->feuille)
        cur = cur->C[0];

    return cur->cles[0];
}

// Fonction qui permet de remplir le noeud avec des valeurs
void Remplir(NoeudBtree *noeud, int idx)
{
    if (idx != 0 && noeud->C[idx - 1]->n >= noeud->t)
        emprunterPrec(noeud, idx);
    else if (idx != noeud->n && noeud->C[idx + 1]->n >= noeud->t)
        emprunterSuiv(noeud, idx);
    else
    {
        if (idx != noeud->n)
            Fusionner(noeud, idx);
        else
            Fusionner(noeud, idx - 1);
    }
    return;
}

void emprunterPrec(NoeudBtree *noeud, int idx)
{
    NoeudBtree *fils = noeud->C[idx];
    NoeudBtree *enfants = noeud->C[idx - 1];

    for (int i = fils->n - 1; i >= 0; --i)
        fils->cles[i + 1] = fils->cles[i];

    if (!fils->feuille)
    {
        for (int i = fils->n; i >= 0; --i)
            fils->C[i + 1] = fils->C[i];
    }

    fils->cles[0] = noeud->cles[idx - 1];

    if (!fils->feuille)
        fils->C[0] = enfants->C[enfants->n];

    noeud->cles[idx - 1] = enfants->cles[enfants->n - 1];

    fils->n += 1;
    enfants->n -= 1;

    return;
}

void emprunterSuiv(NoeudBtree *noeud, int idx)
{
    NoeudBtree *fils = noeud->C[idx];
    NoeudBtree *enfants = noeud->C[idx + 1];

    fils->cles[(fils->n)] = noeud->cles[idx];

    if (!fils->feuille)
        fils->C[(fils->n) + 1] = enfants->C[0];

    noeud->cles[idx] = enfants->cles[0];

    for (int i = 1; i < enfants->n; ++i)
        enfants->cles[i - 1] = enfants->cles[i];

    if (!enfants->feuille)
    {
        for (int i = 1; i <= enfants->n; ++i)
            enfants->C[i - 1] = enfants->C[i];
    }

    fils->n += 1;
    enfants->n -= 1;

    return;
}

// cette fonction permet de faire ls fusions des fils d'index idn t idx+1
void Fusionner(NoeudBtree *noeud, int idx)
{
    NoeudBtree *fils = noeud->C[idx];
    NoeudBtree *enfants = noeud->C[idx + 1];

    fils->cles[noeud->t - 1] = noeud->cles[idx];

    for (int i = 0; i < enfants->n; ++i)
        fils->cles[i + noeud->t] = enfants->cles[i];

    if (!fils->feuille)
    {
        for (int i = 0; i <= enfants->n; ++i)
            fils->C[i + noeud->t] = enfants->C[i];
    }

    for (int i = idx + 1; i < noeud->n; ++i)
        noeud->cles[i - 1] = noeud->cles[i];

    for (int i = idx + 2; i <= noeud->n; ++i)
        noeud->C[i - 1] = noeud->C[i];

    fils->n += enfants->n + 1;
    noeud->n--;

    free(enfants);
    return;
}

// La fonction principale d'insertion
void inserer(BTree *tree, int k)
{
    if (tree->root == NULL)
    {
        tree->root = CreerNoeudArbre(tree->t, true);
        tree->root->cles[0] = k;
        tree->root->n = 1;
    }
    else
    {
        if (chercherVal(tree->root, k))
        {
            printf("la valeur %d existe deja dans l'arbre B\n", k);
            return;
        }
        if (tree->root->n == 2 * tree->t - 1)
        {
            NoeudBtree *s = CreerNoeudArbre(tree->t, false);
            s->C[0] = tree->root;
            Deviser(s, 0, tree->root);
            int i = 0;
            if (s->cles[0] < k)
                i++;
            insererNonRempli(s->C[i], k);
            tree->root = s;
        }
        else
            insererNonRempli(tree->root, k);
    }
}

// La fonction qui permet de supprimer une valeur a partir de l'arbre
void SupprimerCleDuNoeud(BTree *tree, int k)
{
    if (!tree->root)
    {
        printf("l'arbre est vide\n");
        return;
    }

    supprimerCle(tree->root, k);

    if (tree->root->n == 0)
    {
        NoeudBtree *tmp = tree->root;
        if (tree->root->feuille)
            tree->root = NULL;
        else
            tree->root = tree->root->C[0];

        free(tmp);
    }
    return;
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

// Driver program to test above functions
int main()
{
    clock_t start, end;
    BTree *arbre = createBTree(3);

    const char *nomFichier = "nombres_aleatoires.txt";
    printf("Veuillez entrer la taille (le nombre de chiffre que vous souhaitez avoir dans l'arbre) \n");
    int t;
    scanf("%d", &t);
    int taille = t;        // Générer 1 million de nombres
    int borneMin = 1;      // Valeur minimale
    int borneMax = 10 * t; // Valeur maximale
    genererNombresAleatoiresOptimise("nombres_aleatoires.txt", taille, borneMin, borneMax);

    // Creation de l'arbre
    FILE *fichier = fopen(nomFichier, "r");
    int nombre;
    if (!fichier)
    {
        perror("Erreur lors de l'ouverture du fichier");
        return 0;
    }
    start = clock();
    while (fscanf(fichier, "%d", &nombre) == 1)
    {
        inserer(arbre, nombre); // Affiche chaque nombre lu
        // Tu peux ici ajouter du code pour utiliser les nombres selon tes besoins
    }
    end = clock();
    int val;
    int opt = 0;
    printf("\nle temps de creation est %f sec \n", ((double)(end - start)) / CLOCKS_PER_SEC);
    printf("\nAffichage du B-Tree de degrée 3 ou chaque peut contenir au max 2*3-1 clés et 2*3 fils créer : ");
    AfficherArbre(arbre->root);

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
            inserer(arbre, val);
            end = clock();
            printf("Opération terminer\n");
            printf("\nle temps d'execution de cette insertion est %f sec \n", ((double)(end - start)) / CLOCKS_PER_SEC);
            break;
        case 2:
            printf("Veuillez entrez le nombre que vous souhaiter supprimer : ");
            scanf("%d", &val);
            start = clock();
            SupprimerCleDuNoeud(arbre, val);
            end = clock();
            printf("Opération terminer\n");
            printf("\nle temps d'execution de cette suppression est %f sec \n", ((double)(end - start)) / CLOCKS_PER_SEC);
            break;
        case 3:
            printf("Veuillez entrez le nombre que vous souhaiter rechercher : ");
            scanf("%d", &val);
            start = clock();
            rechercher(arbre->root, val);
            end = clock();
            printf("Opération terminer\n");
            printf("\nle temps d'execution de cette recherche est %f sec \n", ((double)(end - start)) / CLOCKS_PER_SEC);
            break;
        case 4:
            printf("Affichage du B-tree : ");
            start = clock();
            AfficherArbre(arbre->root);
            end = clock();
            printf("\nOpération terminer\n");
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
