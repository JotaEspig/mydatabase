#include <cstring>
#include <fstream>
#include <iostream>
#include <list>

#include "mydb/bla.hpp"
#include "mydb/database.hpp"
#include "mydb/db_object.hpp"

struct Student : mydb::DBObject {
    int age;

    Student();
    Student(int age);

    std::size_t size() override;
    std::string serialize(std::size_t *size) override;
    void deserialize(std::string data) override;
};

Student::Student() :
  Student{0} {
}

Student::Student(int age) :
  age(age) {
}

std::size_t Student::size() {
    return sizeof(age);
}

std::string Student::serialize(std::size_t *size) {
    *size = Student::size();
    std::string str = std::string((char *)&age, sizeof(age));
    return str;
}

void Student::deserialize(std::string data) {
    std::memcpy(&age, data.c_str(), sizeof(age));
}

int main() {
    mydb::Database<Student> db;
    db.init("test2.mydb");
    db.insert({20});

    std::ofstream file("test.mydb", std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        return 1;
    }

    // Clean file
    file.clear();

    std::list<Student> students = {};
    Student student1;
    for (int i = 0; i < 3; i++) {
        students.push_back({20 + i});
    }

    file.close();

    std::ifstream file2("test.mydb", std::ios::in | std::ios::binary);
    if (!file2.is_open()) {
        return 1;
    }

    // Read third student
    Student student;
    file2.seekg(2 * sizeof(student), std::ios::beg);
    file2.read((char *)&student, sizeof(student));
    std::cout << "age: " << student.age << std::endl;

    file2.close();
    return 0;
}
