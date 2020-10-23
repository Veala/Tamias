#ifndef CONNECTIONFORM_H
#define CONNECTIONFORM_H

#include <QWidget>
#include <QShortcut>
#include <QtNetwork>

namespace Ui {
class Connection;
}

class Connection : public QWidget
{
    Q_OBJECT

signals:
    void checkDevice(bool);

public:
    explicit Connection(QWidget *parent = 0);
    ~Connection();
    QString getServerIP() const;
    QString getServerPORT() const;
    QString getHostIP() const;
    void setServerIP(QString);
    void setServerPORT(QString);
    void setHostIP(QString);
    void setName(QString);
    bool forBind;

public slots:
    void show_hide_host();

private:
    Ui::Connection *ui;
};

#endif // CONNECTIONFORM_H
