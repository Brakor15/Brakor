#include<stdlib.h>
#include<stdio.h>
#include<graph.h>
#include<math.h>
#include<time.h>
#include <unistd.h>

#define TAILLE_TERRAIN_DE_JEU 3
#define MAX 9
#define LARGEUR_FENETRE 800
#define LONGUEUR_FENETRE 800
#define JOUEUR_PAS_ENCORE_PLACE -1
#define PLACEMENT_JOUEUR_UN 14
#define PLACEMENT_JOUEUR_DEUX 18
#define PHASE_COCHER_CASE1 11
#define PHASE_DEP_JOUEUR1 1
#define PHASE_DEP_JOUEUR2 2
#define PHASE_COCHER_CASE2 22
#define PHASE_GAME_OVER_JOUEUR_1 -4
#define PHASE_GAME_OVER_JOUEUR_2 -8
#define PHASE_GAME_OVER_EGALITE -48
#define FIN_DEPLACEMENT 1
#define DEPLACEMENT_POSSIBLE 0

/*Affichage console, sera retiré */
void print_board(int tab[][9], int taille_tableau){
	int i,z;

	for(i=0; i<taille_tableau; i++){
		for(z=0; z<taille_tableau; z++){
			if (tab[z][i] == 0)
				printf("0");
			else{
				printf("%d", tab[z][i]);
			}
		}
		putchar('\n');
	}
	putchar('\n');
}



/*Verification du nb de cases libre pour un joueur donné. Le joueur doit être donné en coordonnées int (x,y)*/
int dep_nb_case_possibles(int tab[][9], int taille_tableau, int* pos_x, int* pos_y){
	int nb_cases_disponibles = 0;

	if (*pos_x == JOUEUR_PAS_ENCORE_PLACE)
		return -1;
	printf("posx %d\n", *pos_x-1);
	printf("posy %d\n", *pos_y-1);
	if ((*pos_x + 1 < taille_tableau) && (tab[*pos_x + 1][*pos_y] == 0))
		nb_cases_disponibles++;
	if ((*pos_x - 1 >= 0) && (tab[*pos_x - 1][*pos_y] == 0))
		nb_cases_disponibles++;
	if ((*pos_y + 1 < taille_tableau) && (tab[*pos_x][*pos_y + 1] == 0))
		nb_cases_disponibles++;
	if ((*pos_y - 1 >= 0) && (tab[*pos_x][*pos_y - 1] == 0))
		nb_cases_disponibles++;

	if ((*pos_x - 1 >= 0) && (*pos_y - 1 >= 0) && (tab[*pos_x-1][*pos_y-1] == 0)) /* case haut à gauche */
	  nb_cases_disponibles++;
	if ((*pos_x + 1 < taille_tableau) && (*pos_y + 1 < taille_tableau) && (tab[*pos_x+1][*pos_y+1] == 0)) /*Case bas droite */
	  nb_cases_disponibles++;
	if ((*pos_x - 1 >= 0) && (*pos_y + 1 < taille_tableau) && (tab[*pos_x-1][*pos_y+1] == 0)) /*Case bas à gauche */
	  nb_cases_disponibles++;
	if ((*pos_x + 1 < taille_tableau) && (*pos_y - 1 >= 0) && (tab[*pos_x+1][*pos_y-1] == 0)) /*Case bas à droite */
	  nb_cases_disponibles++;


	return nb_cases_disponibles;
}

/*Permet de calculer l'arondi d'un float, utilisé dans la fonction initialisation_jeu
*
* Post-condition : Renvoie un double arrondit à l'unité voulue
*/

double arrondi(double value, int digits){
  return floor(value * pow(10, digits) + 0.5) / pow(10, digits);
}


/* initialisation_jeu est une fonction qui remplis toutes les cases du tableau par un "0", et calcule et renvoie la taille de chaque case qui sera affichée à l'écran
* Pré-condition : tableau d'entiers à double dimension de taille 9x9, la variable int taille_case ainsi que la taille_tableau
* Post-condition : remplie le tableau de 0, et renvoie la taille des cases qui seront affichées
*/
void initialisation_jeu(int tab[][9], int* taille_case, int taille_tableau, int* sp_mur_normal,int* sp_bord_gauche,int* sp_bord_droit,int* sp_bord_haut, int* sp_bord_bas){
	int i, z;
	for(i=0; i<9; i++){
		for(z=0; z<9; z++){
			tab[z][i] = 0;
		}
	}
	*taille_case = (int)arrondi(((double)LARGEUR_FENETRE/(double)taille_tableau), 0);
	if (taille_tableau == 3){
		*sp_mur_normal = ChargerSprite("images/murs/3x3/mur3x3.png");
		*sp_bord_gauche = ChargerSprite("images/murs/3x3/mur_gauche.png");
		*sp_bord_droit = ChargerSprite("images/murs/3x3/mur_droite.png");
		*sp_bord_haut = ChargerSprite("images/murs/3x3/mur_haut.png");
		*sp_bord_bas = ChargerSprite("images/murs/3x3/mur_bas.png");
	}
	if (taille_tableau == 4){
		*sp_mur_normal = ChargerSprite("images/murs/4x4/mur4x4.png");
		*sp_bord_gauche = ChargerSprite("images/murs/4x4/mur_gauche.png");
		*sp_bord_droit = ChargerSprite("images/murs/4x4/mur_droite.png");
		*sp_bord_haut = ChargerSprite("images/murs/4x4/mur_haut.png");
		*sp_bord_bas = ChargerSprite("images/murs/4x4/mur_bas.png");
	}
	if (taille_tableau == 5){
		*sp_mur_normal = ChargerSprite("images/murs/5x5/mur5x5.png");
		*sp_bord_gauche = ChargerSprite("images/murs/5x5/mur_gauche.png");
		*sp_bord_droit = ChargerSprite("images/murs/5x5/mur_droite.png");
		*sp_bord_haut = ChargerSprite("images/murs/5x5/mur_haut.png");
		*sp_bord_bas = ChargerSprite("images/murs/5x5/mur_bas.png");
	}
	if (taille_tableau == 6){
		*sp_mur_normal = ChargerSprite("images/murs/6x6/mur6x6.png");
		*sp_bord_gauche = ChargerSprite("images/murs/6x6/mur_gauche.png");
		*sp_bord_droit = ChargerSprite("images/murs/6x6/mur_droite.png");
		*sp_bord_haut = ChargerSprite("images/murs/6x6/mur_haut.png");
		*sp_bord_bas = ChargerSprite("images/murs/6x6/mur_bas.png");
	}
	if (taille_tableau == 7){
		*sp_mur_normal = ChargerSprite("images/murs/7x7/mur7x7.png");
		*sp_bord_gauche = ChargerSprite("images/murs/7x7/mur_gauche.png");
		*sp_bord_droit = ChargerSprite("images/murs/7x7/mur_droite.png");
		*sp_bord_haut = ChargerSprite("images/murs/7x7/mur_haut.png");
		*sp_bord_bas = ChargerSprite("images/murs/7x7/mur_bas.png");
	}
	if (taille_tableau == 8){
		*sp_mur_normal = ChargerSprite("images/murs/8x8/mur8x8.png");
		*sp_bord_gauche = ChargerSprite("images/murs/8x8/mur_gauche.png");
		*sp_bord_droit = ChargerSprite("images/murs/8x8/mur_droite.png");
		*sp_bord_haut = ChargerSprite("images/murs/8x8/mur_haut.png");
		*sp_bord_bas = ChargerSprite("images/murs/8x8/mur_bas.png");
	}
	if (taille_tableau == 9){
		*sp_mur_normal = ChargerSprite("images/murs/9x9/mur9x9.png");
		*sp_bord_gauche = ChargerSprite("images/murs/9x9/mur_gauche.png");
		*sp_bord_droit = ChargerSprite("images/murs/9x9/mur_droite.png");
		*sp_bord_haut = ChargerSprite("images/murs/9x9/mur_haut.png");
		*sp_bord_bas = ChargerSprite("images/murs/9x9/mur_bas.png");
	}
}


/* verification_case est une fonction qui verifie si une case précise du tableau est utilisée ou non
* Pré-conditon : un tableau d'entiers à double dimension de taille 9x9, les positions de la case à verifier, en entiers, et en X et Y
* Post-condition : Renvoie 1 si la case est utilisée, 0 si elle est libre
*/

int verification_case(int tab[][9], int position_x, int position_y){
	if (tab[position_x][position_y] != 0)
		return 1;
	return 0;
}

void g_gagner_achiv(int numero_achiv, int achivement_unlocked[]){
	int i, succes1 = ChargerSprite("images/achivements/1.png");
	achivement_unlocked[numero_achiv-1] = 1; /* -1 car c'est le premier succès, mais le premier élément dans le fichier dans lequel on va écrire à l'aide du tableau */
	FILE* achiv_file;
	achiv_file = fopen("achivments.txt", "w");
	printf("Ecriture dans le fichier succes, %d\n", achivement_unlocked[0]);
	fprintf(achiv_file, "%d %d", achivement_unlocked[0], achivement_unlocked[1]);
	printf("Ecriture succes numero %d dans le fichier\n", numero_achiv);
	if (numero_achiv == 1){
		for(i=0; i<150; i++){
			AfficherSprite(succes1, LARGEUR_FENETRE-i*2, 100);
			usleep(6000);
		}

	}

	if (numero_achiv == 2){
		for(i=0; i<150; i++){
			AfficherSprite(succes1, LARGEUR_FENETRE-i*2, 100);
			usleep(6000);
		}
	}
	LibererSprite(succes1);
	fclose(achiv_file);
}

int verification_achivement_deja_unlocked(int achivement_unlocked, int tableau_achivement[]){
	FILE* achiv_file;
	achiv_file = fopen("achivments.txt", "r");
	if (achiv_file != NULL){
		fscanf(achiv_file, "%d %d", &tableau_achivement[0], &tableau_achivement[1]);
		printf("Verification succès %d\n", achivement_unlocked);
	}
	else
		printf("Problème lors de l'ouverture du fichier de succès\n");
	fclose(achiv_file);
	if (tableau_achivement[achivement_unlocked-1] == 1){ /* Premier succès, mais dans le fichier c'est le premier élément, et donc l'élément 0 du tableau*/
		printf("Succès %d deja debloque\n", achivement_unlocked);
		return 0;
	}

	else
		printf("Achivement %d n'a pas déjà été réalisé\n", achivement_unlocked);
	return 1;

}

/*Affichage graphique des elements après lecture dans le tableau : affiche un carre vide pour chaque 0, une croix pour chaque 1, un 4 pour le joueur 1, un 8 pour le joueur 2*/
/*Pour le moment, n'affiche que les carrés*/
void g_affichage_plateau(int const tab[][9], int taille_case, int taille_tableau, int phase_de_jeu, int sprite_mur, int sp_bord_gauche, int sp_bord_droit,int sp_bord_haut,int sp_bord_bas){
	ChoisirEcran(1); /* Pour eviter les problemes d'affichage */
	ChargerImage("images/fond.png",0,0,0,0,800,800);
	int pos_case_x, pos_case_y, pos_case_x2, pos_case_y2, i, z, joueur1 = ChargerSprite("images/joueur1_perso.png"), joueur2 = ChargerSprite("images/joueur2_perso.png");
	for (i = 0; i<taille_tableau; i++){
		if (i != 0){
			ChoisirCouleurDessin(CouleurParComposante(255,255,255));
			DessinerSegment(i*taille_case, 0, i*taille_case, LARGEUR_FENETRE);
		}

		for(z = 0; z<taille_tableau; z++){
			if (z != 0){
			ChoisirCouleurDessin(CouleurParComposante(255,255,255));
			DessinerSegment(0, z*taille_case, LONGUEUR_FENETRE, z*taille_case);
			}
		}
	}
	for(i=0; i<taille_tableau; i++){

		for(z=0; z<taille_tableau; z++){

			if (tab[i][z] == 1){
				AfficherSprite(sprite_mur, i*taille_case+5, z*taille_case+5);
				if (i+1 < taille_tableau && tab[i+1][z] != 1)
					AfficherSprite(sp_bord_droit, i*taille_case+taille_case-(taille_case/20), z*taille_case);
				if (i-1 < taille_tableau && tab[i-1][z] != 1)
					AfficherSprite(sp_bord_gauche, i*taille_case-(taille_case/20), z*taille_case);
				if (z+1 < taille_tableau && tab[i][z+1] != 1)
					AfficherSprite(sp_bord_bas, i*taille_case, z*taille_case+taille_case-(taille_case/20));
				if (z-1 < taille_tableau && tab[i][z-1] != 1)
					AfficherSprite(sp_bord_haut, i*taille_case, z*taille_case-(taille_case/20));
			}
			else if (tab[i][z] == 4){
				AfficherSprite(joueur1, i*taille_case+(taille_case-80)/2, z*taille_case+(taille_case-83)/2); /* 80, 83, sont les dimensions des sprites des personnages*/
				LibererSprite(joueur1);
			}
			else if (tab[i][z] == 8){
				AfficherSprite(joueur2, i*taille_case+(taille_case-80)/2, z*taille_case+(taille_case-83)/2);
				LibererSprite(joueur2);
			}

		}

	}

	ChargerImage("images/fond_bordures.png",0,0,0,0,800,800);
	ChoisirCouleurDessin(CouleurParComposante(0,0,0));
	RemplirRectangle(LARGEUR_FENETRE/2-260, LONGUEUR_FENETRE-30, 520, 30);
	ChoisirCouleurDessin(CouleurParComposante(255,255,255));
	/* On affiche en bas de l'écran l'action en cours*/

	if(phase_de_jeu == PHASE_DEP_JOUEUR1)
		ChargerImage("images/textes/J1DEP.png", LARGEUR_FENETRE/2-225, LONGUEUR_FENETRE-30, 0, 0, 500, 200);
	else if (phase_de_jeu == PHASE_DEP_JOUEUR2)
		ChargerImage("images/textes/J2DEP.png", LARGEUR_FENETRE/2-225, LONGUEUR_FENETRE-30, 0, 0, 500, 200);
	else if (phase_de_jeu == PHASE_COCHER_CASE1)
		ChargerImage("images/textes/J1COCHER.png", LARGEUR_FENETRE/2-250, LONGUEUR_FENETRE-25, 0, 0, 500, 200);
	else if (phase_de_jeu == PHASE_COCHER_CASE2)
		ChargerImage("images/textes/J2COCHER.png", LARGEUR_FENETRE/2-250, LONGUEUR_FENETRE-25, 0, 0, 500, 200);
	else if (phase_de_jeu == PLACEMENT_JOUEUR_UN)
		ChargerImage("images/textes/J1DEP.png", LARGEUR_FENETRE/2-225, LONGUEUR_FENETRE-30, 0, 0, 500, 200);
	else if(phase_de_jeu == PLACEMENT_JOUEUR_DEUX)
		ChargerImage("images/textes/J1DEP.png", LARGEUR_FENETRE/2-225, LONGUEUR_FENETRE-30, 0, 0, 500, 200);
	CopierZone(1, 0, 0, 0, 800, 800, 0, 0);
	ChoisirEcran(0);
}


void debloquer_succes(int achivement_unlocked, int tableau_achivement[]){
	if (achivement_unlocked != 0){
		if(verification_achivement_deja_unlocked(achivement_unlocked, tableau_achivement) == 1)
			g_gagner_achiv(achivement_unlocked, tableau_achivement);
		}
	}

/* A finir quand on a les ressources graphiques */
void g_affichage_achivment(int sprite_fond, int achiv_unlocked[]){

	FILE* achiv_file;
	ChargerImageFond("images/fond_achiv3.png");
	achiv_file = fopen("achivments.txt", "r");
	if (achiv_file != NULL){
		fscanf(achiv_file, "%d %d", &achiv_unlocked[0], &achiv_unlocked[1]);
		printf("%d %d\n", achiv_unlocked[0], achiv_unlocked[1]);
	}
	else
		printf("Problème lors de l'ouverture du fichier de succès");
	if (achiv_unlocked[0] == 1){
		ChargerImage("images/achivements/1.png", LARGEUR_FENETRE/4, LONGUEUR_FENETRE/4, 0, 0, 300, 100);
		printf("Affichage succès");
	}
	if (achiv_unlocked[1] == 1){
		ChargerImage("images/achivements/1.png", LARGEUR_FENETRE/4, LONGUEUR_FENETRE/2, 0, 0, 300, 100);
		printf("Affichage succès");
	}

	fclose(achiv_file);
}


/*Calcul des coordonnées de la case cliquée, et stockage dans deux variables clic_x et clic_y*/

void case_cliquee(int taille_case, int mouse_pos_x, int mouse_pos_y, int* clic_x, int* clic_y){
	if (mouse_pos_x>LARGEUR_FENETRE||mouse_pos_y>LONGUEUR_FENETRE)
	return;
	*clic_x = mouse_pos_x/taille_case;
	*clic_y = mouse_pos_y/taille_case;
}


/* deplacement_joueur est une fonction qui permet de deplacer un joueur vers une case voulue, rentrée en coordonnées (x,y) sur le plateau
* Pré-conditions : donner le tableau 9x9, la position actuelle du joueur et la position vers laquelle on veut le déplacer
* Post-condition : le joueur est déplacé vers une case voulue, et la case sur laquelle le joueur était est remplacé par un "0"
*/
void deplacement_joueur(int tab[][9], int* pos_x, int* pos_y, int pos_xx, int pos_yy){
		tab[pos_xx][pos_yy] = tab[*pos_x][*pos_y];
		tab[*pos_x][*pos_y] = 0;
		*pos_x = pos_xx;
		*pos_y = pos_yy;
}

int deplacement_verification(int tab[][9], int* pos_x, int* pos_y, int pos_xx, int pos_yy){
	if ((*pos_x - pos_xx) <= 1 && (*pos_x - pos_xx) >= -1 && (*pos_y - pos_yy) <= 1 && (*pos_y - pos_yy) >= -1)
		return 0;
	return 1;
}


void IA_aleatoire(int* joueur2_x, int* joueur2_y, int modulo){
	*joueur2_x = (rand()%modulo);
	*joueur2_y = (rand()%modulo);
}

void IA_deplacement(int* joueur2_x, int* joueur2_y, int tab[][9], int taille_tableau){
	int pos_x = (*joueur2_x), pos_y = (*joueur2_y), compteur = 0;

		while(compteur != 2){
			compteur = 0;
			*joueur2_x = pos_x;
			*joueur2_y = pos_y;
			*joueur2_x = *joueur2_x+(rand()%3)-1;
			*joueur2_y = *joueur2_y+(rand()%3)-1;
			if (*joueur2_y < taille_tableau && *joueur2_y >= 0 && tab[*joueur2_x][*joueur2_y] == 0)
				compteur++;
			if (*joueur2_x < taille_tableau && *joueur2_x >= 0 && tab[*joueur2_x][*joueur2_y] == 0)
				compteur++;
		}
		printf("tabxy %d %d %d\n",tab[*joueur2_x][*joueur2_y], *joueur2_x, *joueur2_y);

	tab[pos_x][pos_y] = 0;
	tab[*joueur2_x][*joueur2_y] = 8;
}
void IA_cocher_case(int joueur1_x, int joueur1_y, int game_over_joueur1, int tab[][9], int taille_tableau){
	int pos_x = joueur1_x, pos_y = joueur1_y, compteur = 0;
	if(game_over_joueur1 != 0){
		while(compteur != 2){
			compteur = 0;
			joueur1_x = pos_x;
			joueur1_y = pos_y;
			joueur1_x = joueur1_x+(rand()%3)-1;
			joueur1_y = joueur1_y+(rand()%3)-1;
			if (joueur1_y < taille_tableau && joueur1_y >= 0 && tab[joueur1_x][joueur1_y] == 0)
				compteur++;
			if (joueur1_x < taille_tableau && joueur1_x >= 0 && tab[joueur1_x][joueur1_y] == 0)
				compteur++;
		}
		printf("1x 1y %d %d \n", joueur1_x, joueur1_y);
		tab[joueur1_x][joueur1_y] = 1;
		return;
	}
	/*while(tab[joueur1_x][joueur1_y] != 0){
		IA_aleatoire(&joueur1_x, &joueur1_y, taille_tableau);
	}
	tab[joueur1_x][joueur1_y] = 1;
	return;*/
}

void partie_deux_joueurs(int taille_tableau){
	srand(time(NULL));
	int cross, test, tab[MAX][MAX], i, z, taille_case, clic_x, clic_y, test_deplacement, game_over_joueur1 = 1, game_over_joueur2 = 1;
	int tour_de_jeu = 1, phase_de_jeu = PLACEMENT_JOUEUR_UN, joueur1_x = JOUEUR_PAS_ENCORE_PLACE, joueur1_y = JOUEUR_PAS_ENCORE_PLACE, joueur2_x = JOUEUR_PAS_ENCORE_PLACE, joueur2_y = JOUEUR_PAS_ENCORE_PLACE;
	int pause = 0, sprite_fond_achiv, sprite_achiv_1, achiv_unlocked[3] = {0};
	int sp_mur_normal, sp_bord_gauche, sp_bord_droit, sp_bord_haut, sp_bord_bas; /*Sprites */

	initialisation_jeu(tab, &taille_case, taille_tableau, &sp_mur_normal, &sp_bord_gauche, &sp_bord_droit, &sp_bord_haut, &sp_bord_bas); /*On initialise le tableau en fonction de la taille_tableau choisit par l'utilisateur lors du menu */
	sprite_fond_achiv = ChargerSprite("fond_achiv3.png");
	sprite_achiv_1 = ChargerSprite("images/achiv_1.png");
	print_board(tab, taille_tableau); /*Affichage en console pour debeugage*/
	g_affichage_plateau(tab, taille_case, taille_tableau, phase_de_jeu, sp_mur_normal, sp_bord_gauche, sp_bord_droit, sp_bord_haut, sp_bord_bas); /*On affiche le tableau en fonction de ce qu'il y a dans le tableau */
	printf("%d", taille_case);
	while(1){
		/* Affiche un carré d'une case pour chaque "0" dans le tableau, et devra afficher une croix pour chaque "1" dans le tableau */
		if (SourisCliquee()){
			SourisPosition();
			case_cliquee(taille_case, _X, _Y, &clic_x, &clic_y); /*On stocke la valeurs des cases cliquees en X et en Y dans les variables clic_x et clic_y*/
			test = verification_case(tab, clic_x, clic_y); /*On verifie si la case n'est pas deja à "1" */

			if (phase_de_jeu != PLACEMENT_JOUEUR_DEUX){
				game_over_joueur1 = dep_nb_case_possibles(tab, taille_tableau, &joueur1_x, &joueur1_y);
				printf("nb_cases_dispo_apres_fn1 %d\n", game_over_joueur1);
				game_over_joueur2 = dep_nb_case_possibles(tab, taille_tableau, &joueur2_x, &joueur2_y);
				printf("nb_cases_dispo_apres_fn2 %d\n", game_over_joueur2);
			}

			switch(phase_de_jeu){

				case PLACEMENT_JOUEUR_UN: /* Si elle était vide, on la remplis par une croix, un "1"*/
					printf("Placement du joueur 1\n");
					tab[clic_x][clic_y] = 4;
					joueur1_x = clic_x;
					joueur1_y = clic_y;
					phase_de_jeu = PLACEMENT_JOUEUR_DEUX;
					break;

				case PLACEMENT_JOUEUR_DEUX:
						printf("Placement du joueur 2\n");
						IA_aleatoire(&joueur2_x, &joueur2_y, taille_tableau);
						while(verification_case(tab, joueur2_x, joueur2_y) != 0){
							IA_aleatoire(&joueur2_x, &joueur2_y, taille_tableau);
					  }
						tab[joueur2_x][joueur2_y] = 8;
						phase_de_jeu = PHASE_DEP_JOUEUR1;
					break;

				/*Partie où les joueurs déplacent leur pions */
				case PHASE_DEP_JOUEUR1:
					if (test == 0){ /* Si la case cliquee libre et phase dep joueur, on deplace le joueur en fonction du tour*/
						test_deplacement = deplacement_verification(tab, &joueur1_x, &joueur1_y, clic_x, clic_y);
						if (test_deplacement == DEPLACEMENT_POSSIBLE){
							deplacement_joueur(tab, &joueur1_x, &joueur1_y, clic_x, clic_y);
							phase_de_jeu = PHASE_COCHER_CASE1;
							test_deplacement = FIN_DEPLACEMENT;
						}
					}
					break;

				case PHASE_DEP_JOUEUR2:
					IA_deplacement(&joueur2_x, &joueur2_y, tab, taille_tableau);
					phase_de_jeu = PHASE_COCHER_CASE2;
					break;

				/*Partie où les joueurs cochent des cases */
				case PHASE_COCHER_CASE1:
					if (test == 0){
						tab[clic_x][clic_y] = 1;
						phase_de_jeu = PHASE_DEP_JOUEUR2;
					}
					break;
				case PHASE_COCHER_CASE2:
					IA_cocher_case(joueur1_x, joueur1_y, game_over_joueur1, tab, taille_tableau);
					phase_de_jeu = PHASE_DEP_JOUEUR1;
					break;

					break;
				}


			g_affichage_plateau(tab, taille_case, taille_tableau, phase_de_jeu,
					 sp_mur_normal, sp_bord_gauche, sp_bord_droit, sp_bord_haut, sp_bord_bas); /*On affiche le tableau en fonction de ce qu'il y a dans le tableau */
			printf("Tour du joueur %d, Phase de jeu = %d \n", tour_de_jeu, phase_de_jeu);
			print_board(tab, taille_tableau);/*Affichage console pour débeugage*/

			if (phase_de_jeu != PLACEMENT_JOUEUR_DEUX){
				game_over_joueur1 = dep_nb_case_possibles(tab, taille_tableau, &joueur1_x, &joueur1_y);
				printf("nb_cases_dispo_apres_fn1 %d\n", game_over_joueur1);
				game_over_joueur2 = dep_nb_case_possibles(tab, taille_tableau, &joueur2_x, &joueur2_y);
				printf("nb_cases_dispo_apres_fn2 %d\n", game_over_joueur2);
				}


			if (game_over_joueur1 == 0 && phase_de_jeu == PHASE_DEP_JOUEUR2 && game_over_joueur2 != 0){ /*On débloque le succès suicide*/
					debloquer_succes(1, achiv_unlocked);
					printf("Succes suicide");
			}
			if(game_over_joueur1 == 0 && phase_de_jeu == PHASE_DEP_JOUEUR1){
				printf("%d\n", phase_de_jeu);
				printf("GAME OVER JOUEUR 1\n");
				return;
			}

			else if(game_over_joueur2 == 0 && phase_de_jeu == PHASE_DEP_JOUEUR2){
				debloquer_succes(2, achiv_unlocked);
				printf("GAME OVER JOUEUR 2\n");
				return;
			}

		}

		if (ToucheEnAttente()){
			if(Touche() == XK_Escape){ /*Si on appuis sur Echap pendant le prog, ca quitte*/
				FermerGraphique();
				return;
      }
		else{
			printf("Achivement_ouvert\n");

				g_affichage_achivment(sprite_fond_achiv, achiv_unlocked);
				while(1){
					if(Touche()){
						ChargerImage("images/fond.png",0,0,0,0,800,800);
						g_affichage_plateau(tab, taille_case, taille_tableau, phase_de_jeu, sp_mur_normal, sp_bord_gauche, sp_bord_droit, sp_bord_haut, sp_bord_bas); /*On affiche le tableau en fonction de ce qu'il y a dans le tableau */
							break;
						}
				}
				printf("Achivement_fermé\n");

			}
		}

		/*CREER FONCTION POUR QUITTER QUI DECHARGE LES SPRITES*/
	}
	return;
}


void partie_un_joueurs(int taille_tableau){
	int cross, test, tab[MAX][MAX], i, z, taille_case, clic_x, clic_y, test_deplacement, game_over_joueur1 = 1, game_over_joueur2 = 1;
	int tour_de_jeu = 1, phase_de_jeu = PLACEMENT_JOUEUR_UN, joueur1_x = JOUEUR_PAS_ENCORE_PLACE, joueur1_y = JOUEUR_PAS_ENCORE_PLACE, joueur2_x = JOUEUR_PAS_ENCORE_PLACE, joueur2_y = JOUEUR_PAS_ENCORE_PLACE;
	int pause = 0, sprite_fond_achiv, sprite_achiv_1, achiv_unlocked[3] = {0};


	int sp_mur_normal, sp_bord_gauche, sp_bord_droit, sp_bord_haut, sp_bord_bas; /*Sprites */

	initialisation_jeu(tab, &taille_case, taille_tableau, &sp_mur_normal, &sp_bord_gauche, &sp_bord_droit, &sp_bord_haut, &sp_bord_bas); /*On initialise le tableau en fonction de la taille_tableau choisit par l'utilisateur lors du menu */
	sprite_fond_achiv = ChargerSprite("fond_achiv3.png");
	sprite_achiv_1 = ChargerSprite("images/achiv_1.png");
	print_board(tab, taille_tableau); /*Affichage en console pour debeugage*/
	g_affichage_plateau(tab, taille_case, taille_tableau, phase_de_jeu, sp_mur_normal, sp_bord_gauche, sp_bord_droit, sp_bord_haut, sp_bord_bas); /*On affiche le tableau en fonction de ce qu'il y a dans le tableau */
	printf("%d", taille_case);
	while(1){
		/* Affiche un carré d'une case pour chaque "0" dans le tableau, et devra afficher une croix pour chaque "1" dans le tableau */
		if (SourisCliquee()){
			SourisPosition();
			case_cliquee(taille_case, _X, _Y, &clic_x, &clic_y); /*On stocke la valeurs des cases cliquees en X et en Y dans les variables clic_x et clic_y*/
			test = verification_case(tab, clic_x, clic_y); /*On verifie si la case n'est pas deja à "1" */


			switch(phase_de_jeu){

				case PLACEMENT_JOUEUR_UN: /* Si elle était vide, on la remplis par une croix, un "1"*/
					printf("Placement du joueur 1\n");
					tab[clic_x][clic_y] = 4;
					joueur1_x = clic_x;
					joueur1_y = clic_y;
					phase_de_jeu = PLACEMENT_JOUEUR_DEUX;
					break;

				case PLACEMENT_JOUEUR_DEUX:
					if (test == 0){ /* */
						printf("Placement du joueur 2\n");
						tab[clic_x][clic_y] = 8;
						joueur2_x = clic_x;
						joueur2_y = clic_y;
						phase_de_jeu = PHASE_DEP_JOUEUR1;
					}
					break;

				/*Partie où les joueurs déplacent leur pions */
				case PHASE_DEP_JOUEUR1:
					if (test == 0){ /* Si la case cliquee libre et phase dep joueur, on deplace le joueur en fonction du tour*/
						test_deplacement = deplacement_verification(tab, &joueur1_x, &joueur1_y, clic_x, clic_y);
						if (test_deplacement == DEPLACEMENT_POSSIBLE){
							deplacement_joueur(tab, &joueur1_x, &joueur1_y, clic_x, clic_y);
							phase_de_jeu = PHASE_COCHER_CASE1;
							test_deplacement = FIN_DEPLACEMENT;
						}
					}
					break;

				case PHASE_DEP_JOUEUR2:
					if (test == 0){
						test_deplacement = deplacement_verification(tab, &joueur2_x, &joueur2_y, clic_x, clic_y);
						if (test_deplacement == DEPLACEMENT_POSSIBLE){
							deplacement_joueur(tab, &joueur2_x, &joueur2_y, clic_x, clic_y);
							phase_de_jeu = PHASE_COCHER_CASE2;
							test_deplacement = FIN_DEPLACEMENT;
						}
					}
					break;

				/*Partie où les joueurs cochent des cases */
				case PHASE_COCHER_CASE1:
					if (test == 0){
						tab[clic_x][clic_y] = 1;
						phase_de_jeu = PHASE_DEP_JOUEUR2;
					}
					break;
				case PHASE_COCHER_CASE2:
					if (test == 0){
						tab[clic_x][clic_y] = 1;
						phase_de_jeu = PHASE_DEP_JOUEUR1;
					}
					break;
					break;
				}


			g_affichage_plateau(tab, taille_case, taille_tableau, phase_de_jeu,
					 sp_mur_normal, sp_bord_gauche, sp_bord_droit, sp_bord_haut, sp_bord_bas); /*On affiche le tableau en fonction de ce qu'il y a dans le tableau */
			printf("Tour du joueur %d, Phase de jeu = %d \n", tour_de_jeu, phase_de_jeu);
			print_board(tab, taille_tableau);/*Affichage console pour débeugage*/

			if (phase_de_jeu != PLACEMENT_JOUEUR_DEUX){
				game_over_joueur1 = dep_nb_case_possibles(tab, taille_tableau, &joueur1_x, &joueur1_y);
				printf("nb_cases_dispo_apres_fn1 %d\n", game_over_joueur1);
				game_over_joueur2 = dep_nb_case_possibles(tab, taille_tableau, &joueur2_x, &joueur2_y);
				printf("nb_cases_dispo_apres_fn2 %d\n", game_over_joueur2);
				}


			if (game_over_joueur1 == 0 && phase_de_jeu == PHASE_DEP_JOUEUR2 && game_over_joueur2 != 0){ /*On débloque le succès suicide*/
					debloquer_succes(1, achiv_unlocked);
					printf("Succes suicide");
			}
			if(game_over_joueur1 == 0 && phase_de_jeu == PHASE_DEP_JOUEUR1){
				printf("%d\n", phase_de_jeu);
				printf("GAME OVER JOUEUR 1\n");
				return;
			}

			else if(game_over_joueur2 == 0 && phase_de_jeu == PHASE_DEP_JOUEUR2){
				debloquer_succes(2, achiv_unlocked);
				printf("GAME OVER JOUEUR 2\n");
				return;
			}

		}

		if (ToucheEnAttente()){
			if(Touche() == XK_Escape){ /*Si on appuis sur Echap pendant le prog, ca quitte*/
				FermerGraphique();
				return;
      }
		else{
			printf("Achivement_ouvert\n");

				g_affichage_achivment(sprite_fond_achiv, achiv_unlocked);
				while(1){
					if(Touche()){
						ChargerImage("images/fond.png",0,0,0,0,800,800);
						g_affichage_plateau(tab, taille_case, taille_tableau, phase_de_jeu, sp_mur_normal, sp_bord_gauche, sp_bord_droit, sp_bord_haut, sp_bord_bas); /*On affiche le tableau en fonction de ce qu'il y a dans le tableau */
							break;
						}
				}
				printf("Achivement_fermé\n");

			}
		}

		/*CREER FONCTION POUR QUITTER QUI DECHARGE LES SPRITES*/
	}
	return;
}



int main(void){
	int taille_tableau = MAX, selection = 0;

	menu(&taille_tableau, &selection);
	if (selection == 2)
		partie_un_joueurs(taille_tableau);
	if (selection == 1)
		partie_deux_joueurs(taille_tableau);


}
