//
// Created by ryan0 on 24-8-2026.
//

#include "Rdata.h"

#include <istream>
#include <utility>

#include "ARdata.h"
#include "NSRdata.h"


namespace BartelDNS::Rdata {

    [[nodiscard]] std::unique_ptr<Rdata> Rdata::fromBytes(const RRType type, std::istringstream& bytes) {
        Rdata* rdata = nullptr;
        switch (type) {
            case RRType::A: {
                rdata = new ARdata();
                break;
            }
            case RRType::NS: {
                rdata = new NSRdata();
                break;
            }
            default:
                return std::make_unique<ARdata>();
        }
        return rdata->fromBytes(bytes);
    }

}
