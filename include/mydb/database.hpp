#pragma once

#include <fstream>
#include <iostream>
#include <string>

#include "mydb/base_object.hpp"
#include "mydb/db_object.hpp"

namespace mydb {

template <typename T>
class Database {
    static_assert(
        std::is_base_of<DBObject, T>::value, "T must inherit from DBObject"
    );

public:
    Database();

    void init(std::string filename);
    void load(std::string filename);
    void insert(T &object);
    bool loaded() const;

private:
    bool _loaded = false;
    std::size_t _next_id = 0;
    std::string _filename;
    std::size_t _content_root_pos;
    std::size_t _bst_root_pos;
    std::size_t _content_root_pos_pos = 8;
    std::size_t _header_size
        = _content_root_pos_pos + sizeof(std::size_t) + sizeof(std::size_t) + 2;

    std::string generate_header() const;
};

template <typename T>
Database<T>::Database() {
}

template <typename T>
void Database<T>::init(std::string filename) {
    _filename = filename;
    std::ofstream file(_filename, std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    std::string header = generate_header();
    file.write(header.c_str(), header.size());
    if (!file.good()) {
        throw std::runtime_error("Could not write header");
    }

    file.close();
    _loaded = true;
}

template <typename T>
void Database<T>::load(std::string filename) {
    _filename = filename;
    std::ifstream file(_filename, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    file.close();
    _loaded = true;
}

template <typename T>
void Database<T>::insert(T &object) {
    std::fstream file(
        _filename, std::ios::in | std::ios::out | std::ios::binary
    );
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    object.id = ++_next_id;
    NodeObject<T> node_object;

    file.seekg(_content_root_pos_pos, std::ios::beg);
    std::size_t content_root_pos;
    file.read((char *)&content_root_pos, sizeof(content_root_pos));
    // If there is no content root, create it
    if (content_root_pos == 0) {
        file.seekp(_content_root_pos_pos, std::ios::beg);
        file.write((char *)&_header_size, sizeof(std::size_t));
        node_object.object = object;
        node_object.prev_pos = 0;
        node_object.next_pos = 0;

        file.seekp(_header_size, std::ios::beg);
        file.write((char *)&node_object, sizeof(node_object));
    }
    else {
        throw std::runtime_error("Not implemented");
    }

    file.close();
}

template <typename T>
bool Database<T>::loaded() const {
    return _loaded;
}

template <typename T>
std::string Database<T>::generate_header() const {
    // binary bytes: 0xffff, 0xdbdb
    std::string header = "MYDB";
    header += std::string(2, (char)0xdb);
    header += std::string(2, (char)0xff);
    header += std::string(sizeof(std::size_t), (char)_content_root_pos);
    header += std::string(sizeof(std::size_t), (char)_bst_root_pos);
    header += std::string(2, (char)0xff);
    return header;
}

} // namespace mydb
