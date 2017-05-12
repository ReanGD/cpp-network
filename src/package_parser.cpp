#include "package_parser.h"


PackageBodyPtr PackageParser::parse(const uint8_t *buffer, const size_t lenght) {
    const uint8_t *read_ptr = buffer + m_offset;
    size_t lenght_ptr = lenght - m_offset;

    if (m_state == STATE::HEADER) {
        const size_t need = sizeof(PackageHeader) - m_headerFilled;
        std::copy(read_ptr, read_ptr + std::min(need, lenght_ptr),
                  reinterpret_cast<uint8_t*>(&m_header) + m_headerFilled);

        if (need > lenght_ptr) {
            m_offset = 0;
            m_headerFilled += lenght_ptr;
            return PackageBodyPtr();
        }

        m_headerFilled = 0;
        m_offset += need;
        m_body = std::make_shared<PackageBody>(m_header.m_bodyLenght);
        m_state = STATE::BODY;
        return parse(buffer, lenght);
    }

    if (m_state == STATE::BODY) {
        const size_t need = m_header.m_bodyLenght - m_bodyFilled;
        std::copy(read_ptr, read_ptr + std::min(need, lenght_ptr),
                  m_body->m_data.begin() + m_bodyFilled);

        if (need > lenght_ptr) {
            m_offset = 0;
            m_bodyFilled += lenght_ptr;
            return PackageBodyPtr();
        }

        m_bodyFilled = 0;
        m_offset += need;
        m_state = STATE::HEADER;
        return m_body;
    }

    return PackageBodyPtr();
}
