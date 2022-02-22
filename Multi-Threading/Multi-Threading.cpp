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
        for (int i = 0; i < m_size; ++i) {
            {
                //std::unique_lock<std::mutex> push_guard(m_mutex, std::try_to_lock); // if use lock_guard/unique_lock, no need use lock() and unlock()
                std::unique_lock<std::mutex> push_guard(m_mutex, std::defer_lock); // bond m_mutex to push_guard, and init as unlock
                push_guard.lock();
                //if (push_guard.owns_lock()) {
                std::cout << "m_Queue pushed 1 element : " << i << std::endl;
                m_Queue.push_back(i);
                //m_cond.notify_one(); // try to notify only one other thread that is in wait(), and continue execution
                m_cond.notify_all(); // try to notify not only one thread that is in wait()
                //}
                /*else {
                    std::cout << "Did not get the lock, just do something else..." << std::endl;
                }*/
            }
        }
    }

    // std::adopt_lock can be use as second argument for both lock_guard and unique_lock only when the mutex have been locked, so it won't 
    // call lock in constructor, but only will unlock the mutex when destrucor called

    // std::try_to_lock will allow the thread do not have to wait for unlock of the mutex, and continue execution other code
    // try_to_lock require the mutex unlocked before using it, otherwise will cause error

    // std::defer_lock will bond unique_lock obj to mutex obj and init it as unlock, and later we can call unique_lock memeber functioin
    // lock(), unlock(), try_lock() for more flexiable use in the code
    // release() will unbond mutex obj from unique_lock obj, and return pointer to the unbonded mutex for user to unlock 

    // when using unique_lock bond mutex obj with, the mutex obj cannot be copied, but can be moved:
    // std::unique_lock<std::mutex> guard_01(mutex);
    // std::unique_lock<std::mutex> guard_02(std::move(guard_01)
    // also if we return a unique_lock obj from a function, the move constructor will be called to move same mutex to outer scope obj

    // std:: condition_variable is actually a class and need to pair use with mutex object

    void m_Msg_Pop_Queue() {

        //int cmd = 0;
        while (true) {
            //if (m_count == 999) break;
            std::unique_lock<std::mutex> pop_guard(m_mutex);

            // if wait() get false on second argument(func obj), it unlocks and stop execution until other thread call notify_one()
            // if no second argument passed in, it gets false by default
            // once wait() get waked by notify_one() it will try to lock mutex until suncess to continue evaluate true/false again
            // if wait() get true on second argument, it continues execution
            m_cond.wait(pop_guard, [this] { // use lambda to create a func object and pass to wait
                // for thread more than 1, we use shared m_count and check if it reaches the last number, if so wait() will get true
                // and continue execution and break. If don't check m_count here, when 1 thread pop last number out, other threads 
                // will forever wait here since the queue is empty. Also, if we don't share m_count here, other thread will not know
                // the queue is empty and wait in dead loop
                if (!m_Queue.empty() || m_count == m_size - 1) return true;
                return false;
                });

            if (m_count == m_size - 1) break;
            m_count = m_Queue.front();
            m_Queue.pop_front();
            std::cout << "m_Queue popoed 1 element: " << m_count << ", threadID = " << std::this_thread::get_id() << std::endl;
            pop_guard.unlock(); // release lock earlier for code efficiency
        }
        
        //for (int i = 0; i < 10000; ++i) {
        //    {
        //        // add sleep to balance time between last unlock and current lock, otherwise pop will lock mutex faster than push
        //        //std::this_thread::sleep_for(std::chrono::microseconds(1)); 
        //        //m_mutex.lock();
        //        if (!m_Queue.empty()) { // use double lock to add lock only when queue is empty
        //            std::unique_lock<std::mutex> pop_guard(m_mutex); // if use lock_guard/unique_lock, no need use lock() and unlock()
        //            //std::this_thread::sleep_for(std::chrono::milliseconds(200)); // add sleep to test try_to_lock
        //            if (!m_Queue.empty()) { // if queue is not empty
        //                int cmd = m_Queue.front();
        //                m_Queue.pop_front();
        //                // process cmd 
        //                std::cout << "m_Queue popoed 1 element: " << cmd << std::endl;
        //            }
        //        }
        //        else {
        //            std::cout << "Try to pop data from m_Queue, but it is empty now " << i << std::endl;
        //        }
        //    }
        //}
        std::cout << "All numbers pop out" << std::endl;
    }
    
private:
    std::list<int> m_Queue; // share data for multi-thread
    std::mutex m_mutex; // create mutex memeber to lock/unlock data
    std::condition_variable m_cond; //
    int m_count;
    const int m_size = 3000;
};

//int m_count_glb = 0;
std::atomic<int> m_count_glb = 0;
std::mutex m_mutex_glb;
std::atomic<bool> m_is_end = false;

void mythread() {
    for (int i = 0; i < 10000000; ++i) {
        //m_mutex_glb.lock();
        ++m_count_glb; // atomic execution won't be interupt by other threads
        //m_mutex_glb.unlock();
    }
}

void mythreadbool() {
    while (m_is_end == false) {
        std::cout << "mythreadbool thread_id = " << std::this_thread::get_id() << "running" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
}

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

    //A my_obj_A;
    //std::thread my_pop_Queue1(&A::m_Msg_Pop_Queue, &my_obj_A);
    //std::thread my_pop_Queue2(&A::m_Msg_Pop_Queue, &my_obj_A);
    //std::thread my_pop_Queue3(&A::m_Msg_Pop_Queue, &my_obj_A);
    //std::thread my_push_Queue(&A::m_Msg_Push_Queue, &my_obj_A); // use reference here to make sure push/pop same class data memeber
    //my_pop_Queue1.join();
    //my_pop_Queue2.join();
    //my_pop_Queue3.join();
    //my_push_Queue.join();

    std::thread th_obj1(mythreadbool);
    std::thread th_obj2(mythreadbool);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    m_is_end = true;
    th_obj1.join();
    th_obj2.join();

    // use 2 thread to increment m_count_glb 10 million time should get 20 million but the result is less than 20 million
    // this is because 1 thread could interrupt another thread when they write to same variable so increment will not be completed
    // use mutex will lock the data and make write/read saft but the code will become very slow, about total 7 sec
    
    // use std::atomic instead of mutex, will also make the execution not be interupt by other threads and much faster than use mutex
    // however, mutex will work for multiple code lines execution, while atomic only works for single data/variable



    //std::cout << "Final reuslt of m_count : " << m_count_glb << std::endl;



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
