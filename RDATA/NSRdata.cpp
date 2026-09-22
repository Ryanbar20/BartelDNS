//
// Created by ryan0 on 14-9-2026.
//

#include "NSRdata.h"


#include "DomainNameUtils.h"
#include <sstream>

namespace BartelDNS::Rdata {

    [[nodiscard]] bool NSRdata::validate() const {
        return DomainNameUtils::toWireFormat(this->NSDname).has_value();
    }
    [[nodiscard]] std::optional<std::istringstream> NSRdata::toBytes() const {
        return DomainNameUtils::toWireFormat(this->NSDname);
    };
    [[nodiscard]] std::unique_ptr<Rdata> NSRdata::fromBytes(std::istringstream& bytes) {
        auto nsRdata = std::make_unique<NSRdata>();
        if (const auto name = DomainNameUtils::readDomainName(bytes); name.has_value()) {
            nsRdata->NSDname = name.value();
        }
        return nsRdata;
    };
}
