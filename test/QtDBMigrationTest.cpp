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

    void migrateVersion();
    void migrateVersion_data();
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
    QCOMPARE(dbTables.count(), tableNames.count() + 1);
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

void QtDBMigrationTest::migrateVersion()
{
    QFETCH(QString, configPath);
    QFETCH(QList<int>, stepVersions);
    QFETCH(QStringList, finalTableNames);

    QSqlDatabase db = QSqlDatabase::database();

    QtDBMigration mig(configPath);
    foreach (int stepVersion, stepVersions) {
        bool ok = mig.migrate(stepVersion);
        QVERIFY(ok);
    }

    QStringList dbTables = db.tables();
    QCOMPARE(dbTables.count(), finalTableNames.count() + 1);
    foreach (const QString &tableName, finalTableNames) {
        QVERIFY(dbTables.contains(tableName));
    }
}

void QtDBMigrationTest::migrateVersion_data()
{
    QTest::addColumn<QString>("configPath");
    QTest::addColumn<QList<int> >("stepVersions");
    QTest::addColumn<QStringList>("finalTableNames");

    QTest::newRow("Migrate: init -> 0")
        << "test_migrate_1.json"
        << (QList<int>() << 0)
        << (QStringList() << "table1");
    QTest::newRow("Migrate: init -> 0 -> 1")
        << "test_migrate_1.json"
        << (QList<int>() << 0 << 1)
        << (QStringList() << "table1" << "table2");
    QTest::newRow("Migrate: init -> 0 -> 1 -> 0")
        << "test_migrate_1.json"
        << (QList<int>() << 0 << 1 << 0)
        << (QStringList() << "table1");
}

QTEST_APPLESS_MAIN(QtDBMigrationTest)

#include "QtDBMigrationTest.moc"
