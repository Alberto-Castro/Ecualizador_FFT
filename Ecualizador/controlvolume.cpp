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
    for (int n=0; n<blockSize;++n){
        out[n]=(volumeGain)*in[n]*0.02;
    }
}

void controlVolume::filter32(int blockSize, int g32, float *in, float *out){
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
        out[n]=(out2[n]/1024);
    }

    fftw_destroy_plan(dft);
    fftw_destroy_plan(idft);
    fftw_free(x);
    free(out2);
    free(in2);
}
