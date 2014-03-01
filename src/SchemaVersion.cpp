#include "SchemaVersion.h"

namespace QtDBMigrationNS {

SchemaVersion::SchemaVersion(int version,
                             const QString &applySql,
                             const QString &revertSql) :
    m_version(version) ,
    m_applySql(applySql) ,
    m_revertSql(revertSql)
{
}

bool SchemaVersion::apply(const QSqlDatabase &db)
{
    QSqlQuery q(db);
    q.prepare(m_applySql);
    if (q.exec()) {
        QSqlQuery upgradeVer(db);
        upgradeVer.prepare("update qtdbmigration_meta set cur_ver = cur_ver + 1");
        return upgradeVer.exec();
    } else {
        return false;
    }
}

bool SchemaVersion::revert(const QSqlDatabase &db)
{
    QSqlQuery q(db);
    q.prepare(m_revertSql);
    if (q.exec()) {
        QSqlQuery downgradeVer(db);
        downgradeVer.prepare("update qtdbmigration_meta set cur_ver = cur_ver - 1");
        return downgradeVer.exec();
    } else {
        return false;
    }
}

}
