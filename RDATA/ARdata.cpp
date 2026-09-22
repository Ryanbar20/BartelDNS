//
// Created by ryan0 on 24-8-2026.
//

#include "ARdata.h"

#include <istream>
#include <sstream>
#include <winsock2.h>


namespace BartelDNS::Rdata {
    bool ARdata::validate() const {
        return this->addr != 0;
    }

    std::optional<std::istringstream> ARdata::toBytes() const {
        if (!this->validate()) {return std::nullopt;}
        auto strStream = std::ostringstream();
        const auto haddr = htonl(this->addr);
        strStream.write(reinterpret_cast<const char*>(&haddr), sizeof(haddr));
        return std::istringstream(strStream.str());
    }
    [[nodiscard]] std::unique_ptr<Rdata> ARdata::fromBytes(std::istringstream& bytes) {
        auto arData = std::make_unique<ARdata>();
        bytes.read(reinterpret_cast<char*>(&arData->addr), sizeof(arData->addr));
        arData->addr = htonl(arData->addr);
        if (bytes.fail()) {arData->addr = 0;}
        return arData;
    }
}
