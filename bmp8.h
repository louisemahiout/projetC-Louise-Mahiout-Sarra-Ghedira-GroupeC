//
// Created by louis on 21/04/2025.
//
#include<stdint.h>
#include <stdio.h>
#ifndef BMP8_H
#define BMP8_H
// Constantes pour les offsets des champs de l'en-tête BMP
#define BITMAP_MAGIC 0x00
#define BITMAP_SIZE 0x02
#define BITMAP_OFFSET 0x0A
#define BITMAP_WIDTH 0x12
#define BITMAP_HEIGHT 0x16
#define BITMAP_DEPTH 0x1C
#define BITMAP_SIZE_RAW 0x22

#define BMP_TYPE 0x4D42 // 'BM'
#define HEADER_SIZE 0x0E
#define INFO_SIZE 0x28
#define DEFAULT_DEPTH 0x18

#pragma pack(push, 1)
typedef struct {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
} t_bmp_header;

typedef struct {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bits;
    uint32_t compression;
    uint32_t imagesize;
    int32_t xresolution;
    int32_t yresolution;
    uint32_t ncolors;
    uint32_t importantcolors;
} t_bmp_info;
#pragma pack(pop)

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} t_pixel;

typedef struct {
    t_bmp_header header;
    t_bmp_info header_info;
    int width;
    int height;
    int colorDepth;
    t_pixel **data;
} t_bmp24;

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
void bmp8_free(t_bmp8 **img);
void bmp8_negative(t_bmp8 * img);
void bmp8_brightness(t_bmp8 * img, int value);
void bmp8_threshold(t_bmp8 * img, int threshold);
void bmp8_applyFilter(t_bmp8 *img, float **kernel, int kernelSize);
t_pixel **bmp24_allocateDataPixels(int width, int height);
void bmp24_freeDataPixels(t_pixel **pixels, int height);
t_bmp24 *bmp24_allocate(int width, int height, int colorDepth);
void bmp24_free(t_bmp24 *img);
void file_rawRead(uint32_t position, void *buffer, uint32_t size, size_t n, FILE *file);
void file_rawWrite(uint32_t position, void *buffer, uint32_t size, size_t n, FILE *file);
t_bmp24 *bmp24_loadImage(const char *filename);
void bmp24_saveImage(t_bmp24 *img, const char *filename);
void bmp24_readPixelValue(t_bmp24 *image, int x, int y, FILE *file);
void bmp24_readPixelData(t_bmp24 *image, FILE *file);
void bmp24_writePixelValue(t_bmp24 *image, int x, int y, FILE *file);
void bmp24_writePixelData(t_bmp24 *image, FILE *file);
void bmp24_negative(t_bmp24 *img);
void bmp24_grayscale(t_bmp24 *img);
void bmp24_brightness(t_bmp24 *img, int value);


#endif //BMP8_H
