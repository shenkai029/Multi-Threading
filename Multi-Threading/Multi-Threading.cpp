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
    static std::mutex* m_mutex;

public:
    static MySingleton* GetInstance() {
        if (m_mutex == nullptr) {
            m_mutex = new std::mutex();
        }

        if (m_instance == nullptr) { // double lock/check, if not nullptr no need to lock
            std::unique_lock<std::mutex> my_lock(*m_mutex); // add lock

            if (m_instance == nullptr) {
                m_instance = new MySingleton();
                std::cout << "Init instance only once!" << std::endl;
                static MemRelease mr;
            }
        }
        return m_instance;
    }

    class MemRelease {
    public:
        ~MemRelease() {
            if (MySingleton::m_instance != nullptr) {
                delete MySingleton::m_instance;
                MySingleton::m_instance = nullptr;

                delete MySingleton::m_mutex;
                MySingleton::m_mutex = nullptr;
            }
        }
    };

    void func() {
        std::cout << "Test...obj address: " << m_instance<< std::endl;
    }
};

MySingleton* MySingleton::m_instance = nullptr; // static member must be initialize outside class scope
std::mutex* MySingleton::m_mutex = nullptr;

void thread_func() {
    std::cout << "Start excuting thread....." << std::endl;

    MySingleton* obj = MySingleton::GetInstance();
    std::cout << "Thread sleep..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    obj->func();

    std::cout << "Stop excuting thread....." << std::endl;
}

// for use singleton class in muti-threading, it is better to get instance in main thread, so the obj will be destory when main thread end 

int main()
{
   /* MySingleton* my_obj_01 = MySingleton::GetInstance();
    my_obj_01->func();
    
    MySingleton* my_obj_02 = MySingleton::GetInstance();
    my_obj_02->func();*/

    std::thread th_01(thread_func);
    std::thread th_02(thread_func);
    th_01.join();
    th_02.join();

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
