#pragma once

#include <string>
#include <unordered_map>
#include <shared_mutex>

class Database {
public:
    void set(const std::string& key,
             const std::string& value);

    bool get(const std::string& key,
             std::string& value);

    bool del(const std::string& key);

    bool exists(const std::string& key);

private:
    std::unordered_map<std::string, std::string> store_;

    mutable std::shared_mutex mutex_;
};