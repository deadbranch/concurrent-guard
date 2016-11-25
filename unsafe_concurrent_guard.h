#ifndef SW_SHARED_PTR_UNSAFE_CONCURRENT_GUARD_H
#define SW_SHARED_PTR_UNSAFE_CONCURRENT_GUARD_H

#include "concurrent_guard.h"

template <class T>
class unsafe_concurrent_guard: public concurrent_guard<T> {
public:
    unsafe_concurrent_guard(): concurrent_guard<T>() {}

    bool try_set_unsafe(T* pointer) {
        uintptr_t expected;
        uintptr_t desired = reinterpret_cast<uintptr_t>(pointer) + addValue;
        do {
            expected = this->data.load(memory_order_acquire);
            int a = 2;
            if(expected > 0) {
                return false;
            }
        } while (!this->data.compare_exchange_weak(expected, desired));
        return true;
    }


    cg_shared_ptr<T> try_set_and_unsafe(T* pointer) {
        cg_shared_ptr<T> result;
        uintptr_t expected;
        uintptr_t desired = reinterpret_cast<uintptr_t>(pointer) + addTwoValue;
        do {
            expected = this->data.load(memory_order_acquire);
            int a = 2;
            if(expected > 0) {
                //cout << " > 0" << endl;
                return result;
            }
            else {
                //cout << " ======= 0" << endl;
            }
        } while (!this->data.compare_exchange_weak(expected, desired));
        result.host = this;
        return result;
    }

    void decrease_unsafe() {
        this->decrease_counter();
    }
};

#endif //SW_SHARED_PTR_UNSAFE_CONCURRENT_GUARD_H