#pragma once

#include <QThread>
#include <boost/asio.hpp>
#undef signals
#include "piplinebuild.h"
#define signals Q_SIGNALS

class MediaThread : public QThread
{
public:
    MediaThread();
    void startThread();

protected:
    void run() override;
};
