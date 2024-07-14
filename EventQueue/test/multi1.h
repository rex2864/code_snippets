#ifndef _KYLE_EVENTQUEUE_MULTI1_H_
#define _KYLE_EVENTQUEUE_MULTI1_H_

#include <string>
#include "EventQueue.h"

class Multi1 : public Kyle::EventBase
{
private:
    Multi1();
    ~Multi1();
public:
    static Multi1* GetInstance();
    static void Destroy();
    static std::string GetName();
    void SetEQInstance(Kyle::EventQueue* eq);
    bool Process(const uint32_t pidx, const uint32_t sidx, const char* data, const uint32_t size) override;
private:
    void Process_NULL();
private:
    static Multi1* instance;
    Kyle::EventQueue* eq;
};

#endif //_KYLE_EVENTQUEUE_MULTI1_H_
