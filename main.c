#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp8.h"

#define CHEMIN_FICHIER_SORTIE "images_sortie/"////essaibdskqhbvkqdbjvqdsbj

float **allocateKernel(const float values[9]) {
    float **kernel = malloc(3 * sizeof(float *));
    if (!kernel) return NULL;
    for (int i = 0; i < 3; i++) {
        kernel[i] = malloc(3 * sizeof(float));
        if (!kernel[i]) return NULL;
        for (int j = 0; j < 3; j++) {
            kernel[i][j] = values[i * 3 + j];
        }
    }
    return kernel;
}

void freeKernel(float **kernel) {
    if (!kernel) return;
    for (int i = 0; i < 3; i++) {
        free(kernel[i]);
    }
    free(kernel);
}

int main() {
    t_bmp8 *image = NULL;
    char nomFichier[256] = "images_entree/lena.bmp";
    int choix;


    image = bmp8_loadImage(nomFichier);
    if (image != NULL) {
        printf("Image %s chargee automatiquement au demarrage.\n", nomFichier);
    } else {
        printf("Erreur : impossible de charger l’image %s.\n", nomFichier);
        return 1;
    }

    while (1) {
        printf("\n===== MENU =====\n");
        printf("1. Afficher les informations de l'image\n");
        printf("2. Sauvegarder l'image\n");
        printf("3. Appliquer un filtre\n");
        printf("4. Quitter\n");
        printf(">>> Votre choix : ");
        scanf("%d", &choix);
        getchar();

        switch (choix) {
            case 1:
                bmp8_printInfo(image);
                break;

            case 2: {
                char nomSortie[256] = CHEMIN_FICHIER_SORTIE;
                char tmp[256];
                printf("Nom du fichier de sauvegarde (ajouter .bmp à la fin) : ");
                scanf("%255s", tmp);
                strcat(nomSortie, tmp);
                bmp8_saveImage(nomSortie, image);
                break;
            }

            case 3: {
                int filtre;
                printf("\n-- Choisir un filtre --\n");
                printf("1. Negatif\n");
                printf("2. Luminosite\n");
                printf("3. Binarisation (seuillage)\n");
                printf("4. Flou\n");
                printf("5. Flou gaussien\n");
                printf("6. Contours\n");
                printf("7. Relief\n");
                printf("8. Nettete\n");
                printf("9. Retour\n");
                printf(">>> Votre choix : ");
                scanf("%d", &filtre);

                float **kernel = NULL;

                switch (filtre) {
                    case 1:
                        bmp8_negative(image);
                        break;

                    case 2: {
                        int value;
                        printf("Valeur de luminosite (ex: +30/-30) : ");
                        scanf("%d", &value);
                        bmp8_brightness(image, value);
                        break;
                    }

                    case 3: {
                        int seuil;
                        printf("Valeur du seuil (0-255) : ");
                        scanf("%d", &seuil);
                        bmp8_threshold(image, seuil);
                        break;
                    }

                    case 4: {
                        float blur[9] = {
                            1.0f/9, 1.0f/9, 1.0f/9,
                            1.0f/9, 1.0f/9, 1.0f/9,
                            1.0f/9, 1.0f/9, 1.0f/9
                        };
                        kernel = allocateKernel(blur);
                        break;
                    }

                    case 5: {
                        float gauss[9] = {
                            1.0f/16, 2.0f/16, 1.0f/16,
                            2.0f/16, 4.0f/16, 2.0f/16,
                            1.0f/16, 2.0f/16, 1.0f/16
                        };
                        kernel = allocateKernel(gauss);
                        break;
                    }

                    case 6: {
                        float outline[9] = {
                            -1, -1, -1,
                            -1,  8, -1,
                            -1, -1, -1
                        };
                        kernel = allocateKernel(outline);
                        break;
                    }

                    case 7: {
                        float emboss[9] = {
                            -2, -1, 0,
                            -1,  1, 1,
                             0,  1, 2
                        };
                        kernel = allocateKernel(emboss);
                        break;
                    }

                    case 8: {
                        float sharpen[9] = {
                             0, -1,  0,
                            -1,  5, -1,
                             0, -1,  0
                        };
                        kernel = allocateKernel(sharpen);
                        break;
                    }

                    case 9:
                        break;

                    default:
                        printf("Filtre invalide.\n");
                }

                if (kernel != NULL) {
                    bmp8_applyFilter(image, kernel, 3);
                    freeKernel(kernel);
                    printf("Filtre appliqué avec succès.\n");
                }

                break;
            }

            case 4:
                bmp8_free(&image);
                printf("Fermeture du programme.\n");
                return 0;

            default:
                printf("Choix invalide. Veuillez réessayer.\n");
        }
    }

    return 0;
}



