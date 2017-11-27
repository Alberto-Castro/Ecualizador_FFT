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
    dspChanged_(true)
{
    ui->setupUi(this);
    /*
     * Set up a timer 4 times in a second to check if the user
     * changed the equalizer values, and if so, then create a new
     * filter response
     */
    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(update()));
    timer_->start(250);

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

void MainWindow::on_volumeSlider_valueChanged(int value){
    if (!dspChanged_){
        dspChanged_=true;
    }
    //dsp_->updateVolume(value);
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
    dsp_->updateVolume(value32);
    this->repaint();
    ;
}

void MainWindow::on_Filtro63_valueChanged(int value63)
{
    if (!dspChanged_){
        dspChanged_=true;
    }
    //dsp_->updateVolume(value63);
    this->repaint();
    ;
}

void MainWindow::on_Filtro125_valueChanged(int value125)
{
    if (!dspChanged_){
        dspChanged_=true;
    }
    //dsp_->updateVolume(value125);
    this->repaint();
    ;
}

void MainWindow::on_Filtro250_valueChanged(int value250)
{
    if (!dspChanged_){
        dspChanged_=true;
    }
    //dsp_->updateVolume(value250);
    this->repaint();
    ;
}

void MainWindow::on_Filtro500_valueChanged(int value500)
{
    if (!dspChanged_){
        dspChanged_=true;
    }
    //dsp_->updateVolume(value500);
    this->repaint();
    ;
}

void MainWindow::on_Filtro1k_valueChanged(int value1k)
{
    if (!dspChanged_){
        dspChanged_=true;
    }
    //dsp_->updateVolume(value1k);
    this->repaint();
    ;
}

void MainWindow::on_Filtro2k_valueChanged(int value2k)
{
    if (!dspChanged_){
        dspChanged_=true;
    }
    //dsp_->updateVolume(value2k);
    this->repaint();
    ;
}

void MainWindow::on_Filtro4k_valueChanged(int value4k)
{
    if (!dspChanged_){
        dspChanged_=true;
    }
    //dsp_->updateVolume(value4k);
    this->repaint();
    ;
}

void MainWindow::on_Filtro8k_valueChanged(int value8k)
{
    if (!dspChanged_){
        dspChanged_=true;
    }
    //dsp_->updateVolume(value8k);
    this->repaint();
    ;
}

void MainWindow::on_Filtro16k_valueChanged(int value16k)
{
    if (!dspChanged_){
        dspChanged_=true;
    }
    //dsp_->updateVolume(value16k);
    this->repaint();
    ;
}

void MainWindow::on_Predetermiado_activated(int index)
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
