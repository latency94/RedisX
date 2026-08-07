#include "database.hpp"
#include <mutex>

void Database::set(const std::string& key,
                   const std::string& value)
{
    std::unique_lock lock(mutex_);
    store_[key] = value;
}

bool Database::get(const std::string& key,
                   std::string& value)
{
    std::shared_lock lock(mutex_);

    auto it = store_.find(key);

    if(it == store_.end())
        return false;

    value = it->second;

    return true;
}

bool Database::del(const std::string& key)
{
    std::unique_lock lock(mutex_);

    return store_.erase(key);
}

bool Database::exists(const std::string& key)
{
    std::shared_lock lock(mutex_);

    return store_.count(key);
}