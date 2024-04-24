#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

#define PATH_MAX 256

void encontrar_archivo(const char *pRutaOrigen, const char *pNombreArchivo) {
    FILE *archivo = fopen(pRutaOrigen, "r");
    if (!archivo) {
        perror("Error al abrir el archivo de origen");
        exit(EXIT_FAILURE);
    };
    
    // archivos con mismo nombre
    if (strcmp(archivo,pNombreArchivo) == 0){
    	printf("Archivo encontrado en %s \n");
    };

    fclose(archivo);
    
};

void avanzar_directorio(const char *pRutaOrigen, const char *pNombreArchivo) {

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
            avanzar_directorio(rutaOrigenCompleta, pNombreArchivo);
        } else if (S_ISREG(info.st_mode)) {
            encontrar_archivo(rutaOrigenCompleta, pNombreArchivo);
        };
    };

    closedir(dir);
}


int main(int argc, char *argv[]) {

    if (argc != 3) {
        fprintf(stderr, "Uso: %s directorio nombre_archivo \n", argv[0]);
        exit(EXIT_FAILURE);
    };
    // arg1 = directorio, arg2 = nombrea archivo
    avanzar_directorio(argv[1],argv[2]);

    return 0;
}
