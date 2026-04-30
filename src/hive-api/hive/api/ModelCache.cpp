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

#include "hive/api/ModelCache.hpp"
#include "hive/util/Utils.hpp"

namespace hive::api
{
    bool ModelCache::get(
        const std::string& table,
        identification id,
        entity_fields& outRow)
    {
        if (is_disabled()) return false;
        CacheKey key{table, id};
        std::unique_lock lock(mutex_);

        auto it = readCache.find(key);
        if (it == readCache.end())
        {
            stats.misses++;
            return false;
        }

        // TTL check
        if (is_expired(it->second))
        {
            order_.erase(it->second.it);
            readCache.erase(it);
            stats.expired++;
            return false;
        }

        // copy data
        outRow = it->second.fields;

        // move to front (LRU)
        order_.splice(order_.begin(), order_, it->second.it);

        validate_invariants();
        stats.hits++;
        return true;
    }

    void ModelCache::put(
        const std::string& table,
        identification id,
        const entity_fields& row)
    {
        if (is_disabled()) return;

        CacheKey key{table, id};

        std::unique_lock lock(mutex_);

        if (std::get<int64_t>(row[0]) == 0)
        {
            essential::warn << "Id is null: " << table << " " << id << essential::commit;
            return;
        }
        auto it = readCache.find(key);

        if (it != readCache.end())
        {
            // Update + move to front
            it->second.fields = row;
            order_.splice(order_.begin(), order_, it->second.it);
            maybe_shrink_nolock();
            validate_invariants();
            return;
        }

        // capacity eviction
        if (readCache.size() >= capacity_size)
        {
            const CacheKey& oldest = order_.back();
            readCache.erase(oldest);
            stats.evicted_lru++;
            order_.pop_back();
        }

        // insert new
        order_.push_front(key);
        readCache[key] = CacheEntry{
            row,
            order_.begin(),
            unixtime(hive::util::Utils::current_unix_timestamp_ms())
        };

        maybe_shrink_nolock();
        validate_invariants();
    }

    void ModelCache::invalidate(
        const std::string& table,
        identification id)
    {
        if (is_disabled()) return;

        CacheKey key{table, id};

        std::unique_lock lock(mutex_);

        auto it = readCache.find(key);
        if (it == readCache.end())
            return;

        order_.erase(it->second.it);
        readCache.erase(it);

        validate_invariants();
    }

    void ModelCache::shrink_to(std::uint64_t targetSize, essential::ByteUnit unit)
    {
        if (is_disabled()) return;
        uint64_t targetBytes = to_bytes(targetSize, unit);

        std::unique_lock lock(mutex_);

        uint64_t currentBytes = ram_usage_cache_map_only_nolock();

        if (currentBytes <= targetBytes)
            return;

        double factor = double(targetBytes) / double(currentBytes);
        if (factor <= 0.0)
        {
            readCache.clear();
            order_.clear();
            stats.shrinks++;
            return;
        }

        size_t currentSize = readCache.size();
        size_t targetSizeItems = std::max<size_t>(1, size_t(currentSize * factor));

        if (targetSizeItems >= currentSize)
            return;

        size_t toRemove = currentSize - targetSizeItems;

        while (toRemove > 0 && !order_.empty())
        {
            const CacheKey& oldest = order_.back();
            readCache.erase(oldest);
            order_.pop_back();
            --toRemove;
        }

        while (!order_.empty())
        {
            const CacheKey& k = order_.back();
            auto it = readCache.find(k);

            if (it == readCache.end())
            {
                order_.pop_back();
                continue;
            }

            if (is_expired(it->second))
            {
                readCache.erase(it);
                order_.pop_back();
                continue;
            }

            break;
        }

        stats.shrinks++;
        validate_invariants();
    }

    void ModelCache::maybe_shrink_nolock()
    {
        if (capacity_bytes == 0)
            return; // no RAM limit

        static unixtime last_shrinked_at = 0;
        unixtime now = hive::util::Utils::current_unix_timestamp_seconds();

        if (now - last_shrinked_at < 5)
            return;

        last_shrinked_at = now;

        // print_info();  // REMOVE — dangerous inside locked paths

        size_t used = ram_usage_cache_map_only_nolock();
        if (used <= capacity_bytes)
            return;

        // shrink to configured byte limit
        size_t targetBytes = capacity_bytes;
        size_t currentBytes = used;

        double factor = double(targetBytes) / double(currentBytes);
        if (factor <= 0.0)
        {
            readCache.clear();
            order_.clear();
            stats.shrinks++;

            essential::info << "LRU auto-shrink: "
                << used << " -> " << targetBytes
                << essential::commit;

            return;
        }

        size_t currentSize = readCache.size();
        size_t targetItems = std::max<size_t>(1, size_t(currentSize * factor));

        if (targetItems >= currentSize)
            return;

        size_t toRemove = currentSize - targetItems;

        while (toRemove > 0 && !order_.empty())
        {
            const CacheKey& oldest = order_.back();
            readCache.erase(oldest);
            order_.pop_back();
            --toRemove;
        }
        while (!order_.empty())
        {
            const CacheKey& k = order_.back();
            auto it = readCache.find(k);

            if (it == readCache.end())
            {
                order_.pop_back();
                continue;
            }

            if (is_expired(it->second))
            {
                readCache.erase(it);
                order_.pop_back();
                continue;
            }

            break;
        }

        stats.shrinks++;
        essential::info << "LRU auto-shrink: "
            << used << " -> " << targetBytes
            << essential::commit;
    }

    size_t ModelCache::ram_usage_cache_map_only_nolock()
    {
        size_t total = sizeof(readCache);
        for (auto& [key, val] : readCache)
        {
            total += ram_usage_cache_key(key);
            total += ram_usage_entity_fields(val.fields);
        }
        return total;
    }
}