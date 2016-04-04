/*
 * Copyright (c) 2016 Nicolás Alvarez
 *
 * Licensed under the General Public License version 3,
 * or (at your option) any later version. See the LICENSE
 * file for details.
 */

#include <QApplication>

#include <QAction>
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
    webView->setDisabled(true);
    webView->load(QUrl("http://www.foss4g-ar.org/banner.html"));

    QAction* quitAction = new QAction(this);
    quitAction->setShortcut(QKeySequence(Qt::Key_Escape));
    connect(quitAction, &QAction::triggered, app, &QApplication::quit);
    this->addAction(quitAction);

    QAction* refreshAction = new QAction(this);
    refreshAction->setShortcuts({QKeySequence(Qt::CTRL | Qt::Key_R), QKeySequence(Qt::Key_F5)});
    connect(refreshAction, &QAction::triggered, webView, [this] {
        webView->triggerPageAction(QWebPage::Reload);
    });
    this->addAction(refreshAction);
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
