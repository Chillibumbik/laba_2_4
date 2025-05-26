#pragma once
#include <iostream>
#include <string>

// Базовая структура User
struct User {
    std::string name;
    int age;
    int id;

    User() = default;
    User(std::string name_, int age_, int id_ = 0)
        : name(std::move(name_)), age(age_), id(id_) {}

    bool operator==(const User& other) const {
        return name == other.name && age == other.age && id == other.id;
    }

    friend std::ostream& operator<<(std::ostream& os, const User& user) {
        return os << "User(name: " << user.name << ", age: " << user.age << ", id: " << user.id << ")";
    }

    friend std::istream& operator>>(std::istream& is, User& user) {
        std::cout << "Enter name: ";
        is >> user.name;

        std::cout << "Enter age: ";
        is >> user.age;
        while (user.age < 0 || user.age > 150) {
            std::cout << "Age cannot be negative or more than 150. Try again: ";
            is >> user.age;
        }

        std::cout << "Enter id: ";
        is >> user.id;
        while (user.id < 0) {
            std::cout << "ID cannot be negative. Try again: ";
            is >> user.id;
        }

        return is;
    }
};


struct Student : public User {
    std::string group;
    double gpa;

    Student() = default;

    Student(const std::string& name, int age, int id, const std::string& group, double gpa)
        : User(name, age, id), group(group), gpa(gpa) {}

    bool operator==(const Student& other) const {
        return static_cast<const User&>(*this) == static_cast<const User&>(other) &&
               group == other.group && gpa == other.gpa;
    }

    void Print() const {
        std::cout << "Student(name: " << name << ", age: " << age << ", id: " << id
                  << ", group: " << group << ", GPA: " << gpa << ")\n";
    }

    friend std::ostream& operator<<(std::ostream& os, const Student& student) {
        return os << "Student(name: " << student.name
                  << ", age: " << student.age
                  << ", id: " << student.id
                  << ", group: " << student.group
                  << ", GPA: " << student.gpa << ")";
    }

    friend std::istream& operator>>(std::istream& is, Student& student) {
        is >> static_cast<User&>(student);

        std::cout << "Enter group: ";
        is >> student.group;

        std::cout << "Enter GPA: ";
        is >> student.gpa;
        while (student.gpa < 0.0 || student.gpa > 5.0) {
            std::cout << "GPA must be in range [0.0; 5.0]. Try again: ";
            is >> student.gpa;
        }

        return is;
    }
};

struct Teacher : public User {
    std::string subject;
    int experience; 

    Teacher() = default;

    Teacher(const std::string& name, int age, int id,
            const std::string& subject, int experience)
        : User(name, age, id), subject(subject), experience(experience) {}

    bool operator==(const Teacher& other) const {
        return static_cast<const User&>(*this) == static_cast<const User&>(other) &&
               subject == other.subject && experience == other.experience;
    }

    void Print() const {
        std::cout << "Teacher(name: " << name << ", age: " << age << ", id: " << id
                  << ", subject: " << subject << ", experience: " << experience << " years)\n";
    }

    friend std::ostream& operator<<(std::ostream& os, const Teacher& teacher) {
        return os << "Teacher(name: " << teacher.name
                  << ", age: " << teacher.age
                  << ", id: " << teacher.id
                  << ", subject: " << teacher.subject
                  << ", experience: " << teacher.experience << ")";
    }

    friend std::istream& operator>>(std::istream& is, Teacher& teacher) {
        is >> static_cast<User&>(teacher);

        std::cout << "Enter subject: ";
        is >> teacher.subject;

        std::cout << "Enter years of experience: ";
        is >> teacher.experience;
        while (teacher.experience < 0 || teacher.experience > 100) {
            std::cout << "Experience must be in range [0; 100]. Try again: ";
            is >> teacher.experience;
        }

        return is;
    }
};

