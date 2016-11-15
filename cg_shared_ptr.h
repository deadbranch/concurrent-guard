#ifndef ccgsp
#define ccgsp

#include "concurrent_guard.h"


template <class T>
class cg_shared_ptr {
    concurrent_guard<T>* host;
public:
    cg_shared_ptr() {
        host = nullptr;
    }



    friend class concurrent_guard<T>;
    cg_shared_ptr(concurrent_guard<T>* _host): host(_host) {
        if (!host->try_increase_counter())
            host = nullptr;
    }

    T* operator->() const {
        return host->operator->();
    }
    T& operator* () const {
        return *(*host);
    };

    cg_shared_ptr(const cg_shared_ptr& oth) {
        this->host = nullptr;
        if(oth.host->try_increase_counter())
        {
            this->host = oth.host;
        }
    }

    cg_shared_ptr<T>& operator=(const cg_shared_ptr<T>& oth)
    {
        this->host = nullptr;
        if(oth.host->try_increase_counter())
        {
            this->host = oth.host;
        }
        return *this;
    }

    cg_shared_ptr(cg_shared_ptr<T>&& other):host(nullptr) {
        swap(host, other.host);
    }

    cg_shared_ptr<T>& operator=(cg_shared_ptr<T>&& other) {
        host = nullptr;
        swap(host, other.host);
        return *this;
    }

    void reset() {
        if(host) {
            host->decrease_counter();
            host = 0;
        }
    }

    cg_shared_ptr<T>& operator=(nullptr_t _)
    {
        reset();
    }

    explicit operator bool() const
    {
        return (bool)host;
    }
    ~cg_shared_ptr() {
        if(host)
            host->decrease_counter();
    }
};

#endif
