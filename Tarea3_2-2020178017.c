#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

#define PATH_MAX 256

void imprimirArchivo(char *nombre, struct stat pInfoArch, int pNivel) {
	for (int i = 0; i < pNivel; i++) printf("    ");
    printf("Archivo: %s, %ld bytes, Creado: %s \n", nombre, pInfoArch.st_size,  ctime(&pInfoArch.st_ctime));
};

void avanzar_directorio(const char *pRutaOrigen, int pNivel) {

    DIR *dir = opendir(pRutaOrigen);
    if (!dir) {
        perror("Error al abrir el directorio de origen");
        exit(EXIT_FAILURE);
    };

    struct dirent *dp;
    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
            continue;
        }

        char rutaOrigenCompleta[PATH_MAX];
        snprintf(rutaOrigenCompleta, PATH_MAX, "%s/%s", pRutaOrigen, dp->d_name);

        struct stat info;
        if (stat(rutaOrigenCompleta, &info) == -1) {
            perror("Error al obtener información del archivo/directorio de origen");
            exit(EXIT_FAILURE);
        }

        if (S_ISDIR(info.st_mode)) {
            for (int i = 0; i < pNivel; i++) printf("    ");
            printf("%s \n", dp->d_name);
            avanzar_directorio(rutaOrigenCompleta, pNivel+1);
        } else if (S_ISREG(info.st_mode)) {
            imprimirArchivo(rutaOrigenCompleta, info, pNivel+1);
        };
    };

    closedir(dir);
};

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Uso: %s directorio\n", argv[0]);
        exit(EXIT_FAILURE);
    };
    // arg1 = directorio
    avanzar_directorio(argv[1], 0);

    return 0;
};
