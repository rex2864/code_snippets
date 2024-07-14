#include <cstdint>
#include <chrono>
#include <iostream>
#include <thread>
#include "multi1.h"
#include "multi2.h"

using namespace Kyle;

int32_t main()
{
    std::cout << "multi EventQueue test" << std::endl;

    EventQueue* eq1 = new EventQueue();
    Multi1::GetInstance()->SetEQInstance(eq1);
    (void)eq1->Initialize();
    (void)eq1->AddBase(Multi1::GetName(), dynamic_cast<EventBase*>(Multi1::GetInstance()));
    (void)eq1->AddItem(Multi1::GetName(), 0, 0);
    (void)eq1->AddItem(Multi1::GetName(), 1, 0, EventQueue::Delay_1s);
    (void)eq1->AddItem(Multi1::GetName(), 2, 0, EventQueue::Delay_1s * 2);
    (void)eq1->AddItem(Multi1::GetName(), 3, 0, EventQueue::Delay_1s * 3);
    (void)eq1->AddItem(Multi1::GetName(), 4, 0, EventQueue::Delay_1s * 4);

    EventQueue* eq2 = new EventQueue();
    Multi2::GetInstance()->SetEQInstance(eq2);
    (void)eq2->Initialize();
    (void)eq2->AddBase(Multi2::GetName(), dynamic_cast<EventBase*>(Multi2::GetInstance()));
    (void)eq2->AddItem(Multi2::GetName(), 0, 0);
    (void)eq2->AddItem(Multi2::GetName(), 1, 0, EventQueue::Delay_1s);
    (void)eq2->AddItem(Multi2::GetName(), 2, 0, EventQueue::Delay_1s * 2);
    (void)eq2->AddItem(Multi2::GetName(), 3, 0, EventQueue::Delay_1s * 3);
    (void)eq2->AddItem(Multi2::GetName(), 4, 0, EventQueue::Delay_1s * 4);

    std::this_thread::sleep_for(std::chrono::seconds(15));

    eq1->Finalize();
    Multi1::Destroy();
    delete eq1;

    eq2->Finalize();
    Multi2::Destroy();
    delete eq2;

    return 0;
}
