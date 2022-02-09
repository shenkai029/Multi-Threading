// Multi-Threading.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>

void printNum(const int& number) {

    std::cout << "Print Passing number: " << number << std::endl;

}

void myprint() 
{
    std::cout << "Start excuting 2nd thread" << std::endl;

    printNum(10);

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

    std::thread my_thread_obj_01(myprint);

    /*Functor functor;
    std::thread my_thread_obj_02(functor);*/

    //my_thread_obj_01.join();
    my_thread_obj_01.detach();

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
