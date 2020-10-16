#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>

Q_DECLARE_METATYPE(QVector<int>)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    qRegisterMetaType< QVector<int> >();
    MainWindow w;
    w.show();

    return a.exec();
}
