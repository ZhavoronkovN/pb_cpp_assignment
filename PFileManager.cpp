#include "PFileManager.h"
#include "compressor.h"
#include <QtCore>
#include <QString>
#include <QList>
#include <filesystem>
#include <thread>
#include <QQmlEngine>
#include <QQmlComponent>
#include <iostream>

PFileModel::PFileModel(const char *abs_path, QObject *parent) : abs_path(abs_path), QObject(parent)
{
    const auto file_path = std::filesystem::path(abs_path);
    if (!std::filesystem::exists(file_path))
        throw std::runtime_error("File doesn't exist");
    is_file = std::filesystem::is_regular_file(file_path);
    name = file_path.filename();
    if (is_file)
    {
        file_size = std::filesystem::file_size(file_path);
        ext = file_path.extension();
    }
    in_work = false;
}

PFileManager::PFileManager(const char *start_path, QObject *parent) : QObject(parent)
{
    refresh(start_path);
}
Q_INVOKABLE void PFileManager::refresh(const QString &path)
{
    for (auto item : items)
    {
        delete item;
    }
    items = ls(path.toStdString().c_str());
    emit items_changed();
}
Q_INVOKABLE void PFileManager::decode(const QString &path)
{
    std::thread([this](std::string path)
                {set_working(path, true); 
                    try
                    {
                        
                        const auto fs_path = std::filesystem::path(path);
                        const auto uncompressed_file_name = fs_path.parent_path() / (std::string(fs_path.stem()) + "_unpacked.bmp");
                        std::cout<<"Decoding " << path << " to " << uncompressed_file_name << std::endl;
                        Compressor::decompress(Bitmap::load(path.c_str())).dump(uncompressed_file_name.c_str());
                    }
                    catch(const std::exception& e)
                    {
                        std::cout<<"Error " << e.what() << std::endl;
                        show_error(e.what());
                    }
                    set_working(path, false); },
                path.toStdString())
        .detach();
}
Q_INVOKABLE void PFileManager::encode(const QString &path)
{
    std::thread([this](std::string path)
                {set_working(path, true); 
                    try
                    {
                        const auto fs_path = std::filesystem::path(path);
                        const auto compressed_file_name = fs_path.parent_path() / (std::string(fs_path.stem()) + "_packed.barch");
                        std::cout<<"Encoding " << path << " to " << compressed_file_name << std::endl;
                        Compressor::compress(Bitmap::load(path.c_str())).dump(compressed_file_name.c_str());
                    }
                    catch(const std::exception& e)
                    {
                        std::cerr<<"Error " << e.what() << std::endl;
                        show_error(e.what());
                    }
                    set_working(path, false); },
                path.toStdString())
        .detach();
}
Q_INVOKABLE void PFileManager::show_error(const char *what) const
{
    QQmlEngine engine;
    QQmlComponent component(&engine);
    component.loadUrl(QUrl(QStringLiteral("qrc:/PErrorDialog.qml")));
    component.create();
}
void PFileManager::set_working(const std::string &path, bool status)
{
    for (auto &item : items)
    {
        if (item->get_abs().toStdString() == path)
        {
            item->set_working(status);
            break;
        }
    }
}
QList<PFileModel *> PFileManager::ls(const char *start_path)
{
    auto result = QList<PFileModel *>();
    const auto fs_path = std::filesystem::path(start_path);
    if (!std::filesystem::exists(fs_path))
        return result;
    if (!std::filesystem::is_directory(fs_path))
    {
        result.append(new PFileModel(std::filesystem::absolute(fs_path).c_str()));
    }
    else
    {
        for (auto it = std::filesystem::directory_iterator(fs_path); it != std::filesystem::directory_iterator(); it++)
        {
            result.append(new PFileModel(std::filesystem::absolute(*it).c_str()));
        }
    }
    return result;
}
