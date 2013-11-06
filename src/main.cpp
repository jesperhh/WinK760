//
// WinK760: A utility for enabling Fn-key inversion for the Logitech K760 keyboard on Windows.
//
// WinK760 is inspired by the Solaar and UPower libraries.
//
// Copyright 2013 Jesper Hellesø Hansen

#include <QtGui/QGuiApplication>
#include <QQuickView>

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    QQuickView viewer;
    /*viewer.connect(viewer.engine(), SIGNAL(viewer.quit()), SLOT(viewer.close()));*/
    viewer.setSource(QStringLiteral("qrc:/qml/window/main.qml"));
    viewer.show();

    return app.exec();
}
