/*
 * Copyright (c) 2016 Nicolás Alvarez
 *
 * Licensed under the General Public License version 3,
 * or (at your option) any later version. See the LICENSE
 * file for details.
 */

#include <QApplication>

#include <QWebView>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QWebView view;
    view.load(QUrl("http://www.foss4g-ar.org/banner.html"));
    view.showFullScreen();

    return app.exec();
}
