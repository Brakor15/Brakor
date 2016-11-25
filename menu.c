#include<stdlib.h>
#include<stdio.h>
#include<graph.h>
#include"menu.h"

void souris_normale(void){

  ChoisirCouleurDessin(CouleurParComposante(0,0,200));
  RemplirRectangle(150,130,300,50);
  RemplirRectangle(150,205,300,50);
  RemplirRectangle(150,280,300,50);
  RemplirRectangle(150,480,300,50);

  ChoisirCouleurDessin(CouleurParComposante(255,255,255));
  EcrireTexte(250,160,"1 joueur",1);
  EcrireTexte(250,235,"2 joueurs",1);
  EcrireTexte(250,310,"Quitter",1);
  EcrireTexte(200,510,"Reinitialiser Succes",1);
}

void souris_un(void){

  ChoisirCouleurDessin(CouleurParComposante(200,200,0));
  RemplirRectangle(150,130,300,50);
  ChoisirCouleurDessin(CouleurParComposante(0,0,0));
  EcrireTexte(250,160,"1 joueur",1);
}

void souris_deux(void){

  ChoisirCouleurDessin(CouleurParComposante(200,200,0));
  RemplirRectangle(150,205,300,50);
  ChoisirCouleurDessin(CouleurParComposante(0,0,0));
  EcrireTexte(250,235,"2 joueurs",1);

}



void souris_quitter(void){

  ChoisirCouleurDessin(CouleurParComposante(200,200,0));
  RemplirRectangle(150,280,300,50);

  ChoisirCouleurDessin(CouleurParComposante(0,0,0));
  EcrireTexte(250,310,"Quitter",1);
}

void souris_trois(void){

  ChoisirCouleurDessin(CouleurParComposante(200,200,0));
  RemplirRectangle(150,480,300,50);
  ChoisirCouleurDessin(CouleurParComposante(0,0,0));
  EcrireTexte(200,510,"Reinitialiser Succes",1);

}
void reset_achivements(){
    FILE* achiv_file;
    achiv_file = fopen("achivments.txt", "w+");
    printf("Remise à 0 du fichier des succès %d%c%d \n", 0, ' ', 0);
    fprintf(achiv_file, "0 0");
    fclose(achiv_file);
}

/*Affiche un menu de sélection du nombre de joueurs et gère les choix effectués*/

int menu(int* taille_tableau,int* selection){
  *taille_tableau = 3;
  *selection = 0;
  int souris_x,souris_y, selection_image;
  InitialiserGraphique();
  CreerFenetre(150,150,800,800);
  ChargerImageFond("images/fond.png");
  ChargerImage("Titre.png",200,10,10,35,190,120);
  selection_image = ChargerSprite("images/selection.png");
  AfficherSprite(selection_image, 200,600);
  souris_normale();


    while(1){
      while (!SourisCliquee()){
        SourisPosition();
        souris_x=_X;
        souris_y=_Y;
        if((150<=souris_x) && (souris_x<=450) && (130<=souris_y) && (souris_y<=180)){
            souris_un();
            *selection = 1;
            }

        else if((150<=souris_x) && (souris_x<=450) && (205<=souris_y) && (souris_y <= 255)){
          souris_deux();
          *selection = 2;
          }

        else if((150<=souris_x) && (souris_x<=450) && (280<=souris_y) && (souris_y<=330)){
          souris_quitter();
          *selection = 3;
          }
        else if((150<=souris_x) && (souris_x<=450) && (480<=souris_y) && (souris_y<=530)){
          souris_trois();
          *selection = 4;
          }
        else{
          souris_normale();
          *selection = 0;
        }

        if (ToucheEnAttente()){ /*Si on appuis sur Echap pendant le prog, ca quitte*/
          if (Touche() == XK_Escape){
            FermerGraphique();
            exit(0);
            }
          }
        }


        if((souris_x>=200) && (souris_x<=300) && (souris_y>=600)){
          if (*taille_tableau > 3){
              ChoisirCouleurDessin(CouleurParComposante(0,0,0));
            *taille_tableau += -1;
            printf("%d\n", *taille_tableau);
          }
        }
        if((souris_x>=500) && (souris_x<=600) && (souris_y>=600)){
          if (*taille_tableau < 9){
            *taille_tableau += +1;
            printf("%d\n", *taille_tableau);
          }
        }
      if (*selection == 1)
        return 0;
      if (*selection == 3)
        FermerGraphique();
      if (*selection == 4)
        reset_achivements();


      }
    }
