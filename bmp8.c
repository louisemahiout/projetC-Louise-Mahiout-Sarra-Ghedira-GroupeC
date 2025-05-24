
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
    printf("Taille des données: %u\n", img->dataSize);
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
t_pixel **bmp24_allocateDataPixels(int width, int height) {
    t_pixel **pixels = malloc(height * sizeof(t_pixel *));
    if (!pixels) {
        fprintf(stderr, "Erreur d'allocation de lignes de pixels.\n");
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        pixels[i] = malloc(width * sizeof(t_pixel));
        if (!pixels[i]) {
            fprintf(stderr, "Erreur d'allocation de pixels pour la ligne %d.\n", i);
            // Libérer les lignes déjà allouées
            for (int j = 0; j < i; j++) {
                free(pixels[j]);
            }
            free(pixels);
            return NULL;
        }
    }

    return pixels;
}

void bmp24_freeDataPixels(t_pixel **pixels, int height) {
    if (!pixels) return;
    for (int i = 0; i < height; i++) {
        free(pixels[i]);
    }
    free(pixels);
}

t_bmp24 *bmp24_allocate(int width, int height, int colorDepth) {
    t_bmp24 *img = malloc(sizeof(t_bmp24));
    if (!img) return NULL;

    img->width = width;
    img->height = height;
    img->colorDepth = colorDepth;

    img->data = malloc(height * sizeof(t_pixel *));
    for (int i = 0; i < height; i++) {
        img->data[i] = malloc(width * sizeof(t_pixel));
    }

    return img;
}

void bmp24_free(t_bmp24 *img) {
    if (!img) return;
    bmp24_freeDataPixels(img->data, img->height);
    free(img);
}
void file_rawRead(uint32_t position, void *buffer, uint32_t size, size_t n, FILE *file) {
    fseek(file, position, SEEK_SET);
    fread(buffer, size, n, file);
}

void file_rawWrite(uint32_t position, void *buffer, uint32_t size, size_t n, FILE *file) {
    fseek(file, position, SEEK_SET);
    fwrite(buffer, size, n, file);
}
void bmp24_readPixelValue(t_bmp24 *image, int x, int y, FILE *file) {
    uint8_t bgr[3];
    int imageY = image->height - 1 - y;  // Inverser la ligne (bas -> haut)
    uint32_t offset = image->header.offset + (imageY * image->width + x) * 3;

    file_rawRead(offset, bgr, 1, 3, file);
    image->data[y][x].blue  = bgr[0];
    image->data[y][x].green = bgr[1];
    image->data[y][x].red   = bgr[2];
}

void bmp24_readPixelData(t_bmp24 *image, FILE *file) {
    int padding = (4 - (image->width * 3) % 4) % 4;

    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            fread(&image->data[image->height - i - 1][j], sizeof(t_pixel), 1, file);
        }
        fseek(file, padding, SEEK_CUR); // Ignorer les octets de padding
    }
}

void bmp24_writePixelValue(t_bmp24 *image, int x, int y, FILE *file) {
    uint8_t bgr[3];
    bgr[0] = image->data[y][x].blue;
    bgr[1] = image->data[y][x].green;
    bgr[2] = image->data[y][x].red;

    int imageY = image->height - 1 - y;
    uint32_t offset = image->header.offset + (imageY * image->width + x) * 3;

    file_rawWrite(offset, bgr, 1, 3, file);
}

void bmp24_writePixelData(t_bmp24 *image, FILE *file) {
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            bmp24_writePixelValue(image, x, y, file);
        }
    }
}

t_bmp24 *bmp24_loadImage(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Erreur : impossible d’ouvrir %s\n", filename);
        return NULL;
    }

    t_bmp_header header;
    t_bmp_info info;

    fread(&header, sizeof(t_bmp_header), 1, file);
    fread(&info, sizeof(t_bmp_info), 1, file);

    if (info.bits != 24) {
        printf("Erreur : profondeur de couleur non prise en charge (%d bits).\n", info.bits);
        fclose(file);
        return NULL;
    }

    t_bmp24 *img = bmp24_allocate(info.width, info.height, info.bits);
    if (!img) {
        fclose(file);
        return NULL;
    }

    img->header = header;
    img->header_info = info;

    int padding = (4 - (info.width * 3) % 4) % 4;

    fseek(file, header.offset, SEEK_SET); // Très important : aller au début des données pixel

    for (int y = info.height - 1; y >= 0; y--) {
        for (int x = 0; x < info.width; x++) {
            uint8_t bgr[3];
            fread(bgr, 1, 3, file);
            img->data[y][x].blue  = bgr[0];
            img->data[y][x].green = bgr[1];
            img->data[y][x].red   = bgr[2];
        }
        fseek(file, padding, SEEK_CUR); // ignorer le padding
    }

    fclose(file);
    return img;
}


void bmp24_saveImage(t_bmp24 *img, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Erreur : impossible de sauvegarder %s\n", filename);
        return;
    }

    int padding = (4 - (img->width * 3) % 4) % 4;
    int rowSizeWithPadding = img->width * 3 + padding;
    int pixelArraySize = rowSizeWithPadding * img->height;

    // Mise à jour des champs de l'en-tête
    img->header.type = BMP_TYPE;          // "BM"
    img->header.offset = 54;
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

    // Écriture des headers
    fwrite(&img->header, sizeof(t_bmp_header), 1, file);
    fwrite(&img->header_info, sizeof(t_bmp_info), 1, file);

    // Écriture des pixels de bas en haut
    uint8_t pad[3] = {0, 0, 0};
    for (int y = img->height - 1; y >= 0; y--) {
        for (int x = 0; x < img->width; x++) {
            uint8_t bgr[3] = {
                img->data[y][x].blue,
                img->data[y][x].green,
                img->data[y][x].red
            };
            fwrite(bgr, 1, 3, file);
        }
        fwrite(pad, 1, padding, file);
    }

    fclose(file);
}

void bmp24_negative(t_bmp24 *img) {
    printf("width : %d  height : %d \n", img->width, img->height);
    for (int x = 0; x < img->width; x++) {
        for (int y = 0; y < img->height; y++) {
            img->data[x][y].red   = 255 - img->data[x][y].red;
            img->data[x][y].green = 255 - img->data[x][y].green;
            img->data[x][y].blue  = 255 - img->data[x][y].blue;
        }
    }
}
void bmp24_grayscale(t_bmp24 *img) {
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            uint8_t gray = (img->data[y][x].red + img->data[y][x].green + img->data[y][x].blue) / 3;
            img->data[y][x].red = gray;
            img->data[y][x].green = gray;
            img->data[y][x].blue = gray;
        }
    }
}
void bmp24_brightness(t_bmp24 *img, int value) {
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
}
void bmp24_applyFilter(t_bmp24 *img, float **kernel, int kernelSize) {
    if (!img || !img->data) {
        printf("Erreur : image couleur non chargee.\n");
        return;
    }

    int width = img->width;
    int height = img->height;
    int offset = kernelSize / 2;

    // Allocation des matrices temporaires pour chaque canal
    uint8_t **red = bmp24_allocateDataPixels(width, height);
    uint8_t **green = bmp24_allocateDataPixels(width, height);
    uint8_t **blue = bmp24_allocateDataPixels(width, height);

    if (!red || !green || !blue) {
        printf("Erreur d'allocation memoire pour les buffers de filtre.\n");
        return;
    }

    // Appliquer le filtre (ignorer les bords)
    for (int y = offset; y < height - offset; y++) {
        for (int x = offset; x < width - offset; x++) {
            float r = 0.0f, g = 0.0f, b = 0.0f;

            for (int i = -offset; i <= offset; i++) {
                for (int j = -offset; j <= offset; j++) {
                    int yy = y + i;
                    int xx = x + j;

                    float coeff = kernel[i + offset][j + offset];
                    r += img->data[yy][xx].red * coeff;
                    g += img->data[yy][xx].green * coeff;
                    b += img->data[yy][xx].blue * coeff;
                }
            }

            // Clamp dans [0,255]
            red[y][x] = (uint8_t)(fminf(fmaxf(r, 0), 255));
            green[y][x] = (uint8_t)(fminf(fmaxf(g, 0), 255));
            blue[y][x] = (uint8_t)(fminf(fmaxf(b, 0), 255));
        }
    }

    // Copier les résultats dans l'image
    for (int y = offset; y < height - offset; y++) {
        for (int x = offset; x < width - offset; x++) {
            img->data[y][x].red = red[y][x];
            img->data[y][x].green = green[y][x];
            img->data[y][x].blue = blue[y][x];
        }
    }

    // Libération de la mémoire
    for (int i = 0; i < height; i++) {
        free(red[i]);
        free(green[i]);
        free(blue[i]);
    }
    free(red);
    free(green);
    free(blue);

    printf("Filtre couleur de convolution applique avec succes !\n");
}
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

