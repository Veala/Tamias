#ifndef COMBOBOXNOSCROLL_H
#define COMBOBOXNOSCROLL_H

#include <QComboBox>
#include <QWheelEvent>
#include <QDebug>

class ComboBoxNoScroll : public QComboBox
{
public:
    explicit ComboBoxNoScroll(QWidget *parent=0) : QComboBox(parent) {}
    ~ComboBoxNoScroll() {}

    //void wheelEvent(QWheelEvent *) {}
};

#endif // COMBOBOXNOSCROLL_H
