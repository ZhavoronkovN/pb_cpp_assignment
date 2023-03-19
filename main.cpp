#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "PFileManager.h"
#include <filesystem>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    const char *start_path = "images";
    if (argc > 1)
    {
        start_path = argv[1];
    }
    std::filesystem::path start_fs_path = std::filesystem::absolute(std::filesystem::path(start_path));
    QObject *manager = new PFileManager(start_fs_path.c_str());
    engine.rootContext()->setContextProperty("Manager", manager);
    engine.rootContext()->setContextProperty("start_path", start_fs_path.c_str());
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl)
        {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);
    return app.exec();
}
