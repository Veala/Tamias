#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QPrinter>
#include <QStringList>
#include "device.h"
#include "tests/abstracttest.h"
#include "lsctable.h"
#include "settings.h"
#include "deviceselectionform.h"

#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public GlobalState, public LSCTable
{
    Q_OBJECT
signals:
    void newDev(QString);
    void setTestStateIcon(int);
    void set_programmerMode(int);

private:
    enum TreeState {
        running,
        next,
        stopped,
        bigStop,
        smallStop,
        finished
    } treeState;
    int curIndex;

    enum DeviceTypes {
        terasic,
        switch411
    };

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void keyPressEvent(QKeyEvent *e);
    void setTreeState(TreeState);
    TreeState getTreeState() const;
    void message(QString m = "", QString browser = "project");
    void run(int);
    int getCFF() { return cff; }

private:
    Ui::MainWindow *ui;

    Settings settings;
    DeviceSelectionForm deviceSelection;
    bool prjLoaded;
    //bool tstLoaded;
    bool su;
    int cff;
    int progrMode;
    int prepareFirstStart;

    QAction *act_devMode1, *act_devMode2;  // включение режима полных возможностей
    QAction *act_programmerMode;       // включение/выключение режима программиста

    bool loadProject(QSettings& settings);
    void loadTest(AbstractTest* test);

    QString logFileDefault;
    QFile logFile;
    QTextStream logStream;
    int counterForCloseOpen;
    QString helpFile;

    bool clearProject();
    void setReport(QTextDocument& doc);
    int dragIndex, dropIndex;
    QString version();
    QTimer timer;

private slots:
    void onAbout();

    void prepare();
    void onCancellation();
    void onRunTst();
    void onPause();
    void onStop();
    //void onStopMonitor();
    void setSlot(QPushButton*, QPushButton*, QPushButton*);
    void unsetSlot(QPushButton*, QPushButton*, QPushButton*);
    void onDragged();
    void onDropped();
    void addNewDevice(QString, int);
    void delDevice(QString);
    void createTest();
    void openTest();
    void writeLog(QString);

    bool onSavePrj();
    void onLoadPrj();

    void onHelp();
    bool onCreateRep();
    void onPrintRep();
    void printReport(QPrinter* prnt);
    void onMenuDevices(QPoint);
    void onMenuTests(QPoint);
    void onMenuOutput(QPoint);
    void outputClear();
    void actDevMode();
    void actProgrMode();

    void applyToAllTests(QString&,QString&,QString&,QString&);
};

#endif // MAINWINDOW_H
