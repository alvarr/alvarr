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

#define N 1200

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
double calculoImpar(double **matriz, double *v_uni, double *v_aux, double *v_norma,int *num_normas){

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

    //realizamos la normalización del vector obtenido 
    //Obtenemos la norma
    a = 0;
    for(int i = 0;i<N;i++){
        a = a +(v_aux[i]*v_aux[i]);
    }   
    a=sqrt(a);
    v_norma[*num_normas] = a;

    //dividimos todo el vector entre la norma obtenida
    for(int i = 0;i<N;i++){
        v_aux[i]=v_aux[i]/a;
        v_uni[i]=v_aux[i];
    } 
  *num_normas=*num_normas+1;

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
void realizarIteraciones(int num_ite, double **matriz,double *v_uni,double *v_aux,double *v_norma, int *num_normas){
    for(int i = 0;i<num_ite;i++){

        if(i==0){
            primeraIteracion(matriz, v_uni, v_aux);
        }else if(i%2 != 0){ 
               calculoImpar(matriz, v_uni, v_aux, v_norma, num_normas);
            
        }else{
            calculoPar(matriz, v_uni, v_aux);
        }
    }
}

/*Guardar en un fichero información sobre los parámetros de entrada, normas calculadas, norma vector final, tiempos de ejecución, guardado y generado de matriz y total*/
void guardarResumen(FILE *f2, char nom_fich_resm[],char nom_fich_matr[],  int num_iteraciones, int num_normas, double norma_final, double t_gen, double t_guar, double t_ejec, double t_global, double *v_norma){

    f2 = fopen(nom_fich_resm,"w");
    if(f2==NULL){
        printf("No se puede abrir el fichero");
    }

    fprintf(f2,"Numero de iteraciones: %i \n", num_iteraciones);
    fprintf(f2,"Nombre fichero salida: %s \n", nom_fich_resm);
    fprintf(f2,"Nombre fichero matriz: %s \n", nom_fich_matr);
    fprintf(f2,"Normas calculadas:  %i \n", num_normas);
    for(int i = 1; i <= num_normas;i++){
        fprintf(f2,"Norma x%i:  %f \n",i*2, v_norma[i-1]);
    }
    fprintf(f2,"Norma vector final:  %f \n", norma_final);
    fprintf(f2,"Tiempo(milisengundos) de generacion de matriz:  %f \n", t_gen);
    fprintf(f2,"Tiempo(milisengundos) de guardado de matriz:  %f \n", t_guar);
    fprintf(f2,"Teimpo(milisengundos) de ejecucion:  %f \n", t_ejec);
    fprintf(f2,"Tiempo(milisengundos) global:  %f \n", t_global);
    
    fclose(f2);
}

/*Imprime información sobre los parámetros de entrada, normas calculadas, norma vector final, tiempos de ejecución, guardado y generado de matriz y total*/
void imprimirResumen(int num_iteraciones, char *nom_fich_sal, char *nom_fich_matr, int num_normas, double norma_final, double t_gen, double t_guar, double t_ejec, double t_global, double *v_norma){

    printf("Numero de iteraciones: %i \n", num_iteraciones);
    printf("Nombre fichero salida: %s \n", nom_fich_sal);
    printf("Nombre fichero matriz: %s \n", nom_fich_matr);
    for(int i = 1; i <= num_normas;i++){
        printf("Norma x%i:  %f \n",i*2, v_norma[i-1]);
    }
    printf("Normas calculadas:  %i \n", num_normas);
    printf("Norma vector final:  %f \n", norma_final); 
    printf("Tiempo(milisengundos) de generacion de matriz:  %f  \n", t_gen);
    printf("Tiempo(milisengundos) de guardado de matriz:  %f \n", t_guar);
    printf("Tiempo(milisengundos) de ejecucion:  %f \n", t_ejec);
    printf("Tiempo(milisengundos) global:  %f \n", t_global);
}

/*Guarda en un fichero la matriz*/
void guardarMatriz(double **matriz, FILE *f1, char *nom_fich_matr ){
    f1 = fopen(nom_fich_matr,"wb");
    //printf("%s \n", nom_fich_matr);
    for(int i = 0; i < N;i++){
        fwrite(matriz[i], sizeof(double),N, f1);
    }
    fclose(f1);
}

/*Genera una matriz de tamaño NxN*/
void generarMatriz(double **matriz){

    for(int x =0; x<N;x++){
        for(int y =0; y<N;y++){
            if(x == y){
                matriz[x][y] = 0;
            }else if(x < y){
                matriz[x][y] =  (rand( ) % 1000 ) / 1000.0f;
              
            }else{
                matriz[x][y] = -1*((rand( ) % 1000 ) / 1000.0f);
            }
           // printf("%1.3f",matriz[x][y]);
        }
        //  printf("\n");
    }
}

/*Lee los datos de un fichero para rellenar una matriz*/
void leerFichero(double **matriz, FILE *f1, char *nom_fich_matr){
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
    double *v_aux;
    v_aux = (double *)malloc(N*sizeof(double));
    for(int i=0;i<N;i++){
        v_aux[i]=0;
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
        t_gen = ((double)(t_fin-t_ini)/CLOCKS_PER_SEC)*1000;
       
        t_ini=clock();
        guardarMatriz(matriz, f1, nom_fich_matr);
        t_fin = clock();
        t_guar = ((double)(t_fin-t_ini)/CLOCKS_PER_SEC)*1000;
    }else{
        fclose(f1);
        leerFichero(matriz, f1, nom_fich_matr);
        t_guar = 0;
    }
    
    t_ini = clock();
    
    //Realizamos las iteraciones
    realizarIteraciones(num_ite, matriz, v_uni, v_aux, v_norma, &num_normas);
    

    //Calculamos norma del vector final 
    norma_final = 0;
    for(int x = 0;x<N;x++){
            norma_final = norma_final + (v_aux[x]*v_aux[x]);
    
    }
    norma_final = sqrt(norma_final);

    t_fin= clock();
    t_ejec = ((double)(t_fin-t_ini)/CLOCKS_PER_SEC)*1000;


    t_fint = clock();
    t_global = ((double)(t_fint-t_init)/CLOCKS_PER_SEC)*1000;

    //Guardamos los datos obtenidos en in fichero 
    guardarResumen(f2, nom_fich_sal, nom_fich_matr, num_ite, num_normas, norma_final, t_gen, t_guar, t_ejec, t_global, v_norma);

    //Imprimimos por pantalla los datos obtenidos
    imprimirResumen(num_ite, nom_fich_sal, nom_fich_matr, num_normas, norma_final, t_gen, t_guar, t_ejec, t_global,v_norma);

    return 0;
}