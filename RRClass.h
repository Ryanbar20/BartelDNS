//
// Created by ryan0 on 21-8-2026.
//

#ifndef DNS_RRCLASS_H
#define DNS_RRCLASS_H
#include <array>
#include <cstdint>



namespace BartelDNS {
    enum class RRClass : uint16_t {
        CLASS_IN = 1,
        CLASS_CS = 2,
        CLASS_CH = 3,
        CLASS_HS = 4
    };
}



#endif //DNS_RRCLASS_H
