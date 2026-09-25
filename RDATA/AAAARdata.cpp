//
// Created by ryan0 on 25-9-2026.
//

#include "AAAARdata.h"

#include <sstream>
#include <winsock2.h>


namespace BartelDNS::Rdata {
    bool AAAARdata::validate() const {
        return this->address != 0;
    }

    [[nodiscard]] std::optional<std::istringstream> AAAARdata::toBytes() const {
        if (!this->validate()) {return std::nullopt;}
        std::ostringstream out{};
        const auto haddr = htonl(this->address);
        out.write(reinterpret_cast<const char*>(&haddr), sizeof(haddr));
        return std::istringstream(out.str());
    }
    [[nodiscard]] std::unique_ptr<Rdata> AAAARdata::fromBytes(std::istringstream& bytes) {
        auto aaaarData = std::make_unique<AAAARdata>();
        bytes.read(reinterpret_cast<char*>(&aaaarData->address), sizeof(aaaarData->address));
        aaaarData->address = htonl(aaaarData->address);
        if (bytes.fail()) {aaaarData->address = 0;}
        return aaaarData;
    }

}
