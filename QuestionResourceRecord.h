//
// Created by ryan0 on 24-8-2026.
//

#ifndef DNS_QUESTIONRESOURCERECORD_H
#define DNS_QUESTIONRESOURCERECORD_H
#include <optional>
#include <string>
#include <vector>
#include "RRType.h"
#include "RRClass.h"


namespace BartelDNS {
    class QuestionResourceRecord {
        std::string QName{};
        RRType QType{};
        RRClass QClass{};
    public:
        QuestionResourceRecord(
                std::string  name,
                const RRType Qtype,
                const RRClass QrrClass
            ) :
                QName(std::move(name)),
                QType(Qtype),
                QClass(QrrClass)
        {}

        QuestionResourceRecord() = default;

        ~QuestionResourceRecord() = default;
        [[nodiscard]] std::optional<std::istringstream> toBytes() const;
        std::optional<QuestionResourceRecord> static fromBytes(std::istringstream& bytes);
        [[nodiscard]] bool validate() const;
    };
}



#endif //DNS_QUESTIONRESOURCERECORD_H
