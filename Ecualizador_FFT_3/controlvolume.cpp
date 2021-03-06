/*
 * DSP Example is part of the DSP Lecture at TEC-Costa Rica
 * Copyright (C) 2017  Pablo Alvarado
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file   controlVolume.cpp
 *         Implements control volumen in the frequency domain
 * \author Javier Aparicio, Esteban Arias, Alberto Castro
 * \date   2017.04.12
 *
 * $Id: controlVolume.cpp $
 */

#include "controlvolume.h"
#include <cmath>
#include <cstring>
#include <fftw3.h>


#include <iostream>
#include <fstream>

#define REAL 0
#define IMAG 1

/*
 * Se implementará un banco de filtros mediante el uso de la DFT y las
 * bibliotecas de FFT que el software QT Creator ofrece, el banco constará de
 * 10 filtros que serán desde frecuencias bajas hasta los 16 kHz aproximadamente
 */

#undef _DSP_DEBUG
#define _DSP_DEBUG

#ifdef _DSP_DEBUG
#define _debug(x) std::cerr << x
#include <iostream>
#else
#define _debug(x)
#endif


/*
 * Constructor
 */

//DEFINICION DE CONSTANTES PARA LA INTERFAZ GRAFICA
controlVolume::controlVolume(){
    energia32=0;
    energia64=0;energia125=0;energia250=0;energia500=0;energia1k=0;energia2k=0;
    energia4k=0;energia8k=0;energia16k=0;

}


/*
 * Destructor
 */

controlVolume::~controlVolume(){

}


/*
 * Init the filter operation
 */

/*Se realiza una primera prueba y esta es un filtro pasa todo, esto solo para verificar el funcionamiento de
 * las bibliotecas de la DFT e IDFT, como parámetros ingresa el bloque de la canción, un control de ganancia, y
 * un puntero de entrada y uno de salida
 */

void controlVolume::filter(int blockSize, int volumeGain, float *in, float *out){//Parámetros de entrada
    /*for (int n=0; n<blockSize;++n){
        out[n]=in[n]*volumeGain*0.02;
    }*/
    int N = blockSize;
    fftw_complex *x;//Llamado de la biblioteca y transformación a la frecuencia por DFT
    x = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N); // Se definie el tamano del puntero de salida

    double *in2;
    double *out2;
    out2 = (double *) malloc(N*sizeof(double));
    in2 = (double *) malloc(N*sizeof(double));

    for (int n=0; n<N;++n){
        in2[n]=(volumeGain)*in[n]*0.02;
    }

    fftw_plan dft = fftw_plan_dft_r2c_1d(N,in2,x,FFTW_ESTIMATE);//SE ESTIMA LA DFT
    fftw_plan idft = fftw_plan_dft_c2r_1d(N,x,out2,FFTW_ESTIMATE);//SE ESTIMA LA IDFT

    fftw_execute(dft);
    fftw_execute(idft);

    for (int n=0; n<N;++n){
        out[n]=((static_cast<float>(out2[n]))/N);//se obtiene el vector de salida
    }

    //Se eliminan variables para evitar conflictos de memoria
    fftw_destroy_plan(dft);
    fftw_destroy_plan(idft);
    //fftw_free(x);
    //free(out2);
    //free(in2);
    //delete out2;
    //delete in2;
    out2=NULL;
    in2=NULL;
    x=NULL;
}

//Implementación del primer filtro, que es de cuarto orden donde se realiza una ventana para h(n) esto para que ingrese como
//parámetro de entrada a la biblioteca de la FFT. Los parámetros son el bloque de la canción, ganancia, punteros de entrada y
//salida y una condición booleana que permite detectar el ingreso de una canción al Jack

////////////////////////////////////FILTRO 16 KHz////////////////////////////////////////////////////////////////////

void controlVolume::filter16k(int blockSize, int g16k, float *in, float *out, bool inicial){//PARAMETROS DE ENTRADA
    int const M=50;// # número de muestras con el que se puede representar la respuesta al impulso
    int const L=blockSize;
    int const N=(L+M-1);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    double h16k[N];
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (inicial){
        //Constantes de la ecuación de salida del filtro obtenidas por la herramienta de fdatool
        const double s1=0.5651467330681903;
        const double s2=0.5651467330681903;
        const double a21=1.9426463772974421;
        const double b21=-1.9964726664680819;
        const double a31=0.94563931272677437;
        const double a22=-0.17474983025465968;
        const double b22=1.9999964630146043;
        const double a32=0.33371506895512698;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Ecuaciones para obtener el h(n)
        h16k[0]=(s1*s2);
        h16k[1]=((s1*s2)*(b22+b21))-h16k[0]*(a21+a22);
        h16k[2]=((s1*s2)*(b21*b22+2))-h16k[1]*(a21+a22)-h16k[0]*(a32+a21*a22+a31);
        h16k[3]=((s1*s2)*(b22+b21))-h16k[2]*(a21+a22)-h16k[1]*(a32+a21*a22+a31)-h16k[0]*(a32*a21+a22*a31);
        h16k[4]=(s1*s2)-h16k[3]*(-a21-a22)+h16k[2]*(-a32-a21*a22-a31)+h16k[1]*(-a32*a21-a22*a31)+h16k[0]*(-a32*a31);
        for (int n=5; n<M;++n){
            h16k[n]=-h16k[n-1]*(-a21-a22)+h16k[n-2]*(-a32-a21*a22-a31)+h16k[n-3]*(-a32*a21-a22*a31)+h16k[n-4]*(-a32*a31);
        }
    }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Definición de punteros que van a la FFT
    fftw_complex *x;
    fftw_complex *w;
    fftw_complex *y;
    x = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N); // Se define el tamano del puntero de salida
    w = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
    y = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    double *in2 = new double[N];
    double *out2 = new double[N];
    double *aux2 = new double[L];
    double *h = new double[N];
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Se usa metodo de solapamiento y almacenamiento y es por esto que se debe rellenar con M-1 ceros el primer bloque
    //e ir llenando las entradas sigueintes con M-1 muestras del bloque anterior

    for (int n=0; n<M;++n){
        h[n]=h16k[n];
    }

    for (int n=M;n<N;++n){
        h[n]=0;
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (inicial){//condicion de inicio
        for (int n=0;n<(M-1);++n){
            in2[n]=0;
        }
    }
    else{
        for (int n=0;n<(M-1);++n){
            in2[n]=val_ant16k[n];
        }
    }

    for (int n=(M-1); n<N;++n){
        in2[n]=in[n-(M-1)];
    }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (int n=0;n<(M-1);++n){
        val_ant16k[n]=in[n+(L-M+1)];
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //ejecucion de la dft de x(n) y h(n)
    fftw_plan Xdft = fftw_plan_dft_r2c_1d(N,in2,x,FFTW_ESTIMATE);
    fftw_plan Wdft = fftw_plan_dft_r2c_1d(N,h,w,FFTW_ESTIMATE);

    fftw_execute(Xdft);
    fftw_execute(Wdft);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //para obtener la parte real e imaginaria de Y(K) se realiza la multiplicación de los vectores X(K) y H(K)
    for (int k=0; k<(N);++k){
        y[k][REAL] = (((x[k][REAL])/(N))*((w[k][REAL])/(N))-((x[k][IMAG])/(N))*((w[k][IMAG])/(N)));
        y[k][IMAG] = (((x[k][IMAG])/(N))*((w[k][REAL])/(N))+((x[k][REAL])/(N))*((w[k][IMAG])/(N)));
    }

    //Llamado de IDFT
    fftw_plan idft = fftw_plan_dft_c2r_1d(N,y,out2,FFTW_ESTIMATE);
    fftw_execute(idft);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (int n=0; n<L;++n){
        aux2[n]=out2[n+M-1];
    }

    for (int n=0; n<L;++n){
        aux2[n]=(aux2[n])*(g16k*0.02)/(N);
        out[n]=(static_cast<float>(aux2[n]));//conversion a flotante porque jack solo lee flotantes
    }

    energia16k=Cal_Energias(L,aux2);//sentencia para la interfaz gráfica
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //eliminar variables para evitar conflictos de memoria
    fftw_destroy_plan(idft);
    fftw_free(y);
    //delete y;
    //y=NULL;
    free(out2);
    free(aux2);
    //delete out2;
    //delete aux2;
    out2=NULL;
    aux2=NULL;

    fftw_destroy_plan(Xdft);
    fftw_destroy_plan(Wdft);
    fftw_free(w);
    fftw_free(x);
    //free(h16k);
    free(h);
    free(in2);
    //delete x;
    //delete w;
    //delete h16k;
    //delete h;
    //delete in2;
    //x=NULL;
    //w=NULL;
    //h16k=NULL;
    h=NULL;
    in2=NULL;
}

//el proceso de los otros filtros es similar al proceso anterior, por esto se comentará lo mas importante de la implementación del filtro

//Implementación del segundo filtro, que es de cuarto orden donde se realiza una ventana para h(n) esto para que ingrese como
//parámetro de entrada a la biblioteca de la FFT. Los parámetros son el bloque de la canción, ganancia, punteros de entrada y
//salida y una condición booleana que permite detectar el ingreso de una canción al Jack

////////////////////////////////////FILTRO 8 KHz///////////////////////////////////////////////////////////////

void controlVolume::filter8k(int blockSize, int g8k, float *in, float *out, bool inicial){
    int const M=35;//muestra de h(n)
    int const L=blockSize;
    int const N=(L+M-1);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    double h8k[N];
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (inicial){
        _debug("hn8---------------------------------------------");
        //CONSTANTES DE ECUACION DEL FILTRO
        const double s1_8k=0.32727396726746139;
        const double s2_8k=0.32727396726746139;
        const double a21_8k=-1.1508985701373824;
        const double b21_8k=1.9989267286808248;
        const double a31_8k=0.70616690961186612;
        const double a22_8k=0.012347089528947711;
        const double b22_8k=-1.9997881643281059;
        const double a32_8k=0.62169241595734603;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        //OBTENCION DE h(n)
        h8k[0]=(s1_8k*s2_8k);
        h8k[1]=((s1_8k*s2_8k)*(b22_8k+b21_8k))-h8k[0]*(a21_8k+a22_8k);
        h8k[2]=((s1_8k*s2_8k)*(b21_8k*b22_8k+2))-h8k[1]*(a21_8k+a22_8k)-h8k[0]*(a32_8k+a21_8k*a22_8k+a31_8k);
        h8k[3]=((s1_8k*s2_8k)*(b22_8k+b21_8k))-h8k[2]*(a21_8k+a22_8k)-h8k[1]*(a32_8k+a21_8k*a22_8k+a31_8k)-h8k[0]*(a32_8k*a21_8k+a22_8k*a31_8k);
        h8k[4]=(s1_8k*s2_8k)+h8k[3]*(-a21_8k-a22_8k)+h8k[2]*(-a32_8k-a21_8k*a22_8k-a31_8k)+h8k[1]*(-a32_8k*a21_8k-a22_8k*a31_8k)+h8k[0]*(-a32_8k*a31_8k);
        for (int n=5; n<M;++n){
            h8k[n]=h8k[n-1]*(-a21_8k-a22_8k)+h8k[n-2]*(-a32_8k-a21_8k*a22_8k-a31_8k)+h8k[n-3]*(-a32_8k*a21_8k-a22_8k*a31_8k)+h8k[n-4]*(-a32_8k*a31_8k);
        }
    }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //FFT
    fftw_complex *x;
    fftw_complex *w;
    fftw_complex *y;
    x = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N); // Se define el tamano del puntero de salida
    w = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
    y = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    double *in2 = new double[N];
    double *out2 = new double[N];
    double *aux2 = new double[L];
    double *h = new double[N];

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int n=0; n<M;++n){
            h[n]=h8k[n];
        }

        for (int n=M;n<N;++n){
            h[n]=0;
        }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (inicial){
            for (int n=0;n<(M-1);++n){
                in2[n]=0;
            }
        }
        else{
            for (int n=0;n<(M-1);++n){
                in2[n]=val_ant8k[n];
            }
        }

        for (int n=(M-1); n<N;++n){
            in2[n]=in[n-(M-1)];
        }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int n=0;n<(M-1);++n){
            val_ant8k[n]=in[n+(L-M+1)];
        }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        fftw_plan Xdft = fftw_plan_dft_r2c_1d(N,in2,x,FFTW_ESTIMATE);
        fftw_plan Wdft = fftw_plan_dft_r2c_1d(N,h,w,FFTW_ESTIMATE);

        fftw_execute(Xdft);
        fftw_execute(Wdft);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int k=0; k<(N);++k){
            y[k][REAL] = (((x[k][REAL])/(N))*((w[k][REAL])/(N))-((x[k][IMAG])/(N))*((w[k][IMAG])/(N)));
            y[k][IMAG] = (((x[k][IMAG])/(N))*((w[k][REAL])/(N))+((x[k][REAL])/(N))*((w[k][IMAG])/(N)));
        }

        //IDFT

        fftw_plan idft = fftw_plan_dft_c2r_1d(N,y,out2,FFTW_ESTIMATE);
        fftw_execute(idft);


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int n=0; n<L;++n){
            aux2[n]=out2[n+M-1];
        }

        for (int n=0; n<L;++n){
            aux2[n]=(aux2[n])*(g8k*0.02/N);
            out[n]=static_cast<float>(aux2[n]);//vector de salida
        }
        energia8k=Cal_Energias(L,aux2);//interfaz grafica
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

        //libaeracion de variables
        fftw_destroy_plan(idft);
        fftw_free(y);
        //delete y;
        y=NULL;
        free(out2);
        free(aux2);
        //delete out2;
        //delete aux2;
        out2=NULL;
        aux2=NULL;

        fftw_destroy_plan(Xdft);
        fftw_destroy_plan(Wdft);
        fftw_free(w);
        fftw_free(x);
        //free(h8k);
        free(h);
        free(in2);
        //delete x;
        //delete w;
        //delete h16k;
        //delete h;
        //delete in2;
        x=NULL;
        w=NULL;
        h=NULL;
        in2=NULL;
    }


//TERCER FILTRO
////////////////////////////////////FILTRO 4 KHz///////////////////////////////////////////////////////////////

void controlVolume::filter4k(int blockSize, int g4k, float *in, float *out, bool inicial){
    int const M=75;//MUESTRAS DE LA VENTANA
    int const L=blockSize;
    int const N=(L+M-1);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    double h4k[N];
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (inicial){
        _debug("hn4---------------------------------------------");
        //CONSTANTES DE LA ECUACION h(n)
        const double s1_4k=0.181816174126083093;
        const double s2_4k=0.181816174126083093;
        const double a21_4k=-1.7256306579923035;
        const double b21_4k=1.917689682062321;
        const double a31_4k=0.85818657322090064;
        const double a22_4k=-1.2839744807665578;
        const double b22_4k=-1.9995190463285177;
        const double a32_4k=0.75410327843756864;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        //h(n)
        h4k[0]=(s1_4k*s2_4k);
        h4k[1]=((s1_4k*s2_4k)*(b22_4k+b21_4k))-h4k[0]*(a21_4k+a22_4k);
        h4k[2]=((s1_4k*s2_4k)*(b21_4k*b22_4k+2))-h4k[1]*(a21_4k+a22_4k)-h4k[0]*(a32_4k+a21_4k*a22_4k+a31_4k);
        h4k[3]=((s1_4k*s2_4k)*(b22_4k+b21_4k))-h4k[2]*(a21_4k+a22_4k)-h4k[1]*(a32_4k+a21_4k*a22_4k+a31_4k)-h4k[0]*(a32_4k*a21_4k+a22_4k*a31_4k);
        h4k[4]=(s1_4k*s2_4k)+h4k[3]*(-a21_4k-a22_4k)+h4k[2]*(-a32_4k-a21_4k*a22_4k-a31_4k)+h4k[1]*(-a32_4k*a21_4k-a22_4k*a31_4k)+h4k[0]*(-a32_4k*a31_4k);
        for (int n=5; n<M;++n){
            h4k[n]=h4k[n-1]*(-a21_4k-a22_4k)+h4k[n-2]*(-a32_4k-a21_4k*a22_4k-a31_4k)+h4k[n-3]*(-a32_4k*a21_4k-a22_4k*a31_4k)+h4k[n-4]*(-a32_4k*a31_4k);
        }
    }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //FFT
    fftw_complex *x;
    fftw_complex *w;
    fftw_complex *y;
    x = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N); // Se define el tamano del puntero de salida
    w = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
    y = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    double *in2 = new double[N];
    double *out2 = new double[N];
    double *aux2 = new double[L];
    double *h = new double[N];

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int n=0; n<M;++n){
            h[n]=h4k[n];
        }

        for (int n=M;n<N;++n){
            h[n]=0;
        }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (inicial){
            for (int n=0;n<(M-1);++n){
                in2[n]=0;
            }
        }
        else{
            for (int n=0;n<(M-1);++n){
                in2[n]=val_ant4k[n];
            }
        }

        for (int n=(M-1); n<N;++n){
            in2[n]=in[n-(M-1)];
        }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int n=0;n<(M-1);++n){
            val_ant4k[n]=in[n+(L-M+1)];
        }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        fftw_plan Xdft = fftw_plan_dft_r2c_1d(N,in2,x,FFTW_ESTIMATE);
        fftw_plan Wdft = fftw_plan_dft_r2c_1d(N,h,w,FFTW_ESTIMATE);

        fftw_execute(Xdft);
        fftw_execute(Wdft);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int k=0; k<(N);++k){
            y[k][REAL] = (((x[k][REAL])/(N))*((w[k][REAL])/(N))-((x[k][IMAG])/(N))*((w[k][IMAG])/(N)));
            y[k][IMAG] = (((x[k][IMAG])/(N))*((w[k][REAL])/(N))+((x[k][REAL])/(N))*((w[k][IMAG])/(N)));
        }

        //IDFT
        fftw_plan idft = fftw_plan_dft_c2r_1d(N,y,out2,FFTW_ESTIMATE);
        fftw_execute(idft);


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int n=0; n<L;++n){
            aux2[n]=out2[n+M-1];
        }

        for (int n=0; n<L;++n){
            aux2[n]=(aux2[n])*(g4k*0.02/N);
            out[n]=static_cast<float>(aux2[n]);//salida
        }

        energia4k=Cal_Energias(L,aux2);//interfaz grafica
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

        //LIBERACIÓN
        fftw_destroy_plan(idft);
        //fftw_free(y);
        //delete y;
        y=NULL;
        free(out2);
        free(aux2);
        //delete out2;
        //delete aux2;
        out2=NULL;
        aux2=NULL;

        fftw_destroy_plan(Xdft);
        fftw_destroy_plan(Wdft);
        fftw_free(w);
        fftw_free(x);
        //free(h16k);
        free(h);
        free(in2);
        //delete x;
        //delete w;
        //delete h16k;
        //delete h;
        //delete in2;
        x=NULL;
        w=NULL;
        //h4k=NULL;
        h=NULL;
        in2=NULL;
    }

//CUARTO FILTRO
////////////////////////////////////FILTRO 2 KHz///////////////////////////////////////////////////////////////

void controlVolume::filter2k(int blockSize, int g2k, float *in, float *out, bool inicial){
    int const M=75;//MUESTRAS DE LA VENTANA
    int const L=blockSize;
    int const N=(L+M-1);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    double h2k[N];
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if(inicial){
        _debug("hn2---------------------------------------------");
        //CONSTANTES DE LA ECUACION DEL FILTRO
        const double s1_2k=0.095063653045318011;
        const double s2_2k=0.095063653045318011;
        const double a21_2k=-1.7255493545252554;
        const double b21_2k=1.9621384032541549;
        const double a31_2k=0.86666712487268294;
        const double a22_2k=-1.884318898389882;
        const double b22_2k=-1.9999837031331498;
        const double a32_2k=0.92566942562536558;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        //obtencion de h(n)
        h2k[0]=(s1_2k*s2_2k);
        h2k[1]=((s1_2k*s2_2k)*(b22_2k+b21_2k))-h2k[0]*(a21_2k+a22_2k);
        h2k[2]=((s1_2k*s2_2k)*(b21_2k*b22_2k+2))-h2k[1]*(a21_2k+a22_2k)-h2k[0]*(a32_2k+a21_2k*a22_2k+a31_2k);
        h2k[3]=((s1_2k*s2_2k)*(b22_2k+b21_2k))-h2k[2]*(a21_2k+a22_2k)-h2k[1]*(a32_2k+a21_2k*a22_2k+a31_2k)-h2k[0]*(a32_2k*a21_2k+a22_2k*a31_2k);
        h2k[4]=(s1_2k*s2_2k)-h2k[3]*(-a21_2k-a22_2k)+h2k[2]*(-a32_2k-a21_2k*a22_2k-a31_2k)+h2k[1]*(-a32_2k*a21_2k-a22_2k*a31_2k)+h2k[0]*(-a32_2k*a31_2k);
        for (int n=5; n<M;++n){
            h2k[n]=-h2k[n-1]*(-a21_2k-a22_2k)+h2k[n-2]*(-a32_2k-a21_2k*a22_2k-a31_2k)+h2k[n-3]*(-a32_2k*a21_2k-a22_2k*a31_2k)+h2k[n-4]*(-a32_2k*a31_2k);
        }
    }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //DFT
    fftw_complex *x;
    fftw_complex *w;
    fftw_complex *y;
    x = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N); // Se define el tamano del puntero de salida
    w = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
    y = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    double *in2 = new double[N];
    double *out2 = new double[N];
    double *aux2 = new double[L];
    double *h = new double[N];

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int n=0; n<M;++n){
            h[n]=h2k[n];
        }

        for (int n=M;n<N;++n){
            h[n]=0;
        }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (inicial){
            for (int n=0;n<(M-1);++n){
                in2[n]=0;
            }
        }
        else{
            for (int n=0;n<(M-1);++n){
                in2[n]=val_ant2k[n];
            }
        }

        for (int n=(M-1); n<N;++n){
            in2[n]=in[n-(M-1)];
        }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int n=0;n<(M-1);++n){
            val_ant2k[n]=in[n+(L-M+1)];
        }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        fftw_plan Xdft = fftw_plan_dft_r2c_1d(N,in2,x,FFTW_ESTIMATE);
        fftw_plan Wdft = fftw_plan_dft_r2c_1d(N,h,w,FFTW_ESTIMATE);

        fftw_execute(Xdft);
        fftw_execute(Wdft);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int k=0; k<(N);++k){
            y[k][REAL] = (((x[k][REAL])/(N))*((w[k][REAL])/(N))-((x[k][IMAG])/(N))*((w[k][IMAG])/(N)));
            y[k][IMAG] = (((x[k][IMAG])/(N))*((w[k][REAL])/(N))+((x[k][REAL])/(N))*((w[k][IMAG])/(N)));
        }

        //IDFT
        fftw_plan idft = fftw_plan_dft_c2r_1d(N,y,out2,FFTW_ESTIMATE);
        fftw_execute(idft);


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int n=0; n<L;++n){
            aux2[n]=out2[n+M-1];
        }

        for (int n=0; n<L;++n){
            aux2[n]=(aux2[n])*(g2k*0.02)/(N);
            out[n]=static_cast<float>(aux2[n]);//vector salida
        }

        energia2k=Cal_Energias(L,aux2);//salida interfaz
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

        //liberacion de variables
        fftw_destroy_plan(idft);
        fftw_free(y);
        //delete y;
        y=NULL;
        free(out2);
        free(aux2);
        //delete out2;
        //delete aux2;
        out2=NULL;
        aux2=NULL;

        fftw_destroy_plan(Xdft);
        fftw_destroy_plan(Wdft);
        fftw_free(w);
        fftw_free(x);
        //free(h16k);
        free(h);
        free(in2);
        //delete x;
        //delete w;
        //delete h16k;
        //delete h;
        //delete in2;
        x=NULL;
        w=NULL;
        //h2k=NULL;
        h=NULL;
        in2=NULL;
    }

//QUINTO FILTRO
////////////////////////////////////FILTRO 1 KHz///////////////////////////////////////////////////////////////

void controlVolume::filter1k(int blockSize, int g1k, float *in, float *out, bool inicial){
    int const M=50;//MUESTRA h(n)
    int const L=blockSize;
    int const N=(L+M-1);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    double h1k[N];
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (inicial){
        //CONSTANTES DE LA ECUACIÓN DEL FILTRO
        const double s1_1k=0.049546711644191409;
        const double s2_1k=0.049546711644191409;
        const double a21_1k=-1.8936589823711933;
        const double b21_1k=1.8475696057002993;
        const double a31_1k=0.93046639472475323;
        const double a22_1k=-1.9517542709310356;
        const double b22_1k=-1.9999958840297132;
        const double a32_1k=0.9623164396833872;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        //obtención de h(n)
        h1k[0]=(s1_1k*s2_1k);
        h1k[1]=((s1_1k*s2_1k)*(b22_1k+b21_1k))-h1k[0]*(a21_1k+a22_1k);
        h1k[2]=((s1_1k*s2_1k)*(b21_1k*b22_1k+2))-h1k[1]*(a21_1k+a22_1k)-h1k[0]*(a32_1k+a21_1k*a22_1k+a31_1k);
        h1k[3]=((s1_1k*s2_1k)*(b22_1k+b21_1k))-h1k[2]*(a21_1k+a22_1k)-h1k[1]*(a32_1k+a21_1k*a22_1k+a31_1k)-h1k[0]*(a32_1k*a21_1k+a22_1k*a31_1k);
        h1k[4]=(s1_1k*s2_1k)+h1k[3]*(-a21_1k-a22_1k)+h1k[2]*(-a32_1k-a21_1k*a22_1k-a31_1k)+h1k[1]*(-a32_1k*a21_1k-a22_1k*a31_1k)+h1k[0]*(-a32_1k*a31_1k);
        for (int n=5; n<M;++n){
            h1k[n]=h1k[n-1]*(-a21_1k-a22_1k)+h1k[n-2]*(-a32_1k-a21_1k*a22_1k-a31_1k)+h1k[n-3]*(-a32_1k*a21_1k-a22_1k*a31_1k)+h1k[n-4]*(-a32_1k*a31_1k);
        }
    }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //FFT
    fftw_complex *x;
    fftw_complex *w;
    fftw_complex *y;
    x = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N); // Se define el tamano del puntero de salida
    w = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
    y = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    double *in2 = new double[N];
    double *out2 = new double[N];
    double *aux2 = new double[L];
    double *h = new double[N];

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int n=0; n<M;++n){
            h[n]=h1k[n];
        }

        for (int n=M;n<N;++n){
            h[n]=0;
        }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (inicial){
            for (int n=0;n<(M-1);++n){
                in2[n]=0;
            }
        }
        else{
            for (int n=0;n<(M-1);++n){
                in2[n]=val_ant1k[n];
            }
        }

        for (int n=(M-1); n<N;++n){
            in2[n]=in[n-(M-1)];
        }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int n=0;n<(M-1);++n){
            val_ant1k[n]=in[n+(L-M+1)];
        }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        fftw_plan Xdft = fftw_plan_dft_r2c_1d(N,in2,x,FFTW_ESTIMATE);
        fftw_plan Wdft = fftw_plan_dft_r2c_1d(N,h,w,FFTW_ESTIMATE);

        fftw_execute(Xdft);
        fftw_execute(Wdft);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int k=0; k<(N);++k){
            y[k][REAL] = (((x[k][REAL])/N)*((w[k][REAL])/N)-((x[k][IMAG])/N)*((w[k][IMAG])/N));
            y[k][IMAG] = (((x[k][IMAG])/N)*((w[k][REAL])/N)+((x[k][REAL])/N)*((w[k][IMAG])/N));
        }

        //IDFT
        fftw_plan idft = fftw_plan_dft_c2r_1d(N,y,out2,FFTW_ESTIMATE);
        fftw_execute(idft);


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int n=0; n<L;++n){
            aux2[n]=out2[n+M-1];
        }

        for (int n=0; n<L;++n){
            aux2[n]=(aux2[n])*(g1k*0.02/N);
            out[n]=static_cast<float>(aux2[n]);//vector salida
        }
        energia1k=Cal_Energias(L,aux2);//interfaz grafica
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

        //LIBERACION
        fftw_destroy_plan(idft);
        fftw_free(y);
        //delete y;
        y=NULL;
        free(out2);
        free(aux2);
        //delete out2;
        //delete aux2;
        out2=NULL;
        aux2=NULL;

        fftw_destroy_plan(Xdft);
        fftw_destroy_plan(Wdft);
        fftw_free(w);
        fftw_free(x);
        //free(h16k);
        free(h);
        free(in2);
        //delete x;
        //delete w;
        //delete h16k;
        //delete h;
        //delete in2;
        x=NULL;
        w=NULL;
        //h1k=NULL;
        h=NULL;
        in2=NULL;
    }

//SEXTO FILTRO
////////////////////////////////////FILTRO 500 Hz///////////////////////////////////////////////////////////////

void controlVolume::filter500(int blockSize, int g500, float *in, float *out, bool inicial){
    int const M=50;//VENTANA
    int const L=blockSize;
    int const N=(L+M-1);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    double h500[N];
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if(inicial){
        //CONSTANTES
    const double s1_500=0.02652721749126348;
    const double s2_500=0.02652721749126348;
    const double a21_500=-1.955158498047413;
    const double b21_500=1.4486438385428602;
    const double a31_500=0.96454348300491932;
    const double a22_500=-1.9783347787679899;
    const double b22_500=-1.9999989684665151;
    const double a32_500=0.98100213788338464;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //h(n)
    h500[0]=(s1_500*s2_500);
    h500[1]=((s1_500*s2_500)*(b22_500+b21_500))-h500[0]*(a21_500+a22_500);
    h500[2]=((s1_500*s2_500)*(b21_500*b22_500+2))-h500[1]*(a21_500+a22_500)-h500[0]*(a32_500+a21_500*a22_500+a31_500);
    h500[3]=((s1_500*s2_500)*(b22_500+b21_500))-h500[2]*(a21_500+a22_500)-h500[1]*(a32_500+a21_500*a22_500+a31_500)-h500[0]*(a32_500*a21_500+a22_500*a31_500);
    h500[4]=(s1_500*s2_500)+h500[3]*(-a21_500-a22_500)+h500[2]*(-a32_500-a21_500*a22_500-a31_500)+h500[1]*(-a32_500*a21_500-a22_500*a31_500)+h500[0]*(-a32_500*a31_500);
    for (int n=5; n<M;++n){
        h500[n]=h500[n-1]*(-a21_500-a22_500)+h500[n-2]*(-a32_500-a21_500*a22_500-a31_500)+h500[n-3]*(-a32_500*a21_500-a22_500*a31_500)+h500[n-4]*(-a32_500*a31_500);
    }
    }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //FFT
    fftw_complex *x;
    fftw_complex *w;
    fftw_complex *y;
    x = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N); // Se define el tamano del puntero de salida
    w = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
    y = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    double *in2 = new double[N];
    double *out2 = new double[N];
    double *aux2 = new double[L];
    double *h = new double[N];

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int n=0; n<M;++n){
            h[n]=h500[n];
        }

        for (int n=M;n<N;++n){
            h[n]=0;
        }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (inicial){
            for (int n=0;n<(M-1);++n){
                in2[n]=0;
            }
        }
        else{
            for (int n=0;n<(M-1);++n){
                in2[n]=val_ant500[n];
            }
        }

        for (int n=(M-1); n<N;++n){
            in2[n]=in[n-(M-1)];
        }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int n=0;n<(M-1);++n){
            val_ant500[n]=in[n+(L-M+1)];
        }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        fftw_plan Xdft = fftw_plan_dft_r2c_1d(N,in2,x,FFTW_ESTIMATE);
        fftw_plan Wdft = fftw_plan_dft_r2c_1d(N,h,w,FFTW_ESTIMATE);

        fftw_execute(Xdft);
        fftw_execute(Wdft);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int k=0; k<(N);++k){
            y[k][REAL] = (((x[k][REAL])/N)*((w[k][REAL])/N)-((x[k][IMAG])/N)*((w[k][IMAG])/N));
            y[k][IMAG] = (((x[k][IMAG])/N)*((w[k][REAL])/N)+((x[k][REAL])/N)*((w[k][IMAG])/N));
        }

        //IDFT
        fftw_plan idft = fftw_plan_dft_c2r_1d(N,y,out2,FFTW_ESTIMATE);
        fftw_execute(idft);


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int n=0; n<L;++n){
            aux2[n]=out2[n+M-1];
        }

        for (int n=0; n<L;++n){
            aux2[n]=(aux2[n])*(g500*0.02/N);
            out[n]=static_cast<float>(aux2[n]);//SALIDA
        }
        energia500=Cal_Energias(L,aux2);//SLIDER
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

        fftw_destroy_plan(idft);
        fftw_free(y);
        //delete y;
        y=NULL;
        free(out2);
        free(aux2);
        //delete out2;
        //delete aux2;
        out2=NULL;
        aux2=NULL;

        fftw_destroy_plan(Xdft);
        fftw_destroy_plan(Wdft);
        fftw_free(w);
        fftw_free(x);
        //free(h16k);
        free(h);
        free(in2);
        //delete x;
        //delete w;
        //delete h16k;
        //delete h;
        //delete in2;
        x=NULL;
        w=NULL;
        //h500=NULL;
        h=NULL;
        in2=NULL;
    }

//SETIMO FILTRO
/////////////////////////////////////FILTRO 250 Hz////////////////////////////////////////////////////////////

void controlVolume::filter250(int blockSize, int g250, float *in, float *out, bool inicial){
    int const M=50;//VENTANA
    int const L=blockSize;
    int const N=(L+M-1);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    double h250[N];
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (inicial){//CONSTANTES
    const double s1_250=0.01588433703605379;
    const double s2_250=0.01588433703605379;
    const double a21_250=-1.9797366490816914;
    const double b21_250=0.43753936941804866;
    const double a31_250=0.98210510585048971;
    const double a22_250=-1.9897888038856126;
    const double b22_250=-1.9999997419165898;
    const double a32_250=0.99045897307048592;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //h(n)
    h250[0]=(s1_250*s2_250);
    h250[1]=((s1_250*s2_250)*(b22_250+b21_250))-h250[0]*(a21_250+a22_250);
    h250[2]=((s1_250*s2_250)*(b21_250*b22_250+2))-h250[1]*(a21_250+a22_250)-h250[0]*(a32_250+a21_250*a22_250+a31_250);
    h250[3]=((s1_250*s2_250)*(b22_250+b21_250))-h250[2]*(a21_250+a22_250)-h250[1]*(a32_250+a21_250*a22_250+a31_250)-h250[0]*(a32_250*a21_250+a22_250*a31_250);
    h250[4]=(s1_250*s2_250)+h250[3]*(-a21_250-a22_250)+h250[2]*(-a32_250-a21_250*a22_250-a31_250)+h250[1]*(-a32_250*a21_250-a22_250*a31_250)+h250[0]*(-a32_250*a31_250);
    for (int n=5; n<M;++n){
        h250[n]=h250[n-1]*(-a21_250-a22_250)+h250[n-2]*(-a32_250-a21_250*a22_250-a31_250)+h250[n-3]*(-a32_250*a21_250-a22_250*a31_250)+h250[n-4]*(-a32_250*a31_250);
    }
    }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //FFT
    fftw_complex *x;
    fftw_complex *w;
    fftw_complex *y;
    x = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N); // Se define el tamano del puntero de salida
    w = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
    y = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    double *in2 = new double[N];
    double *out2 = new double[N];
    double *aux2 = new double[L];
    double *h = new double[N];

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int n=0; n<M;++n){
            h[n]=h250[n];
        }

        for (int n=M;n<N;++n){
            h[n]=0;
        }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (inicial){
            for (int n=0;n<(M-1);++n){
                in2[n]=0;
            }
        }
        else{
            for (int n=0;n<(M-1);++n){
                in2[n]=val_ant250[n];
            }
        }

        for (int n=(M-1); n<N;++n){
            in2[n]=in[n-(M-1)];
        }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int n=0;n<(M-1);++n){
            val_ant250[n]=in[n+(L-M+1)];
        }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        fftw_plan Xdft = fftw_plan_dft_r2c_1d(N,in2,x,FFTW_ESTIMATE);
        fftw_plan Wdft = fftw_plan_dft_r2c_1d(N,h,w,FFTW_ESTIMATE);

        fftw_execute(Xdft);
        fftw_execute(Wdft);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int k=0; k<(N);++k){
            y[k][REAL] = (((x[k][REAL])/N)*((w[k][REAL])/N)-((x[k][IMAG])/N)*((w[k][IMAG])/N));
            y[k][IMAG] = (((x[k][IMAG])/N)*((w[k][REAL])/N)+((x[k][REAL])/N)*((w[k][IMAG])/N));
        }

        //IDFT
        fftw_plan idft = fftw_plan_dft_c2r_1d(N,y,out2,FFTW_ESTIMATE);
        fftw_execute(idft);


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int n=0; n<L;++n){
            aux2[n]=out2[n+M-1];
        }

        for (int n=0; n<L;++n){
            aux2[n]=(aux2[n])*(g250*0.02/N);
            out[n]=static_cast<float>(aux2[n]);//vector salida
        }
        energia250=Cal_Energias(L,aux2);//interfaz grafica
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

        fftw_destroy_plan(idft);
        fftw_free(y);
        //delete y;
        y=NULL;
        free(out2);
        free(aux2);
        //delete out2;
        //delete aux2;
        out2=NULL;
        aux2=NULL;

        fftw_destroy_plan(Xdft);
        fftw_destroy_plan(Wdft);
        fftw_free(w);
        fftw_free(x);
        //free(h16k);
        free(h);
        free(in2);
        //delete x;
        //delete w;
        //delete h16k;
        //delete h;
        //delete in2;
        x=NULL;
        w=NULL;
        //h250=NULL;
        h=NULL;
        in2=NULL;
    }

//OCTAVO FILTRO
/////////////////////////////////////FILTRO 125 Hz////////////////////////////////////////////////////////////

void controlVolume::filter125(int blockSize, int g125, float *in, float *out, bool inicial){
    int const M=50;//VENTANA
    int const L=blockSize;
    int const N=(L+M-1);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    double h125[N];
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if(inicial){
        //CONSTANTES
    const double s1_125=0.011747444893149193;
    const double s2_125=0.011747444893149193;
    const double a21_125=-1.9904156893429961;
    const double b21_125=-0.87802119888800734;
    const double a31_125=0.99101057819874705;
    const double a22_125=-1.9950503141027367;
    const double b22_125=-1.9999999354725686;
    const double a32_125=0.99521826411123604;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //h(n)
    h125[0]=(s1_125*s2_125);
    h125[1]=((s1_125*s2_125)*(b22_125+b21_125))-h125[0]*(a21_125+a22_125);
    h125[2]=((s1_125*s2_125)*(b21_125*b22_125+2))-h125[1]*(a21_125+a22_125)-h125[0]*(a32_125+a21_125*a22_125+a31_125);
    h125[3]=((s1_125*s2_125)*(b22_125+b21_125))-h125[2]*(a21_125+a22_125)-h125[1]*(a32_125+a21_125*a22_125+a31_125)-h125[0]*(a32_125*a21_125+a22_125*a31_125);
    h125[4]=(s1_125*s2_125)+h125[3]*(-a21_125-a22_125)+h125[2]*(-a32_125-a21_125*a22_125-a31_125)+h125[1]*(-a32_125*a21_125-a22_125*a31_125)+h125[0]*(-a32_125*a31_125);
    for (int n=5; n<M;++n){
        h125[n]=h125[n-1]*(-a21_125-a22_125)+h125[n-2]*(-a32_125-a21_125*a22_125-a31_125)+h125[n-3]*(-a32_125*a21_125-a22_125*a31_125)+h125[n-4]*(-a32_125*a31_125);
    }
    }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    fftw_complex *x;
    fftw_complex *w;
    fftw_complex *y;
    x = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N); // Se define el tamano del puntero de salida
    w = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
    y = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    double *in2 = new double[N];
    double *out2 = new double[N];
    double *aux2 = new double[L];
    double *h = new double[N];

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int n=0; n<M;++n){
            h[n]=h125[n];
        }

        for (int n=M;n<N;++n){
            h[n]=0;
        }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (inicial){
            for (int n=0;n<(M-1);++n){
                in2[n]=0;
            }
        }
        else{
            for (int n=0;n<(M-1);++n){
                in2[n]=val_ant125[n];
            }
        }

        for (int n=(M-1); n<N;++n){
            in2[n]=in[n-(M-1)];
        }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int n=0;n<(M-1);++n){
            val_ant125[n]=in[n+(L-M+1)];
        }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        fftw_plan Xdft = fftw_plan_dft_r2c_1d(N,in2,x,FFTW_ESTIMATE);
        fftw_plan Wdft = fftw_plan_dft_r2c_1d(N,h,w,FFTW_ESTIMATE);

        fftw_execute(Xdft);
        fftw_execute(Wdft);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int k=0; k<(N);++k){
            y[k][REAL] = (((x[k][REAL])/N)*((w[k][REAL])/N)-((x[k][IMAG])/N)*((w[k][IMAG])/N));
            y[k][IMAG] = (((x[k][IMAG])/N)*((w[k][REAL])/N)+((x[k][REAL])/N)*((w[k][IMAG])/N));
        }

        fftw_plan idft = fftw_plan_dft_c2r_1d(N,y,out2,FFTW_ESTIMATE);
        fftw_execute(idft);


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int n=0; n<L;++n){
            aux2[n]=out2[n+M-1];
        }

        for (int n=0; n<L;++n){
            aux2[n]=(aux2[n])*(g125*0.02/N);
            out[n]=static_cast<float>(aux2[n]);//salida
        }
        energia125=Cal_Energias(L,aux2);//grafica
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

        fftw_destroy_plan(idft);
        fftw_free(y);
        //delete y;
        y=NULL;
        free(out2);
        free(aux2);
        //delete out2;
        //delete aux2;
        out2=NULL;
        aux2=NULL;

        fftw_destroy_plan(Xdft);
        fftw_destroy_plan(Wdft);
        fftw_free(w);
        fftw_free(x);
        //free(h16k);
        free(h);
        free(in2);
        //delete x;
        //delete w;
        //delete h16k;
        //delete h;
        //delete in2;
        x=NULL;
        w=NULL;
        //h125=NULL;
        h=NULL;
        in2=NULL;
    }

//NOVENO FILTRO
/////////////////////////////////////FILTRO 64 Hz////////////////////////////////////////////////////////////

void controlVolume::filter64(int blockSize, int g64, float *in, float *out, bool inicial){
    int const M=50;//VENTANA
    int const L=blockSize;
    int const N=(L+M-1);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    double h64[N];
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if(inicial){//CONSTANTES
    const double s1_64=0.010464723392085607;
    const double s2_64=0.010464723392085607;
    const double a21_64=-1.9953086672439;
    const double b21_64=-1.639755651480399;
    const double a31_64=0.99546009806717473;
    const double a22_64=-1.9975447699332796;
    const double b22_64=-1.9999999836036131;
    const double a32_64=0.99758748552658383;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //h(n)
    h64[0]=(s1_64*s2_64);
    h64[1]=((s1_64*s2_64)*(b22_64+b21_64))-h64[0]*(a21_64+a22_64);
    h64[2]=((s1_64*s2_64)*(b21_64*b22_64+2))-h64[1]*(a21_64+a22_64)-h64[0]*(a32_64+a21_64*a22_64+a31_64);
    h64[3]=((s1_64*s2_64)*(b22_64+b21_64))-h64[2]*(a21_64+a22_64)-h64[1]*(a32_64+a21_64*a22_64+a31_64)-h64[0]*(a32_64*a21_64+a22_64*a31_64);
    h64[4]=(s1_64*s2_64)+h64[3]*(-a21_64-a22_64)+h64[2]*(-a32_64-a21_64*a22_64-a31_64)+h64[1]*(-a32_64*a21_64-a22_64*a31_64)+h64[0]*(-a32_64*a31_64);
    for (int n=5; n<M;++n){
        h64[n]=h64[n-1]*(-a21_64-a22_64)+h64[n-2]*(-a32_64-a21_64*a22_64-a31_64)+h64[n-3]*(-a32_64*a21_64-a22_64*a31_64)+h64[n-4]*(-a32_64*a31_64);
    }
    }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    fftw_complex *x;
    fftw_complex *w;
    fftw_complex *y;
    x = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N); // Se define el tamano del puntero de salida
    w = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
    y = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    double *in2 = new double[N];
    double *out2 = new double[N];
    double *aux2 = new double[L];
    double *h = new double[N];

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int n=0; n<M;++n){
            h[n]=h64[n];
        }

        for (int n=M;n<N;++n){
            h[n]=0;
        }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (inicial){
            for (int n=0;n<(M-1);++n){
                in2[n]=0;
            }
        }
        else{
            for (int n=0;n<(M-1);++n){
                in2[n]=val_ant64[n];
            }
        }

        for (int n=(M-1); n<N;++n){
            in2[n]=in[n-(M-1)];
        }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int n=0;n<(M-1);++n){
            val_ant64[n]=in[n+(L-M+1)];
        }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        fftw_plan Xdft = fftw_plan_dft_r2c_1d(N,in2,x,FFTW_ESTIMATE);
        fftw_plan Wdft = fftw_plan_dft_r2c_1d(N,h,w,FFTW_ESTIMATE);

        fftw_execute(Xdft);
        fftw_execute(Wdft);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int k=0; k<(N);++k){
            y[k][REAL] = (((x[k][REAL])/N)*((w[k][REAL])/N)-((x[k][IMAG])/N)*((w[k][IMAG])/N));
            y[k][IMAG] = (((x[k][IMAG])/N)*((w[k][REAL])/N)+((x[k][REAL])/N)*((w[k][IMAG])/N));
        }

        fftw_plan idft = fftw_plan_dft_c2r_1d(N,y,out2,FFTW_ESTIMATE);
        fftw_execute(idft);


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int n=0; n<L;++n){
            aux2[n]=out2[n+M-1];
        }

        for (int n=0; n<L;++n){
            aux2[n]=(aux2[n])*(g64*0.02/N);
            out[n]=static_cast<float>(aux2[n]);//salida
        }
        energia64=Cal_Energias(L,aux2);//innterfaz grafica
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

        fftw_destroy_plan(idft);
        fftw_free(y);
        //delete y;
        y=NULL;
        free(out2);
        free(aux2);
        //delete out2;
        //delete aux2;
        out2=NULL;
        aux2=NULL;

        fftw_destroy_plan(Xdft);
        fftw_destroy_plan(Wdft);
        fftw_free(w);
        fftw_free(x);
        //free(h16k);
        free(h);
        free(in2);
        //delete x;
        //delete w;
        //delete h16k;
        //delete h;
        //delete in2;
        x=NULL;
        w=NULL;
        //h64=NULL;
        h=NULL;
        in2=NULL;
    }

//DECIMO FILTRO
/////////////////////////////////////FILTRO 32 Hz////////////////////////////////////////////////////////////

void controlVolume::filter32(int blockSize, int g32, float *in, float *out, bool inicial){
    int const M=50;
    int const L=blockSize;
    int const N=(L+M-1);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    double h32[N];
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if(inicial){
    const double s1_32=0.010114196347557038;
    const double s2_32=0.010114196347557038;
    const double a21_32=-1.9976882679888277;
    const double b21_32=-1.9033337003343762;
    const double a31_32=0.99772617680134701;
    const double a22_32=-1.9987820532493104;
    const double b22_32=-1.9999999959054762;
    const double a32_32=0.99879273408119773;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    h32[0]=(s1_32*s2_32);
    h32[1]=((s1_32*s2_32)*(b22_32+b21_32))-h32[0]*(a21_32+a22_32);
    h32[2]=((s1_32*s2_32)*(b21_32*b22_32+2))-h32[1]*(a21_32+a22_32)-h32[0]*(a32_32+a21_32*a22_32+a31_32);
    h32[3]=((s1_32*s2_32)*(b22_32+b21_32))-h32[2]*(a21_32+a22_32)-h32[1]*(a32_32+a21_32*a22_32+a31_32)-h32[0]*(a32_32*a21_32+a22_32*a31_32);
    h32[4]=(s1_32*s2_32)+h32[3]*(-a21_32-a22_32)+h32[2]*(-a32_32-a21_32*a22_32-a31_32)+h32[1]*(-a32_32*a21_32-a22_32*a31_32)+h32[0]*(-a32_32*a31_32);
    for (int n=5; n<M;++n){
        h32[n]=h32[n-1]*(-a21_32-a22_32)+h32[n-2]*(-a32_32-a21_32*a22_32-a31_32)+h32[n-3]*(-a32_32*a21_32-a22_32*a31_32)+h32[n-4]*(-a32_32*a31_32);
    }
    }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    fftw_complex *x;
    fftw_complex *w;
    fftw_complex *y;
    x = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N); // Se define el tamano del puntero de salida
    w = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
    y = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    double *in2 = new double[N];
    double *out2 = new double[N];
    double *aux2 = new double[L];
    double *h = new double[N];

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int n=0; n<M;++n){
            h[n]=h32[n];
        }

        for (int n=M;n<N;++n){
            h[n]=0;
        }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (inicial){
            for (int n=0;n<(M-1);++n){
                in2[n]=0;
            }
        }
        else{
            for (int n=0;n<(M-1);++n){
                in2[n]=val_ant32[n];
            }
        }

        for (int n=(M-1); n<N;++n){
            in2[n]=in[n-(M-1)];
        }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int n=0;n<(M-1);++n){
            val_ant32[n]=in[n+(L-M+1)];
        }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        fftw_plan Xdft = fftw_plan_dft_r2c_1d(N,in2,x,FFTW_ESTIMATE);
        fftw_plan Wdft = fftw_plan_dft_r2c_1d(N,h,w,FFTW_ESTIMATE);

        fftw_execute(Xdft);
        fftw_execute(Wdft);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int k=0; k<(N);++k){
            y[k][REAL] = (((x[k][REAL])/N)*((w[k][REAL])/N)-((x[k][IMAG])/N)*((w[k][IMAG])/N));
            y[k][IMAG] = (((x[k][IMAG])/N)*((w[k][REAL])/N)+((x[k][REAL])/N)*((w[k][IMAG])/N));
        }

        fftw_plan idft = fftw_plan_dft_c2r_1d(N,y,out2,FFTW_ESTIMATE);
        fftw_execute(idft);


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (int n=0; n<L;++n){
            aux2[n]=out2[n+M-1];
        }

        for (int n=0; n<L;++n){
            aux2[n]=(aux2[n])*(g32*0.02/N);
        }
        for (int n=0; n<L;++n){
            out[n]=(aux2[n])*(g32*0.02/N);
            out[n]=static_cast<float>(aux2[n]);//SALIDA
        }
        energia32=Cal_Energias(L,aux2);//INTERFAZ GRAFICA
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

        fftw_destroy_plan(idft);
        fftw_free(y);
        //delete y;
        y=NULL;
        free(out2);
        free(aux2);
        //delete out2;
        //delete aux2;
        out2=NULL;
        aux2=NULL;

        fftw_destroy_plan(Xdft);
        fftw_destroy_plan(Wdft);
        fftw_free(w);
        fftw_free(x);
        //free(h16k);
        free(h);
        free(in2);
        //delete x;
        //delete w;
        //delete h16k;
        //delete h;
        //delete in2;
        x=NULL;
        w=NULL;
        //h32=NULL;
        h=NULL;
        in2=NULL;
    }

//PROGRAMA PRINCIPAL QUE LLAMA A TODOS LOS FITLROS, LOS EJECUTA Y LO SUMA EN UN UNICO VECTOR DE SALIDA

/////////////////////////////////////Banco de Filtros/////////////////////////////////////////////////////////
void controlVolume::BancoFiltros(int blockSize, int volumeGain, int g32, int g64, int g125, int g250, int g500, int g1k, int g2k, int g4k, int g8k, int g16k, bool inicial, float *in, float *out){

    //VARIABLES TEMPORALES
    float *tmp16k=new float[blockSize];
    float *tmp8k=new float[blockSize];
    float *tmp4k=new float[blockSize];
    float *tmp2k=new float[blockSize];
    float *tmp1k=new float[blockSize];
    float *tmp500=new float[blockSize];
    float *tmp250=new float[blockSize];
    float *tmp125=new float[blockSize];
    float *tmp64=new float[blockSize];
    float *tmp32=new float[blockSize];

    //BANCO DE FILTROS
    filter32(blockSize,g32,in,tmp32,inicial);
    filter64(blockSize,g64,in,tmp64,inicial);
    filter125(blockSize,g125,in,tmp125,inicial);
    filter250(blockSize,g250,in,tmp250,inicial);
    filter500(blockSize,g500,in,tmp500,inicial);
    filter1k(blockSize,g1k,in,tmp1k,inicial);
    filter2k(blockSize,g2k,in,tmp2k,inicial);
    filter4k(blockSize,g4k,in,tmp4k,inicial);
    filter8k(blockSize,g8k,in,tmp8k,inicial);
    filter16k(blockSize,g16k,in,tmp16k,inicial);


    for(int n=0;n<blockSize;++n){
        out[n]=(0.02)*(volumeGain)*(tmp32[n]+tmp64[n]+tmp125[n]+tmp250[n]+tmp500[n]+tmp1k[n]+tmp2k[n]+tmp4k[n]+tmp8k[n]+tmp16k[n]);

        //SALIDA
    }

    //LIBERACION PARA EVITAR CONFLICTOS EN MEMORIA
    free(tmp16k);
    free(tmp8k);
    free(tmp4k);
    free(tmp2k);
    free(tmp1k);
    free(tmp500);
    free(tmp250);
    free(tmp125);
    free(tmp64);
    free(tmp32);

    tmp16k=NULL;
    tmp8k=NULL;
    tmp4k=NULL;
    tmp2k=NULL;
    tmp1k=NULL;
    tmp500=NULL;
    tmp250=NULL;
    tmp125=NULL;
    tmp64=NULL;
    tmp32=NULL;
}


//IMPLEMENTQCION DE LA INTERFAZ GRAFICA, BASICAMENTE LO QUE SE REALIZA ES EL CALCULO DE LA ENERGIA DEL ESPECTRO DE SALIDA DEL
//FILTRO PARA QUE SEA UTILIZADA EN LAS VARIACIONES DE LA INTERFAZ
/////////////////////////////////////Energias////////////////////////////////////////////////////////////////

//FUNCION QYE PERMITE OBTENER LA ENERGIA DE UNA SEÑAL DE ENTRADA

int controlVolume::Cal_Energias(int blockSize,double *input){
    int N;
    N=blockSize;
    double outtotal=0;

    fftw_complex *out;
    out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
    fftw_plan my_plan;
    my_plan =fftw_plan_dft_r2c_1d(N, input, out,FFTW_ESTIMATE);
    fftw_execute(my_plan);


    for(int i=0;i<(N/2)+1;i++)
    {
        outtotal=outtotal+((pow(out[i][0],2)+pow(out[i][1],2))/N);
    }
    outtotal=10*(2*outtotal-((pow(out[N/2][0],2)+pow(out[N/2][1],2))/N));
    fftw_destroy_plan(my_plan);
    fftw_free(out);

    return static_cast<int>(outtotal);

}

//CALCULO DE LAS ENERGIAS DE TODOS LOS FILTROS

int controlVolume::energia1_(){
    int salida=0;
    salida=energia32+0.2*E1;
    E1=salida;
    return salida;
}

int controlVolume::energia2_(){
    int salida=0;
    salida=energia64+0.2*E2;
    E2=salida;
    return salida;

}

int controlVolume::energia3_(){
    int salida=0;
    salida=energia125+0.2*E3;
    E3=salida;
    return salida;
}

int controlVolume::energia4_(){

    int salida=0;
    salida=energia250+0.2*E4;
    E4=salida;
    return salida;
}

int controlVolume::energia5_(){
    int salida=0;
    salida=energia500+0.2*E5;
    E5=salida;
    return salida;
}

int controlVolume::energia6_(){
    int salida=0;
    salida=energia1k+0.2*E6;
    E6=salida;
    return salida;
}

int controlVolume::energia7_(){
    int salida=0;
    salida=energia2k+0.2*E7;
    E7=salida;
    return salida;
}

int controlVolume::energia8_(){
    int salida=0;
    salida=energia4k+0.2*E8;
    E8=salida;
    return salida;
}

int controlVolume::energia9_(){
    int salida=0;
    salida=energia8k+0.2*E9;
    E9=salida;
    return salida;
}

int controlVolume::energia10_(){
    int salida=0;
    salida=energia16k+0.2*E10;
    E10=salida;
    return salida;
}

//FIN DEL CODIGO
