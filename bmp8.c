//
// Created by louis on 21/04/2025.
//
#include "bmp8.h"
#include <stdio.h>
#include <stdlib.h>
//utile pour malloc

t_bmp8 * bmp8_loadImage(const char * filename) { //allouera dynamiquement de la mémoire pour stocker une image de type t_bmp8, initialisera les champs de cette image et retournera un pointeur vers cette image.
    FILE * file=fopen(filename,"rb"); // Étape 1 : ouvrir le fichier en mode binaire ("rb" = read binary)
    if (file==NULL) {
        printf("Erreur: impossible d'ouvrir le fichier %s\n",filename);
        return NULL;
    }
    // Étape 2 : allouer de la mémoire pour une structure t_bmp8
    t_bmp8 *image=malloc(sizeof(t_bmp8));
    if (image==NULL) {
        printf("Erreur: impossible d'allouer la mémoire\n");
        fclose(file);
        return NULL;
    }
    // Étape 3 : lire les 54 premiers octets du fichier (l'en-tête BMP)
    fread(image->header,sizeof(unsigned char),54,file);

    // Étape 4 : extraire les champs importants de l’en-tête
    image->width = *(unsigned int*)&image->header[18]; // Offset 18 (4 octets)
    image->height = *(unsigned int*)&image->header[22]; // Offset 22 (4 octets)
    image->colorDepth= *(unsigned short*)&image->header[28]; //Offset 28 (2 octets)
    image->dataSize= *(unsigned int*)&image->header[34]; //Offset 34 (4 octets)

    // Étape 5 : vérifier que c’est bien une image en niveaux de gris (8 bits)
    if (image->colorDepth !=8) {
        printf("Erreur : l’image n’est pas en niveaux de gris (profondeur = %d au lieu de 8)\n", image->colorDepth);
        fclose(file);
        free(image);
        return NULL;
    }
    // Étape 6 : lire la table de couleurs (palette) — 256 entrées x 4 octets = 1024 octets
    fread(image->colorTable,sizeof(unsigned char),1024,file);

    // Étape 7 : allouer la mémoire pour les données de l’image (les pixels)
    image->data=malloc(image->dataSize); // Chaque pixel = 1 octet (valeur entre 0 et 255)
    if (image->data==NULL) {
        printf("Erreur : échec de l’allocation mémoire pour les pixels\n");
        fclose(file);
        free(image);
        return NULL;
    }
    // Étape 8 : lire les données de l’image (à partir de la fin de la palette)
    fread(image->data,sizeof(unsigned char),image->dataSize,file);
    // Étape 9 : fermer le fichier
    fclose(file);
    // Étape 10 : retourner l’image chargée
    return image;
}




void bmp8_saveImage(const char * filename, t_bmp8 * img);
void bmp8_free(t_bmp8 * img);
void bmp8_printInfo(t_bmp8 * img);