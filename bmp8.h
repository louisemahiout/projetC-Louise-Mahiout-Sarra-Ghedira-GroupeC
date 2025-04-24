//
// Created by louis on 21/04/2025.
//

#ifndef BMP8_H
#define BMP8_H


typedef struct {
    unsigned char header[54]; //représente l’en-tête du fichier BMP. Cet en-tête est composé de 54 octets.
    unsigned char colorTable[1024]; //représente la table de couleur de l’image. Cette table est composée de 1024 octets.
    unsigned char * data; //représente les données de l’image. Ces données sont stockées sous forme de tableau d’octets.
    unsigned int width; //représente la largeur de l’image en pixels (Située à l’offset 18 de l’en-tête).
    unsigned int height; //représente la hauteur de l’image en pixels (Située à l’offset 22 de l’en-tête).
    unsigned int colorDepth; //représente la profondeur de couleur de l’image. Pour une image en niveaux de gris, cette valeur est égale à 8 (Située à l’offset 28 de l’en-tête).
    unsigned int dataSize; //représente la taille des données de l’image en octets (Située à l’offset 34 de l’en-tête).
} t_bmp8;

t_bmp8 *bmp8_loadImage(const char *filename);
void bmp8_saveImage(const char *filename, t_bmp8 *img);
void bmp8_printInfo(t_bmp8 *img);
void bmp8_free(t_bmp8 *img);
void bmp8_negative(t_bmp8 * img);
void bmp8_brightness(t_bmp8 * img, int value);
void bmp8_threshold(t_bmp8 * img, int threshold);
void bmp8_applyFilter(t_bmp8 *img, float **kernel, int kernelSize);

#endif //BMP8_H
