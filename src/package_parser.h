#pragma once

#include "package.h"


class PackageParser {
    enum class STATE {
        HEADER,
        BODY
    };

public:
    PackageParser() = default;
public:
    PackageBodyPtr parse(const uint8_t* buffer, const size_t lenght);
    std::string serialize(PackageBodyPtr package);
private:
    size_t m_offset = 0;
    STATE m_state = STATE::HEADER;
    PackageHeader m_header;
    size_t m_headerFilled = 0;
    std::shared_ptr<PackageBody> m_body;
    size_t m_bodyFilled = 0;
};
