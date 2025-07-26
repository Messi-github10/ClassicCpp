#include <iostream>
using namespace std;

// 编译时常量
void test11(){
    constexpr int a = 10;
    constexpr int b = a + 5;
    int runtime_val = 20;
    // constexpr int c = runtime_val;  // Error:runtime_val 不是编译期常量
}

// 运行时常量
void test12(){
    const int a = 10;
    const int b = a + 5;
    int runtime_val = 20;
    const int c = runtime_val;  // 运行时常量
}

int main(){

    return 0;
}