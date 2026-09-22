//
// Created by ryan0 on 24-8-2026.
//

#ifndef DNS_DNSMESSAGE_H
#define DNS_DNSMESSAGE_H
#include <cstdint>
#include <vector>

#include "ResourceRecord.h"
#include "QuestionResourceRecord.h"

namespace BartelDNS {
    class DnsMessage {
        uint16_t id {};
        uint16_t hdr_flags {};
        QuestionResourceRecord Question {};
        std::vector<ResourceRecord> Answers {};
        std::vector<ResourceRecord> Authorities {};
        std::vector<ResourceRecord> Additionals {};

    public:
        static constexpr uint16_t FORCED_QUESTION_AMOUNT = 1;

        DnsMessage() = default;
        ~DnsMessage() = default;
        DnsMessage(DnsMessage&&) noexcept = default;
        DnsMessage& operator=(DnsMessage&&) noexcept = default;
        DnsMessage(const DnsMessage&) = delete;
        DnsMessage& operator=(const DnsMessage&) = delete;


        DnsMessage(
            const uint16_t id,
            const uint16_t hdr_flags) :
            id(id), hdr_flags(hdr_flags) {};
        void setHdrFlags(uint16_t code);
        [[nodiscard]] std::optional<std::istringstream> toBytes() const;
        [[nodiscard]] std::optional<DnsMessage> static fromBytes(std::istringstream bytes);
        void setQuestion(const QuestionResourceRecord& question);
        void addAnswer(ResourceRecord&& answer);
        void addAuthority(ResourceRecord&& authority);
        void addAdditional(ResourceRecord&& additional);
        [[nodiscard]] const std::vector<ResourceRecord>& getAnswers() const;
        [[nodiscard]] bool validate() const;
    };
}



#endif //DNS_DNSMESSAGE_H
