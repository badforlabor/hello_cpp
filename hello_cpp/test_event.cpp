
#include "Event.h"

#include <iostream>
#include "AutoRegTestFunc.h"

namespace test_event
{
    struct Client
    {
        int money = 0;
    };

    struct Accountant
    {
        void OnMoneyPaidMemberFunc(int& amount, Client& client)
        {
            std::cout << "OnMoneyPaid called with amount " << amount << " and client Money " << client.money <<
                std::endl;
            client.money += 100;
            amount -= 100;
        }
    };

    void OnShiftStarted()
    {
        std::cout << "OnShiftStarted called" << std::endl;
    }

    void OnTest(int i, Accountant acc)
    {
        std::cout << "OnTest called " << i << std::endl;
    }

    static void TestForward()
    {
        using namespace EventIntern;
        
        Event<void> ShiftStarted;
        Event<int&, Client&> MoneyPaid;

        Client client;
        Accountant accountant;
        int value = 200;

        std::cout << "Starting" << std::endl;

        ShiftStarted += OnShiftStarted;
        MoneyPaid += EventHandler::Bind(&Accountant::OnMoneyPaidMemberFunc, &accountant);
        MoneyPaid.Add(&Accountant::OnMoneyPaidMemberFunc, &accountant);

        std::cout << "Calling" << std::endl;
        MoneyPaid(value, client);
        ShiftStarted();

        std::cout << "Calling with changed Value" << std::endl;
        MoneyPaid(value, client);

        MoneyPaid.Remove(&Accountant::OnMoneyPaidMemberFunc, &accountant);
        MoneyPaid -= EventHandler::Bind(&Accountant::OnMoneyPaidMemberFunc, &accountant);
        ShiftStarted -= OnShiftStarted;

        std::cout << "Calling removed" << std::endl;
        MoneyPaid(value, client);
        ShiftStarted();
        
        
        std::cout << "Ending" << std::endl;
    }
    static AutoRegTestFunc autoTestForward(TestForward);
}
