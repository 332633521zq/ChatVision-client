#include "mediathread.h"

MediaThread::MediaThread(bool is_offer)
    : m_is_offer{is_offer}
{}

void MediaThread::startThread()
{
    if (!isRunning()) {
        start();
    }
}

void MediaThread::run()
{
    PiplineBuild *pb = new PiplineBuild();

    pb->start_pipeline(m_is_offer);

    g_main_loop_run(PiplineBuild::loop);

    g_print("媒体线程结束，发出信号：通话结束");
    emit wasHangUp();
}
