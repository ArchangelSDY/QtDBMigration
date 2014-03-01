#include <QList>

#include "AbstractConfig.h"

namespace QtDBMigrationNS  {

AbstractConfig::AbstractConfig(const QString &path) :
    m_path(path)
{
}

AbstractConfig::~AbstractConfig()
{
    while (!m_schemaVersions.isEmpty()) {
        delete m_schemaVersions.takeFirst();
    }
}

}
