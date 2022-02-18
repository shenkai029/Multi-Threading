// Multi-Threading.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <list>
#include <mutex>
#include <future>


int mythread() {
    std::cout << "mythred() start, " << "threadid = " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    std::cout << "mythred() end, " << "threadid = " << std::this_thread::get_id() << std::endl;
    return 5;
}

// async is a function template to start an asyncronize task and return a std::future obj
// asyncronisze taks means start a thread and run the starting function, we can use future obj get() to access its return result, however
// the result might not be able to get at current moment, but when the functioin execution done

int main()
{
    std::cout << "main() start, " << "threadid = " << std::this_thread::get_id() << std::endl;
    std::future<int> result = std::async(mythread);
    std::cout << "waiting other thread return value...." << std::endl;
    int def = 0;
    std::cout << "mythread() return value: " << result.get() << std::endl; // thread will stop here wait for mythread() to return result
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
