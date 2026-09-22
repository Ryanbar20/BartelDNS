//
// Created by ryan0 on 21-8-2026.
//

#ifndef DNS_STUBRESOLVER_H
#define DNS_STUBRESOLVER_H
#include <cstdint>

#include "DnsMessage.h"

namespace BartelDNS {
    class StubResolver {
        uint16_t error_code = 0;
    public:
        ~StubResolver() = default;
        StubResolver() = default;
        void resolve(DnsMessage &message, uint32_t h_ns_ip_addr, uint32_t h_ns_port = 53, uint16_t timeout_s = 5);
        [[nodiscard]] uint16_t getErrorCode() const;
    };

}


#endif //DNS_STUBRESOLVER_H
