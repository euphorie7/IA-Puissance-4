#define colonne 7
#define ligne 6
#define BLUE_TEXT "\x1b[34m"
#define RESET_TEXT "\x1b[0m"
#define RED_TEXT "\x1b[31m"

typedef struct noeud_
{
  struct noeud_ *pere;
  int player;
  int coup;
  int score;
  struct noeud_ *suivants[7];
} noeud;

noeud *new_noeud(int player, noeud *pere, int coup)
{

  noeud *Noeud = (noeud *)malloc(sizeof(noeud));
  Noeud->score = -1;
  Noeud->pere = pere;
  Noeud->player = player;
  Noeud->coup = coup;
  for (int i = 0; i < colonne; i++)
  {
    Noeud->suivants[i] = NULL;
  }
  return Noeud;
}

int **tableau() // fonction qui alloue un tableau (notre fameuse grille de jeu par exemple)
{

  int **tab = (int **)malloc(ligne * sizeof(int *));
  if (tab == NULL)
  {
    printf("Probleme d'allocation\n");
    return NULL;
  }
  int i, j;
  for (i = 0; i < ligne; i++)
  {
    tab[i] = (int *)malloc(colonne * sizeof(int));
    if (tab[i] == NULL)
    {
      printf("Probleme d'allocation\n");
      return NULL;
    }
    for (j = 0; j < colonne; j++)
    {
      tab[i][j] = 0;
    }
  }
  return tab;
}

void free_tableau(int **tab) // libère la mémoire allouée pour un tableau
{
  int i;
  for (i = 0; i < ligne; i++)
    free(tab[i]);
  free(tab);
}

int test_horizontal(int **tab, int colonne_) // teste s'il y a alignement de quatre jetons de la meme couleur à l'horizontale
{
  int i = 0;
  int j = colonne_;
  while (i < ligne && tab[i][colonne_] == 0)
  {
    i++;
  }
  int c = 0; // compteur
  // rouge (1) ou bleu (2)
  if (i == ligne)
  {
    return 0;
  }
  int f = tab[i][colonne_];
  // f prendra la valeur 1 ou 2
  while (j + 1 < colonne && tab[i][j + 1] == f) // vers la droite
  {
    c++;
    j++;
  }
  j = colonne_;
  while (j - 1 >= 0 && tab[i][j - 1] == f) // vers la gauche
  {
    c++;
    j--;
  }
  return c + 1;
}

int test_vertical(int **tab, int colonne_) // teste s'il y a alignement de quatre jetons de la meme couleur à la verticale
{
  int i = 0;
  int j = colonne_;
  while (i < ligne && tab[i][colonne_] == 0)
  {
    i++;
  }
  if (i == ligne)
  {
    return 0;
  }
  int c = 0;
  int f = tab[i][j];                          // f prendra la valeur 1 ou 2
  while (i + 1 < ligne && tab[i + 1][j] == f) // vers le bas
  {
    c++;
    i++;
  }
  return c + 1;
}

int feuille(noeud *N) // teste si un noeud est une feuille (1) ou non (0) (autrement dit si tous ses fils sont nuls ou non)
{
  if (N != NULL)
  {
    int j;
    for (j = 0; j < colonne; j++)
    {
      if (N->suivants[j] != NULL)
      {
        return 0;
      }
    }
    return 1;
  }
  return 0;
}

int test_oblique(int **tab, int colonne_) // teste s'il y a alignement de quatre jetons de la meme couleur en diagonale
{
  int i = 0;
  int j = colonne_;
  while (i < ligne && tab[i][colonne_] == 0)
  {
    i++;
  }
  if (i == ligne)
  {
    return 0;
  }
  int ordonne = i;
  int c = 0;
  int f = tab[i][j]; // f prendra la valeur 1 ou 2
  while (i - 1 >= 0 && j + 1 < 0 && tab[i - 1][j + 1] == f)
  {
    c++;
    j++;
    i--;
  }
  j = colonne_;
  i = ordonne;
  while (i + 1 < ligne && j - 1 >= 0 && tab[i + 1][j - 1] == f)
  {
    c++;
    j--;
    i++;
  }
  return c + 1;
}

int test_anti_oblique(int **tab, int colonne_) // teste s'il y a alignement de quatre jetons de la meme couleur en anti-diagonale
{
  int i = 0;
  int j = colonne_;
  while (i < ligne && tab[i][colonne_] == 0)
  {
    i++;
  }
  if (i == ligne)
  {
    return 0;
  }
  int ordonne = i;
  int c = 0;
  int f = tab[i][j]; // f prendra la valeur 1 ou 2
  while (j - 1 >= 0 && i - 1 >= 0 && tab[i - 1][j - 1] == f)
  {
    c++;
    i--;
    j--;
  }
  i = ordonne;
  j = colonne_;
  while (i + 1 < ligne && j + 1 < colonne && tab[i + 1][j + 1] == f)
  {
    c++;
    i++;
    j++;
  }
  return c + 1;
}

int max4(int a, int b, int c, int d) // max entre quatre valeurs
{
  int max = d;
  if (a > max)
    max = a;
  if (b > max)
    max = b;
  if (c > max)
    max = c;
  return max;
}

int min(int a, int b) // min entre deux valeurs
{
  int min = a;
  if (b < min)
  {
    min = b;
  }
  return min;
}

int colonne_pleine(int **tab, int c) // teste si une colonne est pleine (1) ou non (0)
{
  if (tab[0][c] == 0)
  {
    return 0;
  }
  return 1;
}

int test4(int **tab, int colonne_) // teste s'il y a alignement de quatre jetons de la meme couleur
{
  int a, b, c, d;
  a = test_horizontal(tab, colonne_);
  b = test_vertical(tab, colonne_);
  c = test_oblique(tab, colonne_);
  d = test_anti_oblique(tab, colonne_);
  return max4(a, b, c, d);
}

int attraperDernierVisite(noeud *racine, noeud *dernier)
{
  if (racine != NULL)
  {
    int i = 0;
    while (i < colonne && racine->suivants[i] != dernier)
    {
      i++;
    }
    if (i + 1 < colonne)
    {
      return i + 1;
    }
    return colonne;
  }
  return colonne;
}

void retirerCoup(int **tab, int c)
{
  int i = 0;
  while (i < ligne && tab[i][c] == 0)
  {
    i++;
  }
  if (i < ligne)
  {
    tab[i][c] = 0;
  }
}

void appliquerCoup(int **tab, int c, int player)
{
  int i = 0;
  while (i + 1 < ligne && tab[i + 1][c] == 0)
  {
    i++;
  }
  if (player == 1)
  {
    tab[i][c] = 1;
  }
  else
  {
    tab[i][c] = 2;
  }
}

int PremiereColonneDisponible(int **tab)
{
  int i = 0;
  while (i < colonne && colonne_pleine(tab, i) == 1)
  {
    i++;
  }
  if (i < colonne)
  {
    return i;
  }
  else
  {
    return colonne;
  }
}

int ToutesLesColonnesPleines(int **tab) // teste si toutes les colonnes sont pleines (1) ou non (0)
{
  int i;
  for (i = 0; i < colonne; i++)
    if (colonne_pleine(tab, i) != 1)
    {
      return 0;
    }
  return 1;
}
void afficher_tab_backup(int **tab)
{
  int i;
  for (i = 0; i < ligne; i++)
  {
    for (int j = 0; j < colonne; j++)
    {
      printf("+-------");
    }
    printf("+\n");
    for (int j = 0; j < colonne; j++)
    {
      printf("|       ");
    }
    printf("|\n");
    for (int j = 0; j < colonne; j++)
    {
      printf("|");
      if (tab[i][j] == 0)
      {
        printf("       ");
      }
      else if (tab[i][j] == 1)
      {
        printf("   B   "); // B pour Bleu 🔵
      }
      else if (tab[i][j] == 2)
      {
        printf("   R   "); // R pour Rouge 🔴
      }
    }
    printf("|\n");
    for (int j = 0; j < colonne; j++)
    {
      printf("|       ");
    }
    printf("|\n");
  }
  for (int j = 0; j < colonne; j++)
  {
    printf("+-------");
  }
  printf("+\n");
}
void afficher_tab(int **tab) // affiche la grille
{
  int i;
  for (i = 0; i < ligne; i++)
  {
    for (int j = 0; j < colonne; j++)
    {
      printf("+-------");
    }
    printf("+\n");
    for (int j = 0; j < colonne; j++)
    {
      printf("|       ");
    }
    printf("|\n");
    for (int j = 0; j < colonne; j++)
    {
      printf("|");
      if (tab[i][j] == 0)
      {
        printf("       ");
      }
      else if (tab[i][j] == 1)
      {
        printf(BLUE_TEXT "   •   " RESET_TEXT); // B pour Bleu 🔵
      }
      else if (tab[i][j] == 2)
      {
        printf(RED_TEXT "   •   " RESET_TEXT); // R pour Rouge 🔴
      }
    }
    printf("|\n");
    for (int j = 0; j < colonne; j++)
    {
      printf("|       ");
    }
    printf("|\n");
  }
  for (int j = 0; j < colonne; j++)
  {
    printf("+-------");
  }
  printf("+\n");
}

int demander_de_jouer(int **tab) // demande à l'utilisateur la colonne qu'il souhaite jouer
{
  int c;
  printf("Veuillez choisir la colonne où vous vouler jouer votre jeton (1 ... 7)\n");
  scanf("%d", &c);
  while (!(c >= 1 && c <= 7))
  {
    printf("Veuillez choisir une colonne entre 1 et 7\n");
    scanf("%d", &c);
  }
  while (tab[0][c - 1] != 0)
  {
    printf("Cette colonne est pleine, veuillez choisir une autre colonne !\n");
    scanf("%d", &c);
  }
  return c - 1;
}

void free_arbre(noeud *racine) // libère la mémoire allouée pour l'arbre de parcours
{
  int d = 0;
  noeud *dernierVisite = NULL;
  noeud *courant = racine;
  while (courant != NULL)
  {
    if (courant->pere == dernierVisite) // descente
    {
      if (feuille(courant))
      {
        noeud *aLiberer = courant; // si feuille, on libère le noeud
        courant = courant->pere;   // on remonte au père
        if (courant != NULL)
        {
          courant->suivants[aLiberer->coup] = NULL;
        }
        free(aLiberer);
        dernierVisite = NULL;
        d++;
      }
      else // noeud courant a des fils -> on continue le parcours
      {
        int i = 0;
        while (i < colonne && courant->suivants[i] == NULL)
        {
          i++;
        }
        dernierVisite = courant;
        courant = courant->suivants[i];
      }
    }
    else // on remonte dans l'arbre
    {
      if (dernierVisite != NULL)
      {
        printf("erreur\n");
      }
      if (feuille(courant))
      {
        noeud *aLiberer = courant;
        courant = courant->pere;
        if (courant != NULL)
        {
          courant->suivants[aLiberer->coup] = NULL;
        }
        free(aLiberer);
        dernierVisite = NULL;
        d++;
      }
      else
      {
        int i = 0;
        while (i < colonne && courant->suivants[i] == NULL)
        {
          i++;
        }
        dernierVisite = courant;
        courant = courant->suivants[i];
      }
    }
  }
}

int calculer_score(int playerIA, int playervs, int empty) // fonction appellée par la fonction eval
{
  if (playerIA == 4)
    return 100;
  if (playervs == 4)
    return -95;
  if (playerIA == 3 && empty == 1)
    return 5;
  if (playervs == 3 && empty == 1)
    return -4;
  if (playerIA == 2 && empty == 2)
    return 2;
  if (playervs == 2 && empty == 2)
    return -1;
  return 0;
}

int eval(noeud *courant, int **tab, int profondeur) // évalue le score de la grille à un état donné
{
  int i, j, k;
  if ((feuille(courant) || profondeur == 5) && courant->pere != NULL)
  {
    int score = 0;
    ///// parcours par ligne
    for (i = 0; i < ligne; i++)
    {
      for (j = 0; j < colonne - 3; j++)
      {
        int playerIA = 0, playervs = 0, empty = 0;
        for (k = 0; k < 4; k++)
        {
          if (tab[i][j + k] == 1)
          {
            playerIA++;
          }
          else if (tab[i][j + k] == 2)
          {
            playervs++;
          }
          else
          {
            empty++;
          }
        }
        score += calculer_score(playerIA, playervs, empty);
      }
    }
    //// parcours par colonne
    for (j = 0; j < colonne; j++)
    {
      for (i = 0; i < ligne - 3; i++)
      {
        int playerIA = 0, playervs = 0, empty = 0;
        for (k = 0; k < 4; k++)
        {
          if (tab[i + k][j] == 1)
          {
            playerIA++;
          }
          else if (tab[i + k][j] == 2)
          {
            playervs++;
          }
          else
          {
            empty++;
          }
        }
        score += calculer_score(playerIA, playervs, empty);
      }
    }
    // parcours par diagonale du bas à gauche vers le haut à droite (oblique)
    for (i = 3; i < ligne; i++)
    {
      for (j = 0; j < colonne - 3; j++)
      {
        int playerIA = 0, playervs = 0, empty = 0;
        for (k = 0; k < 4; k++)
        {
          if (tab[i - k][j + k] == 1)
          {
            playerIA++;
          }
          else if (tab[i - k][j + k] == 2)
          {
            playervs++;
          }
          else
          {
            empty++;
          }
        }
        score += calculer_score(playerIA, playervs, empty);
      }
    }
    // parcours par diagonale du haut à gauche vers le bas à droite (anti-oblique)
    for (j = 0; j < colonne - 3; j++)
    {
      for (i = 0; i < ligne - 3; i++)
      {
        int playerIA = 0, playervs = 0, empty = 0;
        for (k = 0; k < 4; k++)
        {
          if (tab[i + k][j + k] == 1)
          {
            playerIA++;
          }
          else if (tab[i + k][j + k] == 2)
          {
            playervs++;
          }
          else
          {
            empty++;
          }
        }
        score += calculer_score(playerIA, playervs, empty);
      }
    }
    return score; // renvoie le score total de la grille
  }
  else
  {
    if (courant != NULL)
    {
      if (courant->player == 1) // renvoie le meilleur score possible de telle sorte à ce que l'IA gagne (utilisateur (min) ou bien IA (max))
      {
        int j;
        int max = INT_MIN;
        for (j = 0; j < colonne; j++)
        {
          if (courant->suivants[j] != NULL && courant->suivants[j]->score >= max)
          {
            max = courant->suivants[j]->score;
          }
        }
        return max;
      }
      else
      {
        int j;
        int min = INT_MAX;
        for (j = 0; j < colonne; j++)
        {
          if (courant->suivants[j] != NULL && courant->suivants[j]->score <= min)
          {
            min = courant->suivants[j]->score;
          }
        }
        return min;
      }
    }
  }
  return -1;
}

int minimax(noeud *racine, int **tab)
{ // parcourt l'arbre en profondeur et évalue le meilleur coup jouable par l'IA.
  noeud *dernierVisite = NULL;
  noeud *courant = racine;
  int profondeur = 0;
  while (courant != NULL)
  { // parcourt l'arbre jusqu'à ce que tous les noeuds soient visités.
    if (courant->pere == dernierVisite)
    { // descente
      if (ToutesLesColonnesPleines(tab) != 1 && test4(tab, courant->coup) < 4 && profondeur < 5)
      { // grille non complètement pleine, pas de coup gagnant, prof max pas atteinte
        int c = 0;
        while (c < colonne && (colonne_pleine(tab, c) == 1))
        {
          c++;
        }
        if (c < colonne)
        { // colonne disponible
          profondeur++;
          courant->suivants[c] = new_noeud((courant->player % 2) + 1, courant, c); // nouveau noeud pour le coup possible et l'applique sur le plateau.
          appliquerCoup(tab, c, courant->player);
          dernierVisite = courant;
          courant = courant->suivants[c];
        }
        else
        {
          // si aucune colonne n'est disponible, remonte dans l'arbre.
          courant->score = eval(courant, tab, profondeur);
          retirerCoup(tab, courant->coup);
          dernierVisite = courant;
          courant = courant->pere;
          profondeur--;
        }
      }
      else
      {
        // si feuille ou profondeur max, évalue le noeud courant et remonte.
        courant->score = eval(courant, tab, profondeur);
        retirerCoup(tab, courant->coup);
        dernierVisite = courant;
        courant = courant->pere;
        profondeur--;
      }
    }
    else
    {
      // si remontée dans l'arbre ou passage à un frère, trouve le prochain frère disponible ou remonte si aucun frère n'est disponible.
      int c = 0;
      while (c < colonne && ((colonne_pleine(tab, c) == 1) || courant->suivants[c] != NULL))
      {
        c++;
      }
      // si un frère est disponible.
      if (c < colonne)
      {
        profondeur++;
        courant->suivants[c] = new_noeud((courant->player % 2) + 1, courant, c);
        appliquerCoup(tab, c, courant->player);
        dernierVisite = courant;
        courant = courant->suivants[c];
      }
      else
      {
        // si aucun frère n'est disponible, remonte dans l'arbre.
        courant->score = eval(courant, tab, profondeur);
        retirerCoup(tab, courant->coup);
        dernierVisite = courant;
        courant = courant->pere;
        profondeur--;
      }
    }
  }
  // racine de l'arbre atteinte (courant = NULL) -> on réapplique le coup
  appliquerCoup(tab, dernierVisite->coup, (dernierVisite->player % 2) + 1);
  int j;
  for (j = 0; j < colonne; j++)
  {
    if (dernierVisite->suivants[j] != NULL && dernierVisite->suivants[j]->score == dernierVisite->score)
    {
      return j;
    }
  }
  return 1000;
}

void placer_jeton_deux_joueurs(int **tab, int colonne_jouee, int *joueur_1, int *joueur_2) // place le jeton dans la grille de jeu
{
  int i = 0;
  while (i < ligne && tab[i][colonne_jouee] == 0)
  {
    i++;
  }
  if (*(joueur_1) == 1)
  {
    tab[i - 1][colonne_jouee] = 1;
  }
  else
  {
    tab[i - 1][colonne_jouee] = 2;
  }
  *(joueur_1) = (*(joueur_1) + 1) % 2;
  *(joueur_2) = (*(joueur_2) + 1) % 2;
}

void lancer_le_jeux_entre_deux_joueurs(int **tab) // lance Puissance 4 entre deux joueurs (équivalent du main() de la première partie)
{
  int fin = 0;
  int joueur_1 = 1;
  int joueur_2 = 0;
  while (ToutesLesColonnesPleines(tab) != 1 && fin != 1)
  {
    int c = demander_de_jouer(tab);
    placer_jeton_deux_joueurs(tab, c, &joueur_1, &joueur_2);
    system("clear");
    afficher_tab(tab);
    if (test4(tab, c) >= 4)
    {
      int i = 0;
      int j = c;
      while (i < ligne && tab[i][c] == 0)
      {
        i++;
      }
      fin = 1;
      if (tab[i][j] == 1)
      {
        printf("Le BLEU gagne\n");
      }
      else
      {
        printf("Le ROUGE gagne\n");
      }
    }
  }
}
void lancer_jeux_entre_IA(int **grille)
{
  int fin = 0;
  while (ToutesLesColonnesPleines(grille) != 1 && fin != 1)
  {
    time_t start, end;
    int c = demander_de_jouer(grille);
    time(&start);
    appliquerCoup(grille, c, 2);
    system("clear");
    afficher_tab(grille);
    if (test4(grille, c) >= 4)
    {
      fin = 1;
      printf("Vous avez gagné, quel exploit !!\n"); // impossible sauf si profondeur égale à 1 (un joueur de Puissance 4 experimenté pourrait gagner si profondeur égale à 2 ou même 3 (à voir))
    }
    else if (ToutesLesColonnesPleines(grille))
    {
      fin = 1;
      printf("Egalité !!\n");
    }
    else
    {
      noeud *racine = new_noeud(1, NULL, c);
      int n = minimax(racine, grille);
      printf("La colonne choisie par l'IA est : %d \n", n);
      free_arbre(racine);
      racine = NULL;
      appliquerCoup(grille, n, 1);
      system("clear");
      afficher_tab(grille);
      time(&end);
      if (test4(grille, n) >= 4)
      {
        fin = 1;
        printf("L'IA gagne !! \n");
      }
      else if (ToutesLesColonnesPleines(grille))
      {
        fin = 1;
        printf("Egalité !!\n");
      }
    }
    }
}
