/////////////////////////////////////////////////
// Álvaro Aguilar Sabuco                       //
// Computación Paralela, Práctica 0, Tarea 2   //
// Fecha: 20/10/2021                           //
/////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define N 12

/*Reakuza los pasos de la primera iteración*/
void primeraIteracion(double **matriz, double *v_uni, double *v_aux){

    double a;

    //Multiplicamos la matriz por el vector unitario y almacenamos el resultado en el vector auxiliar
    for(int i = 0;i < N;i++){
        a=0;
        for(int u = 0;u < N;u++){
            a = a + (matriz[i][u]*v_uni[u]);
        }
        v_aux[i] = a;
    }

    //Usamos el vector unitario para transformarlo en rand(vector con valores aleatorios |0|<x<|1|)
    for(int i=0;i<N;i++){
        v_uni[i]=0;
        v_uni[i]=((rand()% 2000)-1000)/1000.0f;
    }

    //Realizamos la suma del vector auxiliar y el vecto unitario y lo guardamos en el primero
    for(int i=0;i<N;i++){
        v_aux[i]=v_aux[i]+v_uni[i];
        v_uni[i] = v_aux[i];
    }
}

/*Realiza los pasos de las iteraciones impares*/
double calculoImpar(double **matriz, double *v_uni, double *v_aux){

    //Multiplicamos la matriz por el vector resultado de la anterior iteracion y almacenamos el resultado en el vector auxiliar
    double a;
    for(int i = 0;i < N;i++){ 
        a=0;
        for(int u = 0;u < N;u++){
            a = a + (matriz[i][u]*v_uni[u]);
        }
        v_aux[i] = a;
    }

    //Obtenemos la norma del vector anterior
    a = 0;
    for(int i = 0;i<N;i++){
        a = a +(v_uni[i]*v_uni[i]);
    }   
    a=sqrt(a);

    //Sumamos al primer vector obtenido el escalar
    for(int i = 0;i < N;i++){
        v_aux[i]=v_aux[i]+a;
    }

    //Dividimos entre el valor mayor absoluto
    a=1;

    //dividimos todo el vector entre la norma obtenida
    for(int i = 0;i<N;i++){
        v_aux[i]=v_aux[i]/a;
        v_uni[i]=v_aux[i];
    } 
}

/*Realiza los pasos de las iteraciones pares*/
void calculoPar(double **matriz, double *v_uni, double *v_aux){
    //Multiplicamos la matriz por el vector unitario y almacenamos el resultado en el vector auxiliar
    double a;

    for(int x = 0;x < N;x++){
        a = 0;
        for(int y = 0; y < N;y++){
            a = a+(matriz[x][y]*v_uni[y]);
        }
        v_aux[x]=a;
    }
    for(int i =0;i<N;i++){
        v_uni[i]=v_aux[i];
    }
    
}

/*Realiza según el número de iteración que sea una operación u otra*/
void realizarIteraciones(int num_ite, double **matriz,double *v_uni,double *v_aux){
    for(int i = 0;i<num_ite;i++){

        if(i==0){
            primeraIteracion(matriz, v_uni, v_aux);
        }else if(i%2 != 0){ 
               calculoImpar(matriz, v_uni, v_aux);
            
        }else{
            calculoPar(matriz, v_uni, v_aux);
        }
    }
}

/*Guardar en un fichero información sobre los parámetros de entrada, normas calculadas, norma vector final, tiempos de ejecución, guardado y generado de matriz y total*/
void guardarResumen(char nom_fich_resm[],char nom_fich_matr[],  int num_iteraciones, double t_gen, double t_guar, double t_ejec, double t_global){
    FILE *f1;
    f1 = fopen(nom_fich_resm,"w");
    if(f1==NULL){
        printf("No se puede abrir el fichero");
    }

    fprintf(f1,"Numero de iteraciones: %i \n", num_iteraciones);
    fprintf(f1,"Nombre fichero salida: %s \n", nom_fich_resm);
    fprintf(f1,"Nombre fichero matriz: %s \n", nom_fich_matr);
    
    fprintf(f1,"Tiempo(milisengundos) de generacion de matriz:  %f \n", t_gen);
    fprintf(f1,"Tiempo(milisengundos) de guardado de matriz:  %f \n", t_guar);
    fprintf(f1,"Teimpo(milisengundos) de ejecucion:  %f \n", t_ejec);
    fprintf(f1,"Tiempo(milisengundos) global:  %f \n", t_global);
    
    fclose(f1);
}

/*Imprime información sobre los parámetros de entrada, normas calculadas, norma vector final, tiempos de ejecución, guardado y generado de matriz y total*/
void imprimirResumen(int num_iteraciones, char *nom_fich_sal, char *nom_fich_matr, double t_gen, double t_guar, double t_ejec, double t_global){

    printf("Numero de iteraciones: %i \n", num_iteraciones);
    printf("Nombre fichero salida: %s \n", nom_fich_sal);
    printf("Nombre fichero matriz: %s \n", nom_fich_matr);

    printf("Tiempo(milisengundos) de generacion de matriz:  %f  \n", t_gen);
    printf("Tiempo(milisengundos) de guardado de matriz:  %f \n", t_guar);
    printf("Tiempo(milisengundos) de ejecucion:  %f \n", t_ejec);
    printf("Tiempo(milisengundos) global:  %f \n", t_global);
}

/*Guarda en un fichero la matriz*/
void guardarMatriz(double **matriz, char *nom_fich_matr ){
    FILE *f1;
    f1 = fopen(nom_fich_matr,"wb");
    //printf("%s \n", nom_fich_matr);
    for(int i = 0; i < N;i++){
        fwrite(matriz[i], sizeof(double),N, f1);
    }
    fclose(f1);
}

/*Genera una matriz de tamaño NxN*/
void generarMatriz(double **matriz){
    double aux;
    for(int x =0; x<N;x++){
        for(int y =0; y<N;y++){
            if(x == y){ //Elementos de la diagonal
                matriz[x][y] = 1;
            }else if(x < y){ //Elementos triangular inferior
                aux = rand( ) % 50000;
                matriz[x][y] = aux/1000;
              
            }else{//Elementos triangular superior
                aux = -1*(rand( ) % 50000);
                matriz[x][y] = aux/1000;
            }
            //printf("%.1lf ", matriz[x][y]);
        }
        //printf("\n");
    }
}

/*Lee los datos de un fichero para rellenar una matriz*/
void leerFichero(double **matriz,char *nom_fich_matr){
    FILE *f1;
    f1 = fopen(nom_fich_matr,"rb");
   double aux[1] = {0};
    for(int i = 0; i < N;i++){
        fread(matriz[i], sizeof(double),N, f1);
    }
    fclose(f1);
}



int main(int argc, char *argv[]){

    //Declaramos las variables de tiempo e iniciamos el contador del tiempo total
    clock_t t_ini, t_fin, t_init, t_fint;
    t_init = clock();

    //Si no recibimos los dos parametros la ejecución se para
    if(argc != 4){
        printf("Tienes que indicar el número de iteraciones, el fichero de salida para los datos y el fichero de entrada para la matriz como argumentos, respectivamente\n");
        return 0;
    }

    
    double t_gen, t_guar, t_ejec, t_global, a;
    int diagonal = 0;
    int num_normas = 0;
    double norma_final = 0;
    int num_ite = atoi(argv[1]);
    FILE *f1, *f2;
    srand(time(NULL));

    //Fichero de salida
    char *nom_fich_sal =argv[2];
    strcat(nom_fich_sal,".txt");
    
    //Fichero matriz
    char *nom_fich_matr =argv[3];
    strcat(nom_fich_matr,".bin"); //bin
    
    //Declaramos y reservamos memoria del vector unitario
    double *v_inicial;
    v_inicial = (double *)malloc(N*sizeof(double));
    for(int i=0;i<N;i++){
        v_inicial[i]=1; 
    }

    //Declaramos y reservamos memoria del vector aux
    double *v_auxiliar;
    v_auxiliar = (double *)malloc(N*sizeof(double));
    for(int i=0;i<N;i++){
        v_auxiliar[i]=0;
    }

    //Declaramos y reservamos memoria de la matriz
    double **matriz;
    matriz = (double **)malloc(N*sizeof(double *));
    for(int i =0; i<N;i++){
        matriz[i] = (double *)malloc(N*sizeof(double));
    }

    //Intentamos abrir el fichero matriz, dependiendo de si existe o no realizamos x operaciones
    f1 = fopen(nom_fich_matr,"rb");

    //En caso de que no exista el fichero lo crea y lo guarda
    if(f1==NULL){
        fclose(f1);
        t_ini = clock();
        generarMatriz(matriz);
        t_fin= clock();
        //Tiempo de generación de matriz
        t_gen = ((double)(t_fin-t_ini)/CLOCKS_PER_SEC)*1000;
       
        t_ini=clock();
        guardarMatriz(matriz, nom_fich_matr);
        t_fin = clock();
        //Tiempo de guardado de matriz
        t_guar = ((double)(t_fin-t_ini)/CLOCKS_PER_SEC)*1000;
    }else{
        fclose(f1);
        leerFichero(matriz, nom_fich_matr);
        t_guar = 0;
    }
    
    t_ini = clock();
    
    //Realizamos las iteraciones
    realizarIteraciones(num_ite, matriz, v_inicial, v_auxiliar);

    t_fin= clock();
    //Tiempo que tarda en realizar las iteraciones y sus respectivas operaciones
    t_ejec = ((double)(t_fin-t_ini)/CLOCKS_PER_SEC)*1000;


    t_fint = clock();
    //Tiempo global de todo el programa
    t_global = ((double)(t_fint-t_init)/CLOCKS_PER_SEC)*1000;

    //Guardamos los datos obtenidos en in fichero 
    guardarResumen(nom_fich_sal, nom_fich_matr, num_ite, t_gen, t_guar, t_ejec, t_global);

    //Imprimimos por pantalla los datos obtenidos
    imprimirResumen(num_ite, nom_fich_sal, nom_fich_matr, t_gen, t_guar, t_ejec, t_global);

    return 0;
}