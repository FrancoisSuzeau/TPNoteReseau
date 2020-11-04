SUZEAU Francois

# TPNoteReseau

## INFORMATIONS IMPORTANTES

Je vous demande de compiler le projet avant la première utilisation sinon vous aurez des erreurs de pointeurs. Un makefile est disponible pour le client ET le serveur. Utilisez la commande "make all"

Si le message " make: rien a faire pour "all" " apparait mettez un commentaire quelque part dans le fichier et recommancez.

## Déroulement du programme (& des fonctionnalités)

* Il faut tout d'abord lancer le serveur (./serveurTCP) : il se mets en attente d'une connection.

* Ensuite en lançant le client (./clientTCP) vous avez confirmation du serveur qu'une connection a réussi.

* Le client vous demande de vous identifier, entrez votre nom, si vous êtes déjà enregistrer (via un fichier) le client le signalera au serveur qui vous mettra en attente d'un autre joueur (tout en vous souhaitant un bon retour).

* Si vous n'êtes pas enregister alors le client (et attent la création du compte) le signal au serveur et ce derniers vous enregistre dans la base de donnée (le fichier). Il vous mets aussi en attente par la suite.

* Une fois un deuxième joueur connectée le serveur demande si l'on souhaite avoir un spectateur (pour l'instant il faut choisir 2 car je n'est pas encore pris en compte le spectateur par la suite).

* Cela fait, deux fenêtres graphiques se lance avec pour chacune le nom des joueurs ainsi que la couleurs des pions qu'ils peuvent jouer.

* Le joueur avec les pions blancs commence : Il y a un carré noir en à gauche de la fenêtre qui lui est attribué, c'est un sélecteur et il se déplace avec les flèches directionnels du clavier. Pour selectionner un pion, mettez vous dessus et appuyé sur ESPACE.

* Le client vous empêche de jouer les pions adverse, et aussi de jouer un de vos pions entouré de pions de la même couleurs.

* Une fois votre mouvement exécuté, vous avez une confirmation du client acceptant votre déplacement. Il l'envoi par la suite au serveur.

* Celui ci regarde votre coup et si il est correct (i.e. vous vous êtes déplacé de une case en diagonale en avant et pas en arrière selon votre couleur, ou vous avez mangé un pions adverse) le confirme au client. Le serveur indique aussi que votre tour est terminé et signale au joueur adverse le début de son tour.

* Si le serveur n'est pas d'accord avec votre déplacement il vous indique de recommancer. (pour l'instant il est possible de laisser le pion fautif en bouger un autre correctement et ce sera valider par le serveur).

* Pour l'instant la partie ne se termine pas une fois tout les jetons mangé pour un des deux joueurs.

* Vous pouvez quitter votre fenêtre graphique avec échappe ou la croix, cela arrêtera le serveur mais pas le deuxieme client.

* Si vous le souhaiter vous avez l'historique de vos déplacements personnel sur le terminal associé a votre fenêtre graphique et l'historique total sur le terminal du serveur.