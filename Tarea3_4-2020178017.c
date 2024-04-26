#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define MAXEMPLEADOS 10
#define TIEMPO_ESPERA 2 // Segundos
#define ARCHIVO "empleados2.txt"

typedef struct {
  char nombre[40];
  int edad;
  float salario;
} Empleado;

pthread_mutex_t mutex; // Mutex para proteger el acceso al archivo

void* guardarEmpleados(void *arg) {
	FILE *archivo;
	Empleado empl;
	// Abrir el archivo en modo escritura
	archivo = fopen(ARCHIVO, "wb");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        exit(1);
    };
    
    // Generar los datos de los 10 empleados de manera recursiva y guardarlos en el archivo utilizando un mutex como bloqueo
    for (int i = 0; i < MAXEMPLEADOS; i++) {
        char numero[3];
        sprintf(numero, "%d", i + 1);
        strcpy(empl.nombre, "Empleado ");
        // reiniciar/reciclar el nombre del empleado
        strcpy(empl.nombre, "");
        strcat(empl.nombre, numero);
        empl.edad = i+1;
        empl.salario = 1100.1*(i+1);

        // Bloquear el acceso al archivo antes de escribir
        pthread_mutex_lock(&mutex);

        // Escribir la información del empleado en el archivo binario
        fwrite(&empl, sizeof(Empleado), 1, archivo);

        // Desbloquear el acceso al archivo
        pthread_mutex_unlock(&mutex);

        // Esperar 2 segundos
        sleep(TIEMPO_ESPERA);
    };

    fclose(archivo);
    return NULL;
};

void *leerArchivo(void *arg) {
	FILE *archivo;
	Empleado empl;
    int i = 1;
    for (int i = 0; i < MAXEMPLEADOS; i++) {
        // Abrir el archivo en modo lectura binaria ("rb")
        archivo = fopen(ARCHIVO, "rb");
        if (archivo == NULL) {
            perror("Error al abrir el archivo");
            exit(1);
        };

        // Rebobinar el archivo al inicio
        fseek(archivo, 0, SEEK_SET);

        // Bloquear el acceso al archivo antes de leer
        pthread_mutex_lock(&mutex);

        // Leer la información de los empleados del archivo binario
        printf("\n____________________________________________\n Iteración %d \n", i);
        while (fread(&empl, sizeof(Empleado), 1, archivo) != 0) {
            printf("Nombre: %s - Edad: %d - Salario: %.2f\n", empl.nombre, empl.edad, empl.salario);
        };

        // Desbloquear el acceso al archivo
        pthread_mutex_unlock(&mutex);

        // Cerrar el archivo
        fclose(archivo);

        // Esperar 2 segundos
        sleep(TIEMPO_ESPERA);
    };

    return NULL;
};

int main() {
    pthread_t hilo_escribir, hilo_leer;

    // Inicializar el mutex
    pthread_mutex_init(&mutex, NULL);

    // Crear el hilo para llenar el archivo
    pthread_create(&hilo_escribir, NULL, guardarEmpleados, NULL);
    // Crear el hilo para leer el archivo
    pthread_create(&hilo_leer, NULL, leerArchivo, NULL);
    
    // Esperar a que ambos hilos terminen
    pthread_join(hilo_escribir, NULL);
    pthread_join(hilo_leer, NULL);

    // Destruir el mutex
    pthread_mutex_destroy(&mutex);

    return 0;
};