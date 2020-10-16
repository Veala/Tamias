// Own includes
#include "animatedspinnericon.h"

// Standard includes
#include <cmath>
#include <algorithm>

// Qt includes
#include <QPainter>
//#include <QTimer>
//#include <QDebug>

CAnimatedSpinnerIcon::CAnimatedSpinnerIcon(QWidget *parent,
                                           bool centerOnParent)
    : QWidget(parent),
      _centerOnParent(centerOnParent)
{
    initialize(parent->height());
        /* qDebug() << parentWidget()->width() << " " << width() << " " << parentWidget()->height() << " " << height();
         qDebug() <<  parentWidget()->width() / 2 - width() / 2 << "  " <<
              parentWidget()->height() / 2 - height() / 2;*/
}

 void CAnimatedSpinnerIcon::initialize(int ht)
 {
     int h = ht/5;
     _color = Qt::GlobalColor::blue;
     _roundness = 70.0; //100.0;
     _minimumTrailOpacity = 15.0; // 3.14159265358979323846;
     _trailFadePercentage = 70.0; //80.0;
     _revolutionsPerSecond = 1.0; // 1.57079632679489661923;
     _numberOfLines = 12; // 20;
     _lineLength = h-1;
     _lineWidth = h/2;
     _innerRadius = h;

     _currentCounter = 0;
     _isSpinning = false;

     _timer = new QTimer(this);
     connect(_timer, SIGNAL(timeout()), this, SLOT(doRotate()));
     updateSize();
     updateTimer();
     hide();
     //qDebug() << "Init: " << width() << "  " << height();
 }

 void CAnimatedSpinnerIcon::paintEvent(QPaintEvent *)
 {
     updatePosition();
     QPainter painter(this);
     painter.fillRect(this->rect(), Qt::transparent);
     painter.setRenderHint(QPainter::Antialiasing, true);


     if (_currentCounter >= _numberOfLines) {
         _currentCounter = 0;
     }

     painter.setPen(Qt::NoPen);
     for (int i = 0; i < _numberOfLines; i++) {
         painter.save();
         painter.translate(_innerRadius + _lineLength,
                           _innerRadius + _lineLength);
         qreal rotateAngle =
                 static_cast<qreal>(360 * i) / static_cast<qreal>(_numberOfLines);
         painter.rotate(rotateAngle);
         painter.translate(_innerRadius, 0);
         int distance =
                 lineCountDistanceFromPrimary(i, _currentCounter, _numberOfLines);
         QColor color =
                 currentLineColor(distance, _numberOfLines, _trailFadePercentage,
                                  _minimumTrailOpacity, _color);
         painter.setBrush(color);
         painter.drawRoundedRect(
                     QRect(0, -_lineWidth / 2, _lineLength, _lineWidth), _roundness,
                     _roundness, Qt::RelativeSize);
         painter.restore();
     }
 }

 void CAnimatedSpinnerIcon::spinStart()
 {
     updatePosition();
     _isSpinning = true;
     show();

     if (!_timer->isActive())
     {
         _timer->start();
         _currentCounter = 0;
     }
 }

 void CAnimatedSpinnerIcon::spinStop()
 {
     _isSpinning = false;
     hide();

     if (_timer->isActive()) {
         _timer->stop();
         _currentCounter = 0;
     }
 }

 bool CAnimatedSpinnerIcon::isSpinning() const {
     return _isSpinning;
 }

 void CAnimatedSpinnerIcon::doRotate() {
     ++_currentCounter;
     if (_currentCounter >= _numberOfLines) {
         _currentCounter = 0;
     }
     update();
 }

 void CAnimatedSpinnerIcon::updateSize() {
     int size = (_innerRadius + _lineLength) * 2;
     setFixedSize(size, size);
 }

 void CAnimatedSpinnerIcon::updateTimer() {
     _timer->setInterval(1000 / (_numberOfLines * _revolutionsPerSecond));
 }

 void CAnimatedSpinnerIcon::updatePosition() {
     if (parentWidget() && _centerOnParent) {
         move(parentWidget()->width() / 2 - width() / 2,
              parentWidget()->height() / 2 - height() / 2);
     }
     else move(26, 5);
/*     qDebug() << parentWidget()->width() << " " << width() << " " << parentWidget()->height() << " " << height();
     qDebug() <<  parentWidget()->width() / 2 - width() / 2 << "  " <<
          parentWidget()->height() / 2 - height() / 2;*/
 }

 int CAnimatedSpinnerIcon::lineCountDistanceFromPrimary(int current, int primary,
                                                        int totalNrOfLines) {
     int distance = primary - current;
     if (distance < 0) {
         distance += totalNrOfLines;
     }
     return distance;
 }

 QColor CAnimatedSpinnerIcon::currentLineColor(int countDistance, int totalNrOfLines,
                                               qreal trailFadePerc, qreal minOpacity,
                                               QColor color) {
     if (countDistance == 0) {
         return color;
     }
     const qreal minAlphaF = minOpacity / 100.0;
     int distanceThreshold =
             static_cast<int>(ceil((totalNrOfLines - 1) * trailFadePerc / 100.0));
     if (countDistance > distanceThreshold) {
         color.setAlphaF(minAlphaF);
     } else {
         qreal alphaDiff = color.alphaF() - minAlphaF;
         qreal gradient = alphaDiff / static_cast<qreal>(distanceThreshold + 1);
         qreal resultAlpha = color.alphaF() - gradient * countDistance;

         // If alpha is out of bounds, clip it.
         resultAlpha = std::min(1.0, std::max(0.0, resultAlpha));
         color.setAlphaF(resultAlpha);
     }
     return color;
}

CAnimatedSpinnerIcon* insertSpinner(QLabel* w)
{
    CAnimatedSpinnerIcon* spinner = new CAnimatedSpinnerIcon(w, false);
/*    spinner->setLineLength(7);  //10
    spinner->setLineWidth(4);   // 5
    spinner->setInnerRadius(8);  //10
    spinner->setColor(Qt::GlobalColor::blue);   //(QColor(81, 4, 7)); */

    spinner->spinStart();
    /*qDebug() << w->width() << " " << spinner->width() << " " << w->height() << " " << spinner->height();
    qDebug() <<  w->width() / 2 - spinner->width() / 2 << "  " <<
         w->height() / 2 - spinner->height() / 2;*/
    return spinner;
}

void deleteSpinner(CAnimatedSpinnerIcon* spin)
{
    if (spin)
    {
        {
            if (spin->isSpinning())
                spin->spinStop();
            delete spin;
        }
    }
}
