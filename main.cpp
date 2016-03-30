/*
 * Copyright (c) 2016 Nicolás Alvarez
 *
 * Licensed under the General Public License version 3,
 * or (at your option) any later version. See the LICENSE
 * file for details.
 */

#include "paths.h"
#include "talk-model.h"

#include <QGuiApplication>
#include <QCommandLineParser>

#include <QNetworkAccessManager>

#include <QQmlContext>
#include <QQuickView>
#include <QQuickItem>

class TalkModel: public QObject {
    Q_OBJECT
    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(QString room READ room)

    TalkData m_talk;
    QString m_room;

public:
    TalkModel(const TalkData& talk, const QString& room)
        : m_talk(talk)
        , m_room(room)
    {
    }
    const QString& title() const {
        return m_talk.title;
    }
    const QString& room() const {
        return m_room;
    }
};

// TODO this should probably go to another file.
// Note that 'program' will be freed after this function returns,
// so we can't retain references to any of it.
void updateModel(QQmlContext* ctxt, const ProgramData& program) {
    QList<QObject*> model;
    const int dayNum = 3;
    const int intervalNum = 2;
    Q_ASSERT(program.days[dayNum].rooms.size() == program.days[dayNum].intervals[intervalNum].talks.size());
    for (int i=0; i<program.days[dayNum].rooms.size(); i++) {
        TalkModel* m = new TalkModel(program.days[dayNum].intervals[intervalNum].talks[i], program.days[dayNum].rooms[i]);
        model.append(m);
    }
    ctxt->setContextProperty("currentTalksModel", QVariant::fromValue(model));
}

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

    const QString mainPath = dataPath("main.qml");
    if (mainPath.isEmpty() || !QFile::exists(mainPath)) {
        qFatal("Can't find data files");
    }
    QQuickView view;
    view.setSource(QUrl::fromLocalFile(mainPath));
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

    QNetworkAccessManager qnam;
    ProgramFetcher fetcher(&qnam, QUrl::fromLocalFile("programme-test.json"));
    //ProgramFetcher fetcher(&qnam, QUrl("http://www.foss4g-ar.org/programme.json"));
    QObject::connect(&fetcher, &ProgramFetcher::finished, [&view](const ProgramData& p) {
        updateModel(view.rootContext(), p);
    });
    fetcher.fetchAsync();

    if (parser.isSet("fullscreen")) {
        view.showFullScreen();
    } else {
        view.show();
    }

    return app.exec();
}
#include "main.moc"
