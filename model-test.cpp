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
    ProgramFetcher fetcher(&qnam, QUrl::fromLocalFile("www.foss4g-ar.org/programme.json"));
    QObject::connect(&fetcher, &ProgramFetcher::finished, [&app](const Program& p) {
        debugOutput(p);
        app.quit();
    });
    fetcher.fetchAsync();

    return app.exec();
}
