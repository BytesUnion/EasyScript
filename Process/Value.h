#pragma once
#include <string>
#include <memory>

class Value {
private:
    enum class Type { Null, Double, String };
    Type type;
    union {
        double doubleValue;
        std::string* stringValue;
    };

public:
    Value() : type(Type::Null), doubleValue(0.0) {}
    Value(double d) : type(Type::Double), doubleValue(d) {}
    Value(const std::string& s) : type(Type::String), stringValue(new std::string(s)) {}
    Value(const Value& other) : type(other.type) {
        if (type == Type::String) {
            stringValue = new std::string(*other.stringValue);
        }
        else {
            doubleValue = other.doubleValue;
        }
    }
    Value& operator=(const Value& other) {
        if (this != &other) {
            if (type == Type::String) {
                delete stringValue;
            }
            type = other.type;
            if (type == Type::String) {
                stringValue = new std::string(*other.stringValue);
            }
            else {
                doubleValue = other.doubleValue;
            }
        }
        return *this;
    }
    ~Value() {
        if (type == Type::String) {
            delete stringValue;
        }
    }

    bool IsNull() const { return type == Type::Null; }
    bool IsDouble() const { return type == Type::Double; }
    bool IsString() const { return type == Type::String; }

    double AsDouble() const { return doubleValue; }
    const std::string& AsString() const { return *stringValue; }

    std::string ToString() const {
        switch (type) {
        case Type::Null: return "null";
        case Type::Double: return std::to_string(doubleValue);
        case Type::String: return *stringValue;
        default: return "unknown";
        }
    }
};