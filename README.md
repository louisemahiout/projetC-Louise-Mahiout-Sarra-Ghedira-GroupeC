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
Compiler le projet
1-Ouvrez votre terminal ou IDE (ex: CLion) et compilez le projet à l’aide de gcc, make, ou un environnement C configuré :
gcc main.c bmp8.c -o traitement_image
2-Lancer l’application une fois compilé, exécute l’application avec :
./traitement_image
3-Utilisation du menu, pour traiter une image: premièrement choix entre une image gris ou en coleur, ensuite l'application des filtres.

2-Documentation Technique:

a-Algorithme:

1. Initialisation
  Le programme commence par l’inclusion des bibliothèques standards (stdio.h, stdlib.h, string.h, stdint.h, math.h), le module bmp8.c et le module bmp8.h.
  La fonction main() qui permet de exécuter le programme.
2-Menu:
  1-Afficher un menu de choix initial pour déterminer le type d’image à charger :
    1 → Image en niveaux de gris (8 bits)
    2 → Image couleur (24 bits)
  
  2-Charger l’image correspondante :
    Si 8 bits : barbara_gray.bmp est chargée via bmp8_loadImage.
    Si 24 bits : flowers_color.bmp est chargée via bmp24_loadImage.
    En cas d’erreur, le programme s’arrête.
  
  3-Afficher un menu principal interactif avec les options suivantes :
    1-Afficher les informations de l’image
    2-Appliquer un filtre
    3-Égaliser l’histogramme
    4-Sauvegarder l’image
    5-Quitter
  
  4-Réagir selon le choix utilisateur :
    1-Afficher les métadonnées de l’image (largeur, hauteur, profondeur):
    Pour une image 8 bits : bmp8_printInfo() est appelée.
    Pour une image 24 bits : les dimensions et la profondeur de couleur sont affichées manuellement.
    2-Ouvrir un sous-menu de filtres :
      Pour une image 8 bits :
        Négatif (void bmp8_negative)
        Luminosité (void bmp8_brightness)
        Binarisation (seuillage) (void bmp8_threshold)
        Flou 
        Flou gaussien
        Contours
        Relief
        Netteté

  
      Pour une image couleur :
        Négatif (void bmp24_negative)
        Luminosité (void bmp8_brightness)
        Conversion en niveaux de gris(void bmp24_grayscale)
        Flou
        Flou gaussien
        Contours
        Relief
        Netteté

        Filtres de convontions:
        bmp8_applyFilter (grayscale) ou
        bmp24_convolution pour chaque pixel (couleur)
  
    3-Appliquer une égalisation d’histogramme :

        Pour image 8 bits :
          Calcul de l’histogramme (bmp8_computeHistogram)
          Calcul de la CDF (bmp8_computeCDF)
          Application du mapping égalisé (bmp8_equalize)
        
        Pour image couleur :
          Conversion RGB → YUV
          Égalisation sur Y
          Re-conversion YUV → RGB (bmp24_equalize)
        
    5-Demander un nom de fichier et sauvegarder l’image modifiée dans le dossier images_sortie.
        L’utilisateur entre un nom de fichier.
        L’image (modifiée ou non) est sauvegardée avec :
        bmp8_saveImage() pour une image 8 bits.
        bmp24_saveImage() pour une image 24 bits.
  
    6-Quitter la boucle et terminer le programme.
        Libérer la mémoire allouée dynamiquement à l’image (grise ou couleur) avec :
        bmp8_free pour les images 8 bits
        bmp24_free pour les images 24 bits
        Afficher un message de fin : "Programme terminé. Au revoir !"

b-Fonction implimenté:

Partie 1:

1.2 Lecture et écriture d’une image en niveaux de gris:
-t_bmp8 * bmp8_loadImage(const char * filename): A pour but de charger une image BMP 8 bits à partir d’un fichier/ filename : chemin du fichier BMP/ retourne un pointeur vers la structure image.
-void bmp8_saveImage(const char * filename, t_bmp8 * img): A pour but de sauvegarder l’image dans un fichier BMP 8 bits/ filename: le fichier à sauvegarder, img: image à sauvegarder.
-void bmp8_printInfo(t_bmp8 *img): A pour but d'afficher les informations (dimensions, taille, etc.) de l’image/img: image dont on va estraires les iinformations.
-void bmp8_free(t_bmp8 **img): Libére toute la memoire allouer dynamiquement/ img : double pointeur vers l’image à libérer (permet de mettre à NULL).

1.3 Traitement d’une image en niveaux de gris:
-void bmp8_negative(t_bmp8 * img): Applique un effet négatif (inversion des niveaux de gris)/ img : image à modifiee
-void bmp8_brightness(t_bmp8 * img, int value): Ajuste la luminosité de l’image en ajoutant une valeur à chaque pixel/img: image à modifier, value: valeur à ajouter à chaque pixel (peut être positive ou négative).
-void bmp8_threshold(t_bmp8 * img, int threshold):Binarise l’image selon un seuil donné (seuillage)/ img: image à modifier, threshold : seuil à appliquer (entre 0 et 255)

1.4 Les filtres d’image:
-void bmp8_applyFilter(t_bmp8 * img, float ** kernel, int kernelSize):Applique un filtre de convolution sur l’image en niveaux de gris (flou, flou gaussien, contours, relief, netteté)/img : image à filtrer/ kernel : matrice du filtre à appliquer/ kernelSize: taille du noyau.

Partie 2:

2.3 Fonctions d’allocation et de libération de mémoire:
-t_pixel ** bmp24_allocateDataPixels (int width, int height):Alloue dynamiquement une matrice 2D de pixels pour une image couleur/width : largeur de l’image,height : hauteur de l’image/Retourne une matrice 2D de t_pixel.
-void bmp24_freeDataPixels (t_pixel ** pixels, int height): Libère la mémoire allouée pour les données de pixels d’une image couleur/ pixels : tableau 2D de pixels,height : nombre de lignes (hauteur) à libérer.
-t_bmp24 * bmp24_allocate (int width, int height, int colorDepth):Alloue et initialise une structure t_bmp24 complète pour une image vide/colorDepth : profondeur de couleur (typiquement 24 bits)/Retourne un pointeur vers la structure allouée.
-void bmp24_free (t_bmp24 * img):Libère toute la mémoire associée à une image t_bmp24/img : image à libérer.

2.4 Fonctionnalités : Lecture et écriture d’image 24 bits:
-void bmp24_readPixelValue (t_bmp * image, int x, int y, FILE * file):Lit un pixel RGB dans un fichier BMP et le place dans l’image/image : image dans laquelle écrire le pixel,x, y : coordonnées du pixel,file : fichier image BMP ouvert en lecture binaire.
-void bmp24_readPixelData (t_bmp * image, FILE * file):Lit tous les pixels de l’image à partir du fichier BMP/ image : image dans laquelle charger les données,file : fichier BMP ouvert.
-void bmp24_writePixelValue (t_bmp * image, int x, int y, FILE * file): Écrit un pixel RGB de l’image dans un fichier BMP/  image : image source,x, y : coordonnées du pixel;file : fichier BMP de sortie.
-void bmp24_writePixelData (t_bmp * image, FILE * file):Écrit tous les pixels de l’image dans un fichier BMP.
-t_bmp24 * bmp24_loadImage (const char * filename): Charge une image couleur 24 bits depuis un fichier BMP/ filename : chemin du fichier BMP/Retourne pointeur vers la structure image chargée.
-void bmp24_saveImage (t_bmp * img, const char * filename):Sauvegarde une image couleur 24 bits dans un fichier BMP/img : image à sauvegarder, filename : chemin du fichier de sortie.


2.5 Fonctionnalités : Traitement d’image 24 bits:
-void bmp24_negative (t_bmp * img):Applique un filtre négatif à chaque pixel (inversion RGB)/img : image à modifier
-void bmp24_grayscale (t_bmp * img):Convertit l’image couleur en niveaux de gris (moyenne ou pondération des canaux RGB)/img : image à modifier
-void bmp24_brightness (t_bmp * img, int value):Ajuste la luminosité de chaque canal (R, G, B) uniformément/ img : image à modifier, value : valeur à ajouter aux canaux (positif ou négatif).

2.6 Fonctionnalités : Filtres de convolution:
-t_pixel bmp24_convolution (t_bmp24 * img, int x, int y, float ** kernel, int kernelSize)::Applique un filtre de convolution sur l’image en niveaux de couleur (flou, flou gaussien, contours, relief, netteté)/img : image d’entrée, x, y : coordonnées du pixel cible, kernel : noyau (matrice) de convolution, kernelSize : taille du noyau (typiquement 3)/Retourne un nouveau pixel calculé après application du filtre.

Partie 3:

3.3 Implémentation:
-unsigned int *bmp8_computeHistogram(t_bmp8 *img): Calcule l’histogramme des niveaux de gris d’une image BMP 8 bits/ img : pointeur vers l’image 8 bits/ Retourne un tableau de 256 entiers contenant les fréquences de chaque niveau de gris (0–255).
-unsigned int *bmp8_computeCDF(unsigned int *hist): Calcule la CDF (fonction de distribution cumulée) normalisée à partir d’un histogramme/hist : histogramme de l’image (tableau de 256 entiers)/Retourne un tableau d'entiers contenant la nouvelle valeur égalée (0–255) pour chaque niveau initial.
-void bmp8_equalize(t_bmp8 *img, unsigned int *hist_eq):Applique l’égalisation d’histogramme sur une image 8 bits à partir d’un mapping donnéimg : pointeur vers l’image à modifier/hist_eq : tableau contenant les nouvelles valeurs égalisées pour chaque niveau de gris initial.

3.4 Égalisation d’histogramme en couleur
-void rgb_to_yuv(uint8_t r, uint8_t g, uint8_t b, uint8_t *y, uint8_t *u, uint8_t *v):Convertit un pixel RGB vers le format YUV (luminance + chrominance)/r, g, b : composantes rouge, verte, bleue du pixel,y, u, v : pointeurs pour stocker les composantes YUV calculées.
-void yuv_to_rgb(uint8_t y, uint8_t u, uint8_t v, uint8_t *r, uint8_t *g, uint8_t *b):Convertit un pixel YUV vers le format RGB/y, u, v : composantes YUV,r, g, b : pointeurs pour stocker les composantes RGB reconstruites.
-void bmp24_equalize(t_bmp24 *img): Égalise l’histogramme couleur en travaillant sur la luminance (Y dans YUV)/ img : image 24 bits à traiter.

c-Gestion des Entrées et Erreurs :
1. Traitement des valeurs utilisateur
  Le programme utilise scanf() et getchar() pour lire les entrées au clavier. Les menus sont conçus pour proposer des choix clairs à l’utilisateur :
  Des instructions explicites précèdent chaque saisie (ex. : "Valeur de luminosité :").
2. Vérification de validité des entrées
  Quelques vérifications sont effectuées pour éviter les plantages :
  Vérification du choix d’image initial (1 ou 2) : si le choix est invalide, le programme s’arrête avec un message: "Choix invalide. Fin du programme.".
  Vérification des retours de chargement d’image :
  Si l’image ne se charge pas correctement avec bmp8_loadImage() ou bmp24_loadImage(), un message d’erreur est affiché et le programme quitte.
  Vérification de l’allocation mémoire :
  Chaque appel à malloc() est suivi d’une vérification (if (!ptr)).
  Des messages d’erreur explicites sont affichés en cas d’échec (ex. : "Erreur d’allocation mémoire pour égalisation couleur").
3. Gestion de menus et filtres
Chaque menu propose un retour (9) pour éviter les erreurs de navigation.
En cas de filtre invalide (choix non compris entre les options proposées), un message d’erreur est affiché : "Filtre invalide.".
4. Sauvegarde de l’image
L’utilisateur entre un nom de fichier à sauvegarder, concaténé avec un chemin via snprintf(), ce qui prévient les débordements de tampon.

3-Journal de bord:
a-Chronologie du projet: 
17/03/2025 jusqu'à le 12/04/2025: La partie 1 du projet consiste à implémenter des opérations de base sur les images en niveaux de gris (BMP 8 bits):
                                        -Chargement, affichage et sauvegarde d’une image 8 bits.
                                        -Filtres simples : négatif, luminosité, binarisation(seuil).
                                        -Filtres par convolution : flou, flou gaussien, contours, relief, netteté.
13/04/2025 jusqu'à le 17/05/2025: La partie 2 du projet porte sur le traitement des images couleur (BMP 24 bits):
                                        -Chargement, affichage et sauvegarde d’images couleur
                                        -Filtres simples : négatif, niveaux de gris, luminosité.
                                        -Filtres par convolution : flou, flou gaussien, contours, relief, netteté.
21/05/2025 jusqu'à 24/05/2025: La partie 3 du projet consiste à implémenter l'égalisation d'histogramme, une technique de traitement d’image permettant d'améliorer le contraste, Cela permet d’améliorer visuellement la clarté et la répartition des intensités dans une image:
                                        -Égalisation d’histogramme pour images en niveaux de gris (8 bits) : calculer l’histogramme, sa CDF, et appliquer la transformation.
                                        -Égalisation pour images couleur (24 bits) : convertir l’image en espace YUV, égaliser la composante Y (luminance), puis reconvertir en RGB.

b-Répartition des tâches:
Louise Mahiout: Partie1 / Partie 3: 3.3/ S'occuper de l'interface
Sarra Ghedira: Partie 2/ Partie 3: 3.4/ Readme













