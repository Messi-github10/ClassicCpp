#include <iostream>
#include <thread>
using namespace std;

volatile bool stop_flag = false;

void worker_thread(){
    while(!stop_flag){  // 每次都会从内存中读取
        cout << "Working..." << endl;
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    cout << "Thread stopped" << endl;
}

int main(){
    thread th(worker_thread);
    this_thread::sleep_for(chrono::seconds(1));
    stop_flag = true;
    th.join();
    return 0;
}