//
// Created by robertvokac on 11/17/25.
//

#pragma once
#include <list>
#include <string>
#include <unordered_map>
#include <shared_mutex>

#include "mindnet/essential/ByteUnit.hpp"
#include "mindnet/essential/Global.hpp"
#include "mindnet/essential/Helper.hpp"

//todo: Use LRU cache
namespace mindnet::api
{
    struct CacheKey
    {
        std::string table;
        int64_t id;

        bool operator==(const CacheKey& other) const noexcept
        {
            return id == other.id && table == other.table;
        }
    };

    // Hash for
    struct CacheKeyHash
    {
        std::size_t operator()(const CacheKey& k) const noexcept
        {
            std::size_t h1 = std::hash<std::string>()(k.table);
            std::size_t h2 = std::hash<int64_t>()(k.id);
            return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1<<6) + (h1>>2));
        }
    };

    size_t ram_usage_cache(
const std::unordered_map<CacheKey, entity_fields, CacheKeyHash>& cache);
    inline std::string format_bytes(size_t bytes)
    {
        constexpr double KB = 1024.0;
        constexpr double MB = 1024.0 * KB;
        constexpr double GB = 1024.0 * MB;

        char buffer[64];

        if (bytes < KB) {
            std::snprintf(buffer, sizeof(buffer), "%zu B", bytes);
        }
        else if (bytes < MB) {
            std::snprintf(buffer, sizeof(buffer), "%.2f KB", bytes / KB);
        }
        else if (bytes < GB) {
            std::snprintf(buffer, sizeof(buffer), "%.2f MB", bytes / MB);
        }
        else {
            std::snprintf(buffer, sizeof(buffer), "%.2f GB", bytes / GB);
        }

        return std::string(buffer);
    }

    size_t ram_usage_cache_key(const CacheKey& key);
    size_t ram_usage_entity_fields(const entity_fields& row);

    struct CacheEntry {
        entity_fields fields;
        std::list<CacheKey>::iterator it;
        unixtime created_at;
    };

    struct Stats
    {
        size_t hits = 0;
        size_t misses = 0;
        size_t evicted_lru = 0;
        size_t expired = 0;
        size_t shrinks = 0;

        std::string to_string() const
        {
            return "Stats: hits=" + std::to_string(hits) +
                " misses=" + std::to_string(misses) +
                " evicted_lru=" + std::to_string(evicted_lru) +
                " expired=" + std::to_string(expired) +
                " shrinks=" + std::to_string(shrinks);
        }
    };

    struct ModelCache
    {
    private:
        // CACHE: (table, id) -> row
        size_t capacity_size{};
        size_t capacity_bytes{std::numeric_limits<size_t>::max()};
        size_t ttl_ms = 0;
        std::list<CacheKey> order_;
        std::unordered_map<CacheKey, CacheEntry, CacheKeyHash> readCache;
        Stats stats;

        mutable std::shared_mutex mutex_;

    public:
        explicit ModelCache(size_t capacity = 10000)
            : capacity_size(capacity)
        {
        }

        void validate_invariants() const {
#ifndef NDEBUG
            static size_t validate_counter = 0;
            if (++validate_counter % 1000 == 0)
            {
                assert(order_.size() == readCache.size());
                assert(std::all_of(order_.begin(), order_.end(),
                    [&](const CacheKey& k){
                    return readCache.find(k) != readCache.end();
                    }
                ));
            }
#endif
        }

        void set_ttl_ms(size_t ms)
        {
            std::unique_lock lock(mutex_);
            ttl_ms = ms;
        }

        inline bool is_expired(const CacheEntry& entry) const
        {
            if (ttl_ms == 0) return false;
            unixtime now = mindnet::util::Utils::current_unix_timestamp_ms();
            return (now - entry.created_at) >= ttl_ms;
        }

        bool is_enabled() const
        {
            return capacity_size > 0;
        }
        bool is_disabled() const
        {
            return capacity_size <= 0;
        }
        void set_capacity_size(size_t capacity)
        {
            capacity_size = capacity;
        }
        void set_capacity_bytes(size_t capacity)
        {
            capacity_bytes = capacity;
        }
        size_t get_capacity_size() const
        {
            return capacity_size;
        }
        size_t get_capacity_bytes() const
        {
            return capacity_bytes;
        }
        bool get(
            const std::string& table,
            int64_t id,
            entity_fields& outRow);

        void put(
            const std::string& table,
            int64_t id,
            const entity_fields& row);

        void invalidate(
            const std::string& table,
            int64_t id);
        void clear() {
        std::unique_lock lock(mutex_);
        readCache.clear();
        order_.clear();
        }

        // Just the map (the heavy part)
        size_t ram_usage_cache_map_only_locked()
        {
            std::unique_lock lock(mutex_);
            return ram_usage_cache_map_only_nolock();
        }
        size_t ram_usage_cache_map_only_nolock();

        size_t size()
        {
            std::unique_lock lock(mutex_);
            return readCache.size();
        }
        std::string information_no_lock()
        {
            size_t ram = ram_usage_cache_map_only_nolock();
            size_t count = readCache.size();

            return "ram=" + format_bytes(ram) +
                   " size=" + std::to_string(count) +
                   " capacity=" + std::to_string(capacity_size) +
                       " " + stats.to_string();
        }

        void print_info()
        {
            essential::info << "ModelCache: " << information_no_lock() << essential::commit;
        }

        void shrink_to(std::uint64_t targetSize, essential::ByteUnit unit = essential::ByteUnit::B);
        void maybe_shrink_nolock();

    };

    [[nodiscard]] inline size_t ram_usage_int64(int64_t)
    {
        return sizeof(int64_t);
    }

    [[nodiscard]] inline size_t ram_usage_string(const std::string& s)
    {
        return sizeof(std::string) + s.capacity();
    }

    [[nodiscard]] inline size_t ram_usage_entity_field(const entity_field& v)
    {
        if (std::holds_alternative<std::string>(v))
        {
            return ram_usage_string(std::get<std::string>(v));
        }
        if (std::holds_alternative<int64_t>(v))
        {
            return ram_usage_int64(std::get<int64_t>(v));
        }
        essential::warn << "Unsupported std::variant type for entity_fields." << essential::commit;
        return 0;
    }

    [[nodiscard]] inline size_t ram_usage_entity_fields(const entity_fields& row) {
        size_t total = sizeof(entity_fields) + row.capacity() * sizeof(entity_field);

        for (const auto& value : row) {
            total += ram_usage_entity_field(value);
        }
        return total;
    }
    [[nodiscard]] inline size_t ram_usage_cache_key(const CacheKey& key) {
        return sizeof(CacheKey) + key.table.capacity();
    }
    [[nodiscard]] inline size_t ram_usage_cache_entry(const CacheKey& key, const entity_fields& row) {
        return
            ram_usage_cache_key(key)
            + ram_usage_entity_fields(row);
    }
    [[nodiscard]] inline size_t ram_usage_cache(
    const std::unordered_map<CacheKey, entity_fields, CacheKeyHash>& cache)
    {
        size_t total = sizeof(cache);

        for (const auto& [key, row] : cache) {
            total += ram_usage_cache_entry(key, row);
        }

        total += cache.bucket_count() * sizeof(void*);

        return total;
    }

}
