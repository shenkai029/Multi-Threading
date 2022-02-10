// Multi-Threading.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <list>
#include <mutex>

// read only data is safe to use for multi-thread, so won't allow multiple threads write/read to same data  at same time
std::vector<int> my_vec = { 1, 2, 3 }; 

void myprint(const int& num) 
{
    std::cout << "Start excuting thread with #" << num << std::endl;

    std::cout << "thread id #" << std::this_thread::get_id() << " read data from my_vec: " << my_vec[0] << my_vec[1] << my_vec[2] << std::endl;

    std::cout << "End excuting thread with #" << num << std::endl;
}

// class to simulate push and pop data 
class A {
public:
    void m_Msg_Push_Queue() {
        for (int i = 0; i < 10000; ++i) {
            std::cout << "m_Queue pushed 1 element : " << i << std::endl;

            m_mutex.lock();
            m_Queue.push_back(i);
            m_mutex.unlock();
        }
    }

    void m_Msg_Pop_Queue() {
        for (int i = 0; i < 10000; ++i) {
            if (!m_Queue.empty()) { // if queue is not empty
                m_mutex.lock();
                int cmd = m_Queue.front();
                m_Queue.pop_front();
                m_mutex.unlock();
                // process cmd 
                std::cout << "m_Queue popoed 1 element: " << cmd << std::endl;
            }
            else {
                std::cout << "Try to pop data from m_Queue, but it is empty now " << i << std::endl;
            }
        }
    }
    
private:
    std::list<int> m_Queue; // share data for multi-thread
    std::mutex m_mutex; // create mutex memeber to lock/unlock data
};

int main()
{
    //std::vector<std::thread> my_thread;

    //// all thread might not follow excuation order from 0 to 9
    //for (int i = 0; i < 10; ++i) {
    //    my_thread.push_back(std::thread(myprint, i));
    //}

    //// better to use join wait for all other threads join main thread at the end for stable result
    //for (auto it = my_thread.begin(); it != my_thread.end(); ++it) {
    //    it->join();
    //}

    A my_obj_A;
    std::thread my_pop_Queue(&A::m_Msg_Pop_Queue, &my_obj_A);
    std::thread my_push_Queue(&A::m_Msg_Push_Queue, &my_obj_A); // use reference here to make sure push/pop same class data memeber
    my_pop_Queue.join();
    my_push_Queue.join();

    std::cout << "Hello World!" << std::endl;

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
