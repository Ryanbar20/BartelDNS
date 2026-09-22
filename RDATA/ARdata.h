//
// Created by ryan0 on 24-8-2026.
//

#ifndef DNS_ARDATA_H
#define DNS_ARDATA_H
#include <cstdint>
#include <optional>

#include "Rdata.h"

namespace BartelDNS::Rdata {
    class ARdata : public Rdata {
        uint32_t addr = 0;
    public:
        explicit ARdata(const uint32_t value) : addr(value) {};
        ARdata() = default;
        ~ARdata() override = default;
        [[nodiscard]] bool validate() const override;
        [[nodiscard]] std::optional<std::istringstream> toBytes() const override;
    protected:
        [[nodiscard]] std::unique_ptr<Rdata> fromBytes(std::istringstream& bytes) override;
    };
}



#endif //DNS_ARDATA_H
