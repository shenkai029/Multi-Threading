// Multi-Threading.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <string>

void printNum(const int& number) {

    std::cout << "Print Passing number: " << number << std::endl;

}

void myprint(const int& num, const char* arr) 
// void myprint(const int num, const string& arr) 
{
    std::cout << "Start excuting 2nd thread" << std::endl;

    //printNum(10);
    std::cout << "Passed number is: " << num << std::endl;
    std::cout << "Passed array is: " << arr << std::endl;

    std::cout << "End excuting 2nd thread" << std::endl;
}

class Functor 
{
public:
    void operator () () {
        std::cout << "Start excuting 2nd thread" << std::endl;

        printNum(10);

        std::cout << "End excuting 2nd thread" << std::endl;
    }
};

int main()
{
    int num = 1;
    int& num_ref = num;
    char str_arr[] = "this is a test!";
    // even we use ref to pass num here but thread will still create a copy of num in run, but still better not use ref here
    // char pointer will be directly passed to function, detach will make mem leak so should avoid pass pointer to thread as arg
    // use string ref will still not work, because implicity casting from char[] to string have no garanteen time and excution, the
    // main thread could be completed before casting happen. Also implicity casting call of class constructor will cause same issue
    // so the correct way is to use explicity casting like string(str_arr) to generate a hard copy of str_arr and pass as arg
    // for class A we will also have to use A(*) to explicity casting which will call copy constructor to create new temp object
    // however the function better to take class ref as arg, other wise the function will make second copy of the temp object
    std::thread my_thread_obj_01(myprint, num, str_arr);

    /*Functor functor;
    std::thread my_thread_obj_02(functor);*/

    if (my_thread_obj_01.joinable())
        my_thread_obj_01.join();
    //my_thread_obj_01.detach();

    std::cout << "Hello World!\n";

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
