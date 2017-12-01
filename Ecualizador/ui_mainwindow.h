/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLineEdit *fileEdit;
    QToolButton *fileButton;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *Volumen;
    QSlider *volumen;
    QLabel *volume;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *Filtros;
    QSlider *Filtro32;
    QSlider *Filtro63;
    QSlider *Filtro125;
    QSlider *Filtro250;
    QSlider *Filtro500;
    QSlider *Filtro1k;
    QSlider *Filtro2k;
    QSlider *Filtro4k;
    QSlider *Filtro8k;
    QSlider *Filtro16k;
    QWidget *horizontalLayoutWidget_3;
    QHBoxLayout *Nombrefiltros;
    QLabel *F32;
    QLabel *F63;
    QLabel *F125;
    QLabel *F250;
    QLabel *F500;
    QLabel *F1k;
    QLabel *F2k;
    QLabel *F4k;
    QLabel *F8k;
    QLabel *F16k;
    QComboBox *Predeterminado;
    QMenuBar *menuBar;
    QMenu *menuEcualizador;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(947, 569);
        MainWindow->setStyleSheet(QStringLiteral("background-color: rgb(146, 149, 166);"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 10, 901, 51));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(3);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        fileEdit = new QLineEdit(verticalLayoutWidget);
        fileEdit->setObjectName(QStringLiteral("fileEdit"));
        fileEdit->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"gridline-color: rgb(42, 99, 255);"));

        horizontalLayout->addWidget(fileEdit);

        fileButton = new QToolButton(verticalLayoutWidget);
        fileButton->setObjectName(QStringLiteral("fileButton"));
        fileButton->setStyleSheet(QStringLiteral("background-color: rgb(42, 99, 255);"));

        horizontalLayout->addWidget(fileButton);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayoutWidget = new QWidget(centralWidget);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 60, 41, 391));
        Volumen = new QHBoxLayout(horizontalLayoutWidget);
        Volumen->setSpacing(6);
        Volumen->setContentsMargins(11, 11, 11, 11);
        Volumen->setObjectName(QStringLiteral("Volumen"));
        Volumen->setContentsMargins(0, 0, 0, 0);
        volumen = new QSlider(horizontalLayoutWidget);
        volumen->setObjectName(QStringLiteral("volumen"));
        volumen->setStyleSheet(QStringLiteral(""));
        volumen->setMaximum(50);
        volumen->setOrientation(Qt::Vertical);
        volumen->setTickPosition(QSlider::TicksBothSides);

        Volumen->addWidget(volumen);

        volume = new QLabel(centralWidget);
        volume->setObjectName(QStringLiteral("volume"));
        volume->setGeometry(QRect(10, 450, 67, 17));
        volume->setStyleSheet(QLatin1String("font: 10pt \"Latin Modern Mono Prop\";\n"
"color: rgb(42, 99, 255);"));
        horizontalLayoutWidget_2 = new QWidget(centralWidget);
        horizontalLayoutWidget_2->setObjectName(QStringLiteral("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(60, 230, 751, 221));
        Filtros = new QHBoxLayout(horizontalLayoutWidget_2);
        Filtros->setSpacing(6);
        Filtros->setContentsMargins(11, 11, 11, 11);
        Filtros->setObjectName(QStringLiteral("Filtros"));
        Filtros->setContentsMargins(0, 0, 0, 0);
        Filtro32 = new QSlider(horizontalLayoutWidget_2);
        Filtro32->setObjectName(QStringLiteral("Filtro32"));
        Filtro32->setStyleSheet(QStringLiteral(""));
        Filtro32->setMaximum(50);
        Filtro32->setOrientation(Qt::Vertical);
        Filtro32->setTickPosition(QSlider::TicksBothSides);

        Filtros->addWidget(Filtro32);

        Filtro63 = new QSlider(horizontalLayoutWidget_2);
        Filtro63->setObjectName(QStringLiteral("Filtro63"));
        Filtro63->setStyleSheet(QStringLiteral(""));
        Filtro63->setMaximum(50);
        Filtro63->setOrientation(Qt::Vertical);
        Filtro63->setTickPosition(QSlider::TicksBothSides);

        Filtros->addWidget(Filtro63);

        Filtro125 = new QSlider(horizontalLayoutWidget_2);
        Filtro125->setObjectName(QStringLiteral("Filtro125"));
        Filtro125->setStyleSheet(QStringLiteral(""));
        Filtro125->setMaximum(50);
        Filtro125->setOrientation(Qt::Vertical);
        Filtro125->setTickPosition(QSlider::TicksBothSides);

        Filtros->addWidget(Filtro125);

        Filtro250 = new QSlider(horizontalLayoutWidget_2);
        Filtro250->setObjectName(QStringLiteral("Filtro250"));
        Filtro250->setStyleSheet(QStringLiteral(""));
        Filtro250->setMaximum(50);
        Filtro250->setOrientation(Qt::Vertical);
        Filtro250->setTickPosition(QSlider::TicksBothSides);

        Filtros->addWidget(Filtro250);

        Filtro500 = new QSlider(horizontalLayoutWidget_2);
        Filtro500->setObjectName(QStringLiteral("Filtro500"));
        Filtro500->setStyleSheet(QStringLiteral(""));
        Filtro500->setMaximum(50);
        Filtro500->setOrientation(Qt::Vertical);
        Filtro500->setTickPosition(QSlider::TicksBothSides);

        Filtros->addWidget(Filtro500);

        Filtro1k = new QSlider(horizontalLayoutWidget_2);
        Filtro1k->setObjectName(QStringLiteral("Filtro1k"));
        Filtro1k->setStyleSheet(QStringLiteral(""));
        Filtro1k->setMaximum(50);
        Filtro1k->setOrientation(Qt::Vertical);
        Filtro1k->setTickPosition(QSlider::TicksBothSides);

        Filtros->addWidget(Filtro1k);

        Filtro2k = new QSlider(horizontalLayoutWidget_2);
        Filtro2k->setObjectName(QStringLiteral("Filtro2k"));
        Filtro2k->setStyleSheet(QStringLiteral(""));
        Filtro2k->setMaximum(50);
        Filtro2k->setOrientation(Qt::Vertical);
        Filtro2k->setTickPosition(QSlider::TicksBothSides);

        Filtros->addWidget(Filtro2k);

        Filtro4k = new QSlider(horizontalLayoutWidget_2);
        Filtro4k->setObjectName(QStringLiteral("Filtro4k"));
        Filtro4k->setStyleSheet(QStringLiteral(""));
        Filtro4k->setMaximum(50);
        Filtro4k->setOrientation(Qt::Vertical);
        Filtro4k->setTickPosition(QSlider::TicksBothSides);

        Filtros->addWidget(Filtro4k);

        Filtro8k = new QSlider(horizontalLayoutWidget_2);
        Filtro8k->setObjectName(QStringLiteral("Filtro8k"));
        Filtro8k->setStyleSheet(QStringLiteral(""));
        Filtro8k->setMaximum(50);
        Filtro8k->setOrientation(Qt::Vertical);
        Filtro8k->setTickPosition(QSlider::TicksBothSides);

        Filtros->addWidget(Filtro8k);

        Filtro16k = new QSlider(horizontalLayoutWidget_2);
        Filtro16k->setObjectName(QStringLiteral("Filtro16k"));
        Filtro16k->setStyleSheet(QStringLiteral(""));
        Filtro16k->setMaximum(50);
        Filtro16k->setOrientation(Qt::Vertical);
        Filtro16k->setTickPosition(QSlider::TicksBothSides);

        Filtros->addWidget(Filtro16k);

        horizontalLayoutWidget_3 = new QWidget(centralWidget);
        horizontalLayoutWidget_3->setObjectName(QStringLiteral("horizontalLayoutWidget_3"));
        horizontalLayoutWidget_3->setGeometry(QRect(90, 450, 711, 31));
        Nombrefiltros = new QHBoxLayout(horizontalLayoutWidget_3);
        Nombrefiltros->setSpacing(6);
        Nombrefiltros->setContentsMargins(11, 11, 11, 11);
        Nombrefiltros->setObjectName(QStringLiteral("Nombrefiltros"));
        Nombrefiltros->setContentsMargins(0, 0, 0, 0);
        F32 = new QLabel(horizontalLayoutWidget_3);
        F32->setObjectName(QStringLiteral("F32"));
        F32->setStyleSheet(QLatin1String("font: 10pt \"Latin Modern Mono Prop\";\n"
"color: rgb(42, 99, 255);"));

        Nombrefiltros->addWidget(F32);

        F63 = new QLabel(horizontalLayoutWidget_3);
        F63->setObjectName(QStringLiteral("F63"));
        F63->setStyleSheet(QLatin1String("font: 10pt \"Latin Modern Mono Prop\";\n"
"color: rgb(42, 99, 255);"));

        Nombrefiltros->addWidget(F63);

        F125 = new QLabel(horizontalLayoutWidget_3);
        F125->setObjectName(QStringLiteral("F125"));
        F125->setStyleSheet(QLatin1String("font: 10pt \"Latin Modern Mono Prop\";\n"
"color: rgb(42, 99, 255);"));

        Nombrefiltros->addWidget(F125);

        F250 = new QLabel(horizontalLayoutWidget_3);
        F250->setObjectName(QStringLiteral("F250"));
        F250->setStyleSheet(QLatin1String("font: 10pt \"Latin Modern Mono Prop\";\n"
"color: rgb(42, 99, 255);"));

        Nombrefiltros->addWidget(F250);

        F500 = new QLabel(horizontalLayoutWidget_3);
        F500->setObjectName(QStringLiteral("F500"));
        F500->setStyleSheet(QLatin1String("font: 10pt \"Latin Modern Mono Prop\";\n"
"color: rgb(42, 99, 255);"));

        Nombrefiltros->addWidget(F500);

        F1k = new QLabel(horizontalLayoutWidget_3);
        F1k->setObjectName(QStringLiteral("F1k"));
        F1k->setStyleSheet(QLatin1String("font: 10pt \"Latin Modern Mono Prop\";\n"
"color: rgb(42, 99, 255);"));

        Nombrefiltros->addWidget(F1k);

        F2k = new QLabel(horizontalLayoutWidget_3);
        F2k->setObjectName(QStringLiteral("F2k"));
        F2k->setStyleSheet(QLatin1String("font: 10pt \"Latin Modern Mono Prop\";\n"
"color: rgb(42, 99, 255);"));

        Nombrefiltros->addWidget(F2k);

        F4k = new QLabel(horizontalLayoutWidget_3);
        F4k->setObjectName(QStringLiteral("F4k"));
        F4k->setStyleSheet(QLatin1String("font: 10pt \"Latin Modern Mono Prop\";\n"
"color: rgb(42, 99, 255);"));

        Nombrefiltros->addWidget(F4k);

        F8k = new QLabel(horizontalLayoutWidget_3);
        F8k->setObjectName(QStringLiteral("F8k"));
        F8k->setStyleSheet(QLatin1String("font: 10pt \"Latin Modern Mono Prop\";\n"
"color: rgb(42, 99, 255);"));

        Nombrefiltros->addWidget(F8k);

        F16k = new QLabel(horizontalLayoutWidget_3);
        F16k->setObjectName(QStringLiteral("F16k"));
        F16k->setStyleSheet(QLatin1String("color: rgb(42, 99, 255);\n"
"font: 10pt \"Latin Modern Mono Prop\";"));

        Nombrefiltros->addWidget(F16k);

        Predeterminado = new QComboBox(centralWidget);
        Predeterminado->setObjectName(QStringLiteral("Predeterminado"));
        Predeterminado->setGeometry(QRect(830, 70, 81, 22));
        Predeterminado->setStyleSheet(QLatin1String("background-color: rgb(42, 99, 255);\n"
"font: 45 9pt \"URW Gothic L\";"));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 947, 19));
        menuEcualizador = new QMenu(menuBar);
        menuEcualizador->setObjectName(QStringLiteral("menuEcualizador"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuEcualizador->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        fileButton->setText(QApplication::translate("MainWindow", "...", 0));
        volume->setText(QApplication::translate("MainWindow", "Volume", 0));
        F32->setText(QApplication::translate("MainWindow", "32 Hz", 0));
        F63->setText(QApplication::translate("MainWindow", "63 Hz", 0));
        F125->setText(QApplication::translate("MainWindow", "125 Hz", 0));
        F250->setText(QApplication::translate("MainWindow", "250 Hz", 0));
        F500->setText(QApplication::translate("MainWindow", "500 Hz", 0));
        F1k->setText(QApplication::translate("MainWindow", "1 kHz", 0));
        F2k->setText(QApplication::translate("MainWindow", "2kHz", 0));
        F4k->setText(QApplication::translate("MainWindow", "4 kHz", 0));
        F8k->setText(QApplication::translate("MainWindow", "8 kHz", 0));
        F16k->setText(QApplication::translate("MainWindow", "16 kHz", 0));
        Predeterminado->clear();
        Predeterminado->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "None", 0)
         << QApplication::translate("MainWindow", "Classical", 0)
         << QApplication::translate("MainWindow", "Club", 0)
         << QApplication::translate("MainWindow", "Dance", 0)
         << QApplication::translate("MainWindow", "Full Bass & Trebble", 0)
         << QApplication::translate("MainWindow", "Full Trebble", 0)
         << QApplication::translate("MainWindow", "Pop", 0)
         << QApplication::translate("MainWindow", "Reggae", 0)
         << QApplication::translate("MainWindow", "Rock", 0)
         << QApplication::translate("MainWindow", "Techno", 0)
        );
        menuEcualizador->setTitle(QApplication::translate("MainWindow", "Ecualizador", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
