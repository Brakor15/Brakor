# Jeu "Blokus"

#But final

but : Blokus

#Variables

OFILES = main.o \
	 menu.o

CC = gcc

CFLAGS = -ansi

#Dépendances

main.o : menu.h

menu.o : menu.h

#Dépendances avec commandes

Blokus : $(OFILES)
	  $(CC) $(CFLAGS) -o Blokus $(OFILES) -lgraph -lm

#Nettoyage des fichiers générés

clean :
	-rm -f $(OFILES)

#But factice

.PHONY : but clean
