

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp8.h"
#include <stdint.h>

#define CHEMIN_FICHIER_SORTIE "images_sortie/"

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
    t_bmp24 *image24 = NULL;
    char nomFichier[256] = "images_entree/barbara_gray.bmp";
    int choix;

    image = bmp8_loadImage(nomFichier);
    if (image != NULL) {
        printf("Image %s chargee automatiquement au demarrage.\n", nomFichier);
    } else {
        printf("Erreur : impossible de charger l'image %s.\n", nomFichier);
        return 1;
    }

    while (1) {
        printf("\n===== MENU =====\n");
        printf("PARTIE 1\n");
        printf("1. Afficher les informations de l'image\n");
        printf("2. Appliquer un filtre\n");
        printf("3. Egalisation histogramme\n");
        printf("4. Sauvegarder l'image\n");
        printf("5. Quitter\n");
        printf("PARTIE 2\n");
        printf("6. Ouvrir l'image couleur\n");
        printf("7. Afficher les informations de l'image couleur\n");
        printf("8. Appliquer un filtre a l'image couleur\n");
        printf("9. Histogramme couleur\n");
        printf("10. sauvegarder\n");
        printf("11. Quitter\n");
        printf(">>> Votre choix : ");
        scanf("%d", &choix);
        getchar();

        switch (choix) {
            case 1:
                bmp8_printInfo(image);
                break;

            case 2: {
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
            case 3: {
                unsigned int *hist = bmp8_computeHistogram(image);
                unsigned int *cdf = bmp8_computeCDF(hist);
                bmp8_equalize(image, cdf);
                free(hist);
                free(cdf);
                break;
            }

            case 4: {
                char nomSortie[256] = CHEMIN_FICHIER_SORTIE;
                char tmp[256];
                printf("Nom du fichier de sauvegarde (Ajoutez .bmp a la fin) : ");
                scanf("%255s", tmp);
                strcat(nomSortie, tmp);
                bmp8_saveImage(nomSortie, image);
                break;
            }

            case 5:
                bmp8_free(&image);
                printf("Fermeture du programme.\n");
                return 0;

            case 6: {
                char fichier24[] = "images_entree/flowers_color.bmp";

                // Vérification d'existence du fichier
                FILE *test = fopen(fichier24, "rb");
                if (!test) {
                    printf("Fichier introuvable : %s\n", fichier24);
                    break;
                }
                fclose(test);

                image24 = bmp24_loadImage(fichier24);
                if (image24 != NULL) {
                    printf("Image couleur 24 bits \"%s\" chargee avec succes.\n", fichier24);
                } else {
                    printf("Erreur lors du chargement de l’image couleur \"%s\".\n", fichier24);
                }


                break;
            }


            case 7: {
                if (image24 == NULL) {
                    printf("Aucune image couleur chargee.\n");
                } else {
                    printf("Image couleur :\n");
                    printf("Largeur : %d\n", image24->width);
                    printf("Hauteur : %d\n", image24->height);
                    printf("Profondeur : %d bits\n", image24->colorDepth);
                }
                break;
            }
            case 8: {
                if (!image24) {
                    printf("Aucune image couleur chargee.\n");
                    break;
                }

                int op;
                printf("\n-- Traitement de l'image couleur --\n");
                printf("1. Negatif\n");
                printf("2. Niveaux de gris\n");
                printf("3. Luminosite\n");
                printf("4. Flou\n");
                printf("5. Flou gaussien\n");
                printf("6. Contours\n");
                printf("7. Relief\n");
                printf("8. Nettete\n");
                printf(">>> Votre choix : ");
                scanf("%d", &op);

                float **kernel = NULL;

                switch (op) {
                       case 1: {
                           bmp24_negative(image24);
                           printf("Négatif appliqué.\n");
                           break;
                       }

                       case 2: {
                           bmp24_grayscale(image24);
                           printf("Niveaux de gris appliqués.\n");
                           break;
                       }
                       case 3: {
                            int v;
                            printf("Valeur de luminosité (+/-) : ");
                            scanf("%d", &v);
                            bmp24_brightness(image24, v);
                            printf("Luminosité ajustée.\n");
                            break;
                            }

                        case 4: { // Flou box blur
                            float blur[9] = {
                             1.0f/9, 1.0f/9, 1.0f/9,
                             1.0f/9, 1.0f/9, 1.0f/9,
                             1.0f/9, 1.0f/9, 1.0f/9
                             };
                        kernel = allocateKernel(blur);
                        break;
                        }

                        case 5: { // Flou gaussien
                            float gauss[9] = {
                                1.0f/16, 2.0f/16, 1.0f/16,
                                2.0f/16, 4.0f/16, 2.0f/16,
                                1.0f/16, 2.0f/16, 1.0f/16
                                };
                        kernel = allocateKernel(gauss);
                        break;
                        }

                        case 6: { // Contours
                            float outline[9] = {
                                -1, -1, -1,
                                -1,  8, -1,
                                -1, -1, -1
                            };
                        kernel = allocateKernel(outline);
                        break;
                        }

                        case 7: { // Relief (Emboss)
                           float emboss[9] = {
                               -2, -1,  0,
                               -1,  1,  1,
                                0,  1,  2
                           };
                           kernel = allocateKernel(emboss);
                           break;
                    }


                        case 8: { // Netteté
                            float sharpen[9] = {
                                0, -1,  0,
                                -1,  5, -1,
                                0, -1,  0
                            };
                            kernel = allocateKernel(sharpen);
                            break;
                            }

                 default:
                    printf("Option invalide.\n");
                }

                if (kernel != NULL) {
                    // Créer une image temporaire pour stocker le résultat
                    t_pixel **tempData = bmp24_allocateDataPixels(image24->width, image24->height);
                    if (!tempData) {
                        printf("Erreur d'allocation pour l'image temporaire.\n");
                        freeKernel(kernel);
                        break;
                    }

                    // Appliquer le filtre de convolution pixel par pixel
                    for (int y = 0; y < image24->height; y++) {
                        for (int x = 0; x < image24->width; x++) {
                            tempData[y][x] = bmp24_convolution(image24, x, y, kernel, 3);
                        }
                    }

                    // Copier les données traitées dans l’image originale
                    for (int y = 0; y < image24->height; y++) {
                        for (int x = 0; x < image24->width; x++) {
                            image24->data[y][x] = tempData[y][x];
                        }
                        free(tempData[y]);
                    }
                    free(tempData);

                    freeKernel(kernel);
                    printf("Filtre de convolution appliqué avec succès !\n");
                }


                break;
            }
            case 9: {
                if (image24) {
                    bmp24_equalize(image24);
                    printf("Égalisation d’histogramme couleur effectuée avec succès.\n");
                } else {
                    printf("Aucune image couleur chargée.\n");
                }
                break;

                default:
                    printf("Choix invalide. Veuillez réessayer.\n");
            }
            case 10: {
                if (image24) {
                    char nomSortie[256] = CHEMIN_FICHIER_SORTIE;
                    char tmp[256];
                    printf("Nom du fichier de sauvegarde couleur (ex: flowers_negative.bmp) : ");
                    scanf("%255s", tmp);

                    // Concaténer le chemin de sortie avec le nom du fichier
                    strcat(nomSortie, tmp);

                    // Sauvegarder l'image
                    bmp24_saveImage(image24, nomSortie);

                    printf("Image couleur sauvegardee avec succes sous: %s\n", nomSortie);
                } else {
                    printf("Aucune image couleur à sauvegarder.\n");
                }
                break;
            }
            case 11: {
                if (image24) {
                    bmp24_free(image24);
                    image24 = NULL;
                    printf("Image couleur libérée.\n");
                } else {
                    printf("Aucune image couleur à libérer.\n");
                }

                if (image) {
                    bmp8_free(&image);
                }

                printf("Fermeture du programme.\n");
                return 0;
            }


        }


    }

    return 0;
}
