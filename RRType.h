//
// Created by ryan0 on 21-8-2026.
//

#ifndef DNS_RRTYPE_H
#define DNS_RRTYPE_H
#include <array>
#include <cstdint>

namespace BartelDNS {
    enum class RRType : uint16_t {
        A     = 1,
        NS    = 2,
        MD    = 3,
        MF    = 4,
        CNAME = 5,
        SOA   = 6,
        MB    = 7,
        MG    = 8,
        MR    = 9,
        Null  = 10,
        WKS   = 11,
        PTR   = 12,
        HINFO = 13,
        MINFO = 14,
        MX    = 15,
        TXT   = 16
    };
}
#endif //DNS_RRTYPE_H
