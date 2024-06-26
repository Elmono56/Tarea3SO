#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXEMPLEADOS 10

typedef struct {
	char nombre[40];
	int edad;
	float salario;
} Empleado;

void guardarEmpleados(const char *pNombreArch) {
	FILE *archivo;
	Empleado array_empleados[MAXEMPLEADOS];
	// Abrir el archivo en modo escritura
	archivo = fopen(pNombreArch, "wb");

	// Generar los datos de los 10 empleados de manera recursiva y guardarlos en el archivo
	for(int i = 0; i < MAXEMPLEADOS; i++) {
        char numero[3];
        sprintf(numero, "%d", i + 1);
        strcpy(array_empleados[i].nombre, "Empleado ");
        strcat(array_empleados[i].nombre, numero);
        array_empleados[i].edad = i+1;
        array_empleados[i].salario = 1100.1*(i+1);
		
        // escribir datos del empleado en archivo
		fwrite(&array_empleados[i], sizeof(Empleado), 1, archivo);
	};
	// Cerrar el archivo
	fclose(archivo);
};

void printEmplImpares(const char *pNombreArch) {
	FILE *archivo;
	Empleado empleado;
	// Abrir el archivo en modo lectura
	archivo = fopen(pNombreArch, "rb");

	// Recorrer las posiciones impares del archivo usando fseek
    // empieza en 1 ya que el indice 0 sería el primer empleado y el ind 1 el segundo
	for(int i = 1; i < MAXEMPLEADOS; i += 2) {
		fseek(archivo, i * sizeof(Empleado), SEEK_SET);
		fread(&empleado, sizeof(Empleado), 1, archivo);
		
		printf("Nombre: %s\n", empleado.nombre);
		printf("Edad: %d\n", empleado.edad);
		printf("Salario: %.2f\n\n", empleado.salario);
	};
	// Cerrar el archivo
	fclose(archivo);
};

int main() {
	const char *nombreArchivo = "empleados.txt";

	// Guardar los datos de los empleados en el archivo
	guardarEmpleados(nombreArchivo);
	// Imprimir los datos de los empleados en posiciones impares
	printEmplImpares(nombreArchivo);

	return 0;
};