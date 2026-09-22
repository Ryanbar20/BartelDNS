//
// Created by ryan0 on 24-8-2026.
//

#include "QuestionResourceRecord.h"

#include <istream>
#include <sstream>
#include <winsock2.h>

#include "DomainNameUtils.h"

namespace BartelDNS {
    [[nodiscard]] std::optional<std::istringstream> QuestionResourceRecord::toBytes() const {
        if (!this->validate()) {
            return std::nullopt;
        }
        std::ostringstream result {};
        result << DomainNameUtils::toWireFormat(this->QName)->str();
        const uint16_t type = htons(static_cast<uint16_t>(this->QType));
        result.write(reinterpret_cast<const char*>(&type), sizeof(type));
        const uint16_t cls = htons(static_cast<uint16_t>(this->QClass));
        result.write(reinterpret_cast<const char*>(&cls), sizeof(cls));
        return std::istringstream(result.str());
    }
    [[nodiscard]] bool QuestionResourceRecord::validate() const {
        bool isValid = true;
        isValid &= DomainNameUtils::toWireFormat(this->QName).has_value();
        //TODO: Validate QType & QClass
        return isValid;
    }


    std::optional<QuestionResourceRecord> QuestionResourceRecord::fromBytes(
        std::istringstream& bytes
        ) {
        const std::optional<std::string> name = DomainNameUtils::readDomainName(bytes);
        QuestionResourceRecord Qrr;
        if (!name.has_value() || !bytes) { return std::nullopt; }
        Qrr.QName = name.value();
        bytes.read(reinterpret_cast<char*>(&Qrr.QType), sizeof(Qrr.QType));
        bytes.read(reinterpret_cast<char*>(&Qrr.QClass), sizeof(Qrr.QClass));

        Qrr.QType = static_cast<RRType>(htons(static_cast<uint16_t>(Qrr.QType)));
        Qrr.QClass = static_cast<RRClass>(htons(static_cast<uint16_t>(Qrr.QClass)));

        if (bytes.fail()) { return std::nullopt; }
        return Qrr;
    }
}
