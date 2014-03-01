#ifndef ABSTRACTCONFIG_H
#define ABSTRACTCONFIG_H

#include <QList>
#include <QString>
#include "SchemaVersion.h"

namespace QtDBMigrationNS {

class AbstractConfig
{
public:
    AbstractConfig(const QString &path);
    virtual ~AbstractConfig();

    QList<SchemaVersion *> schemaVersions() { return m_schemaVersions; }

    virtual bool load() = 0;

protected:
    QString m_path;
    QList<SchemaVersion *> m_schemaVersions;
};

}

#endif // ABSTRACTCONFIG_H
