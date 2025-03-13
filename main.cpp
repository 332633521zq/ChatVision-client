#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "client.h"
#include "communicationpagecontroller.h"
#include "followlistpagecontroller.h"
#include "personalpagecontroller.h"

// #include "ConstValue.h"
// #include "mediathread.h"
// #include <boost/asio.hpp>
// #include <iostream>
// #include <nlohmann/json.hpp>
// #include <thread>

using namespace boost::asio::ip;

// #define IPADDRESS "192.168.253.186"
#define IPADDRESS "127.0.0.1"
#define PORT 10086

int main(int argc, char* argv[])
{
    Client client;

    QGuiApplication app(argc, argv);

    // gst_init(&argc, &argv);

    // sleep(3);
    // //媒体线程
    // PiplineBuild::m_socket = &sock;
    // PiplineBuild::m_object_id = object_id;
    // MediaThread* mediaThread = new MediaThread();
    // mediaThread->startThread();

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("followingPageController",
                                             &FollowListPageController::getInstance());
    engine.rootContext()->setContextProperty("personalPageControler",
                                             &PersonalPageController::getInstance());
    engine.rootContext()->setContextProperty("communicationPageControler",
                                             &CommunicationPageController::getInstance());

    const QUrl url(QStringLiteral("qrc:/ChatVision-client/LoginPage.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
