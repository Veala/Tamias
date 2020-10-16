#ifndef ANIMATEDSPINNERICON_H
#define ANIMATEDSPINNERICON_H

#include <QWidget>
#include <QTimer>
#include <QColor>
#include <QLabel>

//#include <QDebug>

class CAnimatedSpinnerIcon : public QWidget
{
    Q_OBJECT

public:
    explicit CAnimatedSpinnerIcon(QWidget *parent, bool centerOnParent);

public slots:
    void spinStart();
    void spinStop();

 public:
     bool isSpinning() const;

 private slots:
     void doRotate();

 protected:
     void paintEvent(QPaintEvent *paintEvent);

 private:
     static int lineCountDistanceFromPrimary(int current, int primary,
                                             int totalNrOfLines);
     static QColor currentLineColor(int distance, int totalNrOfLines,
                                    qreal trailFadePerc, qreal minOpacity,
                                    QColor color);
     void initialize(int ht);
     void updateSize();
     void updateTimer();
     void updatePosition();

 private:
     QColor  _color;
     qreal   _roundness; // 0..100
     qreal   _minimumTrailOpacity;
     qreal   _trailFadePercentage;
     qreal   _revolutionsPerSecond;
     int     _numberOfLines;
     int     _lineLength;
     int     _lineWidth;
     int     _innerRadius;

 private:
    CAnimatedSpinnerIcon(const CAnimatedSpinnerIcon&);
    CAnimatedSpinnerIcon& operator=(const CAnimatedSpinnerIcon&);

    QTimer *_timer;
    bool    _centerOnParent;
    int     _currentCounter;
    bool    _isSpinning;
};

CAnimatedSpinnerIcon* insertSpinner(QLabel* w);
void deleteSpinner(CAnimatedSpinnerIcon* spin);

#endif // ANIMATEDSPINNERICON_H
