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
 *         Implements control volumen in the time domain
 * \author Jose Miguel Barboza
 * \date   2017.31.05
 *
 * $Id: controlVolume.cpp $
 */

#include "controlvolume.h"
#include <cmath>
#include <fftw3.h>


#include <iostream>
#include <fstream>

#define REAL 0
#define IMAG 1

/*
 * Constructor
 */

controlVolume::controlVolume(){

}


/*
 * Destructor
 */

controlVolume::~controlVolume(){

}


/*
 * Init the filter operation
 */

void controlVolume::filter(int blockSize, int volumeGain, float *in, float *out){
    /*for (int n=0; n<blockSize;++n){
        out[n]=in[n]*volumeGain*0.02;
    }*/
    int N = blockSize;
    fftw_complex *x;
    x = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N); // Se definie el tamano del puntero de salida

    double *in2;
    double *out2;
    out2 = (double *) malloc(N*sizeof(double));
    in2 = (double *) malloc(N*sizeof(double));

    for (int n=0; n<N;++n){
        in2[n]=(volumeGain)*in[n]*0.02;
    }

    fftw_plan dft = fftw_plan_dft_r2c_1d(N,in2,x,FFTW_ESTIMATE);
    fftw_plan idft = fftw_plan_dft_c2r_1d(N,x,out2,FFTW_ESTIMATE);

    fftw_execute(dft);
    fftw_execute(idft);

    for (int n=0; n<N;++n){
        out[n]=((static_cast<float>(out2[n]))/N);
    }

    fftw_destroy_plan(dft);
    fftw_destroy_plan(idft);
    fftw_free(x);
    free(out2);
    free(in2);
    //delete out2;
    //delete in2;
    //out2=NULL;
    //in2=NULL;
}

void controlVolume::filter16k(int blockSize, int g16k, float *in, float *out, bool inicial){
    int const M=20;
    int const L=blockSize;
    int const N=(L+M-1);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    double *h16k = new double[N];
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    const double s1=0.54115569484630355;
    const double s2=0.54115569484630355;
    const double a21=-1.9929062330338988;
    const double b21=-1.9995547795128696;
    const double a31=0.99295230173619742;
    const double a22=-0.0072420371320655319;
    const double b22=-1.9999999947970359;
    const double a32=0.31569379867829822;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    h16k[0]=(s1*s2);
    h16k[1]=((s1*s2)*(b22+b21))-h16k[0]*(a21+a22);
    h16k[2]=((s1*s2)*(b21*b22+2))-h16k[1]*(a21+a22)-h16k[0]*(a32+a21*a22+a31);
    h16k[3]=((s1*s2)*(b22+b21))-h16k[2]*(a21+a22)-h16k[1]*(a32+a21*a22+a31)-h16k[0]*(a32*a21+a22*a31);
    h16k[4]=(s1*s2)-h16k[3]*(-a21-a22)+h16k[2]*(-a32-a21*a22-a31)+h16k[1]*(-a32*a21-a22*a31)+h16k[0]*(-a32*a31);
    for (int n=5; n<M;++n){
        h16k[n]=-h16k[n-1]*(-a21-a22)+h16k[n-2]*(-a32-a21*a22-a31)+h16k[n-3]*(-a32*a21-a22*a31)+h16k[n-4]*(-a32*a31);
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
        h[n]=h16k[n];
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
            in2[n]=val_ant16k[n];
        }
    }

    for (int n=(M-1); n<N;++n){
        in2[n]=in[n];
    }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (int n=0;n<(M-1);++n){
        val_ant16k[n]=in[n+(L-M+1)];
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fftw_plan Xdft = fftw_plan_dft_r2c_1d(N,in2,x,FFTW_ESTIMATE);
    fftw_plan Wdft = fftw_plan_dft_r2c_1d(N,h,w,FFTW_ESTIMATE);

    fftw_execute(Xdft);
    fftw_execute(Wdft);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (int k=0; k<(N);++k){
        y[k][REAL] = ((x[k][REAL])*(w[k][REAL])-(x[k][IMAG])*(w[k][IMAG]));
        y[k][IMAG] = ((x[k][IMAG])*(w[k][REAL])+(x[k][REAL])*(w[k][IMAG]));
    }

    fftw_plan idft = fftw_plan_dft_c2r_1d(N,y,out2,FFTW_ESTIMATE);
    fftw_execute(idft);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (int n=0; n<L;++n){
        aux2[n]=out2[n+M-1];
    }

    for (int n=0; n<L;++n){
        out[n]=((static_cast<float>(aux2[n]))*(g16k*0.02/N));
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

    fftw_destroy_plan(idft);
    fftw_free(y);
    //delete y;
    //y=NULL;
    free(out2);
    free(aux2);
    //delete out2;
    //delete aux2;
    //out2=NULL;
    //aux2=NULL;

    fftw_destroy_plan(Xdft);
    fftw_destroy_plan(Wdft);
    fftw_free(w);
    fftw_free(x);
    free(h16k);
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
    //h=NULL;
    //in2=NULL;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void controlVolume::filter32(int blockSize, int g32, float *in, float *out, bool inicial){
    int N = blockSize;
    fftw_complex *x;
    x = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N); // Se definie el tamano del puntero de salida

    double *in2;
    double *out2;
    out2 = (double *) malloc(N*sizeof(double));
    in2 = (double *) malloc(N*sizeof(double));

    for (int n=0; n<N;++n){
        in2[n]=(g32)*in[n]*0.02;
    }

    fftw_plan dft = fftw_plan_dft_r2c_1d(N,in2,x,FFTW_ESTIMATE);
    fftw_plan idft = fftw_plan_dft_c2r_1d(N,x,out2,FFTW_ESTIMATE);

    fftw_execute(dft);
    fftw_execute(idft);

    for (int n=0; n<N;++n){
        out[n]=((static_cast<float>(out2[n]))/N);
    }

    fftw_destroy_plan(dft);
    fftw_destroy_plan(idft);
    fftw_free(x);
    free(out2);
    free(in2);
    //delete out2;
    //delete in2;
    //out2=NULL;
    //in2=NULL;
}

void controlVolume::BancoFiltros(int blockSize, int volumeGain, int g32, int g64, int g125, int g250, int g500, int g1k, int g2k, int g4k, int g8k, int g16k, bool inicial, float *in, float *out){
    float *tmp16k=new float[blockSize];
    float *tmp32=new float[blockSize];

    filter32(blockSize,g32,in,tmp32,inicial);
    filter16k(blockSize,g16k,in,tmp16k,inicial);

    for(int n=0;n<blockSize;++n){
        out[n]=(0.02)*(volumeGain)*(tmp32[n]+tmp16k[n]);//+tmp2[n]+tmp3[n]+tmp4[n]+tmp5[n]+tmp6[n]+tmp7[n]+tmp9[n]+tmp8[n]);

    }

    delete tmp16k;
    delete tmp32;
}
