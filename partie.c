#include <stdlib.h>
#include <stdio.h>
#include <graph.h>
#include <math.h>
#include "affichage.h"
#include "achievements.h"
#include "ia.h"
#include "partie.h"

#define MAX 9
#define LARGEUR_FENETRE 800
#define LONGUEUR_FENETRE 800

#define PLACEMENT 0
#define PHASE_DEP_JOUEUR 1
#define PHASE_COCHER_CASE 2


int partie(int taille_tableau, int nombre_joueurs, int nombre_de_joueurs_ia, int tab_succes_debloques[]){
	int compteur, tab[MAX][MAX], taille_case, clic_x, clic_y, joueur_actuel = 0, i;
	int phase_de_jeu = PLACEMENT;
	int sp_mur_normal, sp_bord_gauche, sp_bord_droit, sp_bord_haut, sp_bord_bas;
	int nombre_joueurs_humain = nombre_joueurs - nombre_de_joueurs_ia;
	Joueur joueur[nombre_joueurs];
	for (i = 0; i<nombre_joueurs_humain; i++){
		joueur[i].ia_ou_joueur = 0;
		joueur[i].numero = 8;
	}
	for (i; i < nombre_joueurs; i++){
		joueur[i].ia_ou_joueur = 1;
		joueur[i].numero = 8; /*Le ou les IA auront le numéro 8*/
	}
	joueur[0].numero = 4; /*Le joueur humain aura le numéro 4*/

	/*On initialise le tableau en fonction de la taille_tableau choisit par l'utilisateur lors du menu */
	initialisation_jeu(tab, &taille_case, taille_tableau, &sp_mur_normal, &sp_bord_gauche, &sp_bord_droit, &sp_bord_haut, &sp_bord_bas);
	print_board(tab, taille_tableau); /*Affichage en console pour debeugage*/
	g_affichage_plateau(tab, taille_case, taille_tableau, phase_de_jeu, sp_mur_normal, sp_bord_gauche, sp_bord_droit, sp_bord_haut, sp_bord_bas); /*On affiche le tableau en fonction de ce qu'il y a dans le tableau */
	printf("nb de joueurs = %d\n", nombre_joueurs);

	while(1){
		if (joueur[joueur_actuel].ia_ou_joueur == 3){
			joueur_actuel++;
			if (joueur_actuel == nombre_joueurs)
				joueur_actuel = 0;
			phase_de_jeu = PHASE_DEP_JOUEUR;
		}
		if (SourisCliquee() || joueur[joueur_actuel].ia_ou_joueur == 1){ /*Si on clique, ou si le joueur actuel est une IA*/
			printf("Joueur %d joue\n", joueur_actuel);
			if (joueur[joueur_actuel].ia_ou_joueur == 0){ /*Si le joueur est humain on récupère la position de son clic*/
				SourisPosition();
				clic_x = _X;
				clic_y = _Y;
			}
			if(phase_de_jeu == PLACEMENT || joueur[joueur_actuel].nombre_de_cases_libres != 0){ /*Si le joueur actuel n'a plus de cases libres, on passe les phases de jeu
																																													pour arriver à la gestion du game_over*/
				switch(phase_de_jeu){
					case PLACEMENT:
					if (joueur[joueur_actuel].ia_ou_joueur == 0 && case_cliquee(taille_case, _X, _Y, &clic_x, &clic_y) == 0){
						joueur[joueur_actuel].position_x = clic_x;
						joueur[joueur_actuel].position_y = clic_y;
						tab[clic_x][clic_y] = joueur[joueur_actuel].numero;
						joueur_actuel++; /*On passe au joueur suivant*/
					}
					else if (joueur[joueur_actuel].ia_ou_joueur == 1){
						usleep(500000); /*On laisse le temps aux joueurs de voir les agissements de l'IA*/
						ia_aleatoire(&joueur[joueur_actuel].position_x, &joueur[joueur_actuel].position_y, taille_tableau);
						while(verification_case(tab, joueur[joueur_actuel].position_x, joueur[joueur_actuel].position_y) != 0){
							ia_aleatoire(&joueur[joueur_actuel].position_x, &joueur[joueur_actuel].position_y, taille_tableau);
						}
						tab[joueur[joueur_actuel].position_x][joueur[joueur_actuel].position_y] = joueur[joueur_actuel].numero;
						joueur_actuel++;
					}
					if (joueur_actuel == nombre_joueurs){ /*Lorsque l'on a placé tous les joueurs, on passe à la phase de deplacement*/
						phase_de_jeu = PHASE_DEP_JOUEUR;
						printf("LE JEU COMMENCE ! \n");
					}
					break;


					/*Partie où les joueurs déplacent leur pions */
					case PHASE_DEP_JOUEUR:
					if (joueur[joueur_actuel].ia_ou_joueur == 0 && case_cliquee(taille_case, _X, _Y, &clic_x, &clic_y) == 0 && tab[clic_x][clic_y] == 0){
						/*Si l'on clique bien dans une case vide, que le joueur est humain, et que la case cliquee est a 1 case de distance du joueur*/
						if (deplacement_verification(tab, joueur[joueur_actuel].position_x, joueur[joueur_actuel].position_y, &clic_x, &clic_y) == 0){
							deplacement_joueur(tab, &joueur[joueur_actuel].position_x, &joueur[joueur_actuel].position_y, clic_x, clic_y);
							phase_de_jeu = PHASE_COCHER_CASE;
						}
					}
					else if (joueur[joueur_actuel].ia_ou_joueur == 1){
						ia_deplacement(&joueur[joueur_actuel].position_x, &joueur[joueur_actuel].position_y, tab, taille_tableau);
						phase_de_jeu = PHASE_COCHER_CASE;
						usleep(500000);
					}
					break;

					/*Partie où les joueurs cochent des cases */
					case PHASE_COCHER_CASE:
					/*On coche la case si la case est vide en remplissant le tableau par un 1 à l'emplacement cliqué sur la grille*/
					if (joueur[joueur_actuel].ia_ou_joueur == 0 && case_cliquee(taille_case, _X, _Y, &clic_x, &clic_y) == 0 && tab[clic_x][clic_y] == 0){
						tab[clic_x][clic_y] = 1;
						phase_de_jeu = PHASE_DEP_JOUEUR;
						joueur_actuel++;

					}
					else if (joueur[joueur_actuel].ia_ou_joueur == 1){
						joueur[0].nombre_de_cases_libres = dep_nb_case_possibles(tab, taille_tableau, joueur[0].position_x, joueur[0].position_y);
						ia_cocher_case(joueur[0].position_x, joueur[0].position_y, joueur[0].nombre_de_cases_libres, tab, taille_tableau);
						usleep(500000);
						phase_de_jeu = PHASE_DEP_JOUEUR;
						joueur_actuel++;
					}
					break;

					default:
					break;
				}

				if(joueur_actuel == nombre_joueurs){ /*Si tous les joueurs ont joué leur tour, on repasse au premier joueur*/
					joueur_actuel = 0;
				}
			}
			g_affichage_plateau(tab, taille_case, taille_tableau, phase_de_jeu,
				sp_mur_normal, sp_bord_gauche, sp_bord_droit, sp_bord_haut, sp_bord_bas);
				/*On affiche à l'écran en fonction de ce qu'il y a dans le tableau */



				/*Gestion des game_over*/
				if(phase_de_jeu != PLACEMENT){
					joueur[joueur_actuel].nombre_de_cases_libres = dep_nb_case_possibles(tab, taille_tableau, joueur[joueur_actuel].position_x, joueur[joueur_actuel].position_y);
					if (joueur[joueur_actuel].nombre_de_cases_libres == 0){
						if(nombre_de_joueurs_ia > 1 && joueur[joueur_actuel].ia_ou_joueur == 1 || joueur[joueur_actuel].ia_ou_joueur == 3){
							joueur[joueur_actuel].ia_ou_joueur = 3;
						}
						else{
							printf("GAME OVER JOUEUR %d, nombre_de_cases_libres = %d\n", joueur_actuel, joueur[joueur_actuel].nombre_de_cases_libres);
							decharger_tous_sprites(sp_mur_normal, sp_bord_bas, sp_bord_haut, sp_bord_gauche, sp_bord_droit);
							if(joueur[joueur_actuel].ia_ou_joueur == 0)
								return 0; /*0 pour la win, 1 pour la loose*/
							else
								return 1;
						}
						compteur = 0;
						for ( i = nombre_joueurs - nombre_de_joueurs_ia; i < nombre_joueurs; i++){
							joueur[i].nombre_de_cases_libres = dep_nb_case_possibles(tab, taille_tableau, joueur[i].position_x, joueur[i].position_y);
							if (joueur[i].nombre_de_cases_libres == 0)
								compteur++;
						}
						if ( compteur == nombre_de_joueurs_ia ){
							printf("GAME OVER DES IA\n");
							decharger_tous_sprites(sp_mur_normal, sp_bord_bas, sp_bord_haut, sp_bord_gauche, sp_bord_droit);
							return 1;
						}
					}
					joueur[0].nombre_de_cases_libres = dep_nb_case_possibles(tab, taille_tableau, joueur[0].position_x, joueur[0].position_y);
					if(joueur[0].nombre_de_cases_libres == 0 && phase_de_jeu == PHASE_DEP_JOUEUR && joueur_actuel != 0){
						debloquer_succes(1, tab_succes_debloques);
					}
				}

				print_board(tab, taille_tableau);/*Affichage console pour débeugage*/

			}/*If souriscliquee...*/


			if (ToucheEnAttente()){
				if(Touche() == XK_Escape){ /*Si on appuis sur Echap pendant le prog, ca quitte*/
					decharger_tous_sprites(sp_mur_normal, sp_bord_bas, sp_bord_haut, sp_bord_gauche, sp_bord_droit);
					FermerGraphique();
					return;
				}
			}

		}/*While (1), grande boucle générale du jeu*/

		/*Au cas ou il y ai eu un problème et que l'on soit sortit de la boucle, on décharge tout et on quitte la partie*/
		decharger_tous_sprites(sp_mur_normal, sp_bord_bas, sp_bord_haut, sp_bord_gauche, sp_bord_droit);
		return;
	}
