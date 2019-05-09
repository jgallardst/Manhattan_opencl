#include <stdio.h>
#include <stdlib.h>
#include "include/LeerFuente.h"

char leerFuentes(char *&src, const char *fileName)
{
    FILE *file = fopen(fileName, "rb");
    if (!file) {
        printf("Error al abrir el fichero '%s'\n", fileName);
        return 0;
    }

	if (fseek(file, 0, SEEK_END))  {
		printf("Error de posicionamiento en el '%s'\n", fileName);
		fclose(file);
		return 0;
    }

    long size = ftell(file);
    if (size == 0)  {
        printf("Error al calcular el tama�o del fichero '%s'\n", fileName);
        fclose(file);
        return 0;
    }

	rewind(file);
    src = (char *)malloc(sizeof(char) * size + 1);
    size_t res = fread(src, 1, sizeof(char) * size, file);
    if (res != sizeof(char) * size)   {
        printf("Error de lectura del fichero '%s'\n", fileName);
        fclose(file);
        free(src);
        return 0;
	}
    src[size] = '\0';
    fclose(file);
	return 1;
}