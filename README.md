# projetC-Louise-Mahiout-Sarra-Ghedira-GroupeC

1-Présentation généralle:

a-Titre: Image Processing en C
Contributeur:Sarra Ghedira (Groupe C)/Louise Mahiout (Groupe C)
Description: Ce projet consiste à implimenter un programme de traitement d'images(noire et bonc/ couleurs) à travers une interface: En effet, le but est d'être capable de lire, modifier, filtrer, sauvegarder des images BMP (8 bits et 24 bits).

b-Fonctionalité:
-Chargement et affichage d’informations sur une image BMP.
-Application de filtres :
  8 bits : négatif, luminosité, seuillage, flou, gaussien, contours, relief, netteté.
  24 bits : négatif, conversion en niveaux de gris, luminosité, filtres de convolution.
-Sauvegarde des images après traitement.
-Égalisation d’histogramme (partie avancée).
-Interface utilisateur interactive par un menu.*

c-Technologie utilisé: 
-Langage : C
-IDE : CLion
-Outils de versionnement : Git + GitHub
-Bibliothèques : stdio.h, stdlib.h, stdint.h, string.h, math.h
-fichiers: bmp8.h/ bmp8.c/ main.c
-Format manipulé : BMP (Bitmap)
-Types structurés définis : t_bmp8, t_bmp24, t_bmp_header, t_bmp_info, t_pixel

d-Installation :
-cloner le projet:
git clone https://github.com/<utilisateur>/traitement-images-bmp.git
cd traitement-images-bmp
-Configuration du projet avec CLion :
Ouvrir CLion.
Aller dans File > New Project from Version Control et coller l’URL du dépôt.
Sélectionner un dossier de destination.
CLion télécharge et configure automatiquement le projet (CMake inclus).
Configuration Git :
Ajouter un .gitignore avec :
.idea/
cmake-build-debug/
cmake-build-release/
CMakeLists.txt

e-Utlisation:

3-Journal de bord:
a-Chronologie du projet: 
17/03/2025 jusqu'à le 12/04/2025: Partie 1 (Lecture, sauvegarde, affichage et traitement (négatif, seuil, flou, etc.) des images 8 bits.)
13/04/2025 jusqu'à le 17/05/2025: Partie 2 (Lecture, sauvegarde, affichage et traitement (négatif, luminausité...) des images 8 bits.)
21/05/2025 jusqu'à 24/05/2025: Partie 3 ()
b-Répartition des tâches:
Louise Mahiout: Partie1 / Partie 3: 3.3/ Readme
Sarra Ghedira: Partie 2/ Partie 3: 3.4/ Readme











