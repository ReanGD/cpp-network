#pragma once
#include <string>
#include <memory>

struct PackageHeader {
    uint8_t* begin() {return reinterpret_cast<uint8_t*>(this);}
    uint8_t* end() {return reinterpret_cast<uint8_t*>(this) + sizeof(PackageHeader);}
    uint64_t m_bodyLenght;
};

struct PackageBody {
    PackageBody() = delete;
    PackageBody(const std::string& data)
        : m_data(data) {
    }
    PackageBody(const size_t length)
        : m_data(length, char(0)) {
    }

    std::string m_data;
};

using PackageBodyPtr = std::shared_ptr<PackageBody>;
