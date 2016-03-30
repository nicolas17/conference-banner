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
#include <QObject>

class QNetworkAccessManager;
class QNetworkReply;

// this is the raw data as read from the JSON

struct TalkData {
    QString title, speaker, from, works;
};

struct IntervalData {
    QDateTime start;
    QDateTime end;
    QList<TalkData> talks; //in the same order as Day::rooms
};

struct DayData {
    QString title;
    QList<QString> rooms;
    QList<IntervalData> intervals;
};

struct ProgramData {
    QList<DayData> days;
};

void debugOutput(const ProgramData& program);

class ProgramFetcher: public QObject {
    Q_OBJECT
private:
    QNetworkAccessManager* const m_qnam;
    //QDateTime m_lastModified;
    QUrl m_url;
    bool m_fetching;

    static ProgramData parseJson(const QByteArray& data);

public:
    ProgramFetcher(QNetworkAccessManager* qnam, const QUrl& url);
    void fetchAsync();
    bool isFetching() const;

signals:
    void error(const QString& err);
    void finished(const ProgramData& program);
};

#endif
