//
//  cxx11_ptr_check.cpp
//  
//
//  Created by 목성균 on 2019/10/28.
//
#include <atomic>
#include <thread>
#include <iostream>
#include <assert.h>
std::atomic<int *> p;
int A, B;
void Thread1() { for (int i = 0; i < 100000; i++) p = &A; }
void Thread2() { for (int i = 0; i < 100000; i++) p = &B; }
void Thread3() { for (int i = 0; i < 100000; i++) assert(*p == 0); }

int main() {
  std::cout << "A=" << &A << " B=" << &B << std::endl;
  p = &A;
  std::thread t1(Thread1);
  std::thread t2(Thread2);
  std::thread t3(Thread3);
  t1.join();
  t2.join();
  t3.join();
}
