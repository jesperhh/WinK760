#ifndef KEYBOARD_EVENT_FILTER_H
#define KEYBOARD_EVENT_FILTER_H

#include <QObject>

class QQuickView;

class KeyboardEventFilter : public QObject
{
    Q_OBJECT

public:
    explicit KeyboardEventFilter(QQuickView &viewer, const QString mainWindowQmlPath, QObject *parent = 0);
    bool eventFilter(QObject *object, QEvent *event);

private:
    QQuickView &viewer;
    const QString mainWindowQmlPath;
};

#endif // KEYBOARD_EVENT_FILTER_H
