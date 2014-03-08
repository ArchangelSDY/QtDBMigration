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
