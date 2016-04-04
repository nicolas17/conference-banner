/*
 * Copyright (c) 2016 Nicolás Alvarez
 *
 * Licensed under the General Public License version 3,
 * or (at your option) any later version. See the LICENSE
 * file for details.
 */

#include <QApplication>

#include <QDebug>
#include <QProcess>

#include <QAction>
#include <QWidget>
#include <QWebView>
#include <QWebFrame>
#include <QWebElement>

class WebBanner: public QWidget {
private:
    QWebView* webView;
    int m_padding = 10;
    int m_paddingRuleIndex = -1;
public:
    WebBanner(QApplication* app);
    void resizeEvent(QResizeEvent* event) override;

    void insertPaddingControlRule();
    void setPadding(int padding);
};

WebBanner::WebBanner(QApplication* app) {
    webView = new QWebView(this);
    webView->setDisabled(true);
    webView->load(QUrl("http://www.foss4g-ar.org/banner.html"));

    QAction* quitAction = new QAction(this);
    quitAction->setShortcut(QKeySequence(Qt::Key_Escape));
    connect(quitAction, &QAction::triggered, app, [app] {
        qDebug() << "Esc pressed, quitting banner";
        app->quit();
    });
    this->addAction(quitAction);

    QAction* refreshAction = new QAction(this);
    refreshAction->setShortcuts({QKeySequence(Qt::CTRL | Qt::Key_R), QKeySequence(Qt::Key_F5)});
    connect(refreshAction, &QAction::triggered, webView, [this] {
        qDebug() << "Refreshing page, requested by keyboard";
        webView->triggerPageAction(QWebPage::Reload);
        m_paddingRuleIndex = -1;
    });
    this->addAction(refreshAction);

    QAction* increasePaddingAction = new QAction(this);
    increasePaddingAction->setShortcut(QKeySequence(Qt::Key_Up));
    connect(increasePaddingAction, &QAction::triggered, this, [this] {
        setPadding(m_padding+1);
    });
    this->addAction(increasePaddingAction);

    QAction* decreasePaddingAction = new QAction(this);
    decreasePaddingAction->setShortcut(QKeySequence(Qt::Key_Down));
    connect(decreasePaddingAction, &QAction::triggered, this, [this] {
        setPadding(m_padding-1);
    });
    this->addAction(decreasePaddingAction);

    QAction* shutdownAction = new QAction(this);
    shutdownAction->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_Delete));
    connect(shutdownAction, &QAction::triggered, this, [this] {
        qDebug() << "Shutting down...";
        qDebug() << "poweroff status code:" << QProcess::execute("sudo poweroff");
    });
    this->addAction(shutdownAction);

    connect(webView, &QWebView::loadFinished, this, [this] {
        qDebug() << "Page finished loading";
        insertPaddingControlRule();
    });
}
void WebBanner::resizeEvent(QResizeEvent* event) {
    webView->resize(this->size());
}
void WebBanner::insertPaddingControlRule() {
    if (m_paddingRuleIndex < 0) {
        qDebug() << "Adding padding-control CSS";
        const QString jsCode = QStringLiteral("document.styleSheets[1].insertRule('section { padding: %1px; }', document.styleSheets[1].cssRules.length);").arg(m_padding);
        qDebug() << "Running " << jsCode;
        QVariant varRuleIndex = webView->page()->mainFrame()->documentElement().evaluateJavaScript(jsCode);
        qDebug() << "Result:" << varRuleIndex;
        m_paddingRuleIndex = varRuleIndex.toInt();
    }

}
void WebBanner::setPadding(int padding) {
    if (m_paddingRuleIndex < 0) return;
    m_padding = padding;
    insertPaddingControlRule();
    const QString jsCode = QStringLiteral("document.styleSheets[1].cssRules[%1].style.padding = '%2px';").arg(m_paddingRuleIndex).arg(m_padding);
    qDebug() << "Running " << jsCode;
    QVariant ret = webView->page()->mainFrame()->documentElement().evaluateJavaScript(jsCode);
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    WebBanner banner(&app);
    banner.showFullScreen();

    return app.exec();
}
