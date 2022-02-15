// Multi-Threading.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <list>
#include <mutex>

class MySingleton {
private :
	MySingleton() {}

    static MySingleton* m_instance;

public:
    static MySingleton* GetInstance() {
        if (m_instance == nullptr) {
            m_instance = new MySingleton();
            static MemRelease mr;
        }
        return m_instance;
    }

    class MemRelease {
    public:
        ~MemRelease() {
            if (MySingleton::m_instance != nullptr) {
                delete MySingleton::m_instance;
                MySingleton::m_instance = nullptr;
            }
        }
    };

    void func() {
        std::cout << "Test...obj address: " << m_instance<< std::endl;
    }
};

MySingleton* MySingleton::m_instance = nullptr; // static member must be initialize outside class scope


int main()
{
    MySingleton* my_obj_01 = MySingleton::GetInstance();
    my_obj_01->func();
    
    MySingleton* my_obj_02 = MySingleton::GetInstance();
    my_obj_02->func();

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
