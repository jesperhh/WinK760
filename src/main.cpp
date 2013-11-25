//
// WinK760: A utility for enabling Fn-key inversion for the Logitech K760 keyboard on Windows.
//
// WinK760 is inspired by the Solaar and UPower libraries.
//
// Copyright 2013 Jesper Hellesø Hansen

#include <QtGui/QGuiApplication>
#include <QQuickView>
#include <QQmlContext>
#include "qt/ApplicationContext.h"
#include "qt/KeyboardEventFilter.h"

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    QQuickView viewer;
    KeyboardEventFilter reloadQml(viewer, QString("C:/Users/Jesper/Code/WinK760/res/qml/window/main.qml"), &viewer);
    ApplicationContext appContext(&app);
    app.installEventFilter(&reloadQml);
    viewer.rootContext()->setContextProperty("appContext", &appContext);
    viewer.setSource(QStringLiteral("qrc:/qml/window/main.qml"));
    viewer.show();

    return app.exec();
}
