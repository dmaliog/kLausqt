#include "AnimationHelper.h"
#include <QGraphicsOpacityEffect>
#include <QPalette>

void AnimationHelper::fadeIn(QWidget *widget, int duration) {
    if (!widget || widget->isVisible()) return;
    auto *opacityEffect = new QGraphicsOpacityEffect(widget);
    widget->setGraphicsEffect(opacityEffect);
    widget->setVisible(true);
    auto *animation = new QPropertyAnimation(opacityEffect, "opacity");
    animation->setDuration(duration);
    animation->setStartValue(0);
    animation->setEndValue(1);
    QAbstractAnimation::connect(animation, &QPropertyAnimation::finished, animation, &QObject::deleteLater);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void AnimationHelper::fadeOut(QWidget *widget, int duration) {
    if (!widget || !widget->isVisible()) return;
    auto *opacityEffect = new QGraphicsOpacityEffect(widget);
    widget->setGraphicsEffect(opacityEffect);
    auto *animation = new QPropertyAnimation(opacityEffect, "opacity");
    animation->setDuration(duration);
    animation->setStartValue(1);
    animation->setEndValue(0);
    QAbstractAnimation::connect(animation, &QPropertyAnimation::finished, [widget]() {
        widget->setVisible(false);
    });
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void AnimationHelper::slideIn(QWidget *widget, const QPoint &startPoint, const QPoint &endPoint, int duration) {
    if (!widget) return;
    widget->move(startPoint);
    auto *animation = new QPropertyAnimation(widget, "pos");
    animation->setDuration(duration);
    animation->setStartValue(startPoint);
    animation->setEndValue(endPoint);
    QAbstractAnimation::connect(animation, &QPropertyAnimation::finished, animation, &QObject::deleteLater);
    animation->start();
}

void AnimationHelper::slideOut(QWidget *widget, const QPoint &startPoint, const QPoint &endPoint, int duration) {
    slideIn(widget, startPoint, endPoint, duration);
}

void AnimationHelper::resizeWidget(QWidget *widget, const QSize &startSize, const QSize &endSize, int duration) {
    if (!widget) return;
    widget->resize(startSize);
    auto *animation = new QPropertyAnimation(widget, "size");
    animation->setDuration(duration);
    animation->setStartValue(startSize);
    animation->setEndValue(endSize);
    QAbstractAnimation::connect(animation, &QPropertyAnimation::finished, animation, &QObject::deleteLater);
    animation->start();
}

void AnimationHelper::scaleIn(QWidget *widget, int duration) {
    if (!widget) return;
    auto *animation = new QPropertyAnimation(widget, "geometry");
    animation->setDuration(duration);
    animation->setStartValue(widget->geometry());
    animation->setEndValue(QRect(widget->geometry().x(), widget->geometry().y(), widget->width() * 1.2, widget->height() * 1.2));
    QAbstractAnimation::connect(animation, &QPropertyAnimation::finished, animation, &QObject::deleteLater);
    animation->start();
}

void AnimationHelper::scaleOut(QWidget *widget, int duration) {
    if (!widget) return;
    auto *animation = new QPropertyAnimation(widget, "geometry");
    animation->setDuration(duration);
    animation->setStartValue(widget->geometry());
    animation->setEndValue(QRect(widget->geometry().x(), widget->geometry().y(), widget->width() * 0.8, widget->height() * 0.8));
    QAbstractAnimation::connect(animation, &QPropertyAnimation::finished, animation, &QObject::deleteLater);
    animation->start();
}

void AnimationHelper::rotateWidget(QWidget *widget, int duration) {
    if (!widget) return;
    auto *animation = new QPropertyAnimation(widget, "rotation");
    animation->setDuration(duration);
    animation->setStartValue(0);
    animation->setEndValue(360);
    QAbstractAnimation::connect(animation, &QPropertyAnimation::finished, animation, &QObject::deleteLater);
    animation->start();
}

void AnimationHelper::changeColor(QWidget *widget, const QColor &startColor, const QColor &endColor, int duration) {
    if (!widget) return;
    QPalette palette = widget->palette();
    auto *animation = new QPropertyAnimation(widget, "palette");
    animation->setDuration(duration);
    animation->setStartValue(startColor);
    animation->setEndValue(endColor);
    QAbstractAnimation::connect(animation, &QPropertyAnimation::valueChanged, [widget, &palette](const QVariant &value) {
        palette.setColor(widget->backgroundRole(), value.value<QColor>());
        widget->setPalette(palette);
    });
    QAbstractAnimation::connect(animation, &QPropertyAnimation::finished, animation, &QObject::deleteLater);
    animation->start();
}
