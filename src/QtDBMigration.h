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
