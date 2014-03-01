#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>

#include "JsonConfig.h"

namespace QtDBMigrationNS {

JsonConfig::JsonConfig(const QString &path) :
    AbstractConfig(path)
{
}

bool JsonConfig::load()
{
    QFile file(m_path);
    if (!file.open(QFile::ReadOnly)) {
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonValue schemaVersionsRaw = doc.object().value("schemaVersions");
    if (!schemaVersionsRaw.isArray()) {
        qWarning() << "Invalid schemaVersions";
        return false;
    }

    QJsonArray schemaVersions = schemaVersionsRaw.toArray();
    QJsonArray::iterator it = schemaVersions.begin();
    for (; it != schemaVersions.end(); ++it) {
        QJsonValue val = *it;

        if (!val.isObject()) {
            continue;
        }

        QJsonObject schemaVersion = val.toObject();
        QJsonValue version = schemaVersion.value("version");
        if (!version.isDouble()) {
            continue;
        }
        QJsonValue applySql = schemaVersion.value("applySql");
        if (!applySql.isString()) {
            continue;
        }
        QJsonValue revertSql = schemaVersion.value("revertSql");
        if (!revertSql.isString()) {
            continue;
        }

        m_schemaVersions << new SchemaVersion(
            version.toInt(), applySql.toString(), revertSql.toString());
    }

    return true;
}

}
