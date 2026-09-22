//
// Created by ryan0 on 14-9-2026.
//

#ifndef DNS_NSRDATA_H
#define DNS_NSRDATA_H
#include "Rdata.h"

#include <utility>
#include <optional>

namespace BartelDNS::Rdata {
    class NSRdata : public Rdata {
        std::string NSDname;

    public:
        explicit NSRdata(std::string name) : NSDname(std::move(name)) {};
        NSRdata() = default;
        ~NSRdata() override = default;
        [[nodiscard]] bool validate() const override;
        [[nodiscard]] std::optional<std::istringstream> toBytes() const override;
    protected:
        [[nodiscard]] std::unique_ptr<Rdata> fromBytes(std::istringstream& bytes) override;
    };
}

#endif //DNS_NSRDATA_H
