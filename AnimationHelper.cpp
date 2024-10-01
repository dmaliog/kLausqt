#include "AnimationHelper.h"
#include <QGraphicsOpacityEffect>
#include <QGraphicsRotation>
#include <QPalette>

void AnimationHelper::fadeIn(QWidget *widget, int duration) {
    widget->setWindowOpacity(0);
    QPropertyAnimation *animation = new QPropertyAnimation(widget, "windowOpacity");
    animation->setDuration(duration);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();
}

void AnimationHelper::fadeOut(QWidget *widget, int duration) {
    QPropertyAnimation *animation = new QPropertyAnimation(widget, "windowOpacity");
    animation->setDuration(duration);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
}

void AnimationHelper::slideIn(QWidget *widget, const QPoint &startPoint, const QPoint &endPoint, int duration) {
    widget->move(startPoint);
    QPropertyAnimation *animation = new QPropertyAnimation(widget, "pos");
    animation->setDuration(duration);
    animation->setStartValue(startPoint);
    animation->setEndValue(endPoint);
    animation->start();
}

void AnimationHelper::slideOut(QWidget *widget, const QPoint &startPoint, const QPoint &endPoint, int duration) {
    widget->move(startPoint);
    QPropertyAnimation *animation = new QPropertyAnimation(widget, "pos");
    animation->setDuration(duration);
    animation->setStartValue(startPoint);
    animation->setEndValue(endPoint);
    animation->start();
}

void AnimationHelper::resizeWidget(QWidget *widget, const QSize &startSize, const QSize &endSize, int duration) {
    widget->resize(startSize);
    QPropertyAnimation *animation = new QPropertyAnimation(widget, "size");
    animation->setDuration(duration);
    animation->setStartValue(startSize);
    animation->setEndValue(endSize);
    animation->start();
}

void AnimationHelper::scaleIn(QWidget *widget, int duration) {
    QPropertyAnimation *animation = new QPropertyAnimation(widget, "geometry");
    animation->setDuration(duration);
    animation->setStartValue(widget->geometry());
    animation->setEndValue(QRect(widget->geometry().x(), widget->geometry().y(), widget->width() * 1.2, widget->height() * 1.2));
    animation->start();
}

void AnimationHelper::scaleOut(QWidget *widget, int duration) {
    QPropertyAnimation *animation = new QPropertyAnimation(widget, "geometry");
    animation->setDuration(duration);
    animation->setStartValue(widget->geometry());
    animation->setEndValue(QRect(widget->geometry().x(), widget->geometry().y(), widget->width() * 0.8, widget->height() * 0.8));
    animation->start();
}

void AnimationHelper::rotateWidget(QWidget *widget, int duration) {
    QPropertyAnimation *animation = new QPropertyAnimation(widget, "rotation");
    animation->setDuration(duration);
    animation->setStartValue(0);  // Начальный угол
    animation->setEndValue(360);  // Конечный угол
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void AnimationHelper::changeColor(QWidget *widget, const QColor &startColor, const QColor &endColor, int duration) {
    QPalette palette = widget->palette();
    QPropertyAnimation *animation = new QPropertyAnimation(widget, "palette");
    animation->setDuration(duration);
    animation->setStartValue(startColor);
    animation->setEndValue(endColor);

    QAbstractAnimation::connect(animation, &QPropertyAnimation::valueChanged, [widget, &palette](const QVariant &value) {
        palette.setColor(widget->backgroundRole(), value.value<QColor>());
        widget->setPalette(palette);
    });

    animation->start(QAbstractAnimation::DeleteWhenStopped);
}
