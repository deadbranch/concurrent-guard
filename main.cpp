#include <iostream>
#include <thread>
#include <mutex>
#include "cg_shared_ptr.h"
#include "unsafe_concurrent_guard.h"

volatile long long i, d, f;
mutex m;

class Test {
public:
    int h = 228;
    Test() {
        ++d;
        cout << "()" << endl;
    }
    ~Test() {
        ++f;
        cout << "~()" << endl;
    }
    int getA() {
        return h;
    }
};

int main() {
    unsafe_concurrent_guard<Test> guard;
    guard.try_set_unsafe(new Test());
    auto ptr = guard.try_get();
    cout << ptr->h << endl;
    ptr = nullptr;
    guard.decrease_unsafe();
    return 0;
}

/*
 * cout << sizeof(h) << endl;
    Test* t = new Test();
    {
        cg_shared_ptr<Test> p = h.try_set(t);
        dump = p;
        if(p) {
            cout << "ok!" << endl;
            cout << h->getA() << endl;
            //Test& res = *h;
            //cout << res.getA() << endl;
            int a = 2;
        }
        else {
            cout << "bad pointer!" << endl;
        }
        int a = 2;
    }
    cg_shared_ptr<Test> d2 = dump;
    dump.reset();
    cout << d2->getA() << endl;
    int* a = new int[5]();
    int* ptr = new int(55465);
    printf("Address of x is %p\n", (void *)&ptr);
    std::cout << &ptr << std::endl;
 */