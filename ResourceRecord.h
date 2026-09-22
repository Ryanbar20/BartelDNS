//
// Created by ryan0 on 21-8-2026.
//

#ifndef DNS_RESOURCERECORD_H
#define DNS_RESOURCERECORD_H
#include <array>
#include <cstdint>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "RRClass.h"
#include "RRType.h"
#include "RDATA/ARdata.h"

namespace BartelDNS {
    class ResourceRecord {
        std::string Name{};
        RRType Type{};
        RRClass Class{};
        int32_t TTL{};
        std::unique_ptr<Rdata::Rdata> Rdata = {std::make_unique<Rdata::ARdata>()};
        uint16_t  RdLength{};
    public:
        ResourceRecord(
            std::string  name,
            const RRType type,
            const RRClass rrClass,
            const int32_t ttl,
            std::unique_ptr<Rdata::Rdata> rdata
        ) :
            Name(std::move(name)),
            Type(type),
            Class(rrClass),
            TTL(ttl),
            Rdata(std::move(rdata)) {
            if (const auto data = this->Rdata->toBytes(); data.has_value()) {
                this->RdLength = data->str().length();
            }
        }

        ResourceRecord() = default;
        ResourceRecord(ResourceRecord&&) noexcept = default;
        ResourceRecord& operator=(ResourceRecord&&) noexcept = default;

        ~ResourceRecord() = default;
        [[nodiscard]] std::optional<std::istringstream> toBytes() const;
        std::optional<ResourceRecord>  static fromBytes(std::istringstream& bytes);
        [[nodiscard]] bool validate() const;
    };

}


#endif //DNS_RESOURCERECORD_H
