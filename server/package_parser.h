#pragma once

#include <memory>
#include <vector>

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

class PackageParser {
    enum class STATE {
        HEADER,
        BODY
    };

public:
    PackageParser() = default;
public:
    std::shared_ptr<PackageBody> parse(const uint8_t* buffer, const size_t lenght);
private:
    size_t m_offset = 0;
    STATE m_state = STATE::HEADER;
    PackageHeader m_header;
    size_t m_headerFilled = 0;
    std::shared_ptr<PackageBody> m_body;
    size_t m_bodyFilled = 0;
};
