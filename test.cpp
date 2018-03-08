#include <iostream>
#include "source/Event.h"
#include "source/EventManager.h"

class SystemA
{
public:
    SystemA()
    {
        REGISTER_EVENT(&SystemA::OnTest, this);
    }

    void OnTest(TestEvent& event)
    {
        std::cout << "From SystemA a = " << event.a << " b = " << event.b << std::endl;
    }
};


class SystemB
{
public:
    SystemB()
    {
        REGISTER_EVENT(&SystemB::OnTest, this);
    }

    void OnTest(TestEvent& event)
    {
        std::cout << "From SystemB a = " << event.a << " b = " << event.b << std::endl;
    }
};


int main()
{
    SystemA a;
    SystemB b;

    TestEvent event{1, 2.5};
    FIRE_EVENT(event);
    return 0;
}