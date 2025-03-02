#include "mediathread.h"

MediaThread::MediaThread() {}

void MediaThread::startThread()
{
    if (!isRunning()) {
        start();
    }
}

void MediaThread::run()
{
    PiplineBuild *pb = new PiplineBuild();

    pb->start_pipeline(1);

    g_main_loop_run(PiplineBuild::loop);
}
