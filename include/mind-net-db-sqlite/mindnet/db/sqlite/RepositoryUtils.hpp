/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once
#include "mindnet/orm/SqlUtils.hpp"



namespace mindnet::db::sqlite
{
    identification create_model(const entity_fields& fields, const model::ModelDefinition& definition, string& error);

    entity_fields read_model(model::ModelDefinition& def, identification id, string& error);

    bool update_model(identification id, model::ModelDefinition& def, entity_fields& fields, string& error);

    bool delete_model(model::ModelDefinition& def, identification id, string& error);

    std::vector<entity_fields> list_models(
        model::ModelDefinition& def,
        mindnet::orm::QueryParams& query_params,
        string& error,
        orm::SelectMode select_mode = orm::STAR
    );

}

