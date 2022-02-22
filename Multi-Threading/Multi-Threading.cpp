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


int main()
{
    std::cout << "main() start, " << "threadid = " << std::this_thread::get_id() << std::endl;    
    std::future<int> result = std::async(mythread, 10); 
    //std::cout << "mythread() return value: " << result.get() << std::endl; // print thread result using get()
    std::future_status status = result.wait_for(std::chrono::seconds(6));
    if (status == std::future_status::timeout) { // means thread execution not finished
        std::cout << "Thread execution timeout!" << std::endl;
    }
    else if (status == std::future_status::ready) { // means thread execution finished
        std::cout << "Thread execution done!" << std::endl;
    }
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
