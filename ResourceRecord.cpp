//
// Created by ryan0 on 21-8-2026.
//

#include "ResourceRecord.h"

#include <istream>
#include <sstream>
#include <winsock2.h>

#include "DomainNameUtils.h"


namespace BartelDNS {

    [[nodiscard]] std::optional<std::istringstream> ResourceRecord::toBytes() const {
        if (!this->validate()) {
            return std::nullopt;
        }
        std::ostringstream result {};
        result << DomainNameUtils::toWireFormat(this->Name).value().str();

        const uint16_t type = htons(static_cast<uint16_t>(this->Type));
        const uint16_t cls = htons(static_cast<uint16_t>(this->Class));
        const uint32_t ttl = htonl(static_cast<uint32_t>(this->TTL));
        const uint16_t rdl = htons(this->RdLength);

        result.write(reinterpret_cast<const char*>(&type), sizeof(type));
        result.write(reinterpret_cast<const char*>(&cls), sizeof(cls));
        result.write(reinterpret_cast<const char*>(&ttl), sizeof(ttl));
        result.write(reinterpret_cast<const char*>(&rdl), sizeof(rdl));

        result << this->Rdata->toBytes().value().str();
        return std::istringstream(result.str());
    }

    [[nodiscard]] bool ResourceRecord::validate() const {
        bool isValid = true;
        isValid &= DomainNameUtils::toWireFormat(this->Name).has_value();
        isValid &= this->Rdata->validate() && this->Rdata->toBytes()->str().length() == this->RdLength;
        isValid &= this->TTL > 0;
        // TODO: check enum correctness?
        return isValid;
    }
    std::optional<ResourceRecord> ResourceRecord::fromBytes(std::istringstream& bytes) {
        const std::optional<std::string> name = DomainNameUtils::readDomainName(bytes);
        if (!name.has_value() || !bytes) {return std::nullopt; }
        ResourceRecord resourceRecord{};
        bytes.read(reinterpret_cast<char *>(&resourceRecord.Type), sizeof(resourceRecord.Type));
        bytes.read(reinterpret_cast<char *>(&resourceRecord.Class), sizeof(resourceRecord.Class));
        bytes.read(reinterpret_cast<char *>(&resourceRecord.TTL), sizeof(resourceRecord.TTL));
        bytes.read(reinterpret_cast<char *>(&resourceRecord.RdLength), sizeof(resourceRecord.RdLength));

        resourceRecord.Type     = static_cast<RRType>(ntohs(static_cast<uint16_t>(resourceRecord.Type)));
        resourceRecord.Class    = static_cast<RRClass>(ntohs(static_cast<uint16_t>(resourceRecord.Class)));
        resourceRecord.TTL      = static_cast<int32_t>(ntohl(resourceRecord.TTL));
        resourceRecord.RdLength = ntohs(resourceRecord.RdLength);

        resourceRecord.Rdata    = std::move(Rdata::Rdata::fromBytes(resourceRecord.Type, bytes));
        resourceRecord.Name     = name.value();
        if (bytes.fail()) { return std::nullopt; }
        return resourceRecord;
    }


}
