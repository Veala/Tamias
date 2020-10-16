#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QShortcut>
#include <QRegExpValidator>

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

    void setPauseTime(QString time);
    QString getPauseTime();
    void setPauseChecked(bool ch);
    bool getPauseChecked();
    void setLogFileName(QString name);
    QString getLogFileName();

private:
    Ui::Settings *ui;
    QRegExpValidator validator;
};

#endif // SETTINGS_H
