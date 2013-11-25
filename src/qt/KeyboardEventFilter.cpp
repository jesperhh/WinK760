#include "KeyboardEventFilter.h"
#include <QEvent>
#include <QKeyEvent>
#include <QQuickView>
#include <QQmlEngine>

KeyboardEventFilter::KeyboardEventFilter(QQuickView &viewer, const QString mainWindowQmlPath, QObject *parent):
    QObject(parent), mainWindowQmlPath(mainWindowQmlPath), viewer(viewer)
{
}

bool KeyboardEventFilter::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *KeyEvent = (QKeyEvent*)event;
        switch(KeyEvent->key())
        {
        case Qt::Key_F5:
            this->viewer.engine()->clearComponentCache();
            this->viewer.setSource(QUrl::fromLocalFile(mainWindowQmlPath));
            return true;
        }
    }

    return false;
}