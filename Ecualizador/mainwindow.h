/*
 * DSP Example is part of the DSP Lecture at TEC-Costa Rica
 * Copyright (C) 2010  Pablo Alvarado
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
 * \file   mainwindow.h
 *         Implements the equalizer H(w) computation
 * \author Pablo Alvarado/Jose Miguel Barboza
 * \date   2010.12.12/2017.05.26
 *
 * $Id: mainwindow.h $
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QFileDialog>

#include "dspsystem.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    int volumeGain;
    /**
      *Tolerance value
      */
    static const float Epsilon;

     /**
      * Verbose flag
      */
     bool verbose_;

     /**
      * Timer used to recompute the filter once the user changes the
      * values
      */
     QTimer *timer_;

     /**
      * List of selected files so far
      */
     QStringList selectedFiles_;

     /**
      * Pointer to an inherited class of processor, which does
      * all the real work.
      */
     dspSystem* dsp_;

     /**
      *DSP change
      */
     bool dspChanged_;
     bool iniciorep;
     int value1;
     int value2;
     int value3;
     int value4;
     int value5;
     int value6;
     int value7;
     int value8;
     int value9;
     int value10;

protected:
     void paintEvent(QPaintEvent *e);
     
   private slots:
     void on_fileEdit_returnPressed();
     void on_fileButton_clicked();
     void on_volumen_valueChanged(int value);


     void update();

     void on_Filtro32_valueChanged(int value32);
     void on_Filtro63_valueChanged(int value63);
     void on_Filtro125_valueChanged(int value125);
     void on_Filtro250_valueChanged(int value250);
     void on_Filtro500_valueChanged(int value500);
     void on_Filtro1k_valueChanged(int value1k);
     void on_Filtro2k_valueChanged(int value2k);
     void on_Filtro4k_valueChanged(int value4k);
     void on_Filtro8k_valueChanged(int value8k);
     void on_Filtro16k_valueChanged(int value16k);
     void on_Predeterminado_activated(int index);
};




#endif // MAINWINDOW_H
