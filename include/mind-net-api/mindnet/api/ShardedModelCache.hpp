#pragma once
#include <array>
#include <string>
#include <cstdint>

#include "mindnet/api/ModelCache.hpp"

namespace mindnet::api
{
    struct ShardedModelCache
    {
        static constexpr size_t NUM_SHARDS = 32;

        std::array<ModelCache, NUM_SHARDS> shards;

        ShardedModelCache()
        {
            // All shards get same capacity defaults
            for (auto& s : shards)
            {
                // nothing to init
            }
        }

        inline size_t shard_index(const std::string& table, int64_t id) const noexcept
        {
            CacheKey tmp{table, id};
            CacheKeyHash hasher;
            return hasher(tmp) & (NUM_SHARDS - 1);
        }


        inline ModelCache& pick(const std::string& table, int64_t id) noexcept
        {
            return shards[shard_index(table, id)];
        }

        // PUBLIC API — DROP-IN replacement
        bool get(const std::string& table, int64_t id, entity_fields& outRow)
        {
            return pick(table, id).get(table, id, outRow);
        }

        void put(const std::string& table, int64_t id, const entity_fields& row)
        {
            pick(table, id).put(table, id, row);
        }

        void invalidate(const std::string& table, int64_t id)
        {
            pick(table, id).invalidate(table, id);
        }

        void clear()
        {
            for (auto& s : shards)
                s.clear();
        }

        // Set same capacity into all shards
        void set_capacity_size(size_t cap)
        {
            size_t per = cap / NUM_SHARDS;
            if (per == 0) per = 1;

            for (auto& s : shards)
                s.set_capacity_size(per);
        }

        void set_capacity_bytes(size_t cap)
        {
            size_t per = cap / NUM_SHARDS;
            if (per == 0) per = 1;

            for (auto& s : shards)
                s.set_capacity_bytes(per);
        }

        void set_ttl_ms(size_t ms)
        {
            for (auto& s : shards)
                s.set_ttl_ms(ms);
        }

        // Information from all shards
        std::string information()
        {
            size_t ram = 0;
            size_t total = 0;

            for (auto& s : shards)
            {
                ram += s.ram_usage_cache_map_only_locked();
                total += s.size();
            }

            return "ShardedModelCache: shards=" + std::to_string(NUM_SHARDS) +
                " ram_total=" + format_bytes(ram) +
                " items=" + std::to_string(total);
        }

        void print_info()
        {
            essential::info << information() << essential::commit;
        }
    };
} // namespace mindnet::api
