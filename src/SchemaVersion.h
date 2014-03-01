#ifndef SCHEMAVERSION_H
#define SCHEMAVERSION_H

#include <QtSql>
#include <QString>

namespace QtDBMigrationNS {

class SchemaVersion
{
public:
    SchemaVersion(int version,
                  const QString &applySql, const QString &revertSql);

    int version() const { return m_version; }

    bool apply(const QSqlDatabase &db = QSqlDatabase::database());
    bool revert(const QSqlDatabase &db = QSqlDatabase::database());

private:
    int m_version;

    QString m_applySql;
    QString m_revertSql;
};

}

#endif // SCHEMAVERSION_H
