#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp8.h"

int main() {
    t_bmp8 *image = NULL;
    char nomFichier[256] = "../barbara_gray.bmp";  // Fichier à ouvrir par défaut
    int choix;

    // Chargement automatique de l’image
    image = bmp8_loadImage(nomFichier);
    if (image != NULL) {
        printf("Image %s chargée automatiquement au démarrage.\n", nomFichier);
    } else {
        printf("Erreur : impossible de charger l’image %s.\n", nomFichier);
        return 1;
    }

    // Menu utilisateur
    while (1) {
        printf("\n===== MENU =====\n");
        printf("1. Afficher les informations de l'image\n");
        printf("2. Sauvegarder l'image\n");
        printf("3. Quitter\n");
        printf(">>> Votre choix : ");
        scanf("%d", &choix);
        getchar();  // consomme le '\n'

        switch (choix) {
            case 1:
                bmp8_printInfo(image);
            break;

            case 2: {
                char nomSortie[256];
                printf("Nom du fichier de sauvegarde : ");
                scanf("%255s", nomSortie);
                bmp8_saveImage(nomSortie, image);
                break;
            }

            case 3:
                bmp8_free(image);
            printf("Fermeture du programme.\n");
            return 0;

            default:
                printf("Choix invalide. Veuillez réessayer.\n");
        }
    }

    return 0;
}

