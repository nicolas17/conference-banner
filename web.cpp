/*
 * Copyright (c) 2016 Nicolás Alvarez
 *
 * Licensed under the General Public License version 3,
 * or (at your option) any later version. See the LICENSE
 * file for details.
 */

#include <QApplication>

#include <QWidget>
#include <QWebView>

class WebBanner: public QWidget {
private:
    QWebView* webView;
public:
    WebBanner(QApplication* app);
    void resizeEvent(QResizeEvent* event) override;
};

WebBanner::WebBanner(QApplication* app) {
    webView = new QWebView(this);
    webView->load(QUrl("http://www.foss4g-ar.org/banner.html"));
}
void WebBanner::resizeEvent(QResizeEvent* event) {
    webView->resize(this->size());
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    WebBanner banner(&app);
    banner.showFullScreen();

    return app.exec();
}
