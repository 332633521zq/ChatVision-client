#pragma once

#include <boost/asio.hpp>
#include <mutex>

class TokenBucket
{
public:
    TokenBucket(const int &capacity, const int &rate);
    ~TokenBucket();
    bool acquire(const int &num);

private:
    void refill();

    int _token_num;
    int _capacity;
    int _rate;
    std::mutex _mutex;
    std::condition_variable _cv;
    std::thread _refill_thread;
    bool _stop = false;
};
