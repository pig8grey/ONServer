#ifndef LIVESEED_H
#define LIVESEED_H

#include <string>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <cmath>
#include <QDir>
#include <QString>
#include <native.h>
#include <QMutex>

extern "C"{
#include <libmseed.h>


void fillmsr(MS3Record *&msr,const char* starttime,const char* sid,double sr,char sp);
void fillmsrINT(MS3Record *&msr,const char* starttime,const char* sid, double sr);
void filldata(MS3Record *&msr,float* data,int size);
void filldataINT(MS3Record *&msr,int* data,int size);
void seedOut3CH(const char* start,const char* filename,
                const char* sh1, const char* sh2, const char* sh3,
                int * c_ch1,int * c_ch2,int* c_ch3,int size);
void seedOut3CHf(const char* start,const char* filename,
                const char* sh1, const char *sh2, const char* sh3,
                float * c_ch1,float * c_ch2,float* c_ch3, int size);
void seedOutf(const char* start,const char* filename, const char* sh1,float data,int size);
void seedOut(const char* start,const char* filename, const char* sh1,int data,int size);

}


void writeseed (std::vector<std::vector<float>> data, std::string start,
                QString folder, QString ID,QString fn, QString network);

void writeseed (std::vector<std::vector<int>> data, std::string start,
                QString folder, QString ID,QString fn, QString network);

void writeseed (float data, std::string start,
                QString folder, QString ID,QString fn, QString network,QString folderType);

void writeseed (int data, std::string start,
                QString folder, QString ID,QString fn, QString network);

void writedis (std::vector<std::vector<float>> data, std::string start,
               QString folder, QString ID,QString fn, QString network);

#endif
