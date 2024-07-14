#ifndef _KYLE_EVENTQUEUE_SINGLETON_H_
#define _KYLE_EVENTQUEUE_SINGLETON_H_

#include <string>
#include "EventQueue.h"

class Singleton : public Kyle::EventBase
{
private:
    Singleton();
    ~Singleton();
public:
    static Singleton* GetInstance();
    static void Destroy();
    static std::string GetName();
    bool Process(const uint32_t pidx, const uint32_t sidx, const char* data, const uint32_t size) override;
private:
    void Process_NULL();
private:
    static Singleton* instance;
};

#endif //_KYLE_EVENTQUEUE_SINGLETON_H_
