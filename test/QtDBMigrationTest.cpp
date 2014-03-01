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
