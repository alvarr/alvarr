//////////////////////////////////////////////////////////////////
// Álvaro Aguilar Sabuco                                        //
// Computación Paralela, Práctica 0, Tarea 2                    //
// Fecha: 26/09/2021                                            //
//////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

int main(int *arc, char *argv[]){
    char opcion = 'r';

    do{
        printf("¿Que filtrado quieres realizar?:\n");
        printf("1.Filtrado por media:\n");
        printf("2.Filtrado por mediana\n");
        printf("3.Deteccion de bordes SOBEL\n");
        printf("4.Salir\n");
        printf("Opcion:");
        fflush( stdin );
        scanf("%c", &opcion);
        switch(opcion){

            case '1': printf("Has elegido el filtrado por mediana\n");
                break;
            case '2': printf("Has elegido el filtrado por mediana\n");
                break;
            case '3': printf("Has elegido el filtrado por mediana\n");
                break;
            case '4': printf("Has elegido el filtrado por mediana\n");
                break;
            default: printf("Introduce opcion valida\n");
        }
    }while(opcion != '4');
    return 0;

}
