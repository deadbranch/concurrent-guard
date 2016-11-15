#include <iostream>
#include <thread>
#include <mutex>
#include "cg_shared_ptr.h"

volatile long long i, d, f;
mutex m;

class Test {
public:
    int h = 228;
    Test() {
        h = rand() % 10;
        ++d;
        //cout << "()" << endl;
    }
    ~Test() {
        ++f;
        //cout << "~()" << endl;
    }
    int getA() {
        return 42;
    }
};

Test* t;

cg_shared_ptr<Test> dump;
concurrent_guard<Test> h;
volatile bool work = true;

void ShowH(cg_shared_ptr<Test> test) {
    cout << "))) " << test->h << endl;
}

void reader() {
    while(work) {
        auto ptr = h.try_get();
        if(ptr) {
            ++i;
            int a = ptr->h;
            cout << ptr->h << endl;
            ShowH(move(ptr));
        }
        else {
            //cout << "-" << endl;
        }
    }
}

void reader2() {
    while(work) {
        auto ptr = t;
        if(ptr) {
            ++i;
            int a = ptr->h;
            cout << ptr->h << endl;
        }
        else {
            ++f;
            //cout << "-" << endl;
        }
    }
}

void writer() {
    while(true) {
        Test* t = new Test();
        auto ptr = h.try_set(t);
        if(ptr) {
            ++d;
            sleep(1);
            //cout << "SEEEEEEEEEEEEEEEEEEEEEEEET" << endl;
        }
        else {
            delete t;
            //cout << "NON SET--------------------------" << endl;
        }

    }
}

int main() {
    i=d=0;
    t = new Test();
    thread(writer).detach();
    thread(reader).detach();
    thread(reader).detach();
    thread(reader).detach();
    sleep(11);
    work = false;
    cout << i << " "<< d << " " << f << endl;
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