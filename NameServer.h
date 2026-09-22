//
// Created by ryan0 on 17-9-2026.
//

#ifndef DNS_NAMESERVER_H
#define DNS_NAMESERVER_H
#include <functional>
#include <future>

#include "DnsMessage.h"

namespace BartelDNS {
    class NameServer {
        const uint32_t h_ip4_addr {};
        const uint16_t h_port {};
        const std::function<void(DnsMessage&)> handle {};
    public:
        explicit NameServer(const std::function<void(DnsMessage&)> &handle,
            const uint32_t h_ip4_addr, const uint16_t h_port) :
            h_ip4_addr(h_ip4_addr), h_port(h_port), handle(handle) {};
        ~NameServer() = default;

        void start(std::optional<std::promise<void>> started = std::nullopt,
            const std::optional<std::stop_token>& stopToken = std::nullopt) const;
    };



}

#endif //DNS_NAMESERVER_H