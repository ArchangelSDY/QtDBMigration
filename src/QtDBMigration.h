// Copyright (c) 2014 Archangel.SDY@gmail.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.


#ifndef QTDBMIGRATION_H
#define QTDBMIGRATION_H

#include <QtSql>
#include <QString>

class QtDBMigrationPrivate;

class QtDBMigration
{
    friend class QtDBMigrationPrivate;
public:
    static const char *DEFAULT_CONFIG;

    QtDBMigration(const QString &configPath = QtDBMigration::DEFAULT_CONFIG,
                  const QSqlDatabase &db = QSqlDatabase::database());
    ~QtDBMigration();

    bool migrate();
    int version();

private:
    static const int EMPTY_VERSION = -1;

    QString m_configPath;
    QSqlDatabase m_db;
    QtDBMigrationPrivate *m_p;
};

#endif // QTDBMIGRATION_H
