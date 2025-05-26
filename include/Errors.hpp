#pragma once

#include <string>
#include <vector>
#include <exception>
#include <stdexcept>

struct Error {
    int code;
    std::string message;
};

enum class ErrorCode {
    OK = 0,
    TREE_EMPTY,
    KEY_NOT_FOUND,
    INVALID_PATH,
    UNKNOWN_ORDER,
    DESERIALIZE_FAILED,
    INDEX_OUT_OF_RANGE,
    INVALID_ARGUMENT

};

inline std::vector<Error> ErrorsList = {
    {0, "Success"},
    {1, "Binary tree is empty"},
    {2, "Key not found in tree"},
    {3, "Invalid path string"},
    {4, "Unknown traversal order"},
    {5, "Failed to deserialize tree from string"},
    {6, "Index out of range"},
    {7, "Invalid argument"}
};

namespace Errors {

    class BaseError : public std::exception {
    protected:
        ErrorCode code;
        std::string msg;
    public:
        BaseError(ErrorCode code_, const std::string& custom_message = "") 
            : code(code_) 
        {
            if (custom_message.empty()) {
                msg = ErrorsList[static_cast<int>(code_)].message;
            } else {
                msg = ErrorsList[static_cast<int>(code_)].message + ": " + custom_message;
            }
        }

        const char* what() const noexcept override {
            return msg.c_str();
        }

        ErrorCode Code() const noexcept {
            return code;
        }
    };

    inline std::runtime_error TreeEmpty() {
        return std::runtime_error(ErrorsList[static_cast<int>(ErrorCode::TREE_EMPTY)].message);
    }
    
    inline std::invalid_argument KeyNotFound() {
        return std::invalid_argument(ErrorsList[static_cast<int>(ErrorCode::KEY_NOT_FOUND)].message);
    }
    
    inline std::invalid_argument InvalidPath() {
        return std::invalid_argument(ErrorsList[static_cast<int>(ErrorCode::INVALID_PATH)].message);
    }
    
    inline std::invalid_argument UnknownOrder(const std::string& order) {
        return std::invalid_argument(ErrorsList[static_cast<int>(ErrorCode::UNKNOWN_ORDER)].message + ": " + order);
    }
    
    inline std::invalid_argument DeserializeFailed() {
        return std::invalid_argument(ErrorsList[static_cast<int>(ErrorCode::DESERIALIZE_FAILED)].message);
    }
    
    inline std::out_of_range IndexOutOfRange() {
        return std::out_of_range(ErrorsList[static_cast<int>(ErrorCode::INDEX_OUT_OF_RANGE)].message);
    }

    inline std::invalid_argument InvalidArgument(const std::string& message = "") {
        if (message.empty())
            return std::invalid_argument(ErrorsList[static_cast<int>(ErrorCode::INVALID_ARGUMENT)].message);
        else
            return std::invalid_argument(ErrorsList[static_cast<int>(ErrorCode::INVALID_ARGUMENT)].message + ": " + message);
    }
}
