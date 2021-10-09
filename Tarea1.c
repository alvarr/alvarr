//////////////////////////////////////////////////////////////////
// Alvaro Aguilar Sabuco                                        //
// Computacion Paralela, Practica 0, Tarea 1                    //
// Fecha: 26/09/2021                                            //
//////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#define ELEMENTOS 9

void leerArchivo(char *nombre_fichero,int tamanyo, unsigned char **matriz){
    FILE *f1;
    f1 = fopen(nombre_fichero,"rb");
    if(f1){
        //Lectura del fichero
        for(int i = 0; i < tamanyo; i++){
            for(int j = 0; j < tamanyo; j++){
                //Leemos caracter por caracter
                matriz[i][j] = getc(f1);
            }
        }
        fclose(f1);
    }else{
        printf("Error al abrir el fichero");
    }
}

void guardarImagenSalida(unsigned char **matrizSalida,char *imagen_salida, int tamanyo){
    FILE *f1;
    f1 = fopen(imagen_salida, "wb");
    for(int i = 0;i<tamanyo; i++){
        for(int j = 0;j<tamanyo;j++){
            fprintf(f1,"%c", matrizSalida[i][j]);
        }
    }
    fclose(f1);
}

void ordenarVector(unsigned char elementos[]){
    unsigned char elemento, array_auxiliar[9], elemento_auxiliar;
    //qsort(elementos, tamanyo, sizeof(unsigned char));

    for(int i = 0;i<(ELEMENTOS-1);i++){
       
        for (int j = i+1; j < ELEMENTOS; j++){
           // printf("ordenar %d %d\n", i,j);
            if(elementos[i] > elementos[j]){
                
                elemento_auxiliar = elementos[j];
                elementos[j] = elementos[i];
                elementos[i] = elemento_auxiliar;
            }
        }
        
    }

}

void filtradoMedia(unsigned char **matriz, char *imagen_salida,int tamanyo){
    printf("Filtrado por Media");

    unsigned char **matrizSalida, elementoSeleccionado, elementos[9], media;
    media = 0;
    matrizSalida = (unsigned char **)malloc(tamanyo*sizeof(unsigned char *));
    for(int i = 0; i<tamanyo;i++){
        matrizSalida[i] = (unsigned char *)malloc(tamanyo*sizeof(unsigned char));
    }

    for(int i = 0;i<tamanyo;i++){
        for(int j = 0;j<tamanyo;j++){
            elementoSeleccionado = matriz[i][j];
            //printf("%c\n",elementoSeleccionado);
            if(i == 0 || j == 0 || i == (tamanyo-1) || j == (tamanyo-1)){
                matrizSalida[i][j] == matriz[i][j];
            }else{
                media = matriz[i-1][j-1] +matriz[i][j-1]+ matriz[i+1][j-1] +matriz[i-1][j] +matriz[i][j] +matriz[i+1][j] +matriz[i-1][j+1] + matriz[i][j+1] +matriz[i+1][j+1] ;
                media = (unsigned char)fabs(media/9);
                matrizSalida[i][j] = media;
            }
        }
        guardarImagenSalida(matrizSalida,imagen_salida,tamanyo);
    }
}

void filtradoMediana(unsigned char **matriz, char *imagen_salida, int tamanyo){
    
    unsigned char **matrizSalida, elementoSeleccionado, elementos[9];
    matrizSalida = (unsigned char **)malloc(tamanyo*sizeof(unsigned char *));
    for(int i = 0; i<tamanyo;i++){
        matrizSalida[i] = (unsigned char *)malloc(tamanyo*sizeof(unsigned char));
    }

    for(int i = 0;i<tamanyo;i++){
        for(int j = 0;j<tamanyo;j++){
            elementoSeleccionado = matriz[i][j];

            if(i == 0 || j == 0 || i == (tamanyo-1) || j == (tamanyo-1)){
                matrizSalida[i][j] ==matriz[i][j];
            }else{
                elementos[0] =  matriz[i-1][j-1];
                elementos[1] = matriz[i][j-1];
                elementos[2] = matriz[i+1][j-1];
                elementos[3] = +matriz[i-1][j] ;
                elementos[4] = matriz[i][j] ;
                elementos[5] = matriz[i+1][j] ;
                elementos[6] = matriz[i-1][j+1];
                elementos[7] = matriz[i][j+1];
                elementos[8] = matriz[i+1][j+1];
                ordenarVector(elementos);
                
                matriz[i][j] = elementos[4];
            }
        }
    }
    printf("Filtrado por Mediana");
    guardarImagenSalida(matrizSalida,imagen_salida,tamanyo);

}

void deteccionBordes(unsigned char **matriz, char *imagenSalida, int tamanyo){
    printf("Filtrado SOBEL");

}

//Guarda en un fichero los parámetros de ejecución, fichero de entrada, fichero de salida, filtrado escogido y tiempo de ejecucion
void ficheroSalida(char *imagen_entrada , char *imagen_salida, char *fichero_salida, char *tipo_proceso, double t_ejecucion){
    FILE *f1;
	
	f1 = fopen(fichero_salida,"w");
    if(f1==NULL){
        printf("No se puede abrir el fichero");
    }
	
	fprintf(f1,"Fichero de entrada: %s\n", imagen_entrada);
	fprintf(f1,"Fichero de salida: %s\n", imagen_salida);
    fprintf(f1,"Tipo de proceso: %s\n",  tipo_proceso);
	fprintf(f1,"Tiempo de ejecucion: %f\n",  t_ejecucion);

}

int main(int argc, char *argv[]){
    clock_t t_inicio, t_fin;
    double t_ejecucion;
    t_inicio = clock();
    
    //Indicamos un parámetro de entrada que explique que información hay que pasar como parametro
    if(argc == 2 && strcmp("HELP",argv[1])==0){
        printf("Debes indicar el nombre de la imagen de entrada ,imange de salida, fichero de resultados de salida, y el tipo de proceso(MEDIA, MEDIANA O SOBEL). Por ejemplo './Tarea1 fotoEntrada fotoSalida resultadosSalida MEDIA'\n");
        return 0;
    }
        
    if(argc != 5){
        printf("Debes indicar varios paramentros. Para mas ayuda pasa como parametro HELP\n");
        return 0;
    }

    struct stat sb;
    char *imagen_entrada,*imagen_salida, *tipo_proceso, *fichero_salida;
    int tamanyo_matriz;
    char opcion;
    unsigned char **matriz;

    //Como el tamanyo de un caracter unsigned char es de 1 byte leemos directamente 
    //el tamanyo del fichero y una vez que tenemos el número de bytes hacemos la raiz cuadrada
    //para saber la altura y la anchura de la matriz que es la misma cantidad
    imagen_entrada = argv[1];
    imagen_salida = argv[2];
    fichero_salida = argv[3];
    tipo_proceso = argv[4];
    if (stat(imagen_entrada, &sb) == -1) {
        return 0;
    }
    tamanyo_matriz = sqrt(sb.st_size);

    matriz = (unsigned char **)malloc(tamanyo_matriz*sizeof(unsigned char *));
    for(int i = 0;i < tamanyo_matriz; i++){
        matriz[i] = (unsigned char *)malloc(tamanyo_matriz*sizeof(unsigned char));
    }
    //Lectura de matriz
    leerArchivo(imagen_entrada,tamanyo_matriz,matriz);

    if(strcmp("MEDIA",tipo_proceso)==0){
        filtradoMedia(matriz,imagen_salida,tamanyo_matriz);
        
    }else if (strcmp("MEDIANA",tipo_proceso)==0){
        filtradoMediana(matriz,imagen_salida,tamanyo_matriz);
        
    }else if (strcmp("SOBEL",tipo_proceso)==0){
        deteccionBordes(matriz,imagen_salida,tamanyo_matriz);
      
    }else{
        printf("Solo puedes pasar como proceso: MEDIA, MEDIANA O SOBEL %s\n",tipo_proceso);
    }

    t_fin = clock();
    t_ejecucion = ((double)(t_fin-t_inicio)/CLOCKS_PER_SEC)*1000;

    ficheroSalida(imagen_entrada, imagen_salida, fichero_salida, tipo_proceso, t_ejecucion);
    return 0;

}
