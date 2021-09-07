/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libutils/hash.h>
#include <libutils/vector.h>

namespace Utils
{

template <typename TKey, typename TValue>
struct HashMap
{
private:
        struct Item
    {
        uint32_t hash;
        TKey key;
        TValue value;
    };

    static constexpr int BUCKET_COUNT = 256;

    Vector<Vector<Item>> _buckets{};

    Vector<Item> &bucket(uint32_t hash)
    {
        return _buckets[hash % BUCKET_COUNT];
    }

    Item *item_by_key(const TKey &key)
    {
        return item_by_key(key, hash<TKey>(key));
    }

        Item *item_by_key(const TKey &key, uint32_t hash)
    {
        Item *result = nullptr;
        auto &b = bucket(hash);

        b.foreach([&](Item &item) {
            if (item.hash == hash && item.key == key)
            {
                result = &item;
                return Iteration::STOP;
            }
            else
            {
                return Iteration::CONTINUE;
            }
        });

        return result;
    }

public:
    size_t count() const
    {
        size_t result = 0;

        for (size_t i = 0; i < BUCKET_COUNT; i++)
        {
            result += _buckets[i].count();
        }

        return result;
    }

    HashMap()
    {
        for (size_t i = 0; i < BUCKET_COUNT; i++)
        {
            _buckets.push_back({});
        }
    }

    HashMap(const HashMap &other)
        : _buckets(other._buckets)
    {
    }

    HashMap(HashMap &&other)
        : _buckets(std::move(other._buckets))
    {
    }

    void clear()
    {
        _buckets.clear();
    }

    void remove_key(TKey &key)
    {
        uint32_t h = hash<TKey>(key);

        bucket(h).remove_all_match([&](auto &item) {
            return item.hash == h && item.key == key;
        });
    }

};

}