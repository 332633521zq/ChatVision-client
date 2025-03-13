#pragma once

#include <QThread>
#include <boost/asio.hpp>
#undef signals
#include "piplinebuild.h"
#define signals Q_SIGNALS

class MediaThread : public QThread
{
    Q_OBJECT
public:
    MediaThread(bool is_offer);
    void startThread();
signals:
    void wasHangUp();

protected:
    void run() override;

private:
    bool m_is_offer;
};
