// Multi-Threading.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <list>
#include <mutex>
#include <future>

class A {
public:

    int mythread(int num) {
        std::cout << "mythred() start, " << "threadid = " << std::this_thread::get_id() << std::endl;
        std::cout << "num value: " << num << ", threadid = " << std::this_thread::get_id() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        std::cout << "mythred() end, " << "threadid = " << std::this_thread::get_id() << std::endl;
        return 5;
    }
};

int mythread(int num) {
    std::cout << "mythred() start, " << "threadid = " << std::this_thread::get_id() << std::endl;
    std::cout << "num value: " << num << ", threadid = " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    std::cout << "mythred() end, " << "threadid = " << std::this_thread::get_id() << std::endl;
    return 5;
}

void mythreadPrms(std::promise<int>& prms, int num) { // take promise obj ref and a passed variable
    std::cout << "mythredPrms() start, " << "threadid = " << std::this_thread::get_id() << std::endl;
    std::cout << "num value: " << num << ", threadid = " << std::this_thread::get_id() << std::endl;
    ++num;
    num *= 10;
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    std::cout << "mythredPrms() end, " << "threadid = " << std::this_thread::get_id() << std::endl;
    prms.set_value(num);
}

void mythreadFutr(std::future<int>& futr) { // take future obj ref and print
    std::cout << "mythredFutr() start, " << "threadid = " << std::this_thread::get_id() << std::endl;
    std::cout << "futr value: " << futr.get() << ", threadid = " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    std::cout << "mythredFutr() end, " << "threadid = " << std::this_thread::get_id() << std::endl;
}


// async is a function template to start an asyncronize task and return a std::future obj
// asyncronisze taks means start a thread and run the starting function, we can use future obj get() to access its return result, however
// the result might not be able to get at current moment, but when the functioin execution done
// wait() will only wait other thread complete and join main
// if we don't use either wait() or get() after async, main will still wait other thread complete and exit

// the difference between std::async and std::thread is that, async will not be creating new thread in some cases
// i.e. if we use std::launch::deferred in argument, and use get() in main thread, new thread will not be created
// 

// we can pass std::launch type as argument to async for certain purpose:
// std::launch::deferred, means the thread calling function will only start when wait() or get() called
// if we don't use wait()/get() after passing std::launch::deferred, new thread will never be created
// std::launch::async is the default argument, which means the thread creation will happen right away

// std::packaged_task is class template that take function obj, and pack it as a thread calling function

// std::promise is class template that allow assign variable value in one thread, and get that value in other thread 

int main()
{
    std::cout << "main() start, " << "threadid = " << std::this_thread::get_id() << std::endl;
    //std::promise<int> m_prms;
    //std::thread th_obj(mythreadPrms, std::ref(m_prms), 180); // get its ref and pass argument
    //th_obj.join(); // since we use std::thread here to create new thread, have to use join() here to wait
    //std::future<int> result = m_prms.get_future(); // use get_future to return a std::future obj
    ////std::cout << "mythread() return value: " << result.get() << std::endl; // print thread result using get()
    //std::thread th_obj2(mythreadFutr, std::ref(result)); // pass future result to another thread
    //th_obj2.join();
    //std::cout << "Hello World!" << std::endl;
    std::future<int> result = std::async(std::launch::deferred, mythread, 180);
    std::cout << "thread running result : " << result.get() << std::endl;

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
