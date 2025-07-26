#include <iostream>
#include <thread>
#include <atomic>
using namespace std;

atomic<bool> stop_flag(false);

void worker_thread(){
    while(!stop_flag.load(memory_order_relaxed)){
        cout << "Working..." << endl;
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    cout << "Thread stopped..." << endl;
}

int main(){
    thread th(worker_thread);
    this_thread::sleep_for(chrono::seconds(1));
    stop_flag.store(true, memory_order_relaxed);
    th.join();
    return 0;
}