#ifndef _KYLE_EVENTQUEUE_MULTI2_H_
#define _KYLE_EVENTQUEUE_MULTI2_H_

#include <string>
#include "EventQueue.h"

class Multi2 : public Kyle::EventBase
{
private:
    Multi2();
    ~Multi2();
public:
    static Multi2* GetInstance();
    static void Destroy();
    static std::string GetName();
    void SetEQInstance(Kyle::EventQueue* eq);
    bool Process(const uint32_t pidx, const uint32_t sidx, const char* data, const uint32_t size) override;
private:
    void Process_NULL();
private:
    static Multi2* instance;
    Kyle::EventQueue* eq;
};

#endif //_KYLE_EVENTQUEUE_MULTI2_H_
