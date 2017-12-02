/*
 * DSP Example is part of the DSP Lecture at TEC-Costa Rica
 * Copyright (C) 2017  Jose Miguel Barboza
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
 * \file   controlVolume.h
 *         Implements control volumen in the time domain
 * \author Jose Miguel Barboza
 * \date   2017.31.05
 *
 * $Id: controlVolume.cpp $
 */


#ifndef CONTROLVOLUME_H
#define CONTROLVOLUME_H

/**
 * Control Volume class
 *
 * This filter has control in the volume gain of the signal.
 *
 * The filter follows the difference equation
 * \f[
 * y(n)=\cvGain x(n)
 * \f]
 */

class controlVolume {
public:
    /**
     * Constructor
     */
    controlVolume();

    /**
     * Destructor
     */
    ~controlVolume();

   /**
    * Filter the in buffer and leave the result in out
    */

   float val_ant16k[19];
   float val_ant8k[34];
   float val_ant4k[74];
   float val_ant2k[199];
   float val_ant1k[299];
   float val_ant500[549];
   float val_ant250[949];
   float val_ant125[1799];
   float val_ant64[3499];
   float val_ant32[6999];

   void BancoFiltros(int blockSize,
                     int volumeGain,int g32,int g64,int g125,int g250,int g500,int g1k,int g2k,int g4k,int g8k,int g16k,bool inicial,
                     float *in,
                     float *out);

   void filter(int blockSize,
               int volumeGain,
               float* in,
               float* out);

   void filter32(int blockSize,
                 int g32,
                 float* in,
                 float* out,
                 bool inicial);

   void filter64(int blockSize,
                 int g64,
                 float* in,
                 float* out,
                 bool inicial);

   void filter125(int blockSize,
                 int g125,
                 float* in,
                 float* out,
                 bool inicial);

   void filter250(int blockSize,
                  int g250,
                  float* in,
                  float* out,
                  bool inicial);

   void filter500(int blockSize,
                 int g500,
                 float* in,
                 float* out,
                 bool inicial);

   void filter1k(int blockSize,
                  int g1k,
                  float* in,
                  float* out,
                  bool inicial);

   void filter2k(int blockSize,
                  int g2k,
                  float* in,
                  float* out,
                  bool inicial);


   void filter4k(int blockSize,
                  int g4k,
                  float* in,
                  float* out,
                  bool inicial);


   void filter8k(int blockSize,
                  int g8k,
                  float* in,
                  float* out,
                  bool inicial);

   void filter16k(int blockSize,
                  int g16k,
                  float* in,
                  float* out,
                  bool inicial);

};


#endif // CONTROLVOLUME_H
