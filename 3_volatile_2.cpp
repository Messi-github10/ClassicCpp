#include <iostream>
#include <thread>
using namespace std;

volatile int counter = 0;

void increment(){
    for (int i = 0; i < 10000000; i++){
        ++counter;  // volatile并不能保证原子操作
    }
}

int main(){
    thread th1(increment);
    thread th2(increment);

    th1.join();
    th2.join();

    cout << "Counter:" << counter << endl;

    return 0;
}