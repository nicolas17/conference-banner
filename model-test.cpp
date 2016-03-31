/*
 * Copyright (c) 2016 Nicolás Alvarez
 *
 * Licensed under the General Public License version 3,
 * or (at your option) any later version. See the LICENSE
 * file for details.
 */

#include "talk-model.h"

#include <QCoreApplication>
#include <QNetworkAccessManager>

int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);

    QNetworkAccessManager qnam;
    ProgramFetcher fetcher(&qnam, QUrl::fromLocalFile("programme-test.json"));
    QObject::connect(&fetcher, &ProgramFetcher::finished, [&app](const ProgramData& p) {
        debugOutput(p);
        Program prog = Program::fromProgramData(p);
        qDebug() << prog.days()[1].date();
        qDebug() << prog.getDayByDate(QDate(2016, 4, 7))->title;
        app.quit();
    });
    fetcher.fetchAsync();

    return app.exec();
}
