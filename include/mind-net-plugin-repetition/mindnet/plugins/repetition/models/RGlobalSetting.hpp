#ifndef RGLOBAL_SETTING_H
#define RGLOBAL_SETTING_H

#include <string>
#include "mindnet/model/BaseModel.hpp"

// ***** MACROS : START *****
#define Model RGlobalSetting
#define MODEL R_GLOBAL_SETTING
#define COLS columns::RGlobalSettingColumns
#include "../columns/RGlobalSettingColumns.hpp"
// ***** MACROS : END *****

namespace mindnet::plugins::repetition::models
{
    using mindnet::model::def;
    using mindnet::model::coldef;
    using_flags();

    inline const def R_GLOBAL_SETTING_DEFINITION =
        def(COLS::MODEL_NAME)
        .set_all_rest_operations()
        .set_group("Repetition", 200)
        .set_title_column(COLS::KEY)
        .set_columns({
            coldef(COLS::KEY, MANDATORY | UNIQUE),
            coldef(COLS::VALUE, MANDATORY),
        });

    struct Model : mindnet::model::BaseModel
    {
        string key;
        string value;

        create_model_h_methods(Model, MODEL)

        bool operator==(const Model& other) const
        {
            return id == other.id &&
                created_at == other.created_at &&
                updated_at == other.updated_at &&
                key == other.key &&
                value == other.value;
        }
    };
}
#undef Model
#undef MODEL
#undef COLS
#endif // RGLOBAL_SETTING_H
