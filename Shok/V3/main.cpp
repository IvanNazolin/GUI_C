#include <iostream>
#include <stdio.h>
#include <dos.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <math.h>
#include <locale.h>
#include <string>
#include <cmath>
#include <stdio.h>
#include "QFileDialog"
#include "widget.h"
#include "ui_widget.h"
#include "QString"
#include <filesystem>
#include <fstream>
#include <string>
#include <direct.h>

using namespace  std;

QString strx;
QString strf;
QString floadf ;
string text;
QString  startedPath;

Widget::Widget(QWidget *parent): QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void get_files(vector<string>& paths, const string& current_path)
{
    for (const auto& file : filesystem::directory_iterator(current_path))
    {
        if (filesystem::is_directory(file))
        {
            get_files(paths, file.path().string());
        }
        else
        {
            paths.push_back(file.path().string());
        }
    }
}

void Widget::on_pushButton_clicked()
{
    ui->Edit1Fmax1->setPlainText("-200");
    ui->Edit1Fmax2->setPlainText("-500");

    ui->Edit2Fmax1->setPlainText("-500");
    ui->Edit2Fmax2->setPlainText("-1000");

    ui->Edit3Fmax1->setPlainText("-1000");
    ui->Edit3Fmax2->setPlainText("-1500");

    ui->Edit4Fmax1->setPlainText("-2000");
    ui->Edit4Fmax2->setPlainText("-3000");

    ui->Edit5Fmax1->setPlainText("-3000");
    ui->Edit5Fmax2->setPlainText("-4000");

    ui->Edit6Fmax1->setPlainText("-4000");
    ui->Edit6Fmax2->setPlainText("-5000");
}

std::string getFilePathWithoutFileNameAndExtension(const std::string& path) {
    size_t lastSlashIndex = path.find_last_of("/\\");
    size_t lastDotIndex = path.find_last_of(".");

    if (lastSlashIndex != std::string::npos && lastDotIndex != std::string::npos && lastDotIndex > lastSlashIndex) {
        return path.substr(0, lastSlashIndex);
    } else {
        return "";
    }
}

void MakeFXNew()
{
    string line;
    QString path_to_dat[8];
    QString path_to_new_dat[8];
    QString path_to_new_Txo[8];
    string new_dat[8]{"Channel1.dat","Channel2.dat","Channel3.dat","Channel4.dat","Channel5.dat","Channel6.dat","Channel7.dat","Channel8.dat"};
    string new_txo[8]{"Channel1.TX0","Channel2.TX0","Channel3.TX0","Channel4.TX0","Channel5.TX0","Channel6.TX0","Channel7.TX0","Channel8.TX0"};
    QString path_to_txo[8];
    string mas[150];
    string mask1k7[1500];
    double fmask0txo[8];
    double fmask1txo[8];
    float fmask1[8];
    float fmask0[8];

    //получение всех файлов в стартовой директории.
    string star_path = startedPath.toStdString();
    vector<string>startPath;
    string path_to_M[1];
    get_files(startPath, star_path);
    for (int cpunter = 0, c = 0; cpunter < startPath.size(); cpunter++)
    {
        string path_mera;
        if (startPath[cpunter].substr(startPath[cpunter].length() - 4, startPath[cpunter].length()) == "mera")
        {

            path_mera = startPath[cpunter];

            //string path_mera = "C:\\Users\\nazol\\Desktop\\Programming\\Languages\\Cu_and_C++\\Shok\\Source\\V3\\vibro001\\vibro001.mera";
            ifstream mera(path_mera);
            for (int counteri = 0; !mera.eof(); counteri++)
            {
                getline(mera, mas[counteri]);
            }

            //сортировка исходного массива для разделения его на дава массив mask1 и mask0 и паралельное преобразование данных в тип float;
            for (int ik1 = 0, ik0 = 0, i = 0; i < 150; i++)
            {
                if (mas[i].substr(0, 2) == "k0") //перевод элемента в mask0
                {
                    string s = mas[i].substr(3, mas[i].length());
                    fmask0[ik0] = stof(s);
                    ik0 = ik0 + 1;
                }

                if (mas[i].substr(0, 2) == "k1") //перевод элемента в mask1
                {
                    string s = mas[i].substr(3, mas[i].length());
                    fmask1[ik1] = stof(s);
                    ik1 = ik1 + 1;
                }
            }

            //создане массива со всеми именами в папке и сортировка для получения имен файлов которые оканчиваются на .dat;
            vector<string>paths;
            string path_to_dir = path_mera.substr(0, path_mera.length() - 13);
            get_files(paths, path_to_dir);
            for (int i = 0, i1 = 0, i2 = 0; i < paths.size(); i++)
            {
                if (paths[i].substr(paths[i].length() - 3, paths[i].length()) == "TX0")
                {
                    path_to_txo[i2] =  QString::fromStdString(paths[i]);
                    path_to_new_Txo[i2] = path_to_txo[i2].sliced(0, path_to_txo[i2].length() - 17) +  QString::fromStdString(new_txo[i2]);
                    i2 = i2 + 1;
                }
                if (paths[i].substr(paths[i].length() - 3, paths[i].length()) == "dat")
                {
                    path_to_dat[i1] =  QString::fromStdString(paths[i]);
                    path_to_new_dat[i1] = path_to_dat[i1].sliced(0, path_to_dat[i1].length() - 17) +  QString::fromStdString(new_dat[i1]);
                    i1 += 1;
                }
            }

            // копирование файлов .dat с изменением имени для того что бы их можно было прочитать
            for (int i = 0; i < 8; i++)
            {
                QString simbolk = "\"";
                QString strcopy = "copy /B " + simbolk + path_to_dat[i] + simbolk + " " + simbolk + path_to_new_dat[i] + simbolk;
                system(qPrintable((strcopy.toStdString()).c_str()));
            }
            if (path_to_txo[0]!="")
            {
                for (int i = 0; i < 8; i++)
                {
                    QString simbolk = "\"";
                    QString strcopy = "copy /A " + simbolk + path_to_txo[i] + simbolk + " " + simbolk + path_to_new_Txo[i] + simbolk;
                    system(qPrintable((strcopy.toStdString()).c_str()));
                }
            }

            //создание папки в деректории
            QString qw1;
            qw1 = QString::fromStdString(path_to_dir) + "Channel_1";
            _mkdir(qw1.toStdString().c_str());
            QString qw2 = QString::fromStdString(path_to_dir) + "Channel_2";
            _mkdir(qw2.toStdString().c_str());
            QString qw3 = QString::fromStdString(path_to_dir) + "Channel_3";
            _mkdir(qw3.toStdString().c_str());
            QString qw4 = QString::fromStdString(path_to_dir) + "Channel_4";
            _mkdir(qw4.toStdString().c_str());
            QString qw5 = QString::fromStdString(path_to_dir) + "Channel_5";
            _mkdir(qw5.toStdString().c_str());
            QString qw6 = QString::fromStdString(path_to_dir) + "Channel_6";
            _mkdir(qw6.toStdString().c_str());
            QString qw7 = QString::fromStdString(path_to_dir) + "Channel_7";
            _mkdir(qw7.toStdString().c_str());
            QString path_to_xd[7] = { qw1,qw2,qw3,qw4,qw5,qw6,qw7 };

            //чтение данных пути к которым лежал в path_to_txo исправь то что не все чила переходят;

            for (int i = 0, c = 0; i < 8; i++)
            {
                string s;
                ifstream filetxo(path_to_new_Txo[i].toStdString());
                getline(filetxo, s);
                int pointer = s.find(" ");
                fmask0txo[c] = stod(s.substr(0,pointer));
                fmask1txo[c] = stod(s.substr(pointer, s.length()));
                c += 1;
                filetxo.close();
            }

            //чтение данных из файлов пути к которым лежат в  path_to_dat;
            for (int i = 0; i < 7; i++)
            {
                ifstream q(path_to_new_dat[7].toStdString(), ios_base::binary);
                ifstream f(path_to_new_dat[i].toStdString(), ios_base::binary);
                ofstream fx(path_to_xd[i].toStdString() + "\\x.dat", ios_base::binary);
                ofstream ff(path_to_xd[i].toStdString() + "\\f.dat", ios_base::binary);
                while (!f.eof())
                {
                    short num = 0;
                    f.read((char*)&num, sizeof(short));
                    float x = fmask1txo[i]*((fmask1[i]*(num-fmask0[i]))+fmask0txo[i]);
                    fx.write((char*)&x, sizeof(float));
                }
                while (!q.eof())
                {
                    short num = 0;
                    q.read((char*)&num, sizeof(short));
                    float y = fmask1txo[7]*((fmask1[7]*(num-fmask0[7]))+fmask0txo[7]);
                    ff.write((char*)&y, sizeof(float));
                }
                q.close();
                f.close();
                fx.close();
                ff.close();
            }
            for (int i = 0; i < 8; i++)
            {
                QString simbolk = "\"";
                QString strcopy = "del  " + simbolk + path_to_new_dat[i] + simbolk;
                system(qPrintable((strcopy.toStdString()).c_str()));
                strcopy = "del  " + simbolk + path_to_new_Txo[i] + simbolk;
                system(qPrintable((strcopy.toStdString()).c_str()));
            }
        }
    }
}

void MakeFX()
{
    string line;
    QString path_to_dat[8];
    QString path_to_new_dat[8];
    string new_dat[8]{"Channel1.dat","Channel2.dat","Channel3.dat","Channel4.dat","Channel5.dat","Channel6.dat","Channel7.dat","Channel8.dat"};
    string mas[150];
    string mask1k7[1500];
    float fmask1[8];
    float fmask0[8];

    //получение всех файлов в стартовой директории.
    string star_path = startedPath.toStdString();
    //string star_path = "C:\\Users\\nazol\\Desktop\\tests";
    vector<string>startPath;
    string path_to_M[1];
    get_files(startPath, star_path);
    for (long cpunter = 0; cpunter < startPath.size(); cpunter++)
    {
        string path_mera;
        if (startPath[cpunter].substr(startPath[cpunter].length() - 4, startPath[cpunter].length()) == "mera")
        {
            
            vector<string>paths;
            string path_to_dir = "/"+getFilePathWithoutFileNameAndExtension(path_mera);
            string path_to_new_mera = path_to_dir+"fileForTest.mera";
            path_mera = startPath[path_to_new_mera];
            //string path_mera = "C:\\Users\\nazol\\Desktop\\tests\\vibro001\\vibro.mera";
            ifstream mera(path_mera);
            for (int counteri = 0; !mera.eof(); counteri++)
            {
                getline(mera, mas[counteri]);
            }
            mera.close();

            //сортировка исходного массива для разделения его на дава массив mask1 и mask0 и паралельное преобразование данных в тип float;
            for (int ik1 = 0, ik0 = 0, i = 0; i < 150; i++)
            {
                if (mas[i].substr(0, 2) == "k0") //перевод элемента в mask0
                {
                    string s = mas[i].substr(3, mas[i].length());
                    fmask0[ik0] = stof(s);
                    ik0 = ik0 + 1;
                }

                if (mas[i].substr(0, 2) == "k1") //перевод элемента в mask1
                {
                    string s = mas[i].substr(3, mas[i].length());
                    fmask1[ik1] = stof(s);
                    ik1 = ik1 + 1;
                }
            }



            get_files(paths, path_to_dir);
            for (int i = 0, i1 = 0; i < paths.size(); i++)
            {
                if (paths[i].substr(paths[i].length() - 3, paths[i].length()) == "dat")
                {
                    path_to_dat[i1] =  QString::fromStdString(paths[i]);
                    path_to_new_dat[i1] = path_to_dat[i1].sliced(0, path_to_dat[i1].length() - 18) +  QString::fromStdString(new_dat[i1]);
                    i1 += 1;
                }
            }



            // копирование файлов .dat с изменением имени для того что бы их можно было прочитать
            for (int i = 0; i < 8; i++)
            {
                QString simbolk = "\"";
                QString strcopy = "copy /B " + simbolk + path_to_dat[i] + simbolk + " " + simbolk + path_to_new_dat[i] + simbolk;
                system(qPrintable((strcopy.toStdString()).c_str()));
            }

            //создание папки в деректории
            QString qw1;
            qw1 = QString::fromStdString(path_to_dir) + "Channel_1";
            _mkdir(qw1.toStdString().c_str());
            QString qw2 = QString::fromStdString(path_to_dir) + "Channel_2";
            _mkdir(qw2.toStdString().c_str());
            QString qw3 = QString::fromStdString(path_to_dir) + "Channel_3";
            _mkdir(qw3.toStdString().c_str());
            QString qw4 = QString::fromStdString(path_to_dir) + "Channel_4";
            _mkdir(qw4.toStdString().c_str());
            QString qw5 = QString::fromStdString(path_to_dir) + "Channel_5";
            _mkdir(qw5.toStdString().c_str());
            QString qw6 = QString::fromStdString(path_to_dir) + "Channel_6";
            _mkdir(qw6.toStdString().c_str());
            QString qw7 = QString::fromStdString(path_to_dir) + "Channel_7";
            _mkdir(qw7.toStdString().c_str());
            QString path_to_xd[7] = { qw1,qw2,qw3,qw4,qw5,qw6,qw7 };

            //чтение данных из файлов пути к которым лежат в  path_to_dat;
            for (int i = 0; i < 7; i++)
            {
                ifstream q(path_to_new_dat[7].toStdString(), ios_base::binary);
                ifstream f(path_to_new_dat[i].toStdString(), ios_base::binary);
                ofstream fx(path_to_xd[i].toStdString() + "\\x.dat", ios_base::binary);
                ofstream ff(path_to_xd[i].toStdString() + "\\f.dat", ios_base::binary);
                while (!f.eof())
                {
                    short num = 0;
                    f.read((char*)&num, sizeof(short));
                    float x = fmask1[i]*(num-fmask0[i]);
                    fx.write((char*)&x, sizeof(float));
                }
                while (!q.eof())
                {
                    short num = 0;
                    q.read((char*)&num, sizeof(short));
                    float y = fmask1[7]*(num-fmask0[7]);
                    ff.write((char*)&y, sizeof(float));
                }
                q.close();
                f.close();
                fx.close();
                ff.close();
            }
            for (int i = 0; i < 8; i++)
            {
                QString simbolk = "\"";
                QString strcopy = "del  " + simbolk + path_to_new_dat[i] + simbolk;
                system(qPrintable((strcopy.toStdString()).c_str()));
            }
        }
    }
}

void Widget::on_pushButton_4_clicked(){
    if(ui->checkBoxTXO->isChecked())
    {
        MakeFX();
    }
    else
    {
        MakeFXNew();
    }

    qInfo()<<"Hello";

    QTextStream cout (stdout);

    #define Path 150

    FILE* frd1, * frd2, * fwr1, * fwr2;

    setlocale(LC_ALL,"Russian");


    int i, N, f;
    char
        fw[Path],		// путь с именем файла записи сигнала ударных импульсов
        xw[Path];		// путь с именем файла записи сигнала реакции

    float x, x_old, mean1, mean2, data, p,
        Fmax[300];		// значения амплитуд ударных импульсов;

    //----------------- Обязательные для задания параметры ---------------------

    long k, j, point,
        koeff = -1.0,		// коэффициент умножения сигнала силы
        zero_point = 100,	// число точек до удара
        tau1 = 2, 			// постоянная времени обработки удара и отклика до удара

        N_bpf = 16384,		// число точек между импульсами
        tau2= 3200.0,		// постоянная времени затухания для N_bpf = 16384


        //			N_bpf = 65536,
        //			tau2 = 3200 * 4,	// постоянная времени затухания для N_bpf = 65536

        //		 N_bpf = 32768,
        //		 tau2 = 3200*2,		// постоянная времени затухания для N_bpf = 65536

        //		 tau2= 150000.0,	// tau2=max без затухания

        Fmax_1 = -200,   	//-300 -900  -2500 нижний порог отбора амплитуд ударных импульсов
        Fmax_2 = -500,	    //-500 -1200 -2800 верхний порог отбора амплитуд ударных импульсов
        n_otskok = 500,	// минимальное число точек до двойного удара
        Porog_udar2 = 10,	//8 порог обнаружения двойного удара
        Porog_tau = -20,	//пророг определения длительности удара
        N_max[300],		// координаты амплитуд ударных импульсов
        N_st_end[300][2];	// массив точек начала и конца импульсов


    //--------- Конец обязательных для задания параметров--------------------
  //QString  Rubbish = ui->plainTextEdit->toPlainText();
  //QString  Rubbish2 = ui->plainTextEdit2->toPlainText();
  //long Fmax_1 = Rubbish.toLong();
  //long Fmax_2 = Rubbish2.toLong();

   // qInfo()<<"\n Разработчик Назолин А.Л. 29.03.2023г.";
  //  qInfo()<<"\n Программа формирует сигналы силы fw.dat и реакции xw.dat для последующего расчета АЧХ ускряемости статора.";
    //		printf("\n Отбираются импульсов с амплитудой удара:\n Fmin = %ld,H\n Fmax = %ld,H",-Fmax_1,-Fmax_2);
  //  qInfo()<<"\n Число точек между импульсами: N_bpf = %ld точек"<< N_bpf;
  //  qInfo()<<"\n Постоянная времени exp окна временной фильтрации сигналов силы и удара до удара:    tau1= %ld точек"<< tau2;
  //  qInfo()<<"\n Постоянная времени exp окна временной фильтрации сигналов силы и удара после удара: tau2 = %ld точек"<< tau2;

    const char* StartedPath = startedPath.toStdString().c_str();
    vector<string>paths;
   /*!*!!!!*/ get_files(paths, StartedPath);
    //for (int i = 0; i < paths.size(); i++) qInfo() << paths[i] ;
    qInfo()<<paths[1];
    int stertsize=0;
    stertsize=paths.size();
    qInfo()<<stertsize;
    string filefdat [stertsize];
    string filexdat [stertsize];
    long countf=0;
    long countx=0;
    //далее бдожен быть алгорим разделения оного массива на два по признаку окончания на x.dat и f.dat;
    for(long countCommon=0; countCommon<paths.size(); ++countCommon)
    {
        qInfo()<<paths[countCommon].substr(paths[countCommon].length()-5, paths[countCommon].length());
        if(paths[countCommon].substr(paths[countCommon].length()-5, paths[countCommon].length())=="f.dat")
        {
            filefdat[countf] = paths[countCommon];
            countf+=1;
            //qInfo()<<"findedfdat";
        }
        if(paths[countCommon].substr(paths[countCommon].length()-5, paths[countCommon].length())=="x.dat")
        {
            filexdat[countx] = paths[countCommon];
            countx+=1;
            //qInfo()<<"findedxdat";
        }
    }


    for(long counter=0; counter<stertsize/2; counter++)
    {
        string text = filefdat[counter].substr(0,filefdat[counter].length()-5);
        for (f = 1; f <= 6; f++) //f<6
        {
            const char* CharF = filefdat[counter].c_str();
            const char* CharX = filexdat[counter].c_str();
            if ((frd1 = fopen(CharF, "rb")) == NULL) {
                fprintf(stderr, " Немогу открыть исходный файл f.dat\n");
                fclose(frd1);

            };

            if ((frd2 = fopen(CharX, "rb")) == NULL) {
                fprintf(stderr, " Немогу открыть исходный файл x.dat \n");
                fclose(frd2);

            };
            switch (f)
            {

            case 1:
                if(ui->Edit1Fmax1->toPlainText()==""&& ui->Edit1Fmax2->toPlainText()=="")
                {}
                else{
                    if(ui->checkBox1->isChecked()){
                    QString  Fmax1_String = ui->Edit1Fmax1->toPlainText();
                    QString  Fmax2_String = ui->Edit1Fmax2->toPlainText();
                    Fmax_1 = Fmax1_String.toLong();
                    Fmax_2 = Fmax2_String.toLong(); //значение с бокса
                    string path = text+"fw"+to_string(abs(Fmax_1))+"-"+to_string(abs(Fmax_2))+".dat";
                    string path1 = text+"xw"+to_string(abs(Fmax_1))+"-"+to_string(abs(Fmax_2))+".dat";
                    const char* Path1 = path.c_str();
                    const char* Path2 = path1.c_str();

                    strcpy(fw, Path1);
                    strcpy(xw, Path2);
                    break;
                    }
                }

            case 2:
                if(ui->Edit2Fmax1->toPlainText()==""&& ui->Edit2Fmax2->toPlainText()==""){}
                else{
                    if(ui->checkBox2->isChecked()){
                    QString  Fmax1_String = ui->Edit2Fmax1->toPlainText();
                    QString  Fmax2_String = ui->Edit2Fmax2->toPlainText();
                    Fmax_1 = Fmax1_String.toLong();
                    Fmax_2 = Fmax2_String.toLong();
                    string path2 = text+"fw"+to_string(abs(Fmax_1))+"-"+to_string(abs(Fmax_2))+".dat";
                    string path3 = text+"xw"+to_string(abs(Fmax_1))+"-"+to_string(abs(Fmax_2))+".dat";
                    const char* Path3 = path2.c_str();
                    const char* Path4 = path3.c_str();

                    strcpy(fw, Path3);
                    strcpy(xw, Path4);
                    break;
                    }
                }

            case 3:
                if(ui->Edit3Fmax1->toPlainText()==""&& ui->Edit3Fmax2->toPlainText()==""){}
                else{
                    if(ui->checkBox3->isChecked()){
                    QString  Fmax1_String = ui->Edit3Fmax1->toPlainText();
                    QString  Fmax2_String = ui->Edit3Fmax2->toPlainText();
                    Fmax_1 = Fmax1_String.toLong();
                    Fmax_2 = Fmax2_String.toLong();
                    string path4 = text+"fw"+to_string(abs(Fmax_1))+"-"+to_string(abs(Fmax_2))+".dat";
                    string path5 = text+"xw"+to_string(abs(Fmax_1))+"-"+to_string(abs(Fmax_2))+".dat";
                    const char* Path5 = path4.c_str();
                    const char* Path6 = path5.c_str();

                    strcpy(fw, Path5);
                    strcpy(xw, Path6);
                        break;
                    }
                }

            case 4:
                if(ui->Edit4Fmax1->toPlainText()==""&& ui->Edit4Fmax2->toPlainText()==""){}
                else{
                    if(ui->checkBox4->isChecked()){
                    QString  Fmax1_String = ui->Edit4Fmax1->toPlainText();
                    QString  Fmax2_String = ui->Edit4Fmax2->toPlainText();
                    Fmax_1 = Fmax1_String.toLong();
                    Fmax_2 = Fmax2_String.toLong();
                    string path6 = text+"fw"+to_string(abs(Fmax_1))+"-"+to_string(abs(Fmax_2))+".dat";
                    string path7 = text+"xw"+to_string(abs(Fmax_1))+"-"+to_string(abs(Fmax_2))+".dat";
                    const char* Path7 = path6.c_str();
                    const char* Path8 = path7.c_str();

                    strcpy(fw, Path7);
                    strcpy(xw, Path8);
                break;
                    }
                }


            case 5:
                if(ui->Edit5Fmax1->toPlainText()==""&& ui->Edit5Fmax2->toPlainText()==""){}
                else{
                    if(ui->checkBox5->isChecked()){
                QString  Fmax1_String = ui->Edit5Fmax1->toPlainText();
                QString  Fmax2_String = ui->Edit5Fmax2->toPlainText();
                Fmax_1 = Fmax1_String.toLong();
                Fmax_2 = Fmax2_String.toLong();
                string path8 = text+"fw"+to_string(abs(Fmax_1))+"-"+to_string(abs(Fmax_2))+".dat";
                string path9 = text+"xw"+to_string(abs(Fmax_1))+"-"+to_string(abs(Fmax_2))+".dat";
                const char* Path9 = path8.c_str();
                const char* Path10 = path9.c_str();

                strcpy(fw, Path9);
                strcpy(xw, Path10);
                    break;
                    }
                }

            case 6:
                if(ui->Edit6Fmax1->toPlainText()==""&& ui->Edit6Fmax2->toPlainText()==""){}
                else{
                    if(ui->checkBox6->isChecked()){
                QString  Fmax1_String = ui->Edit6Fmax1->toPlainText();
                QString  Fmax2_String = ui->Edit6Fmax2->toPlainText();
                Fmax_1 = Fmax1_String.toLong();
                Fmax_2 = Fmax2_String.toLong();
                string path10 = text+"fw"+to_string(abs(Fmax_1))+"-"+to_string(abs(Fmax_2))+".dat";
                string path11 = text+"xw"+to_string(abs(Fmax_1))+"-"+to_string(abs(Fmax_2))+".dat";
                const char* Path11 = path10.c_str();
                const char* Path12 = path11.c_str();

                strcpy(fw, Path11);
                strcpy(xw, Path12);
               // break;
                    }
                }
            };


            if ((fwr1 = fopen(fw, "wb")) == NULL) {
                fprintf(stderr, "Немогу открыть файл %s для записи \n",fw);

            };

            if ((fwr2 = fopen(xw, "wb")) == NULL) {
                fprintf(stderr, "Немогу открыть файл %s для записи \n",xw);

            };

           // qInfo()<<"\n Отбор импульсов с амплитудой удара:\n Fmin = %ld,H\n Fmax = %ld,H"<< -Fmax_1<< -Fmax_2;

            point = -1;
            data = 0;
            N_max[0] = 0;
            Fmax[0] = 0;
            j = 0;
            i = 1;
            k = 0;
            x_old = 0;
            p = 0.0;
            while (fread(&x, sizeof(float), 1, frd1) != 0)
            {
                point++;
                if ((x > Fmax_1))
                {
                    j++;

                    if ((j > n_otskok) && (j < N_bpf) && (fabs(x - x_old) > Porog_udar2))
                    {
                        p++;
                    };

                    if ((k == 1) && (j >= N_bpf))
                    {
                        if ((p == 0) && (data > Fmax_2))
                        {
                            i++;
                        }
                        else {
                            p = 0;
                        };
                        k = 0;
                        data = 0;
                    };

                }
                else {
                    if (x < data)
                    {
                        data = x;
                        N_max[i] = point;
                        Fmax[i] = data;
                        k = 1;
                        j = 0;
                    };
                };
                x_old = x;
            };


            if (p > 0) i--;

         //  qInfo()<<"\n№\tИндекс\t\tТочек\tfв,Гц \tFmax,Н";

            for (i--, j = 1; j <= i; j++)
            {
                fseek(frd1, N_max[j] * 4, SEEK_SET);
                fread(&x, sizeof(float), 1, frd1);

                k = 0;
                while (x < Porog_tau)
                {
                    k++;
                    fseek(frd1, (N_max[j] - k) * 4, SEEK_SET);
                    fread(&x, sizeof(float), 1, frd1);
                };
                N_st_end[j][0] = N_max[j] - k + 1;

                fseek(frd1, N_max[j] * 4, SEEK_SET);
                fread(&x, sizeof(float), 1, frd1);

                k = 0;
                while (x < Porog_tau)
                {
                    k++;
                    fseek(frd1, (N_max[j] + k) * 4, SEEK_SET);
                    fread(&x, sizeof(float), 1, frd1);
                };

                N_st_end[j][1] = N_max[j] + k;
                data = N_st_end[j][1] - N_st_end[j][0];
            //    qInfo()<<"\n%d\t%d\t\t%2.0f\t%5.0f\t%5.0f"<< j<< N_max[j]<< data<< 1.5 * 64000 / data<< -Fmax[j];
                //		printf("\n %ld", N_st_end[j][1] - N_st_end[j][0]);
            };

            for (j = 1; j <= i; j++)
            {
                fseek(frd1, N_st_end[j][1] * 4, SEEK_SET);
                fseek(frd2, N_st_end[j][0] * 4, SEEK_SET);

                for (mean1 = mean2 = 0, k = 1; k <= N_bpf; k++)
                {

                    fread(&x, sizeof(float), 1, frd1);
                    mean1 += x;

                    fread(&x, sizeof(float), 1, frd2);
                    mean2 += x;
                };
                mean1 /= N_bpf;
                mean2 /= N_bpf;


                fseek(frd1, (N_st_end[j][0] - zero_point) * 4, SEEK_SET);
                fseek(frd2, (N_st_end[j][0] - zero_point) * 4, SEEK_SET);
                N = N_st_end[j][1] - N_st_end[j][0] + zero_point;

                for (p = 1, k = 1; k <= N_bpf; k++, p++)
                {
                    //  обработка до удара
                    if (k <= zero_point)
                    //экспоненциальное окно для силы и отклика создание файлов
                    {
                        fread(&x, sizeof(float), 1, frd1);
                        data = koeff * (x - mean1) * exp(-(zero_point - p) / tau1);

                        fwrite(&data, sizeof(float), 1, fwr1);
                        //				fwrite(&x, sizeof(float), 1, fwr3);

                        fread(&x, sizeof(float), 1, frd2);
                        data = (x - mean2) * exp(-(zero_point - p) / tau1);
                        fwrite(&data, sizeof(float), 1, fwr2);
                    }
                    else {
                        // обработка во время и после импульса

                        if (k <= N)	// единичное окно силы и отклика во время импульса
                        {
                            fread(&x, sizeof(float), 1, frd1);
                            data = koeff * (x - mean1);
                            fwrite(&data, sizeof(float), 1, fwr1);
                            //						fwrite(&x, sizeof(float), 1, fwr3);

                            fread(&x, sizeof(float), 1, frd2);
                            data = (x - mean2);
                            fwrite(&data, sizeof(float), 1, fwr2);

                        }
                        else {
                            //экспоненциальное окно силы и отклика после импульса
                            fread(&x, sizeof(float), 1, frd1);
                            data = koeff * (x - mean1) * exp(-(p - N) / tau1);
                            fwrite(&data, sizeof(float), 1, fwr1);
                            //						fwrite(&x, sizeof(float), 1, fwr3);

                            fread(&x, sizeof(float), 1, frd2);
                            data = (x - mean2) * exp(-(p - zero_point) / tau2);
                            //					data = exp(-(p-zero_point)/tau2);    // запись exp
                            fwrite(&data, sizeof(float), 1, fwr2);
                        };
                    };
                }

            };

            fclose(frd1);
            fclose(frd2);
            fclose(fwr1);
        };
    }
    qInfo()<<"Finished";
    }

void Widget::on_btn1_clicked()
{
 //   qInfo()<<"Hellow";
    ui->Edit1Fmax1->setPlainText("");
    ui->Edit1Fmax2->setPlainText("");

    ui->Edit2Fmax1->setPlainText("");
    ui->Edit2Fmax2->setPlainText("");

    ui->Edit3Fmax1->setPlainText("");
    ui->Edit3Fmax2->setPlainText("");

    ui->Edit4Fmax1->setPlainText("");
    ui->Edit4Fmax2->setPlainText("");

    ui->Edit5Fmax1->setPlainText("");
    ui->Edit5Fmax2->setPlainText("");

    ui->Edit6Fmax1->setPlainText("");
    ui->Edit6Fmax2->setPlainText("");
}

void Widget::on_btn2_clicked()
{
    ui->checkBox1->setChecked(true);
    ui->checkBox2->setChecked(true);
    ui->checkBox3->setChecked(true);
    ui->checkBox4->setChecked(true);
    ui->checkBox5->setChecked(true);
    ui->checkBox6->setChecked(true);
}

void Widget::on_button_fill_clicked()
{    ui->checkBox1->setChecked(false);
    ui->checkBox2->setChecked(false);
    ui->checkBox3->setChecked(false);
    ui->checkBox4->setChecked(false);
    ui->checkBox5->setChecked(false);
    ui->checkBox6->setChecked(false);
}

void Widget::on_pushButton_9_clicked()
{
    startedPath = QFileDialog::getExistingDirectory(0, "Выбор папки", "");
}

void Widget::on_pushButton_10_clicked()
{

}


