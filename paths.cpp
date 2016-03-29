/*
 * Copyright (c) 2016 Nicolás Alvarez
 *
 * Licensed under the General Public License version 3,
 * or (at your option) any later version. See the LICENSE
 * file for details.
 */

#include "paths.h"

#include <QDir>
#include <QCoreApplication>

#ifdef CMAKE_INSTALL_DATADIR
# define HARDCODED_DATA_PATH DATADIR
#else
# define HARDCODED_DATA_PATH "/usr/share/foss4g-banner"
#endif

QString dataPath(const QString& filename) {
#if defined(Q_OS_WIN)
    return QDir(QCoreApplication::applicationDirPath()).filePath(filename);
#elif defined(Q_OS_UNIX)
    QString candidate = QDir(HARDCODED_DATA_PATH).filePath(filename);
    if (QFile::exists(candidate)) { return candidate; }
    candidate = QDir().filePath(filename);
    if (QFile::exists(candidate)) { return candidate; }
    return QString();
#else
#warning "unknown data path for this platform; using QSP"
    return QStandardPaths::locate(QStandardPaths::AppDataLocation, filename);
#endif
}
