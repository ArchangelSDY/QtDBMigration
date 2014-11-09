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
    bool migrate(int version);

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
    if (m_config->schemaVersions().isEmpty()) {
        qWarning() << "Empty schema versions";
        return false;
    }

    int latestVersion = m_config->schemaVersions().last()->version();
    return migrate(latestVersion);
}

bool QtDBMigrationPrivate::migrate(int destVer)
{
    int curVer = version();
    if (curVer != QtDBMigration::EMPTY_VERSION &&
            (curVer < 0 || curVer >= m_config->schemaVersions().count())) {
        qWarning() << "Invalid current schema version:" << curVer;
        return false;
    }

    if (curVer < destVer) {
        for (int i = curVer + 1; i <= destVer; ++i) {
            SchemaVersion *schemaVersion = m_config->schemaVersions()[i];
            bool ok = schemaVersion->apply(m_q->m_db);

            if (!ok) {
                qWarning() << "Fail to apply schema version:" << i
                           << ", aborting...";
                return false;
            }
        }
    } else if (curVer > destVer) {
        for (int i = curVer; i > destVer; --i) {
            SchemaVersion *schemaVersion = m_config->schemaVersions()[i];
            bool ok = schemaVersion->revert(m_q->m_db);

            if (!ok) {
                qWarning() << "Fail to revert schema version:" << i
                           << ", aborting...";
                return false;
            }
        }
    } else {
        // curVers == destVer
        // No need to migrate
        return true;
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

bool QtDBMigration::migrate(int version)
{
    return m_p->migrate(version);
}
