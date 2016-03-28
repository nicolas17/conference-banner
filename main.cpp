/*
 * Copyright (c) 2016 Nicolás Alvarez
 *
 * Licensed under the General Public License version 3,
 * or (at your option) any later version. See the LICENSE
 * file for details.
 */

#include <QGuiApplication>
#include <QCommandLineParser>
#include <QQuickView>
#include <QQuickItem>

int main(int argc, char** argv) {
    QGuiApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addOption(QCommandLineOption("single-loop", "Run a single loop iteration and quit."));
    parser.addOption(QCommandLineOption("pos", "Set the position of the window (top-left corner). Used for recording.", "x,y"));
    parser.addOption(QCommandLineOption("fullscreen", "Run banner in full screen"));
    parser.process(app);

    if (parser.isSet("pos") && parser.isSet("fullscreen")) {
        qFatal("Can't set position and fullscreen at the same time");
    }

    QQuickView view;
    view.setSource(QUrl::fromLocalFile("../main.qml"));
    view.setResizeMode(QQuickView::SizeRootObjectToView);

    if (parser.isSet("single-loop")) {
        QObject::connect(view.rootObject(), SIGNAL(loopEnded()), &app, SLOT(quit()));
    }
    if (parser.isSet("pos")) {
        const QStringList coords = parser.value("pos").split(',');
        if (coords.length() != 2) {
            qFatal("Invalid --pos");
        }
        bool ok;
        int posx = coords[0].toInt(&ok);
        if (!ok) qFatal("Invalid --pos");
        int posy = coords[1].toInt(&ok);
        if (!ok) qFatal("Invalid --pos");
        view.setPosition(posx, posy);
    }

    if (parser.isSet("fullscreen")) {
        view.showFullScreen();
    } else {
        view.show();
    }

    return app.exec();
}
