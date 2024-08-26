#pragma once

#include <cstddef>
#include <string>

namespace mydb {

struct DBObject {
    std::size_t id = 0;

    virtual std::size_t size() = 0;
    virtual std::string serialize(std::size_t *size) = 0;
    virtual void deserialize(std::string data) = 0;
};

} // namespace mydb
