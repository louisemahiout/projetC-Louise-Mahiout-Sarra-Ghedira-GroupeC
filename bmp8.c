



//
// Created by louis on 21/04/2025.
//
#include "bmp8.h"
#include <stdio.h>
#include <stdlib.h>//utile pour malloc
#include <math.h> // pour round()
#include <string.h>


// Partie 1
t_bmp8 * bmp8_loadImage(const char * filename) { //allouera dynamiquement de la mémoire pour stocker une image de type t_bmp8, initialisera les champs de cette image et retournera un pointeur vers cette image.
    FILE * file=fopen(filename,"rb"); // Étape 1 : ouvrir le fichier en mode binaire ("rb" = read binary)
    if (file==NULL) {
        printf("Erreur: impossible d'ouvrir le fichier %s\n",filename);
        return NULL;
    }
    // Étape 2 : allouer de la mémoire pour une structure t_bmp8
    t_bmp8 *image=malloc(sizeof(t_bmp8));
    if (image==NULL) {
        printf("Erreur: impossible d'allouer la memoire\n");
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
        printf("Erreur : l'image n'est pas en niveaux de gris (profondeur = %d au lieu de 8)\n", image->colorDepth);
        fclose(file);
        free(image);
        return NULL;
    }

    // Étape 6 : lire la table de couleurs (palette) — 256 entrées x 4 octets = 1024 octets
    fread(image->colorTable,sizeof(unsigned char),1024,file);

    // Étape 7 : allouer la mémoire pour les données de l’image (les pixels)
    image->data=malloc(image->dataSize); // Chaque pixel = 1 octet (valeur entre 0 et 255)
    if (image->data==NULL) {
        printf("Erreur : echec de l’allocation memoire pour les pixels\n");
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
        printf("Erreur : impossible de creer le fichier %s\n", filename);
        return;
    }
    // Étape 2 : écrire l’en-tête (54 octets)
    size_t written = fwrite(img->header, sizeof(unsigned char), 54, file);
    if (written != 54) {
        printf("Erreur : echec de l’ecriture de l’en-tête dans le fichier %s\n", filename);
        fclose(file);
        return;
    }
    // Étape 3 : écrire la table des couleurs (palette : 256 x 4 = 1024 octets)
    written = fwrite(img->colorTable, sizeof(unsigned char), 1024, file);
    if (written != 1024) {
        printf("Erreur : echec de l’ecriture de la palette dans le fichier %s\n", filename);
        fclose(file);
        return;
    }
    // Étape 4 : écrire les données de l’image (pixels)
    written = fwrite(img->data, sizeof(unsigned char), img->dataSize, file);
    if (written != img->dataSize) {
        printf("Erreur : echec de l’ecriture des donnees dans le fichier %s\n", filename);
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
    printf("Informations sur l'image:\n");
    printf("Largeur: %u\n", img->width);
    printf("Hauteur: %u\n", img->height);
    printf("Profondeur de l'image en bits: %u\n", img->colorDepth);
    printf("Taille des donnees: %u\n", img->dataSize);
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
        printf("Erreur : image non chargee.\n");
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

    printf("Luminosite ajustee avec succes (value = %d).\n", value);
}


void bmp8_threshold(t_bmp8 * img, int threshold) {
    if (img == NULL || img->data == NULL) {
        printf("Erreur : image non chargee.\n");
        return;
    }

    for (unsigned int i = 0; i < img->dataSize; i++) {
        if (img->data[i] >= threshold) {
            img->data[i] = 255;
        } else {
            img->data[i] = 0;
        }
    }

    printf("Seuillage applique avec succes (threshold = %d).\n", threshold);
}


void bmp8_applyFilter(t_bmp8 *img, float **kernel, int kernelSize) {
    if (img == NULL || img->data == NULL) {
        printf("Erreur : image non chargee.\n");
        return;
    }

    int width = img->width;
    int height = img->height;
    int offset = kernelSize / 2;

    // Allocation de la mémoire pour stocker temporairement l'image filtrée
    unsigned char *result = malloc(img->dataSize);
    if (result == NULL) {
        printf("Erreur : echec de l’allocation memoire pour l’image filtree.\n");
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

    printf("Filtre de convolution applique avec succes !\n");
}

// Partie 2
// Alloue dynamiquement un tableau 2D de pixels (t_pixel) de taille width x height
t_pixel **bmp24_allocateDataPixels(int width, int height) {
    // Allocation d'un tableau de pointeurs vers t_pixel, un pointeur par ligne (height lignes)
    t_pixel **pixels = malloc(height * sizeof(t_pixel *));
    if (!pixels) { // Vérifie si l'allocation a échoué
        fprintf(stderr, "Erreur d'allocation de lignes de pixels.\n");
        return NULL; // Retourne NULL en cas d'échec
    }

    // Pour chaque ligne, alloue un tableau de pixels (width pixels)
    for (int i = 0; i < height; i++) {
        pixels[i] = malloc(width * sizeof(t_pixel));
        if (!pixels[i]) { // Vérifie si l'allocation de la ligne i a échoué
            fprintf(stderr, "Erreur d'allocation de pixels pour la ligne %d.\n", i);
            // Libère toutes les lignes déjà allouées pour éviter une fuite mémoire
            for (int j = 0; j < i; j++) {
                free(pixels[j]);
            }
            // Libère aussi le tableau de pointeurs
            free(pixels);
            return NULL; // Retourne NULL en cas d'échec
        }
    }

    return pixels; // Retourne le pointeur vers le tableau 2D alloué
}

// Libère la mémoire allouée pour un tableau 2D de pixels de hauteur "height"
void bmp24_freeDataPixels(t_pixel **pixels, int height) {
    if (!pixels) return; // Rien à faire si le pointeur est NULL
    // Libère chaque ligne (tableau de pixels)
    for (int i = 0; i < height; i++) {
        free(pixels[i]);
    }
    // Libère le tableau de pointeurs
    free(pixels);
}

// Alloue une structure t_bmp24 et initialise ses données pixel
t_bmp24 *bmp24_allocate(int width, int height, int colorDepth) {
    // Allocation de la structure t_bmp24
    t_bmp24 *img = malloc(sizeof(t_bmp24));
    if (!img) return NULL; // Échec d'allocation

    // Initialisation des dimensions et profondeur de couleur
    img->width = width;
    img->height = height;
    img->colorDepth = colorDepth;

    // Appel de la fonction dédiée pour allouer le tableau 2D de pixels
    img->data = bmp24_allocateDataPixels(width, height);
    if (!img->data) {
        free(img); // Libère la structure si l’allocation échoue
        return NULL;
    }

    return img; // Retourne l'image allouée
}

// Libère la mémoire d'une structure t_bmp24 ainsi que ses pixels
void bmp24_free(t_bmp24 *img) {
    if (!img) return; // Rien à faire si NULL
    bmp24_freeDataPixels(img->data, img->height); // Libère le tableau 2D de pixels
    free(img); // Libère la structure elle-même
}
// Lit des données brutes dans un fichier à partir d'une position donnée
// position : position dans le fichier où commencer la lecture (en octets)
// buffer : zone mémoire où stocker les données lues
// size : taille en octets de chaque élément à lire
// n : nombre d'éléments à lire
// file : pointeur vers le fichier ouvert en lecture
void file_rawRead(uint32_t position, void *buffer, uint32_t size, size_t n, FILE *file) {
    fseek(file, position, SEEK_SET);  // Se positionner à "position" dans le fichier (début + offset)
    fread(buffer, size, n, file);      // Lire n éléments de taille "size" dans "buffer"
}

// Écrit des données brutes dans un fichier à partir d'une position donnée
// position : position dans le fichier où commencer l'écriture (en octets)
// buffer : zone mémoire contenant les données à écrire
// size : taille en octets de chaque élément à écrire
// n : nombre d'éléments à écrire
// file : pointeur vers le fichier ouvert en écriture
void file_rawWrite(uint32_t position, void *buffer, uint32_t size, size_t n, FILE *file) {
    fseek(file, position, SEEK_SET);  // Se positionner à "position" dans le fichier
    fwrite(buffer, size, n, file);     // Écrire n éléments de taille "size" depuis "buffer"
}

// Lit la valeur d'un pixel (coordonnées x,y) depuis un fichier BMP 24 bits
// image : pointeur vers la structure image BMP 24 bits (contient largeur, hauteur, données, header, etc.)
// x, y : coordonnées du pixel à lire (origine en haut à gauche)
// file : fichier BMP ouvert en lecture
void bmp24_readPixelValue(t_bmp24 *image, int x, int y, FILE *file) {
    uint8_t bgr[3]; // Tableau temporaire pour stocker la couleur BGR lue (BMP stocke en ordre Blue Green Red)

    // Conversion coordonnée y pour lire depuis le bas (BMP stocke les pixels ligne par ligne du bas vers le haut)
    int imageY = image->height - 1 - y;

    // Calcul de l'offset dans le fichier (offset des données + position du pixel dans la matrice)
    uint32_t offset = image->header.offset + (imageY * image->width + x) * 3;

    // Lecture des 3 octets BGR à partir de l'offset calculé
    file_rawRead(offset, bgr, 1, 3, file);

    // Stockage des composantes dans la structure image (en conservant l'ordre RGB dans la structure)
    image->data[y][x].blue  = bgr[0];
    image->data[y][x].green = bgr[1];
    image->data[y][x].red   = bgr[2];
}

// Lit toutes les données pixels d'une image BMP 24 bits depuis un fichier
// image : structure BMP à remplir
// file : fichier BMP ouvert en lecture, positionné au début des données pixels
void bmp24_readPixelData(t_bmp24 *image, FILE *file) {
    int padding = (4 - (image->width * 3) % 4) % 4;

    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            bmp24_readPixelValue(image, x, y, file);  // Appel de ta fonction dédiée
        }
    }
}



// Écrit la valeur d'un pixel (x,y) dans un fichier BMP 24 bits
// image : structure contenant les pixels à écrire
// x, y : coordonnées du pixel à écrire
// file : fichier BMP ouvert en écriture
void bmp24_writePixelValue(t_bmp24 *image, int x, int y, FILE *file) {
    uint8_t bgr[3]; // Tableau temporaire pour stocker la couleur au format BGR

    // Préparation des composantes dans l'ordre BGR (Blue, Green, Red)
    bgr[0] = image->data[y][x].blue;
    bgr[1] = image->data[y][x].green;
    bgr[2] = image->data[y][x].red;

    // Conversion coordonnée y inverse pour écrire depuis le bas du fichier BMP
    int imageY = image->height - 1 - y;

    // Calcul de l'offset dans le fichier
    uint32_t offset = image->header.offset + (imageY * image->width + x) * 3;

    // Écriture des 3 octets BGR à l'offset calculé
    file_rawWrite(offset, bgr, 1, 3, file);
}

// Écrit toutes les données pixels d'une image BMP 24 bits dans un fichier
// image : structure contenant les pixels à écrire
// file : fichier BMP ouvert en écriture
void bmp24_writePixelData(t_bmp24 *image, FILE *file) {
    // Parcours de tous les pixels
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            // Écriture pixel par pixel en appelant la fonction dédiée
            bmp24_writePixelValue(image, x, y, file);
        }
    }
}
// Charge une image BMP 24 bits depuis un fichier
// filename : chemin du fichier BMP à lire
// Retourne un pointeur vers une structure t_bmp24 contenant l'image chargée, ou NULL en cas d'erreur
t_bmp24 *bmp24_loadImage(const char *filename) {
    // 1. Ouverture du fichier en mode lecture binaire
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Erreur : impossible d’ouvrir %s\n", filename);
        return NULL;
    }

    // 2. Lecture des en-têtes BMP
    t_bmp_header header;
    t_bmp_info info;

    file_rawRead(0, &header, sizeof(t_bmp_header), 1, file);
    file_rawRead(sizeof(t_bmp_header), &info, sizeof(t_bmp_info), 1, file);

    // 3. Vérification que l’image est bien en 24 bits
    if (info.bits != 24) {
        printf("Erreur : profondeur de couleur non prise en charge (%d bits).\n", info.bits);
        fclose(file);
        return NULL;
    }

    // 4. Allocation mémoire pour l'image
    t_bmp24 *img = bmp24_allocate(info.width, info.height, info.bits);
    if (!img) {
        fclose(file);
        return NULL;
    }

    // 5. Copie des en-têtes dans la structure image
    img->header = header;
    img->header_info = info;

    // 6. Lecture des données pixels à partir de l’offset indiqué dans le header
    fseek(file, header.offset, SEEK_SET);
    bmp24_readPixelData(img, file);

    // 7. Fermeture du fichier
    fclose(file);

    // 8. Retour de l'image chargée
    return img;
}



// Sauvegarde une image BMP 24 bits dans un fichier
// img : pointeur vers la structure contenant l'image à sauvegarder
// filename : chemin du fichier BMP à écrire
void bmp24_saveImage(t_bmp24 *img, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Erreur : impossible de sauvegarder %s\n", filename);
        return;
    }

    // --- 1. Mise à jour des en-têtes BMP ---
    int padding = (4 - (img->width * 3) % 4) % 4;
    int rowSizeWithPadding = img->width * 3 + padding;
    int pixelArraySize = rowSizeWithPadding * img->height;

    img->header.type = BMP_TYPE; // 'BM' = 0x4D42
    img->header.offset = 54;     // 14 (header) + 40 (info)
    img->header.size = 54 + pixelArraySize;
    img->header.reserved1 = 0;
    img->header.reserved2 = 0;

    img->header_info.size = 40;
    img->header_info.width = img->width;
    img->header_info.height = img->height;
    img->header_info.planes = 1;
    img->header_info.bits = 24;
    img->header_info.compression = 0;
    img->header_info.imagesize = pixelArraySize;
    img->header_info.xresolution = 2835;
    img->header_info.yresolution = 2835;
    img->header_info.ncolors = 0;
    img->header_info.importantcolors = 0;

    // --- 2. Écriture des en-têtes avec file_rawWrite ---
    file_rawWrite(0, &img->header, sizeof(t_bmp_header), 1, file);
    file_rawWrite(sizeof(t_bmp_header), &img->header_info, sizeof(t_bmp_info), 1, file);

    // --- 3. Positionnement au début des pixels (offset 54) ---
    fseek(file, img->header.offset, SEEK_SET);

    // --- 4. Écriture des pixels avec bmp24_writePixelData ---
    bmp24_writePixelData(img, file);

    fclose(file);
}


// Applique un effet négatif sur l'image BMP 24 bits
// Pour chaque pixel, inverse la valeur de chaque composante couleur (R, G, B)
void bmp24_negative(t_bmp24 *img) {
    if (!img || !img->data) {
        printf("Erreur : image non chargee ou invalide.\n");
        return;
    }

    for (int x = 0; x < img->width; x++) {
        for (int y = 0; y < img->height; y++) {
            img->data[x][y].red   = 255 - img->data[x][y].red;
            img->data[x][y].green = 255 - img->data[x][y].green;
            img->data[x][y].blue  = 255 - img->data[x][y].blue;
        }
    }

    printf("Filtre negatif applique avec succes.\n");
}

// Convertit l'image en niveaux de gris
// Pour chaque pixel, calcule la moyenne des composantes R, G et B et met cette moyenne dans chaque composante

void bmp24_grayscale(t_bmp24 *img) {
    if (!img || !img->data) {
        printf("Erreur : image non chargee ou invalide.\n");
        return;
    }

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            uint8_t gray = (img->data[y][x].red + img->data[y][x].green + img->data[y][x].blue) / 3;
            img->data[y][x].red   = gray;
            img->data[y][x].green = gray;
            img->data[y][x].blue  = gray;
        }
    }

    printf("Filtre niveaux de gris applique avec succes !\n");
}

// Modifie la luminosité de l'image en ajoutant 'value' à chaque composante couleur
// value peut être positif (image plus claire) ou négatif (image plus sombre)
void bmp24_brightness(t_bmp24 *img, int value) {
    if (!img || !img->data) {
        printf("Erreur : image non chargee ou invalide.\n");
        return;
    }

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            int r = img->data[y][x].red + value;
            int g = img->data[y][x].green + value;
            int b = img->data[y][x].blue + value;

            img->data[y][x].red   = (r > 255) ? 255 : (r < 0) ? 0 : r;
            img->data[y][x].green = (g > 255) ? 255 : (g < 0) ? 0 : g;
            img->data[y][x].blue  = (b > 255) ? 255 : (b < 0) ? 0 : b;
        }
    }

    printf("Filtre de luminosite applique avec succes !\n");
}

// Applique une convolution sur le pixel (x, y) de l'image img avec un noyau (kernel) carré
// Le noyau est une matrice de coefficients de taille kernelSize x kernelSize
// Retourne un pixel résultant après application du filtre de convolution
t_pixel bmp24_convolution(t_bmp24 *img, int x, int y, float **kernel, int kernelSize) {
    t_pixel result = {0, 0, 0};  // Valeur par défaut si image invalide

    if (img == NULL || img->data == NULL) {
        printf("Erreur : image non chargee.\n");
        return result;
    }

    int offset = kernelSize / 2;
    float r = 0.0f, g = 0.0f, b = 0.0f;

    for (int i = -offset; i <= offset; i++) {
        for (int j = -offset; j <= offset; j++) {
            int px = x + j;
            int py = y + i;

            if (px >= 0 && px < img->width && py >= 0 && py < img->height) {
                t_pixel p = img->data[py][px];
                float coeff = kernel[i + offset][j + offset];
                r += p.red   * coeff;
                g += p.green * coeff;
                b += p.blue  * coeff;
            }
        }
    }

    // Clamp de chaque composante dans [0, 255]
    result.red   = (uint8_t)(r < 0 ? 0 : (r > 255 ? 255 : r));
    result.green = (uint8_t)(g < 0 ? 0 : (g > 255 ? 255 : g));
    result.blue  = (uint8_t)(b < 0 ? 0 : (b > 255 ? 255 : b));

    return result;
}


// Partie 3
unsigned int *bmp8_computeHistogram(t_bmp8 *img) {
    if (!img || !img->data) {
        printf("Erreur : image non chargee.\n");
        return NULL;
    }

    unsigned int *hist = calloc(256, sizeof(unsigned int)); // initialisé à 0
    if (!hist) {
        printf("Erreur : allocation memoire echouee pour l'histogramme.\n");
        return NULL;
    }

    for (unsigned int i = 0; i < img->dataSize; i++) {
        hist[img->data[i]]++;
    }

    return hist;
}

unsigned int *bmp8_computeCDF(unsigned int *hist) {
    if (!hist) return NULL;

    unsigned int *hist_eq = malloc(256 * sizeof(unsigned int));
    if (!hist_eq) {
        printf("Erreur : allocation memoire echouee pour la CDF.\n");
        return NULL;
    }

    unsigned int cdf[256] = {0};
    cdf[0] = hist[0];
    for (int i = 1; i < 256; i++) {
        cdf[i] = cdf[i - 1] + hist[i];
    }

    // Corrigé ici : bloc bien fermé
    unsigned int cdfmin = 0;
    for (int i = 0; i < 256; i++) {
        if (cdf[i] != 0) {
            cdfmin = cdf[i];
            break;
        }
    }

    unsigned int N = cdf[255];

    if (N == cdfmin) {
        for (int i = 0; i < 256; i++) {
            hist_eq[i] = 0;
        }
    } else {
        for (int i = 0; i < 256; i++) {
            hist_eq[i] = (unsigned int)round(((double)(cdf[i] - cdfmin) / (N - cdfmin)) * 255);
        }
    }

    return hist_eq;
}

void bmp8_equalize(t_bmp8 *img, unsigned int *hist_eq) {
    if (!img || !img->data || !hist_eq) {
        printf("Erreur : image ou histogramme non valides.\n");
        return;
    }

    for (unsigned int i = 0; i < img->dataSize; i++) {
        img->data[i] = (unsigned char)hist_eq[img->data[i]];
    }

    printf("Egalisation d'histogramme appliquee avec succes !\n");
}
#include <math.h>

// Conversion RGB → YUV
void rgb_to_yuv(uint8_t r, uint8_t g, uint8_t b, uint8_t *y, uint8_t *u, uint8_t *v) {
    // Coefficients standard BT.601
    float R = r / 255.0f;
    float G = g / 255.0f;
    float B = b / 255.0f;

    float Yf = 0.299f * R + 0.587f * G + 0.114f * B;
    float Uf = (B - Yf) * 0.565f;
    float Vf = (R - Yf) * 0.713f;

    *y = (uint8_t)(Yf * 255.0f);
    *u = (uint8_t)(Uf * 255.0f + 128);
    *v = (uint8_t)(Vf * 255.0f + 128);
}


// Conversion YUV → RGB
void yuv_to_rgb(uint8_t y, uint8_t u, uint8_t v, uint8_t *r, uint8_t *g, uint8_t *b) {
    float Y = y;
    float U = u - 128;
    float V = v - 128;

    int r_ = round(Y + 1.13983 * V);
    int g_ = round(Y - 0.39465 * U - 0.58060 * V);
    int b_ = round(Y + 2.03211 * U);

    *r = (uint8_t)(r_ < 0 ? 0 : (r_ > 255 ? 255 : r_));
    *g = (uint8_t)(g_ < 0 ? 0 : (g_ > 255 ? 255 : g_));
    *b = (uint8_t)(b_ < 0 ? 0 : (b_ > 255 ? 255 : b_));
}
// Fonction d'égalisation d'histogramme sur une image BMP 24 bits
// L'égalisation est effectuée sur la composante de luminance Y du modèle de couleur YUV
// L'image est convertie RGB → YUV, égalisation sur Y, puis reconvertie YUV → RGB

void bmp24_equalize(t_bmp24 *img) {
    int width = img->width;
    int height = img->height;
    int size = width * height;  // Nombre total de pixels

    unsigned int histogram[256] = {0};  // Histogramme des valeurs Y (luminance)
    unsigned int cdf[256] = {0};        // Fonction de distribution cumulée (CDF)
    unsigned int cdf_min = 0;            // Valeur minimale non nulle de la CDF
    uint8_t *Y_values = malloc(size);   // Tableau pour stocker les valeurs Y de chaque pixel
    uint8_t *U = malloc(size);          // Tableau pour stocker les valeurs U (chrominance)
    uint8_t *V = malloc(size);          // Tableau pour stocker les valeurs V (chrominance)

    // Vérification d'allocation mémoire
    if (!Y_values || !U || !V) {
        printf("Erreur d'allocation mémoire pour égalisation couleur.\n");
        return;
    }

    // Étape 1 : Conversion de chaque pixel RGB → YUV et construction de l'histogramme des Y
    int index = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            t_pixel p = img->data[i][j];      // Récupérer pixel courant
            uint8_t y, u, v;
            rgb_to_yuv(p.red, p.green, p.blue, &y, &u, &v); // Conversion RGB → YUV
            Y_values[index] = y;              // Stocker Y
            U[index] = u;                    // Stocker U
            V[index] = v;                    // Stocker V
            histogram[y]++;                  // Incrémenter l'histogramme pour la valeur Y
            index++;
        }
    }

    // Étape 2 : Calcul de la fonction de distribution cumulée (CDF)
    unsigned int totalPixels = size;
    for (int i = 0; i < 256; i++) {
        cdf[i] = (i == 0) ? histogram[i] : cdf[i - 1] + histogram[i];  // Cumul des valeurs histogramme
        if (cdf_min == 0 && cdf[i] > 0)  // Trouver la première valeur non nulle de la CDF
            cdf_min = cdf[i];
    }

    // Étape 3 : Normalisation de la CDF pour obtenir la fonction d'égalisation (mapping histogramme)
    uint8_t hist_eq[256];
    for (int i = 0; i < 256; i++) {
        // Application de la formule d'égalisation d'histogramme classique
        hist_eq[i] = (uint8_t)round(((float)(cdf[i] - cdf_min) / (totalPixels - cdf_min)) * 255);
    }

    // Étape 4 : Appliquer la transformation d'égalisation sur chaque pixel (Y) puis reconvertir en RGB
    index = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            uint8_t y_eq = hist_eq[Y_values[index]];  // Valeur Y égalisée
            uint8_t r, g, b;
            yuv_to_rgb(y_eq, U[index], V[index], &r, &g, &b); // Reconversion YUV → RGB avec Y égalisé
            img->data[i][j].red = r;            // Mise à jour du pixel dans l'image
            img->data[i][j].green = g;
            img->data[i][j].blue = b;
            index++;
        }
    }

    // Libération de la mémoire allouée pour les composantes Y, U et V
    free(Y_values);
    free(U);
    free(V);
}
