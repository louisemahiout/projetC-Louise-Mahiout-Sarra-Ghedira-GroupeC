#include <stdio.h>
#include "bmp8.h"

int main() {
    // Étape 1 : chemin vers l’image BMP à lire
    const char *inputFilename = "barbara_gray.bmp";
    const char *outputFilename = "barbara_copy.bmp";

    // Étape 2 : charger l’image depuis le fichier
    t_bmp8 *image = bmp8_loadImage(inputFilename);

    // Étape 3 : vérifier que l’image a bien été chargée
    if (image == NULL) {
        printf("Échec du chargement de l’image.\n");
        return 1; // sortir du programme avec une erreur
    }

    // Étape 4 : afficher les infos de l’image
    bmp8_printInfo(image);

    // Étape 5 : enregistrer l’image dans un nouveau fichier
    bmp8_saveImage(outputFilename, image);

    // Étape 6 : libérer la mémoire allouée
    bmp8_free(image);

    // Fin du programme
    printf("Fin du programme\n");
    return 0;
}
