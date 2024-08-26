#pragma once

#include <type_traits>

#include "mydb/db_object.hpp"

namespace mydb {

template <typename T>
struct NodeObject {
    static_assert(
        std::is_base_of<DBObject, T>::value, "T must inherit from DBObject"
    );

    T object;
    std::size_t prev_pos;
    std::size_t next_pos;
};

} // namespace mydb
