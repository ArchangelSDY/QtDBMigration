#include "AbstractConfig.h"
#include "JsonConfig.h"
#include "QtDBMigration.h"

using namespace QtDBMigrationNS;

class QtDBMigrationPrivate
{
    friend class QtDBMigration;
public:
    QtDBMigrationPrivate(QtDBMigration *q);
    ~QtDBMigrationPrivate();

    int version();
    bool migrate();

private:
    bool createMetaTable();

    QtDBMigration *m_q;
    AbstractConfig *m_config;
};

QtDBMigrationPrivate::QtDBMigrationPrivate(QtDBMigration *q) :
    m_q(q) ,
    m_config(new JsonConfig(q->m_configPath))
{
    // Load config
    if (!m_config->load()) {
        qWarning() << "Fail to load migration config";
    }
}

QtDBMigrationPrivate::~QtDBMigrationPrivate()
{
    delete m_config;
}

int QtDBMigrationPrivate::version()
{
    int curVer = QtDBMigration::EMPTY_VERSION;

    QSqlQuery q(m_q->m_db);
    q.prepare("select cur_ver from qtdbmigration_meta limit 1");
    if (!q.exec()) {
        // Table not exists, create it
        if (!createMetaTable()) {
            qWarning() << "Fail to create QtDBMigration meta table!";
        }
    } else {
        // Table exists, get current version
        if (q.first()) {
            curVer = q.value(0).toInt();
        } else {
            // Table exists but no record
            // Leave curVer = QtDBMigration::EMPTY_VERSION
        }
    }

    return curVer;
}

bool QtDBMigrationPrivate::migrate()
{
    int curVer = version();

    if (m_config->schemaVersions().count() > 0) {
        int latestVersion = m_config->schemaVersions().last()->version();

        if (curVer < latestVersion) {
            for (int i = curVer + 1; i <= latestVersion; ++i) {
                SchemaVersion *schemaVersion = m_config->schemaVersions()[i];
                bool ok = schemaVersion->apply(m_q->m_db);

                if (!ok) {
                    qWarning() << "Fail to apply schema version:" << i
                               << ", aborting...";
                    return false;
                }
            }
        }
    }

    return true;
}

bool QtDBMigrationPrivate::createMetaTable()
{
    QSqlQuery q(m_q->m_db);
    q.prepare("create table qtdbmigration_meta (cur_ver integer)");
    if (!q.exec()) {
        return false;
    }

    q.prepare("insert into qtdbmigration_meta(cur_ver) values (?)");
    q.addBindValue(QtDBMigration::EMPTY_VERSION);
    if (!q.exec()) {
        return false;
    }

    return true;
}

const char * QtDBMigration::DEFAULT_CONFIG = "migration.json";

QtDBMigration::QtDBMigration(const QString &configPath,
                             const QSqlDatabase &db) :
    m_configPath(configPath) ,
    m_db(db) ,
    m_p(new QtDBMigrationPrivate(this))
{
}

QtDBMigration::~QtDBMigration()
{
    delete m_p;
}

int QtDBMigration::version()
{
    return m_p->version();
}

bool QtDBMigration::migrate()
{
    return m_p->migrate();
}
