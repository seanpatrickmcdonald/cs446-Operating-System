#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "SeanBasics.h"
#include "Application.h"

void sortSJF(std::vector<Application> &apps);
void sortPS(std::vector<Application> &apps);

#endif