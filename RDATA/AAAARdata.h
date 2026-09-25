//
// Created by ryan0 on 25-9-2026.
//

#ifndef BARTELDNS_AAAARDATA_H
#define BARTELDNS_AAAARDATA_H
#include "Rdata.h"

namespace BartelDNS::Rdata {
    class AAAARdata : public Rdata {
        __uint128_t address{};

        public:
        explicit AAAARdata(const __uint128_t value) : address(value) {};
        AAAARdata() = default;

        [[nodiscard]] bool validate() const override;
        [[nodiscard]] std::optional<std::istringstream> toBytes() const override;
    protected:
        [[nodiscard]] std::unique_ptr<Rdata> fromBytes(std::istringstream& bytes) override;



    };
}



#endif //BARTELDNS_AAAARDATA_H
