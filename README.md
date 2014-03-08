QtDBMigration
=============

Simple Database Migration for Qt.


Example
=======

Config file:

```json
{
    "schemaVersions": [
        {
            "version": 0,
            "applySql": "create table table1(id integer)",
            "revertSql": "drop table table1"
        },
        {
            "version": 1,
            "applySql": "create table table2(id integer)",
            "revertSql": "drop table table2"
        }
    ]
}
```

Apply migration:

```cpp
QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
db.setDatabaseName("test.db");
if (db.open()) {
    QtDBMigration mig("./config.json");
    mig.migrate();
}
```


License
=======
MIT
