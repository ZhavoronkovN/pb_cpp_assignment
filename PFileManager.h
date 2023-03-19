#ifndef PFILEMANAGER_H
#define PFILEMANAGER_H
#include <QtCore>
#include <QString>
#include <QList>

class PFileModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ get_name CONSTANT)
    Q_PROPERTY(QString ext READ get_ext CONSTANT)
    Q_PROPERTY(QString full_path READ get_abs CONSTANT)
    Q_PROPERTY(long size READ get_size CONSTANT)
    Q_PROPERTY(bool is_file READ get_if_file CONSTANT)
    Q_PROPERTY(bool in_work READ get_if_work WRITE set_working NOTIFY in_work_changed)
public:
    PFileModel(const char *abs_path, QObject *parent = nullptr);
    QString get_name() const { return QString(name.c_str()); }
    QString get_ext() const { return QString(ext.c_str()); }
    QString get_abs() const { return QString(abs_path.c_str()); }
    long get_size() const { return file_size; }
    bool get_if_file() const { return is_file; }
    bool get_if_work() const { return in_work; }
    void set_working(bool new_status)
    {
        in_work = new_status;
        emit in_work_changed(new_status);
    }

signals:
    void in_work_changed(bool);

private:
    std::string abs_path;
    std::string name;
    long file_size;
    std::string ext;
    bool is_file;
    bool in_work;
};

class PFileManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<PFileModel *> items READ get_items NOTIFY items_changed)
public:
    PFileManager(const char *start_path, QObject *parent = nullptr);
    Q_INVOKABLE void refresh(const QString &path);
    Q_INVOKABLE void decode(const QString &path);
    Q_INVOKABLE void encode(const QString &path);
    Q_INVOKABLE void show_error(const char *what) const;
    QList<PFileModel *> get_items() const { return items; }
signals:
    void items_changed(void);

private:
    QList<PFileModel *> items;
    void set_working(const std::string &path, bool status);
    QList<PFileModel *> ls(const char *start_path);
};

#endif // PFILEMANAGER_H
