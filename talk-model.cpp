/*
 * Copyright (c) 2016 Nicolás Alvarez
 *
 * Licensed under the General Public License version 3,
 * or (at your option) any later version. See the LICENSE
 * file for details.
 */

#include "talk-model.h"

#include <QDebug>
#include <QTimeZone>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

ProgramFetcher::ProgramFetcher(QNetworkAccessManager* qnam, const QUrl& url)
    : m_qnam(qnam)
    , m_url(url)
{
    ;
}
void ProgramFetcher::fetchAsync() {
    QNetworkRequest req(m_url);
    //req.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    req.setHeader(QNetworkRequest::UserAgentHeader, "foss4g-banner/0.1");
    QNetworkReply* reply = m_qnam->get(QNetworkRequest(m_url));
    qDebug() << "Fetching JSON...";
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        qDebug() << "Got JSON";
        ProgramData p = parseJson(reply->readAll());
        emit this->finished(p);
    });
}

TalkData parseTalk(const QJsonObject& obj) {
    TalkData talk;
#define PARSE_TALK_PROPERTY(P)             \
    if (obj.contains(#P)) {                \
        Q_ASSERT(obj.value(#P).isString());\
        talk.P = obj.value(#P).toString();\
    }
    PARSE_TALK_PROPERTY(title);
    PARSE_TALK_PROPERTY(speaker);
    PARSE_TALK_PROPERTY(from);
    PARSE_TALK_PROPERTY(works);
#undef PARSE_TALK_PROPERTY
    return talk;
}

IntervalData parseInterval(const QJsonObject& agendaObj) {
    const QTimeZone TIMEZONE("America/Argentina/Buenos_Aires");

    IntervalData interval;

    Q_ASSERT(agendaObj.value("start").isString());
    QString startDateStr = agendaObj.value("start").toString();
    QDateTime startDate = QDateTime::fromString(startDateStr, "yyyy-MM-dd HH:mm:ss");
    Q_ASSERT(startDate.isValid());
    startDate.setTimeZone(TIMEZONE);
    interval.start = startDate;

    Q_ASSERT(agendaObj.value("end").isString());
    QString endDateStr = agendaObj.value("end").toString();
    QDateTime endDate = QDateTime::fromString(endDateStr, "yyyy-MM-dd HH:mm:ss");
    Q_ASSERT(endDate.isValid());
    endDate.setTimeZone(TIMEZONE);
    interval.end = endDate;

    if (agendaObj.contains("tracks")) {
        Q_ASSERT(agendaObj.value("tracks").isArray());
        Q_FOREACH (const QJsonValue& trackVal, agendaObj.value("tracks").toArray()) {
            Q_ASSERT(trackVal.isObject());
            interval.talks.append(parseTalk(trackVal.toObject()));
        }
    }
    return interval;
}

ProgramData ProgramFetcher::parseJson(const QByteArray& data) {
    QJsonDocument doc = QJsonDocument::fromJson(data);
    Q_ASSERT(doc.isArray());

    ProgramData p;
    QJsonArray arr = doc.array();
    Q_FOREACH (const QJsonValue& val, arr) {
        Q_ASSERT(val.isObject());
        QJsonObject dayObj = val.toObject();

        DayData day;

        Q_ASSERT(dayObj.value("title").isString());
        day.title = dayObj.value("title").toString();

        if (dayObj.contains("content")) {
            Q_ASSERT(dayObj.value("content").isObject());
            QJsonObject contentObj = dayObj.value("content").toObject();

            Q_ASSERT(contentObj.value("rooms").isArray());
            QJsonArray roomsArr = contentObj.value("rooms").toArray();
            Q_FOREACH (const QJsonValue& roomVal, roomsArr) {
                Q_ASSERT(roomVal.isObject());
                Q_ASSERT(roomVal.toObject().value("title").isString());
                day.rooms.append(roomVal.toObject().value("title").toString());
            }

            Q_ASSERT(contentObj.value("agenda").isArray());
            QJsonArray agendaArr = contentObj.value("agenda").toArray();
            Q_FOREACH (const QJsonValue& agendaVal, agendaArr) {
                Q_ASSERT(agendaVal.isObject());
                day.intervals.append(parseInterval(agendaVal.toObject()));
            }
        }
        p.days.append(day);
    }
    return p;
}
void debugOutput(const ProgramData& program) {
    qDebug() << "Beginning program";
    Q_FOREACH (const DayData& day, program.days) {
        qDebug() << "Day:" << day.title;
        qDebug() << " Rooms:";
        Q_FOREACH (const QString& room, day.rooms) {
            qDebug() << "  " << room;
        }
        qDebug() << " Intervals:";
        Q_FOREACH (const IntervalData& interval, day.intervals) {
            qDebug() << "  " << interval.start << "-" << interval.end;
            Q_FOREACH (const TalkData& talk, interval.talks) {
                qDebug() << "    " << talk.title;
            }
        }
        qDebug() << "End day";
    }
}

Program Program::fromProgramData(const ProgramData& data)
{
    Program program;
    Q_FOREACH (const DayData& dayData, data.days) {
        Day day;
        day.title = dayData.title;
        Q_FOREACH (const IntervalData& intervalData, dayData.intervals) {
            Interval interval;
            interval.start = intervalData.start;
            interval.end = intervalData.end;
            Q_ASSERT(intervalData.talks.isEmpty() || dayData.rooms.size() == intervalData.talks.size());
            for (int i = 0; i < intervalData.talks.size(); ++i) {
                Talk talk;
                talk.title = intervalData.talks.at(i).title;
                // TODO other fields, if I need them
                talk.roomName = dayData.rooms.at(i);
                interval.talks.append(talk);
            }
            day.intervals.append(interval);
        }
        program.m_days.append(day);
    }
    program.validate();
    return program;
}

void Program::validate() const
{
    const Interval* previousInterval = nullptr;

    for (int dayIdx = 0; dayIdx < m_days.size(); ++dayIdx) {
        const Day& day = m_days.at(dayIdx);
        QDate daysDate;
        for (int intervalIdx = 0; intervalIdx < day.intervals.size(); ++intervalIdx) {
            const Interval* interval = &day.intervals[intervalIdx];
            // check that intervals start before they end
            Q_ASSERT(interval->start < interval->end);
            if (previousInterval) {
                // check that intervals are sorted
                Q_ASSERT(previousInterval->end <= interval->start);
            }
            previousInterval = interval;
            // check that intervals start and end in the same day
            Q_ASSERT(interval->start.date() == interval->end.date());

            // check that all intervals in a "day" do in fact have the same date
            if (daysDate.isValid()) {
                Q_ASSERT(daysDate == interval->start.date());
            } else {
                daysDate = interval->start.date();
            }
        }
    }
}

QDate Day::date() const
{
    if (intervals.isEmpty()) {
        return QDate();
    } else {
        // this is known to be correct if validate() passed
        return intervals[0].start.date();
    }
}

const QList<Day>& Program::days() const
{
    return m_days;
}
const Day* Program::getDayByName(const QString& name) const
{
    Q_FOREACH(const Day& day, m_days) {
        if (day.title == name) {
            return &day;
        }
    }
    return nullptr;
}
const Day* Program::getDayByDate(const QDate& date) const
{
    Q_FOREACH(const Day& day, m_days) {
        if (day.date() == date) {
            return &day;
        }
    }
    return nullptr;
}
