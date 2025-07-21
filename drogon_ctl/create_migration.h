/**
 *
 *  create_migration.h
 *  An Tao
 *
 *  Copyright 2018, An Tao.  All rights reserved.
 *  https://github.com/an-tao/drogon
 *  Use of this source code is governed by a MIT license
 *  that can be found in the License file.
 *
 *  Drogon
 *
 */

#pragma once

#include <drogon/DrObject.h>
#include <drogon/DrTemplateBase.h>
#include <unordered_map>
#include "CommandHandler.h"
using namespace drogon;

namespace drogon_ctl
{
class create_migration : public DrObject<create_migration>,
                          public CommandHandler
{
  public:
    void handleCommand(std::vector<std::string> &parameters) override;

    std::string script() override
    {
        return "create migration files";
    }

  protected:
    void createMigration(std::string migrationName, std::unordered_map<std::string,std::string> &migrationColumnsAndTypes);
};
}  // namespace drogon_ctl
