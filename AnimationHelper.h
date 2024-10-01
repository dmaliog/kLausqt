#ifndef ANIMATIONHELPER_H
#define ANIMATIONHELPER_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QPoint>
#include <QSize>

class AnimationHelper
{
public:
    static void fadeIn(QWidget *widget, int duration = 1000);
    static void fadeOut(QWidget *widget, int duration = 1000);
    static void slideIn(QWidget *widget, const QPoint &startPoint, const QPoint &endPoint, int duration = 1000);
    static void slideOut(QWidget *widget, const QPoint &startPoint, const QPoint &endPoint, int duration = 1000);
    static void resizeWidget(QWidget *widget, const QSize &startSize, const QSize &endSize, int duration = 1000);
    static void scaleIn(QWidget *widget, int duration = 1000);
    static void scaleOut(QWidget *widget, int duration = 1000);
    static void rotateWidget(QWidget *widget, int duration = 1000);
    static void changeColor(QWidget *widget, const QColor &startColor, const QColor &endColor, int duration = 1000);
};

#endif // ANIMATIONHELPER_H
