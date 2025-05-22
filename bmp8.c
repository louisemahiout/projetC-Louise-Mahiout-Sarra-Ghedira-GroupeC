//
// Created by louis on 21/04/2025.
//
#include "bmp8.h"
#include <stdio.h>
#include <stdlib.h>//utile pour malloc
#include <math.h> // pour round()
#include <string.h>



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
    //image->dataSize= *(unsigned int*)&image->header[34]; //Offset 34 (4 octets) // est à 0...
    image->dataSize= (image->width * image->height) * (image->colorDepth/8); // car dataSize à 0... (?)

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


void bmp8_saveImage(const char * filename, t_bmp8 * img) {
    // Étape 1 : ouvrir le fichier en écriture binaire
    FILE *file = fopen(filename, "wb");  // "wb" = write binary
    printf("%s\n", filename);
    if (file == NULL) {
        printf("Erreur : impossible de créer le fichier %s\n", filename);
        return;
    }
    // Étape 2 : écrire l’en-tête (54 octets)
    size_t written = fwrite(img->header, sizeof(unsigned char), 54, file);
    if (written != 54) {
        printf("Erreur : échec de l’écriture de l’en-tête dans le fichier %s\n", filename);
        fclose(file);
        return;
    }
    // Étape 3 : écrire la table des couleurs (palette : 256 x 4 = 1024 octets)
    written = fwrite(img->colorTable, sizeof(unsigned char), 1024, file);
    if (written != 1024) {
        printf("Erreur : échec de l’écriture de la palette dans le fichier %s\n", filename);
        fclose(file);
        return;
    }
    // Étape 4 : écrire les données de l’image (pixels)
    written = fwrite(img->data, sizeof(unsigned char), img->dataSize, file);
    if (written != img->dataSize) {
        printf("Erreur : échec de l’écriture des données dans le fichier %s\n", filename);
        fclose(file);
        return;
    }
    // Étape 5 : fermer le fichier
    fclose(file);
    printf("Image enregistree avec succes dans le fichier %s\n", filename);
}


void bmp8_free(t_bmp8 ** img) {
    // on vérifie que le pointeur n’est pas NULL avant de libérer
    if (*img == NULL) {
        return;  // on a rien à libérer
    }
    // Libérer les données de l’image si elles ont été allouées
    if ((*img)->data != NULL) {
        free((*img)->data);
        (*img)->data = NULL;  // on annule le pointeur au cas où
    }
    // Libérer la structure elle-même
    free(*img);
    *img = NULL;  //à voir si c'est vraiment utile
}


void bmp8_printInfo(t_bmp8 * img) {
    if (img == NULL) {
        printf("Erreur : image vide (pointeur NULL).\n");
        return;
    }
    // Affichage des infos
    printf("Image Info:\n");
    printf("Width: %u\n", img->width);
    printf("Height: %u\n", img->height);
    printf("Color Depth: %u\n", img->colorDepth);
    printf("Data Size: %u\n", img->dataSize);
}

void bmp8_negative(t_bmp8 * img) {
    if (img == NULL || img->data == NULL) {
        printf("Erreur : image non chargee.\n");
        return;
    }

    for (unsigned int i = 0; i < img->dataSize; i++) {
        img->data[i] = 255 - img->data[i];  // inversion
    }

    printf("Filtre negatif applique avec succes !\n");
}


void bmp8_brightness(t_bmp8 * img, int value) {
    if (img == NULL || img->data == NULL) {
        printf("Erreur : image non chargée.\n");
        return;
    }

    for (unsigned int i = 0; i < img->dataSize; i++) {
        int pixel = img->data[i] + value;

        if (pixel > 255) {
            pixel = 255;
        } else if (pixel < 0) {
            pixel = 0;
        }

        img->data[i] = (unsigned char)pixel;
    }

    printf("Luminosité ajustée avec succès (value = %d).\n", value);
}


void bmp8_threshold(t_bmp8 * img, int threshold) {
    if (img == NULL || img->data == NULL) {
        printf("Erreur : image non chargée.\n");
        return;
    }

    for (unsigned int i = 0; i < img->dataSize; i++) {
        if (img->data[i] >= threshold) {
            img->data[i] = 255;
        } else {
            img->data[i] = 0;
        }
    }

    printf("Seuillage appliqué avec succès (threshold = %d).\n", threshold);
}


void bmp8_applyFilter(t_bmp8 *img, float **kernel, int kernelSize) {
    if (img == NULL || img->data == NULL) {
        printf("Erreur : image non chargée.\n");
        return;
    }

    int width = img->width;
    int height = img->height;
    int offset = kernelSize / 2;

    // Allocation de la mémoire pour stocker temporairement l'image filtrée
    unsigned char *result = malloc(img->dataSize);
    if (result == NULL) {
        printf("Erreur : échec de l’allocation mémoire pour l’image filtrée.\n");
        return;
    }

    // Copie de l'image originale pour éviter de modifier l'image en direct
    memcpy(result, img->data, img->dataSize);

    // Application de la convolution (en ignorant les bords)
    for (int y = offset; y < height - offset; y++) {
        for (int x = offset; x < width - offset; x++) {
            float sum = 0.0f;

            for (int i = -offset; i <= offset; i++) {
                for (int j = -offset; j <= offset; j++) {
                    int pixel = img->data[(y + i) * width + (x + j)];
                    float coeff = kernel[i + offset][j + offset];
                    sum += pixel * coeff;
                }
            }

            // Clamp la valeur finale dans [0, 255]
            int newValue = (int)roundf(sum);
            if (newValue < 0) newValue = 0;
            if (newValue > 255) newValue = 255;

            result[y * width + x] = (unsigned char)newValue;
        }
    }

    // Remplace les données d’origine par le résultat filtré
    memcpy(img->data, result, img->dataSize);
    free(result);

    printf("Filtre de convolution appliqué avec succès !\n");
}



