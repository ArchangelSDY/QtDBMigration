#ifndef JSONCONFIG_H
#define JSONCONFIG_H

#include "AbstractConfig.h"

namespace QtDBMigrationNS {

class JsonConfig : public AbstractConfig
{
public:
    JsonConfig(const QString &path);

    bool load();
};

}

#endif // JSONCONFIG_H
