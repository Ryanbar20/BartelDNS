//
// Created by ryan0 on 24-8-2026.
//

#ifndef DNS_MESSAGEHEADERFLAGS_H
#define DNS_MESSAGEHEADERFLAGS_H
#include <cstdint>

namespace BartelDNS::MessageHeader {
    constexpr uint16_t QR = 0x8000;
    constexpr uint16_t OPCODE_STANDARD = 0x0000;
    constexpr uint16_t OPCODE_INVERSE = 0x0800;
    constexpr uint16_t OPCODE_STATUS = 0x1000;
    constexpr uint16_t AA = 0x0400;
    constexpr uint16_t TC = 0x0200;
    constexpr uint16_t RD = 0x0100;
    constexpr uint16_t RA = 0x0080;
    constexpr uint16_t Z = 0x0000;
    constexpr uint16_t RCODE_NOERR = 0x0000;
    constexpr uint16_t RCODE_FORMAT_ERR = 0x0001;
    constexpr uint16_t RCODE_SERVER_ERR = 0x0002;
    constexpr uint16_t RCODE_NAME_ERR = 0x0003;
    constexpr uint16_t RCODE_NIMPL = 0x0004;
    constexpr uint16_t RCODE_REFUSED = 0x0005;
}

#endif //DNS_MESSAGEHEADERFLAGS_H
