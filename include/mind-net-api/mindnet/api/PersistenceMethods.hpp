//
// Created by robertvokac on 9/23/25.
//

#pragma once

#define gen_find_h(plugin, Model, model) \
std::pair<plugins :: plugin :: models::Model, string> find_##model(const mindnet::api::RequestContext& ctx, int id);

#define gen_find_cpp(plugin, Model, model, MODEL)\
std::pair< mindnet::plugins:: plugin ::models::Model, string> find_##model(const mindnet::api::RequestContext& ctx, int id)\
{\
auto result = ctx.db->read(mindnet::plugins:: plugin ::models::MODEL##_DEFINITION, ctx.token, id);\
if (result.second.ko()) return {{}, result.second.error};\
mindnet::plugins:: plugin ::models::Model entity;\
entity.from_values(result.first);\
return {entity, ""};\
}
