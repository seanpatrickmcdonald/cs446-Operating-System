#ifndef RESOURCE_HANDLER_H_
#define RESOURCE_HANDLER_H_

#include <pthread.h>


class ResourceHandler
{
public: 

    ResourceHandler();
    ~ResourceHandler();

    void Initialize();
    
    unsigned int GetAvailablePrinter();
    unsigned int GetAvailableHdd();

    unsigned int num_printers;
    unsigned int num_hdds;
    unsigned int last_printer;
    unsigned int last_hdd; 

    pthread_mutex_t* prtMutexes;
    pthread_mutex_t* hddMutexes;
    pthread_mutex_t* kybMutex;
    pthread_mutex_t* monMutex;
};







#endif
