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
