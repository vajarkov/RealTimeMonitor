#pragma once
#include <string>
#include "src/rtklib.h"

using namespace std;



//Переменная для хранения пути к файлу 
string path;

//Переменная для хранения IP-адреса приемника
string ip_address;

//Переменная для хранения формата "сырых" данных
int format;

//Переменная для включения или отключения потока
int stream_type;

//Переменная для локальной папки
string local_dir;

//Переменная, где храняться данные с сервера
rtksvr_t rtksvr;

//Переменная для потока данных
stream_t monistr;

/* processing options type */
//Переменная для храния параметров процесса
prcopt_t PrcOpt;

//Переменная для хранения параметров "решения"
solopt_t SolOpt;

//Цикл опроса данных NMEA
int nmea_cycle;

//Координаты NMEA
double nmea_pos;

//Команда сброса 
string ResetCmd;

//Переменная для хранения путей к файлам и адресам
char* Path[8];

//Переменная для хранения форматов получаемых или отдаваемых данных
int Format[MAXSTRRTK];

//Текущее состояние сбора данных
int SolCurrentStat;

string RcvOpt[3] = { "", "", "" };

//Функция запуска сервера отображения данных
void __fastcall SvrStart(void);

//Функция периодического обновления данных 
void __fastcall Timer(void);

//Функция запуска сервера сохранения данных в файл
void __fastcall SvrStartStream(void);

//Функция остановки сервера сохранения данных в файл
void __fastcall SvrStop(void);

//Переменная параметров потока
int SvrOpt[6];

//Переменная цикла опроса NMEA
int NmeaReq; 

//Переменная обмена данными из файла 
int FileSwapMargin;

//Ретрансляция выходного потока
int RelayBack;

//Координаты NMEA
double AntPos[3], AntOff[3];