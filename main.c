


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "bmp8.h"

#define CHEMIN_FICHIER_ENTREE "images_entree/"
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
    t_bmp8 *image8 = NULL;
    t_bmp24 *image24 = NULL;
    int choixImage = 0;
    int quitter = 0;

    // === CHOIX INITIAL DE L'IMAGE À TRAITER ===
    printf("Quelle image souhaitez-vous utiliser ?\n");
    printf("1. Femme en niveaux de gris (8 bits) [barbara_gray.bmp]\n");
    printf("2. Fleurs en couleur (24 bits)       [flowers_color.bmp]\n");
    printf(">>> Votre choix : ");
    scanf("%d", &choixImage);
    getchar();

    if (choixImage == 1) {
        // Charger image 8 bits
        char chemin[256];
        snprintf(chemin, sizeof(chemin), "%sbarbara_gray.bmp", CHEMIN_FICHIER_ENTREE);
        image8 = bmp8_loadImage(chemin);
        if (!image8) {
            printf("Erreur lors du chargement de l'image en niveaux de gris.\n");
            return 1;
        }
        printf("Image en niveaux de gris chargee avec succes.\n");

    } else if (choixImage == 2) {
        // Charger image 24 bits
        char chemin[256];
        snprintf(chemin, sizeof(chemin), "%sflowers_color.bmp", CHEMIN_FICHIER_ENTREE);
        image24 = bmp24_loadImage(chemin);
        if (!image24) {
            printf("Erreur lors du chargement de l'image couleur.\n");
            return 1;
        }
        printf("Image couleur chargee avec succes.\n");

    } else {
        printf("Choix invalide. Fin du programme.\n");
        return 1;
    }

    // === MENU PRINCIPAL ADAPTÉ SELON TYPE D'IMAGE ===
    while (!quitter) {
        printf("\n===== MENU IMAGE %s =====\n", (choixImage == 1) ? "8 bits (gris)" : "24 bits (couleur)");
        printf("1. Afficher les informations de l'image\n");
        printf("2. Appliquer un filtre\n");
        printf("3. Egalisation de l'histogramme\n");
        printf("4. Sauvegarder l'image\n");
        printf("5. Quitter\n");
        printf(">>> Votre choix : ");

        int choix;
        scanf("%d", &choix);
        getchar();

        switch (choix) {
            case 1: {
                if (choixImage == 1)
                    bmp8_printInfo(image8);
                else
                    printf("Largeur : %d\nHauteur : %d\nProfondeur : %d bits\n",
                        image24->width, image24->height, image24->colorDepth);
                break;
            }

            case 2: {
                printf("\n-- Filtres disponibles --\n");
                printf("1. Negatif\n");
                printf("2. Luminosite\n");

                if (choixImage == 1) {
                    printf("3. Binarisation (seuil)\n");
                    printf("4. Flou\n5. Flou gaussien\n6. Contours\n7. Relief\n8. Nettete\n");
                } else {
                    printf("3. Niveaux de gris\n4. Flou\n5. Flou gaussien\n6. Contours\n7. Relief\n8. Nettete\n");
                }

                printf("9. Retour\n>>> Votre choix : ");
                int filtre;
                scanf("%d", &filtre);
                getchar();

                float **kernel = NULL;

                if (choixImage == 1) { // GRIS 8 BITS
                    switch (filtre) {
                        case 1:
                            bmp8_negative(image8);
                            break;
                        case 2: {
                            int v;
                            printf("Valeur (+/-) : ");
                            scanf("%d", &v);
                            bmp8_brightness(image8, v);
                            break;
                        }
                        case 3: {
                            int seuil;
                            printf("Valeur du seuil : ");
                            scanf("%d", &seuil);
                            bmp8_threshold(image8, seuil);
                            break;
                        }
                        case 4: {
                            float blur[9] = {1.0f/9,1.0f/9,1.0f/9, 1.0f/9,1.0f/9,1.0f/9, 1.0f/9,1.0f/9,1.0f/9};
                            kernel = allocateKernel(blur);
                            break;
                        }
                        case 5: {
                            float gauss[9] = {1.0f/16,2.0f/16,1.0f/16, 2.0f/16,4.0f/16,2.0f/16, 1.0f/16,2.0f/16,1.0f/16};
                            kernel = allocateKernel(gauss);
                            break;
                        }
                        case 6: {
                            float edge[9] = {-1,-1,-1, -1,8,-1, -1,-1,-1};
                            kernel = allocateKernel(edge);
                            break;
                        }
                        case 7: {
                            float emboss[9] = {-2,-1,0, -1,1,1, 0,1,2};
                            kernel = allocateKernel(emboss);
                            break;
                        }
                        case 8: {
                            float sharp[9] = {0,-1,0, -1,5,-1, 0,-1,0};
                            kernel = allocateKernel(sharp);
                            break;
                        }
                        case 9: break;
                        default: printf("Filtre invalide.\n");
                    }

                    if (kernel) {
                        bmp8_applyFilter(image8, kernel, 3);
                        freeKernel(kernel);
                    }

                } else { // COULEUR 24 BITS
                    switch (filtre) {
                        case 1:
                            bmp24_negative(image24);
                            printf("Filtre negatif applique avec succes !\n");
                            break;
                        case 2: {
                            int v;
                            printf("Valeur (+/-) : ");
                            scanf("%d", &v);
                            bmp24_brightness(image24, v);
                            printf("Filtre luminosite applique avec succes !\n");
                            break;
                        }
                        case 3:
                            bmp24_grayscale(image24);
                            printf("Filtre niveaux de gris applique avec succes !\n");
                            break;
                        case 4: {
                            float blur[9] = {1.0f/9,1.0f/9,1.0f/9, 1.0f/9,1.0f/9,1.0f/9, 1.0f/9,1.0f/9,1.0f/9};
                            kernel = allocateKernel(blur);
                            printf("Filtre flou applique avec succes !\n");
                            break;
                        }
                        case 5: {
                            float gauss[9] = {1.0f/16,2.0f/16,1.0f/16, 2.0f/16,4.0f/16,2.0f/16, 1.0f/16,2.0f/16,1.0f/16};
                            kernel = allocateKernel(gauss);
                            printf("Filtre flou gaussien applique avec succes !\n");
                            break;
                        }
                        case 6: {
                            float edge[9] = {-1,-1,-1, -1,8,-1, -1,-1,-1};
                            kernel = allocateKernel(edge);
                            printf("Filtre contours applique avec succes !\n");
                            break;
                        }
                        case 7: {
                            float emboss[9] = {-2,-1,0, -1,1,1, 0,1,2};
                            kernel = allocateKernel(emboss);
                            printf("Filtre relief applique avec succes !\n");
                            break;
                        }
                        case 8: {
                            float sharp[9] = {0,-1,0, -1,5,-1, 0,-1,0};
                            kernel = allocateKernel(sharp);
                            printf("Filtre nettete applique avec succes !\n");
                            break;
                        }
                        case 9: break;
                        default: printf("Filtre invalide.\n");
                    }

                    if (kernel) {
                        t_pixel **temp = bmp24_allocateDataPixels(image24->width, image24->height);
                        for (int y = 0; y < image24->height; y++) {
                            for (int x = 0; x < image24->width; x++) {
                                temp[y][x] = bmp24_convolution(image24, x, y, kernel, 3);
                            }
                        }
                        for (int y = 0; y < image24->height; y++) {
                            for (int x = 0; x < image24->width; x++) {
                                image24->data[y][x] = temp[y][x];
                            }
                            free(temp[y]);
                        }
                        free(temp);
                        freeKernel(kernel);
                        printf("Filtre applique avec succes !\n");
                    }
                }
                break;
            }

            case 3: {
                if (choixImage == 1) {
                    unsigned int *hist = bmp8_computeHistogram(image8);
                    unsigned int *cdf = bmp8_computeCDF(hist);
                    bmp8_equalize(image8, cdf);
                    free(hist);
                    free(cdf);
                } else {
                    bmp24_equalize(image24);
                }
                break;
            }

            case 4: {
                char nom[256];
                printf("Nom du fichier de sauvegarde (ajoutez .bmp) : ");
                scanf("%s", nom);

                char chemin[512];
                snprintf(chemin, sizeof(chemin), "%s%s", CHEMIN_FICHIER_SORTIE, nom);

                if (choixImage == 1)
                    bmp8_saveImage(chemin, image8);
                else
                    bmp24_saveImage(image24, chemin);
                printf("Image enregistre avec succes !\n");

                break;
            }

            case 5: {
                quitter = 1;
                break;
            }

            default:
                printf("Choix invalide.\n");
        }
    }

    // === LIBÉRATION DE MÉMOIRE ===
    if (image8) bmp8_free(&image8);
    if (image24) bmp24_free(image24);

    printf("Programme termine. Au revoir !\n");
    return 0;
}
