#pragma once
#include <vector>
#include <memory>

struct PackageHeader {
    uint64_t m_bodyLenght;
};

struct PackageBody {
    PackageBody() = delete;
    PackageBody(size_t length)
        : m_data(length) {
    }

    std::vector<char> m_data;
};

using PackageBodyPtr = std::shared_ptr<PackageBody>;
