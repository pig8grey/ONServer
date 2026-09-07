#ifndef NATIVE_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define NATIVE_H

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <thread>
#include <mutex>

#include <iterator>
#include <algorithm>
#include <string>
#include <cstdlib>




void filter (std::vector<float> &v,bool strong,std::vector<double> &prev);
void findDisplacement (std::vector<float> &v,bool strong,std::vector<double> &prev);

void applyfilter (std::vector<std::vector<float>> &v,bool strong, std::vector<std::vector<double>> &prev);
void applyDisplacement (std::vector<std::vector<float>> &v,bool strong, std::vector<std::vector<double>> &prev);
void findMaxDisplacement (std::vector<float> &v,bool strong,std::vector<double> &prev,
                          float &maxDis, float &minDis);
void applyMaxDisplacement (std::vector<std::vector<float>> &v,bool strong,
                           std::vector<std::vector<double>> &prev,
                           std::vector<float> &maxDis,
                           std::vector<float> &minDis);
#endif
