#ifndef ccgrd
#define ccgrd

#include <zconf.h>
#include <iostream>
#include "atomic"
#include "cg_shared_ptr.h"

using std::atomic_uintptr_t;
using std::atomic_intptr_t;
using std::atomic_int;
using std::memory_order;
using namespace std;

template <class T>
class cg_shared_ptr;

#define ptrMask     0x0000FFFFFFFFFFFF
#define addValue    0x0001000000000000
#define addTwoValue 0x0002000000000000
#define counterMask 0xFFFF000000000000
#define getCount(value) ((value & counterMask) >> 48)

template <class T>
class concurrent_guard {
protected:
    volatile atomic_uintptr_t data;
    bool try_increase_counter() {
        uintptr_t expected;
        uintptr_t desired;
        do {
            expected = data.load(memory_order_acquire);
            if(!expected) {
                return false;
            }
            desired = expected + addValue;
        } while (!data.compare_exchange_weak(expected, desired));
        //cout << "increased" << endl;
        return true;
    }

    void decrease_counter() {
        uintptr_t expected;
        uintptr_t desired;
        bool isDeleted;
        do {
            expected = data.load(memory_order_acquire);
            if(getCount(expected) == 1) {
                desired = 0;
                isDeleted = true;
            } else {
                isDeleted = false;
                desired = expected - addValue;
            }
        } while (!data.compare_exchange_weak(expected, desired));
        if(isDeleted)
            delete reinterpret_cast<T*>(expected & ptrMask);
    }
public:
    concurrent_guard() {
        data.store(0, memory_order_release);
    }

    cg_shared_ptr<T> try_set(T* pointer) {
        cg_shared_ptr<T> result;
        uintptr_t expected;
        uintptr_t desired = reinterpret_cast<uintptr_t>(pointer) + addValue;
        do {
            expected = data.load(memory_order_acquire);
            int a = 2;
            if(expected > 0) {
                //cout << " > 0" << endl;
                return result;
            }
            else {
                //cout << " ======= 0" << endl;
            }
        } while (!data.compare_exchange_weak(expected, desired));
        result.host = this;
        return result;
    }

    cg_shared_ptr<T> try_get() {
        cg_shared_ptr<T> result;
        if(try_increase_counter()) {
            result.host = this;
        }
        return result;
    }

    T* operator->() {
        return reinterpret_cast<T*>(data & ptrMask);
    }
    T& operator* () {
        return *(operator->());
    };

    bool is_set() const {
            return (bool) data;
    }

    explicit operator bool() const
    {
        return (bool) data;
    }

    friend class cg_shared_ptr<T>;
};

#endif