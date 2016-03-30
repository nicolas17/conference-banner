/*
 * Copyright (c) 2016 Nicolás Alvarez
 *
 * Licensed under the General Public License version 3,
 * or (at your option) any later version. See the LICENSE
 * file for details.
 */

#ifndef BANNER_TALK_MODEL_H
#define BANNER_TALK_MODEL_H

#include <QList>
#include <QDateTime>
#include <QUrl>

class QNetworkAccessManager;
class QNetworkReply;

struct Talk {
    QString title, speaker, from, works;
};

struct Interval {
    QDateTime start;
    QDateTime end;
    QList<Talk> talks; //in the same order as Day::rooms
};

struct Day {
    QString title;
    QList<QString> rooms;
    QList<Interval> intervals;
};

struct Program {
    QList<Day> days;
};

void debugOutput(const Program& program);

class ProgramFetcher: public QObject {
    Q_OBJECT
private:
    QNetworkAccessManager* const m_qnam;
    //QDateTime m_lastModified;
    QUrl m_url;
    bool m_fetching;

    static Program parseJson(const QByteArray& data);

public:
    ProgramFetcher(QNetworkAccessManager* qnam, const QUrl& url);
    void fetchAsync();
    bool isFetching() const;

signals:
    void error(const QString& err);
    void finished(const Program& program);
};

#endif
