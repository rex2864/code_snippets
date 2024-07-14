#include <cstdint>
#include <chrono>
#include <iostream>
#include <thread>
#include "multi2.h"

using namespace Kyle;

Multi2* Multi2::instance = nullptr;

Multi2::Multi2()
{
}

Multi2::~Multi2()
{
}

Multi2* Multi2::GetInstance()
{
    if(Multi2::instance == nullptr)
    {
        Multi2::instance = new Multi2();
    }
    return Multi2::instance;
}

void Multi2::Destroy()
{
    if(Multi2::instance != nullptr)
    {
        delete Multi2::instance;
        Multi2::instance = nullptr;
    }
}

std::string Multi2::GetName()
{
    return "Multi2";
}

void Multi2::SetEQInstance(EventQueue* eq)
{
    this->eq = eq;
}

bool Multi2::Process(const uint32_t pidx, const uint32_t sidx, const char* data, const uint32_t size)
{
    std::chrono::system_clock::time_point tp_now = std::chrono::system_clock::now();
    std::chrono::system_clock::duration dur = tp_now.time_since_epoch();
    uint64_t ts = dur.count();
    std::string time_str = std::to_string(ts / 1000000);
    time_str += ".";
    time_str += std::to_string(ts % 1000000);

    std::cout << "Multi2 -> pidx: " << pidx << " " << time_str << std::endl;
    if(pidx < 10)
    {
        this->eq->AddItem(Multi2::GetName(), pidx + 10, 0, EventQueue::Delay_1s * 5);
    }
    this->Process_NULL();

    return true;
}

void Multi2::Process_NULL()
{
    //std::cout << "Multi2::Process_NULL()" << std::endl;
}
