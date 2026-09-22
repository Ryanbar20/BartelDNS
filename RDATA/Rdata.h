//
// Created by ryan0 on 24-8-2026.
//

#ifndef DNS_RDATA_H
#define DNS_RDATA_H
#include <memory>
#include <optional>

#include "RRType.h"

namespace BartelDNS::Rdata {
    class Rdata {
    public:
        Rdata() = default;
        virtual ~Rdata() = default;
        [[nodiscard]] virtual bool validate() const = 0;
        [[nodiscard]] virtual std::optional<std::istringstream> toBytes() const = 0;

        [[nodiscard]] static std::unique_ptr<Rdata> fromBytes(RRType type, std::istringstream& bytes);
    protected:
        [[nodiscard]] virtual std::unique_ptr<Rdata> fromBytes(std::istringstream& bytes) = 0;
    };
}



#endif //DNS_RDATA_H
