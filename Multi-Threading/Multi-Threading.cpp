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
        // lock_guard basicly lock the data when its obj created by constructor call and unlock when obj is out scope and destructor called
        for (int i = 0; i < 10000; ++i) {
            {
                //std::unique_lock<std::mutex> push_guard(m_mutex, std::try_to_lock); // if use lock_guard/unique_lock, no need use lock() and unlock()
                std::unique_lock<std::mutex> push_guard(m_mutex, std::defer_lock); // bond m_mutex to push_guard, and init as unlock
                push_guard.lock();
                //if (push_guard.owns_lock()) {
                std::cout << "m_Queue pushed 1 element : " << i << std::endl;
                m_Queue.push_back(i);
                //}
                /*else {
                    std::cout << "Did not get the lock, just do something else..." << std::endl;
                }*/
            }
        }
    }

    // std::adopt_lock can be use as second argument for both lock_guard and unique_lock only when the mutex have been locked, so it won't 
    // call lock in constructor, but only will unlock the mutex when destrucor called

    // std::try_to_lock will allow the thread do not have to wait for unlock of the mutex, and continue excution other code
    // try_to_lock require the mutex unlocked before using it, otherwise will cause error

    // std::defer_lock will bond unique_lock obj to mutex obj and init it as unlock, and later we can call unique_lock memeber functioin
    // lock(), unlock(), try_lock() for more flexiable use in the code
    // release() will unbond mutex obj from unique_lock obj, and return pointer to the unbonded mutex for user to unlock 

    // when using unique_lock bond mutex obj with, the mutex obj cannot be copied, but can be moved:
    // std::unique_lock<std::mutex> guard_01(mutex);
    // std::unique_lock<std::mutex> guard_02(std::move(guard_01)
    // also if we return a unique_lock obj from a function, the move constructor will be called to move same mutex to outer scope obj

    void m_Msg_Pop_Queue() {
        
        for (int i = 0; i < 10000; ++i) {
            {
                // add sleep to balance time between last unlock and current lock, otherwise pop will lock mutex faster than push
                //std::this_thread::sleep_for(std::chrono::microseconds(1)); 
                //m_mutex.lock();
                if (!m_Queue.empty()) { // use double lock to add lock only when queue is empty
                    std::unique_lock<std::mutex> pop_guard(m_mutex); // if use lock_guard/unique_lock, no need use lock() and unlock()
                    //std::this_thread::sleep_for(std::chrono::milliseconds(200)); // add sleep to test try_to_lock
                    if (!m_Queue.empty()) { // if queue is not empty
                        int cmd = m_Queue.front();
                        m_Queue.pop_front();
                        // process cmd 
                        std::cout << "m_Queue popoed 1 element: " << cmd << std::endl;
                    }
                }
                else {
                    std::cout << "Try to pop data from m_Queue, but it is empty now " << i << std::endl;
                }
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
