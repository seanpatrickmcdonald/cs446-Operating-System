#include "ResourceHandler.h"

ResourceHandler::ResourceHandler()
{
    num_printers = 0;
    num_hdds = 0;
    last_printer = 0;
    last_hdd = 0; 

    prtMutexes = nullptr;
    hddMutexes = nullptr;
    kybMutex = nullptr;
    monMutex = nullptr;
}

ResourceHandler::~ResourceHandler()
{
    if (prtMutexes)
        delete[] prtMutexes;

    if (hddMutexes)
        delete[] hddMutexes;

    if (kybMutex)
        delete kybMutex;

    if (monMutex)
        delete monMutex;
}

void ResourceHandler::Initialize()
{    
    last_printer = num_printers - 1;  //set to the last printer so we'll start at 0 for the next
    last_hdd = num_hdds - 1;  //set to the last hdd so we'll start at 0 for the next

    prtMutexes = new pthread_mutex_t[num_printers];
    hddMutexes = new pthread_mutex_t[num_hdds];
    kybMutex = new pthread_mutex_t;
    monMutex = new pthread_mutex_t;

    for (unsigned int i = 0; i < num_printers; i++)
    {
        pthread_mutex_init(&prtMutexes[i], NULL);
    }

    for (unsigned int i = 0; i < num_hdds; i++)
    {
        pthread_mutex_init(&hddMutexes[i], NULL);
    }

    pthread_mutex_init(kybMutex, NULL);
    pthread_mutex_init(monMutex, NULL);
}

unsigned int ResourceHandler::GetAvailablePrinter()
{
    last_printer = (last_printer + 1) % num_printers;
    return last_printer;
}

unsigned int ResourceHandler::GetAvailableHdd()
{
    last_hdd = (last_hdd + 1) % num_hdds;
    return last_hdd;
}
