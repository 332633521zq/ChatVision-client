#include "tokenbucket.h"

TokenBucket::TokenBucket(const int &capacity, const int &rate)
    : _capacity(capacity)
    , _token_num(capacity)
    , _rate(rate)
{
    _refill_thread = std::thread(&TokenBucket::refill, this);
}

TokenBucket::~TokenBucket()
{
    _stop = true;
    _refill_thread.join();
}

bool TokenBucket::acquire(const int &num)
{
    std::unique_lock<std::mutex> lock(_mutex);

    if (_token_num < num)
        return false;

    _token_num -= num;
    return true;
}

void TokenBucket::refill()
{
    while (!_stop) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::unique_lock<std::mutex> lock(_mutex);
        _token_num = std::min(_capacity, _token_num + _rate);
        // _cv.notify_all();
    }
}
