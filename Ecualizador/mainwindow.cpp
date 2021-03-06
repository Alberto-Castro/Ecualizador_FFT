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
 * \file   mainwindow.cpp
 *         Implements the equalizer H(w) computation
 * \author Pablo Alvarado
 * \date   2010.12.12
 *
 * $Id: equalizer.cpp $
 */


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "jack.h"
#include <string>
#include <qpainter.h>


#undef _DSP_DEBUG
#define _DSP_DEBUG

#ifdef _DSP_DEBUG
#define _debug(x) std::cerr << x
#include <iostream>
#else
#define _debug(x)
#endif


/**
 * Precision used by trimming
 */
const float MainWindow::Epsilon = 0.001;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    verbose_(false),
    dspChanged_(true),
    iniciorep(true),
    value1(25),
    value2(25),
    value3(25),
    value4(25),
    value5(25),
    value6(25),
    value7(25),
    value8(25),
    value9(25),
    value10(25)
{
    ui->setupUi(this);
    /*
     * Set up a timer 4 times in a second to check if the user
     * changed the equalizer values, and if so, then create a new
     * filter response
     */
    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(update()));
    timer_->start(25);

    dsp_ = new dspSystem;
    jack::init(dsp_);

    // parse some command line arguments
    QStringList argv(QCoreApplication::arguments());

    QStringList::const_iterator it(argv.begin());
    while(it!=argv.end()) {
      if ((*it)=="-v" || (*it)=="--verbose") {
        verbose_=true;
      } else if ((*it).indexOf(".wav",0,Qt::CaseInsensitive)>0) {
        ui->fileEdit->setText(*it);
        std::string tmp(qPrintable(*it));
        jack::playAlso(tmp.c_str());
      }
      ++it;
    }

}


MainWindow::~MainWindow()
{
    jack::close();
    delete timer_;
    delete ui;
    delete dsp_;
}

void MainWindow::update() {
    if(dspChanged_){
        _debug("Updating" << std::endl);

        dspChanged_=false;
    }
    
}

void MainWindow::on_volumen_valueChanged(int value){
    if (!dspChanged_){
        dspChanged_=true;
    }
    dsp_->updateVolume(value);
    ;
}

void MainWindow::on_fileButton_clicked() {
  selectedFiles_ =
      QFileDialog::getOpenFileNames(this,
                                   "Select one or more audio files to open",
                                   ui->fileEdit->text(),
                                   "WAV Files (*.wav)");

  if (!selectedFiles_.empty()) {
    ui->fileEdit->setText(*selectedFiles_.begin());

    jack::stopFiles();
    QStringList::iterator it;
    for (it=selectedFiles_.begin();it!=selectedFiles_.end();++it) {

        dsp_->updateinicio(iniciorep);

      std::string tmp(qPrintable(*it));
      jack::playAlso(tmp.c_str());
    }
  }
}

void MainWindow::on_fileEdit_returnPressed() {
  jack::stopFiles();
  std::string tmp(qPrintable(ui->fileEdit->text()));
  if (!tmp.empty()) {
    jack::playAlso(tmp.c_str());
  }
}

void MainWindow::on_Filtro32_valueChanged(int value32)
{
    if (!dspChanged_){
        dspChanged_=true;
    }
    dsp_->updatefiltro32(value32);
    MainWindow::value1=value32;
    this->repaint();
    ;
}

void MainWindow::on_Filtro63_valueChanged(int value63)
{
    if (!dspChanged_){
        dspChanged_=true;
    }
    //dsp_->updatefiltro63(value63);
    MainWindow::value2=value63;
    this->repaint();
    ;
}

void MainWindow::on_Filtro125_valueChanged(int value125)
{
    if (!dspChanged_){
        dspChanged_=true;
    }
    //dsp_->updatefiltro125(value125);
    MainWindow::value3=value125;
    this->repaint();
    ;
}

void MainWindow::on_Filtro250_valueChanged(int value250)
{
    if (!dspChanged_){
        dspChanged_=true;
    }
    //dsp_->updatefiltro250(value250);
    MainWindow::value4=value250;
    this->repaint();
    ;
}

void MainWindow::on_Filtro500_valueChanged(int value500)
{
    if (!dspChanged_){
        dspChanged_=true;
    }
    //dsp_->updatefiltro500(value500);
    MainWindow::value5=value500;
    this->repaint();
    ;
}

void MainWindow::on_Filtro1k_valueChanged(int value1k)
{
    if (!dspChanged_){
        dspChanged_=true;
    }
    //dsp_->updatefiltro1k(value1k);
    MainWindow::value6=value1k;
    this->repaint();
    ;
}

void MainWindow::on_Filtro2k_valueChanged(int value2k)
{
    if (!dspChanged_){
        dspChanged_=true;
    }
    //dsp_->updatefiltro2k(value2k);
    MainWindow::value7=value2k;
    this->repaint();
    ;
}

void MainWindow::on_Filtro4k_valueChanged(int value4k)
{
    if (!dspChanged_){
        dspChanged_=true;
    }
    //dsp_->updatefiltro4k(value4k);
    MainWindow::value8=value4k;
    this->repaint();
    ;
}

void MainWindow::on_Filtro8k_valueChanged(int value8k)
{
    if (!dspChanged_){
        dspChanged_=true;
    }
    //dsp_->updatefiltro8k(value8k);
    MainWindow::value9=value8k;
    this->repaint();
    ;
}

void MainWindow::on_Filtro16k_valueChanged(int value16k)
{
    if (!dspChanged_){
        dspChanged_=true;
    }
    dsp_->updatefiltro16k(value16k);
    MainWindow::value10=value16k;
    this->repaint();
    ;
}

void MainWindow::on_Predeterminado_activated(int index)
{
    int Eq_preset = index;

    if (Eq_preset==0)
    {
        float value1=25;
        float value2=25;
        float value3=25;
        float value4=25;
        float value5=25;
        float value6=25;
        float value7=25;
        float value8=25;
        float value9=25;
        float value10=25;

        //Asignacion del valor del slider de cada filtro
        ui->Filtro32->setValue(value1);
        ui->Filtro63->setValue(value2);
        ui->Filtro125->setValue(value3);
        ui->Filtro250->setValue(value4);
        ui->Filtro500->setValue(value5);
        ui->Filtro1k->setValue(value6);
        ui->Filtro2k->setValue(value7);
        ui->Filtro4k->setValue(value8);
        ui->Filtro8k->setValue(value9);
        ui->Filtro16k->setValue(value10);

    }

    if (Eq_preset==1)
    {
        float value1=25;
        float value2=25;
        float value3=25;
        float value4=25;
        float value5=25;
        float value6=25;
        float value7=16;
        float value8=16;
        float value9=16;
        float value10=13;

        //Asignacion del valor del slider de cada filtro
        ui->Filtro32->setValue(value1);
        ui->Filtro63->setValue(value2);
        ui->Filtro125->setValue(value3);
        ui->Filtro250->setValue(value4);
        ui->Filtro500->setValue(value5);
        ui->Filtro1k->setValue(value6);
        ui->Filtro2k->setValue(value7);
        ui->Filtro4k->setValue(value8);
        ui->Filtro8k->setValue(value9);
        ui->Filtro16k->setValue(value10);
    }

    if (Eq_preset==2)
    {
        float value1=25;
        float value2=25;
        float value3=35;
        float value4=32;
        float value5=32;
        float value6=32;
        float value7=29;
        float value8=25;
        float value9=25;
        float value10=25;

        //Asignacion del valor del slider de cada filtro
        ui->Filtro32->setValue(value1);
        ui->Filtro63->setValue(value2);
        ui->Filtro125->setValue(value3);
        ui->Filtro250->setValue(value4);
        ui->Filtro500->setValue(value5);
        ui->Filtro1k->setValue(value6);
        ui->Filtro2k->setValue(value7);
        ui->Filtro4k->setValue(value8);
        ui->Filtro8k->setValue(value9);
        ui->Filtro16k->setValue(value10);

    }

    if (Eq_preset==3)
    {
        float value1=37;
        float value2=34;
        float value3=28;
        float value4=25;
        float value5=25;
        float value6=18;
        float value7=16;
        float value8=16;
        float value9=25;
        float value10=25;

        //Asignacion del valor del slider de cada filtro
        ui->Filtro32->setValue(value1);
        ui->Filtro63->setValue(value2);
        ui->Filtro125->setValue(value3);
        ui->Filtro250->setValue(value4);
        ui->Filtro500->setValue(value5);
        ui->Filtro1k->setValue(value6);
        ui->Filtro2k->setValue(value7);
        ui->Filtro4k->setValue(value8);
        ui->Filtro8k->setValue(value9);
        ui->Filtro16k->setValue(value10);

    }

    if (Eq_preset==4)
    {
        float value1=34;
        float value2=32;
        float value3=25;
        float value4=15;
        float value5=19;
        float value6=27;
        float value7=35;
        float value8=39;
        float value9=40;
        float value10=40;

        //Asignacion del valor del slider de cada filtro
        ui->Filtro32->setValue(value1);
        ui->Filtro63->setValue(value2);
        ui->Filtro125->setValue(value3);
        ui->Filtro250->setValue(value4);
        ui->Filtro500->setValue(value5);
        ui->Filtro1k->setValue(value6);
        ui->Filtro2k->setValue(value7);
        ui->Filtro4k->setValue(value8);
        ui->Filtro8k->setValue(value9);
        ui->Filtro16k->setValue(value10);

    }

    if (Eq_preset==5)
    {
        float value1=13;
        float value2=13;
        float value3=13;
        float value4=20;
        float value5=28;
        float value6=39;
        float value7=45;
        float value8=45;
        float value9=45;
        float value10=46;

        //Asignacion del valor del slider de cada filtro
        ui->Filtro32->setValue(value1);
        ui->Filtro63->setValue(value2);
        ui->Filtro125->setValue(value3);
        ui->Filtro250->setValue(value4);
        ui->Filtro500->setValue(value5);
        ui->Filtro1k->setValue(value6);
        ui->Filtro2k->setValue(value7);
        ui->Filtro4k->setValue(value8);
        ui->Filtro8k->setValue(value9);
        ui->Filtro16k->setValue(value10);
    }

    if (Eq_preset==6)
    {
        float value1=27;
        float value2=31;
        float value3=34;
        float value4=35;
        float value5=32;
        float value6=25;
        float value7=22;
        float value8=22;
        float value9=27;
        float value10=27;

        //Asignacion del valor del slider de cada filtro
        ui->Filtro32->setValue(value1);
        ui->Filtro63->setValue(value2);
        ui->Filtro125->setValue(value3);
        ui->Filtro250->setValue(value4);
        ui->Filtro500->setValue(value5);
        ui->Filtro1k->setValue(value6);
        ui->Filtro2k->setValue(value7);
        ui->Filtro4k->setValue(value8);
        ui->Filtro8k->setValue(value9);
        ui->Filtro16k->setValue(value10);
    }

    if (Eq_preset==7)
    {
        float value1=25;
        float value2=25;
        float value3=25;
        float value4=18;
        float value5=25;
        float value6=33;
        float value7=33;
        float value8=25;
        float value9=25;
        float value10=25;

        //Asignacion del valor del slider de cada filtro
        ui->Filtro32->setValue(value1);
        ui->Filtro63->setValue(value2);
        ui->Filtro125->setValue(value3);
        ui->Filtro250->setValue(value4);
        ui->Filtro500->setValue(value5);
        ui->Filtro1k->setValue(value6);
        ui->Filtro2k->setValue(value7);
        ui->Filtro4k->setValue(value8);
        ui->Filtro8k->setValue(value9);
        ui->Filtro16k->setValue(value10);
    }

    if (Eq_preset==8)
    {
        float value1=35;
        float value2=31;
        float value3=18;
        float value4=15;
        float value5=21;
        float value6=30;
        float value7=36;
        float value8=39;
        float value9=39;
        float value10=39;

        //Asignacion del valor del slider de cada filtro
        ui->Filtro32->setValue(value1);
        ui->Filtro63->setValue(value2);
        ui->Filtro125->setValue(value3);
        ui->Filtro250->setValue(value4);
        ui->Filtro500->setValue(value5);
        ui->Filtro1k->setValue(value6);
        ui->Filtro2k->setValue(value7);
        ui->Filtro4k->setValue(value8);
        ui->Filtro8k->setValue(value9);
        ui->Filtro16k->setValue(value10);
    }

    if (Eq_preset==9)
    {
        float value1=35;
        float value2=32;
        float value3=25;
        float value4=18;
        float value5=19;
        float value6=25;
        float value7=35;
        float value8=37;
        float value9=37;
        float value10=36;

        //Asignacion del valor del slider de cada filtro
        ui->Filtro32->setValue(value1);
        ui->Filtro63->setValue(value2);
        ui->Filtro125->setValue(value3);
        ui->Filtro250->setValue(value4);
        ui->Filtro500->setValue(value5);
        ui->Filtro1k->setValue(value6);
        ui->Filtro2k->setValue(value7);
        ui->Filtro4k->setValue(value8);
        ui->Filtro8k->setValue(value9);
        ui->Filtro16k->setValue(value10);
    }
}


void MainWindow::paintEvent(QPaintEvent *e)//funcion encargada de graficar el nivel de ganancia
{
    QPoint begin,Ctrl0,Ctrl1,Ctrl2,Ctrl3,Ctrl4,Ctrl5,Ctrl6,Ctrl7,Ctrl8,Ctrl9;//QPoint es una clase que define un par ordenado
    QPoint end0,end1,end2,end3,end4,end5,end6,end7,end8,end9;
    int Posy = 150; // posicion en y del total de la grafica, en otras palabras desplaza la grafia de arriba y abajo
    int ofsetYC = 275; // desplaza las maximos y minimos de la funcion sinuseidal
    int ofsetX = 150; // posicion en x del total de la grafica, en otras palabras desplaza la grafia de derecha a izquierda
    int scale = -5;  // escala las maximos y minimos de la funcion sinuseidal

    //coordenadas en la ventana para colocar la grafica
    begin.setX(-5+ofsetX);
    begin.setY(Posy);

    Ctrl0.setX(30+ofsetX);
    Ctrl0.setY(value1*scale + ofsetYC);
    end0.setX(65+ofsetX);
    end0.setY(Posy);

    Ctrl1.setX(100+ofsetX);
    Ctrl1.setY(value2*scale + ofsetYC);
    end1.setX(135+ofsetX);
    end1.setY(Posy);

    Ctrl2.setX(170+ofsetX);
    Ctrl2.setY(value3*scale + ofsetYC);
    end2.setX(205+ofsetX);
    end2.setY(Posy);

    Ctrl3.setX(240+ofsetX);
    Ctrl3.setY(value4*scale + ofsetYC);
    end3.setX(275+ofsetX);
    end3.setY(Posy);

    Ctrl4.setX(310+ofsetX);
    Ctrl4.setY(value5*scale + ofsetYC);
    end4.setX(345+ofsetX);
    end4.setY(Posy);

    Ctrl5.setX(380+ofsetX);
    Ctrl5.setY(value6*scale + ofsetYC);
    end5.setX(415+ofsetX);
    end5.setY(Posy);

    Ctrl6.setX(450+ofsetX);
    Ctrl6.setY(value7*scale + ofsetYC);
    end6.setX(485+ofsetX);
    end6.setY(Posy);

    Ctrl7.setX(520+ofsetX);
    Ctrl7.setY(value8*scale + ofsetYC);
    end7.setX(555+ofsetX);
    end7.setY(Posy);

    Ctrl8.setX(590+ofsetX);
    Ctrl8.setY(value9*scale + ofsetYC);
    end8.setX(625+ofsetX);
    end8.setY(Posy);

    Ctrl9.setX(660+ofsetX);
    Ctrl9.setY(value10*scale + ofsetYC);
    end9.setX(695+ofsetX);
    end9.setY(Posy);



    // se declara la clase QPainterPath encargada de generar curvas entre otras figuras
    QPainterPath myPath,myPath1,myPath2,myPath3,myPath4,myPath5,myPath6,myPath7,myPath8,myPath9;
    myPath.moveTo(begin);//mueve el punto de inicio de la grafica
    myPath.quadTo(Ctrl0,end0); // quadTo es el metodo encargado de generar una curva sinuseidal, Ctrl mueve la cresta y end determina el punto final

    myPath1.moveTo(end0);
    myPath1.quadTo(Ctrl1,end1);

    myPath2.moveTo(end1);
    myPath2.quadTo(Ctrl2,end2);

    myPath3.moveTo(end2);
    myPath3.quadTo(Ctrl3,end3);

    myPath4.moveTo(end3);
    myPath4.quadTo(Ctrl4,end4);

    myPath5.moveTo(end4);
    myPath5.quadTo(Ctrl5,end5);

    myPath6.moveTo(end5);
    myPath6.quadTo(Ctrl6,end6);

    myPath7.moveTo(end6);
    myPath7.quadTo(Ctrl7,end7);

    myPath8.moveTo(end7);
    myPath8.quadTo(Ctrl8,end8);

    myPath9.moveTo(end8);
    myPath9.quadTo(Ctrl9,end9);

    QLinearGradient myGradient; //genera un sombreado debajo de la grafica
    myGradient.setColorAt(0,QColor(57, 110, 141,200)); //le da el color deseado en formato RGB

    QPen myPen; // tipo de lines
    myPen.setColor(QColor(57, 110, 141));//color
    myPen.setWidth(2); // grueso


    QPainter painter(this); // Clase para pintar lo descrito anteriormente
    painter.setBrush(myGradient);
    painter.setPen(myPen);

    painter.drawPath(myPath);
    painter.drawPath(myPath1);
    painter.drawPath(myPath2);
    painter.drawPath(myPath3);
    painter.drawPath(myPath4);
    painter.drawPath(myPath5);
    painter.drawPath(myPath6);
    painter.drawPath(myPath7);
    painter.drawPath(myPath8);
    painter.drawPath(myPath9);

}
