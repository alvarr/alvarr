//////////////////////////////////////////////////////////////////
// Alvaro Aguilar Sabuco                                        //
// Computacion Paralela, Practica 1, Tarea 1                    //
// Fecha: 02/11/2021                                            //
//////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <mpi.h>
#define ELEMENTOS 9

/**
 * @brief Lee la imagen en formato raw, que es binario
 * 
 * @param nombre_fichero Nombre del fichero de imagen raw
 * @param tamanyo tamanyo de la matriz
 * @param matriz matriz que guardara la imagen raw
 */
void leerArchivo(char *nombre_fichero,int filas, int columnas, unsigned char **matriz){
    FILE *f1;
    f1 = fopen(nombre_fichero,"rb");

    if(f1){
        //Lectura del fichero
        for(int i = 1; i <= filas; i++){
            for(int j = 1; j <= columnas; j++){
                fread(&matriz[i][j],sizeof(unsigned char), 1, f1);
            }
        }
       
        fclose(f1);
    }else{
        printf("Error al abrir el fichero");
    }
}

/**
 * @brief Añade a la matriz original las columnas y filas necesarias para que no descrimine ningun caracter
 * 
 * @param matriz matriz que guarda la imagen
 * @param filas número de filas de la imagen
 * @param columnas numero de columnas de la imagen
 */
void rellenarMatriz(unsigned char **matriz, int filas, int columnas){
    int filas_relleno = filas +2;
    int columnas_relleno = columnas + 2;

    for(int i = 0;i<filas_relleno;i++){
        for(int j = 0;j<columnas_relleno;j++){
            
            if(i == 0 || j == 0 || i == (filas_relleno-1) || j == (columnas_relleno-1)){
                if(i == 0){
                    if(i == 0 && j == 0){
                        matriz[i][j] = matriz[i+2][j+2];
                    }else if(i == 0 && j == (columnas_relleno - 1)){
                        matriz[i][j] = matriz[i+2][j-2];
                    }else {
                        matriz[i][j] = matriz[i+2][j];
                    }
                }else if(i == (filas_relleno-1)){
                    if(i == (filas_relleno - 1) && j == (columnas_relleno - 1)){
                        matriz[i][j] = matriz[i-2][j-2];
                    }else if(i == (filas_relleno - 1) && j == 0){
                        matriz[i][j] = matriz[i-2][j+2] ;
                    }else{
                        matriz[i][j] = matriz[i-2][j] ;
                    }
                }else if(j == 0){
                    matriz[i][j] = matriz[i][j+2];
                }else if(j == (columnas_relleno-1)){
                    matriz[i][j] = matriz[i][j-2];
                }
                
            }
        }
    }
}


/**
 * @brief Guarda una imagen en formato raw, que es binario
 * 
 * @param matrizSalida matriz resultado del filtrado
 * @param imagen_salida nombre del fichero que guardará la imagen de salida
 * @param tamanyo tamanyo de la mtriz
 */
void guardarImagenSalida(unsigned char **matrizSalida,char *imagen_salida, int filas, int columnas){
    FILE *f1;
    f1 = fopen(imagen_salida, "wb");
    for(int i = 1;i<=filas; i++){
        for(int j = 1;j<=columnas;j++){
            fwrite(&matrizSalida[i][j],sizeof(unsigned char),1,f1);
        }
    }
    fclose(f1);
}

/**
 * @brief Ordena un vector de menor a mayor
 * 
 * @param elementos vector que contiene los numeros a ordenar
 */
void ordenarVector(unsigned char elementos[]){
    int  elemento_auxiliar;
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

/**
 * @brief Filtra la imagen usando la media de los elementos de alrededor
 * 
 * @param matriz matriz que tenemos que filtrar
 * @param imagen_salida nombre del fichero de salida que guardara la imagen
 * @param tamanyo tamanño de la matriz
 */
void filtradoMedia(unsigned char **matriz, char *imagen_salida,int filas, int columnas){
    printf("Filtrado por Media");

    unsigned char **matrizSalida, elementoSeleccionado, elementos[9];
    int media = 0;
    int filas_rellenado = filas+2;
    int columnas_rellenado = columnas +2;
    
    matrizSalida = (unsigned char **)malloc(filas_rellenado*sizeof(unsigned char *));
    for(int i = 0; i<filas_rellenado;i++){
        matrizSalida[i] = (unsigned char *)malloc(columnas_rellenado*sizeof(unsigned char));
    }
    
    for(int i = 0;i<filas_rellenado;i++){
        for(int j = 0;j<columnas_rellenado;j++){
            media = 0;
            elementoSeleccionado = matriz[i][j];
            //printf("%c\n",elementoSeleccionado);
            if(i == 0 || j == 0 || i == (filas_rellenado-1) || j == (columnas_rellenado-1)){
                matrizSalida[i][j] == matriz[i][j];
            }else{
                media = matriz[i-1][j-1] +matriz[i][j-1]+ matriz[i+1][j-1] +matriz[i-1][j] +matriz[i][j] +matriz[i+1][j] +matriz[i-1][j+1] + matriz[i][j+1] +matriz[i+1][j+1] ;
                media = fabs(media/9);
                matrizSalida[i][j] = (unsigned char)media;
            }
        }
        guardarImagenSalida(matrizSalida,imagen_salida,filas, columnas);
    }
}

/**
 * @brief Filtra la imagen usando la mediana de los elementos de alrededor
 * 
 * @param matriz matriz que contiene la imagen raw
 * @param imagen_salida nombre del fichero que contiene la imagen de salida
 * @param tamanyo tamanño de la matriz
 */
void filtradoMediana(unsigned char **matriz, char *imagen_salida, int filas, int columnas){
    
    unsigned char **matrizSalida, elementoSeleccionado,elementos[9];
    int filas_relleno = filas+2;
    int columnas_relleno = columnas+2;
    matrizSalida = (unsigned char **)malloc(filas_relleno*sizeof(unsigned char *));
    for(int i = 0; i<filas_relleno;i++){
        matrizSalida[i] = (unsigned char *)malloc(columnas_relleno*sizeof(unsigned char));
    }
    printf("Hola\n");
    for(int i = 0;i<filas_relleno;i++){
        for(int j = 0;j<columnas_relleno;j++){
            elementoSeleccionado = matriz[i][j];

            if(i == 0 || j == 0 || i == (filas_relleno-1) || j == (columnas_relleno-1)){
                matrizSalida[i][j] ==matriz[i][j];
            }else{
                elementos[0] = matriz[i-1][j-1];
                elementos[1] = matriz[i][j-1];
                elementos[2] = matriz[i+1][j-1];
                elementos[3] = matriz[i-1][j] ;
                elementos[4] = matriz[i][j] ;
                elementos[5] = matriz[i+1][j] ;
                elementos[6] = matriz[i-1][j+1];
                elementos[7] = matriz[i][j+1];
                elementos[8] = matriz[i+1][j+1];
                ordenarVector(elementos);
                
                matrizSalida[i][j] = elementos[4];
            }
        }
    }
    printf("Filtrado por Mediana\n");
    guardarImagenSalida(matrizSalida,imagen_salida,filas , columnas);

}

/**
 * @brief Filtra la imagen usando la técnica de detección de bordes SOBEL
 * 
 * @param matriz matriz que contiene la imagen raw
 * @param imagen_salida nombre del fichero que contiene la imagen de salida
 * @param tamanyo tamanño de la matriz
 */
void deteccionBordes(unsigned char **matriz, char *imagen_salida, int filas, int columnas){
    unsigned char **matrizSalida, elementoSeleccionado, elementos[9];
    int c = 0, f = 0;
    int filas_relleno = filas + 2;
    int columnas_relleno = columnas +2;
    matrizSalida = (unsigned char **)malloc(filas_relleno*sizeof(unsigned char *));
    for(int i = 0; i<filas_relleno;i++){
        matrizSalida[i] = (unsigned char *)malloc(columnas_relleno*sizeof(unsigned char));
    }

    for(int i = 0;i<filas_relleno;i++){
        for(int j = 0;j<columnas_relleno;j++){
            elementoSeleccionado = matriz[i][j];
            

            if(i == 0 || j == 0 || i == (filas_relleno-1) || j == (columnas_relleno-1)){
                matrizSalida[i][j] ==matriz[i][j];
            }else{
                elementos[0] = matriz[i-1][j-1];
                elementos[1] = matriz[i][j-1];
                elementos[2] = matriz[i+1][j-1];
                elementos[3] = matriz[i-1][j] ;
                elementos[4] = matriz[i][j] ;
                elementos[5] = matriz[i+1][j] ;
                elementos[6] = matriz[i-1][j+1];
                elementos[7] = matriz[i][j+1];
                elementos[8] = matriz[i+1][j+1];

                c = (elementos[0] * -1) + (elementos[1] * 0) + (elementos[2] * 1) + (elementos[3] * -2) + (elementos[4] * 0) + (elementos[5] * 2) + (elementos[6] *-1) + (elementos[7] * 0) + (elementos[8] * 1);
                f = (elementos[0] * -1) + (elementos[1] * -2) + (elementos[2] * -1) + (elementos[3] * 0) + (elementos[4] * 0) + (elementos[5] * 0) + (elementos[6] *1) + (elementos[7] * 2) + (elementos[8] * 1);
                matrizSalida[i][j] = (unsigned char)sqrt(pow(c,2)+pow(f,2));
            }
        }
    }
    printf("Filtrado SOBEL");
    guardarImagenSalida(matrizSalida,imagen_salida,filas, columnas);
}

//Guarda en un fichero los parámetros de ejecución, fichero de entrada, fichero de salida, filtrado escogido y tiempo de ejecucion
/**
 * @brief Guarda los datos de ejecución del programa en un fichero txt
 * 
 * @param imagen_entrada nombre del fichero que contiene la imagen que pasamos al programa
 * @param imagen_salida nombre del fichero que contiene la imagen filtrada
 * @param fichero_salida nombre del fichero que contiene los parámetros de ejecución
 * @param tipo_proceso nombre del filtrado escogido
 * @param t_ejecucion tiempo que tarda en ejecutarse el programa
 */
void ficheroSalida(char *imagen_entrada , char *imagen_salida, char *fichero_salida, char *tipo_proceso, double t_ejecucion, int filas, int columnas){
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

/**
 * @brief Función principal
 * 
 * @param argc Número de parametros que pasamos al ejecutar el programa
 * @param argv Parametros que pasamos al ejecutar el programa
 * @return int 
 */
int main(int argc, char *argv[]){

    MPI_Status status;
    MPI_Init(&argc,&argv);
    
    clock_t t_inicio, t_fin;
    double t_ejecucion;
    t_inicio = clock();
    int nproces, myrank, ierr;

    MPI_Comm_size(MPI_COMM_WORLD,&nproces);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
    
    //Indicamos un parámetro de entrada que explique que información hay que pasar como parametro
    if(argc == 2 && strcmp("HELP",argv[1])==0){
        printf("Debes indicar el nombre de la imagen de entrada ,imange de salida, fichero de resultados de salida, sin formato, el tipo de proceso(MEDIA, MEDIANA O SOBEL) y el número de filas y columnas. Por ejemplo './Tarea1 fotoEntrada fotoSalida resultadosSalida MEDIA 512 512'\n");
        MPI_Finalize();
        return 0;
    }
        
    if(argc != 7){
        printf("Debes indicar varios paramentros. Para mas ayuda pasa como parametro HELP\n");
        MPI_Finalize();
        return 0;
    }

    char *imagen_entrada,*imagen_salida, *tipo_proceso, *fichero_salida;
    int filas, columnas, *filas_por_proceso;
    char opcion;
    unsigned char **matriz;
    
  
    if(myrank == 0){

        *filas_por_proceso = (int*)malloc(nproces*sizeof(int));
        imagen_entrada = argv[1];
        imagen_salida = argv[2];
        fichero_salida = argv[3];
        tipo_proceso = argv[4];

        strcat(imagen_entrada,".raw");
        strcat(imagen_salida,".raw"); 
        strcat(fichero_salida,".txt");

        filas = atoi(argv[5]);
        columnas = atoi(argv[6]);
        //Balanceo de carga
        for(int i = 0;i<nproces;i++){
            filas_por_proceso[i] = filas/nproces;
        }
        filas_por_proceso[i-1] += filas%nproces;

        //Envio información de las filas que recibirá cada uno
        for(int i = 1;i<nproces;i++){
            MPI_Send(&filas_por_proceso[i],1,MPI_INT,i,99,MPI_COMM_WORLD);
        }
        
        matriz = (unsigned char **)malloc((filas+2)*sizeof(unsigned char *));
        for(int i = 0;i < (filas+2); i++){
            matriz[i] = (unsigned char *)malloc((columnas+2)*sizeof(unsigned char));
        } 
        leerArchivo(imagen_entrada,filas, columnas,matriz);
    
        rellenarMatriz(matriz, filas, columnas);

    }else{
        
        MPI_Recv(&filas,1,MPI_INT,0,99,MPI_COMM_WORLD,&status);
        matriz = (int**)malloc((filas+2)*sizeof(int*));
        for(int i = 0;i<(filas+2);i++){
            matriz[i] = (int*)malloc((columnas+2)*sizeof(int));
        }
    }
    
    if(strcmp("MEDIA",tipo_proceso)==0){
        
        filtradoMedia(matriz,imagen_salida,filas, columnas);
        
    }else if (strcmp("MEDIANA",tipo_proceso)==0){
        
        filtradoMediana(matriz,imagen_salida,filas, columnas);
        
    }else if (strcmp("SOBEL",tipo_proceso)==0){
        deteccionBordes(matriz,imagen_salida,filas, columnas);
      
    }else{
        printf("Solo puedes pasar como proceso: MEDIA, MEDIANA O SOBEL %s\n",tipo_proceso);
    }

    t_fin = clock();
    t_ejecucion = ((double)(t_fin-t_inicio)/CLOCKS_PER_SEC)*1000;

    ficheroSalida(imagen_entrada, imagen_salida, fichero_salida, tipo_proceso, t_ejecucion, filas,  columnas);

    MPI_Finalize();

    return 0;
}
