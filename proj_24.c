#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "proj_24.h"

noeud *new_noeud(int player, noeud *pere, int coup);
int test_horizontal(int **tab, int colonne_);
int test_vertical(int **tab, int colonne_);
int test_oblique(int **tab, int colonne_);
int test_anti_oblique(int **tab, int colonne_);
int max4(int a, int b, int c, int d);
int min(int a, int b);
int feuille(noeud *N);
int **tableau();
void free_tableau(int **tab);
int colonne_pleine(int **tab, int c);
int ToutesLesColonnesPleines(int **tab);
int test4(int **tab, int colonne_);
int PremiereColonneDisponible(int **tab);
void appliquerCoup(int **tab, int c, int player);
void afficher_tab(int **tab);
void retirerCoup(int **tab, int c);
int demander_de_jouer(int **tab);
int attraperDernierVisite(noeud *racine, noeud *dernier);
int minimax(noeud *racine, int **tab);
void free_arbre(noeud *racine);
int eval(noeud *racine, int **tab, int profondeur);
void placer_jeton_deux_joueurs(int **tab, int colonne_jouee, int *joueur_1, int *joueur_2);
void lancer_le_jeux_entre_deux_joueurs(int **tab);
void lancer_jeux_entre_IA(int **grille);

int main()
{
  system("clear");
  printf("             Puissance 4\n");
  printf("            -------------\n");
  printf("\n\n");
  printf("Voulez-vous jouer contre l'IA ? Tapez 1\n");
  printf("Vous voulez jouer à deux, tapez 2\n");
  int c;
  scanf("%d", &c);
  while (c != 1 && c != 2)
  {
    system("clear");
    printf("Voulez-vous jouer contre l'IA ? Tapez 1\n");
    printf("Vous voulez jouer à deux, tapez 2\n");
    scanf("%d", &c);
  }
  system("clear");
  int **grille = tableau();
  afficher_tab(grille);
  if (c == 1)
  {
    lancer_jeux_entre_IA(grille);
  }
  else
  {
    lancer_le_jeux_entre_deux_joueurs(grille);
  }
  free_tableau(grille);
  return 1;
}
