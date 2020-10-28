# ================== Rappel ======================
# $@ partie gauche de la dépendance (ou cible)
# $< est la premiere dependance
# $^ la liste des dépendances
# @$ pour desactiver l'echo des lignes de commandes en cas d'erreur

CFLAGS=-Wall -Wextra -lm
SDLFLAG=-lSDLmain -lSDL 
SDLIMAGEFLAGS=-lSDL_image
EXEC=clientTCP
CC=gcc

#génération de la liste des fichiers objets
# SRC contient liste des fichier sources du projet 
# on pourrait utiliser une construction automatique a l'aide du joker *.c et la commande wildcard mais il faudra gerer les dépendances vis a vis des .h
# SRC=$(wildcard *.c)
SRC=clientTCP.c draughtGame/draught_game.c profil/profil.c
OBJET=$(SRC:.c=.o)

all:${EXEC}

${EXEC}: ${OBJET}
	@${CC}  -o $@ $^ ${SDLFLAG} ${SDLIMAGEFLAGS} ${CFLAGS}

# au cas ou .h est modifié
clientTCP.o: draughtGame/draught_game.h constant/constant.h profil/profil.h
draught_game.o: draught_game.h constant/constant.h
profil.o: profil.h constant/constant.h

%.o:%.c
	@$(CC) -o $@ -c $< ${CFLAGS}

#La partie en commentaire représente ce que la ligne au dessus fait
#function.o: function.c
#	@${CC} -o function.o -c function.c ${CFLAGS}

#main.o:main.c headers.h
#	@${CC} -o main.o -c main.c ${CFLAGS}


# Sur unix seulement
.PHONY: clean mrpoper

clean:
	@rm -rf *.o

mrpoper: clean
	@rm -rf $(EXEC)

# Sur windows seulement
#.PHONY: clean mrpoper

#clean:
#	del ${OBJET}

#mrpoper: clean
#	del $(EXEC)
