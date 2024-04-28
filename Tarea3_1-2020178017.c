#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

#define MAX_PATH 4096

int buscar_archivo(const char *pRutaOrigen, const char *pNomArch) {
    DIR *dir;
    struct dirent *entrada;
    struct stat info_archivo;
    char ruta_completa[MAX_PATH];

    dir = opendir(pRutaOrigen);
    if (dir == NULL) {
        fprintf(stderr, "Error al abrir el directorio %s: %s\n", pRutaOrigen, strerror(errno));
        return 0;
    };

    while ((entrada = readdir(dir)) != NULL) {
        if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0) {
            continue;
        };

        snprintf(ruta_completa, MAX_PATH, "%s/%s", pRutaOrigen, entrada->d_name);

        if (lstat(ruta_completa, &info_archivo) == -1) {
            fprintf(stderr, "Error al obtener información del archivo %s: %s\n", ruta_completa, strerror(errno));
            continue;
        };

        if (S_ISDIR(info_archivo.st_mode)) {
            if (buscar_archivo(ruta_completa, pNomArch)==1){
                close(dir);
                return 1;
            };
        }
        else if (S_ISREG(info_archivo.st_mode) && strcmp(entrada->d_name, pNomArch) == 0) {
            printf("Archivo encontrado en: %s\n", ruta_completa);
            closedir(dir);
            return 1;
        };
    };

    closedir(dir);
    return 0;
};

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s directorio pNomArch\n", argv[0]);
        return 0;
    };

    // arg[1] es el directorio, arg[2] el nombre del archivo
    // return fue 0, no fue encontrado
    if (!buscar_archivo(argv[1], argv[2])) {
        printf("Archivo no encontrado en todo el directorio ni subdirectorios\n");
    };

    return 0;
};