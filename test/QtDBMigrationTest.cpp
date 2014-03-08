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


#include <QtSql>
#include <QString>
#include <QtTest>

#include "QtDBMigration.h"

class QtDBMigrationTest : public QObject
{
    Q_OBJECT

public:
    QtDBMigrationTest();

private Q_SLOTS:
    void initTestCase();
    void init();

    void migrate();
    void migrate_data();
};

QtDBMigrationTest::QtDBMigrationTest()
{
}

void QtDBMigrationTest::initTestCase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("test.db");
    db.open();
}

void QtDBMigrationTest::init()
{
    QSqlDatabase db = QSqlDatabase::database();
    db.exec("drop table qtdbmigration_meta");
    db.exec("drop table table1");
    db.exec("drop table table2");
}

void QtDBMigrationTest::migrate()
{
    QFETCH(QString, configPath);
    QFETCH(QStringList, tableNames);
    QFETCH(int, finalVer);

    QSqlDatabase db = QSqlDatabase::database();

    QtDBMigration mig(configPath);
    bool ok = mig.migrate();
    QVERIFY(ok);

    QStringList dbTables = db.tables();
    foreach (const QString &tableName, tableNames) {
        QVERIFY(dbTables.contains(tableName));
    }

    QCOMPARE(mig.version(), finalVer);
}

void QtDBMigrationTest::migrate_data()
{
    QTest::addColumn<QString>("configPath");
    QTest::addColumn<QStringList>("tableNames");
    QTest::addColumn<int>("finalVer");

    QTest::newRow("normal")
        << "test_migrate_1.json"
        << (QStringList() << "table1" << "table2")
        << 1;
}

QTEST_APPLESS_MAIN(QtDBMigrationTest)

#include "QtDBMigrationTest.moc"
