#include <iostream>
#include <filesystem>
#include <cstring>
#include "QFileDialog"

#include <iostream>
#include <stdio.h>
#include <dos.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <math.h>
#include <locale.h>
#include <fstream>


#include "widget.h"
#include "ui_widget.h"

#define Path 150

int brprog=0;


using namespace std;

QString  startedPath;


Widget::Widget(QWidget *parent)
    : QWidget(parent)
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
    //ui->label->setText("");

    FILE* frd1, * frd2, * fwr1, * fwr2, * fwr3;
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

    const char* StartedPath = startedPath.toStdString().c_str();
    vector<string>paths;
    get_files(paths, StartedPath);
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

            case 1:{
                        Fmax_1 = -200;	Fmax_2 = -500;
                        string path = text+"fw"+to_string(abs(Fmax_1))+"-"+to_string(abs(Fmax_2))+".dat";
                        string path1 = text+"xw"+to_string(abs(Fmax_1))+"-"+to_string(abs(Fmax_2))+".dat";
                        const char* Path1 = path.c_str();
                        const char* Path2 = path1.c_str();

                        strcpy(fw, Path1);
                        strcpy(xw, Path2);
                       break;

                }

            case 2:{
                        Fmax_1 = -500;	Fmax_2 = -1000;
                        string path2 = text+"fw"+to_string(abs(Fmax_1))+"-"+to_string(abs(Fmax_2))+".dat";
                        string path3 = text+"xw"+to_string(abs(Fmax_1))+"-"+to_string(abs(Fmax_2))+".dat";
                        const char* Path3 = path2.c_str();
                        const char* Path4 = path3.c_str();

                        strcpy(fw, Path3);
                        strcpy(xw, Path4);
                        break;

                }

            case 3:{
                        Fmax_1 = -1000;	Fmax_2 = -1500;
                        string path4 = text+"fw"+to_string(abs(Fmax_1))+"-"+to_string(abs(Fmax_2))+".dat";
                        string path5 = text+"xw"+to_string(abs(Fmax_1))+"-"+to_string(abs(Fmax_2))+".dat";
                        const char* Path5 = path4.c_str();
                        const char* Path6 = path5.c_str();

                        strcpy(fw, Path5);
                        strcpy(xw, Path6);
                        break;

                }

            case 4:{
                                Fmax_1 = -2000;	Fmax_2 = -3000;
                                string path6 = text+"fw"+to_string(abs(Fmax_1))+"-"+to_string(abs(Fmax_2))+".dat";
                                string path7 = text+"xw"+to_string(abs(Fmax_1))+"-"+to_string(abs(Fmax_2))+".dat";
                                const char* Path7 = path6.c_str();
                                const char* Path8 = path7.c_str();

                                strcpy(fw, Path7);
                                strcpy(xw, Path8);
                                break;

                        }


            case 5:{
                        Fmax_1 = -3000;	Fmax_2 = -4000;
                        string path8 = text+"fw"+to_string(abs(Fmax_1))+"-"+to_string(abs(Fmax_2))+".dat";
                        string path9 = text+"xw"+to_string(abs(Fmax_1))+"-"+to_string(abs(Fmax_2))+".dat";
                        const char* Path9 = path8.c_str();
                        const char* Path10 = path9.c_str();

                        strcpy(fw, Path9);
                        strcpy(xw, Path10);
                        break;

                }

            case 6:{
                        Fmax_1 = -4000;	Fmax_2 = -5000;
                        string path10 = text+"fw"+to_string(abs(Fmax_1))+"-"+to_string(abs(Fmax_2))+".dat";
                        string path11 = text+"xw"+to_string(abs(Fmax_1))+"-"+to_string(abs(Fmax_2))+".dat";
                        const char* Path11 = path10.c_str();
                        const char* Path12 = path11.c_str();

                        strcpy(fw, Path11);
                        strcpy(xw, Path12);
                        // break;

                }
            };

            if ((fwr1 = fopen(fw, "wb")) == NULL) {
                fprintf(stderr, "Немогу открыть файл %s для записи \n",fw);

            };

            if ((fwr2 = fopen(xw, "wb")) == NULL) {
                fprintf(stderr, "Немогу открыть файл %s для записи \n",xw);

            };
            string filePathlog = text+"log.txt";
            const char* PathlogFile = filePathlog.c_str();

            //ofstream MyFile(PathlogFile);
            //MyFile<<"\n Отбор импульсов с амплитудой удара:\n Fmin = %ld,H\n Fmax = %ld,H";
            //MyFile<<"-Fmax_1= "<<-Fmax_1<<"\n"<<"-Fmax_2= "<<-Fmax_2<<"\n";

            //MyFile <<"\n Отбор импульсов с амплитудой удара:\n"; MyFile<< Fmin = %ld,H\n Fmax = %ld,H<< -Fmax_1<< -Fmax_2;

            FILE *log;
            log=fopen(PathlogFile, "a");
            //fclose(log);
            //log1=fopen(PathlogFile, "w");
            fprintf(log,"\n Отбор импульсов с амплитудой удара:\n Fmin = %ld,H\n Fmax = %ld,H");
            fprintf(log, "%d%d", -Fmax_1, -Fmax_2);

            qInfo()<<"\n Отбор импульсов с амплитудой удара:\n Fmin = %ld,H\n Fmax = %ld,H"<< -Fmax_1<< -Fmax_2;

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
            //MyFile<<"\n№\tИндекс\t\tТочек\tfв,Гц \tFmax,Н";
            fprintf(log,"\n\n№\tИндекс\t\tТочек\tfв,Гц \tFmax,Н\n");

            qInfo()<<"\n№\tИндекс\t\tТочек\tfв,Гц \tFmax,Н";

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
                //				("\n%d\t%d\t\t%2.0f\t%5.0f\t%5.0f", j, N_max[j], data, 1.5 * 64000 / data, -Fmax[j]);
                //MyFile<<"\n%d\t%d\t\t%2.0f\t%5.0f\t%5.0f"<< j<< N_max[j]<< data<< 1.5 * 64000 / data<< -Fmax[j];
                fprintf(log,"\n%d\t%d\t\t%2.0f\t%5.0f\t%5.0f",j, N_max[j], data, 1.5 * 64000 / data, -Fmax[j]);
                qInfo()<<"\n%d\t%d\t\t%2.0f\t%5.0f\t%5.0f"<< j<< N_max[j]<< data<< 1.5 * 64000 / data<< -Fmax[j];
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
            fclose(log);
            fclose(frd1);
            fclose(frd2);
            fclose(fwr1);
        };
        qInfo()<<"Finished";
        brprog=counter*100/paths.size();
        brprog=brprog*10;
    ui->progressBar->setValue(brprog);
    }
   // ui->label->setText("Выполенно");
    ui->progressBar->setValue(100);

}

void Widget::on_pushButton_2_clicked()
{
    startedPath = QFileDialog::getExistingDirectory(0, "Выбор папки", "");
}

