//
// Created by ryan0 on 24-8-2026.
//

#include "DnsMessage.h"

#include <algorithm>
#include <istream>
#include <ranges>
#include <sstream>
#include <winsock2.h>

#include "QuestionResourceRecord.h"


namespace BartelDNS {

    void DnsMessage::setHdrFlags(const uint16_t code) {
        this->hdr_flags = code;
    }
    [[nodiscard]] std::optional<std::istringstream> DnsMessage::toBytes() const {
        if (!this->validate()) {return {};}
        std::ostringstream bytes = {};
        const uint16_t nid = htons(this->id);
        const uint16_t nhdr = htons(this->hdr_flags);
        const uint16_t Qamt = htons(FORCED_QUESTION_AMOUNT);
        const uint16_t AnsAmt = htons(this->Answers.size());
        const uint16_t AuthAmt = htons(this->Authorities.size());
        const uint16_t AddAmt = htons(this->Additionals.size());

        bytes.write(reinterpret_cast<const char*>(&nid), sizeof(nid));
        bytes.write(reinterpret_cast<const char*>(&nhdr), sizeof(nhdr));
        bytes.write(reinterpret_cast<const char*>(&Qamt), sizeof(Qamt));
        bytes.write(reinterpret_cast<const char*>(&AnsAmt), sizeof(AnsAmt));
        bytes.write(reinterpret_cast<const char*>(&AuthAmt), sizeof(AuthAmt));
        bytes.write(reinterpret_cast<const char*>(&AddAmt), sizeof(AddAmt));

        const auto QuestionBytes = this->Question.toBytes();
        if (QuestionBytes == std::nullopt) {return {};}
        bytes << QuestionBytes->str();
        for (const std::array resourceRecords{&this->Answers, &this->Authorities, &this->Additionals};
            const auto resourceRecordArray: resourceRecords) {
            for (const ResourceRecord& resourceRecord : *resourceRecordArray) {
                auto RRbytes = resourceRecord.toBytes();
                if (RRbytes == std::nullopt) {return {};}
                bytes << RRbytes->str();
            }
        }
        return std::istringstream(bytes.str());
    }
    void DnsMessage::setQuestion(const QuestionResourceRecord& question) {
        this->Question = question;
    }
    void DnsMessage::addAnswer(ResourceRecord&& answer) {
        this->Answers.push_back(std::move(answer));
    }
    void DnsMessage::addAuthority(ResourceRecord&& authority) {
        this->Authorities.push_back(std::move(authority));
    }
    void DnsMessage::addAdditional(ResourceRecord&& additional) {
        this->Additionals.push_back(std::move(additional));
    }
    [[nodiscard]] const std::vector<ResourceRecord>& DnsMessage::getAnswers() const {
        return this->Answers;
    }

    [[nodiscard]] bool DnsMessage::validate() const {
        bool isValid = true;
        isValid &= this->Question.validate();
        isValid &= std::ranges::all_of(this->Answers, [](const auto& Answer) {return Answer.validate();} );
        isValid &= std::ranges::all_of(this->Authorities, [](const auto& Authority) {return Authority.validate();} );
        isValid &= std::ranges::all_of(this->Additionals, [](const auto& Additional) {return Additional.validate();} );
        return isValid;
    }

    [[nodiscard]] std::optional<DnsMessage> DnsMessage::fromBytes(std::istringstream bytes) {
        DnsMessage dnsMessage;
        bytes.read(reinterpret_cast<char*>(&dnsMessage.id), sizeof(dnsMessage.id));
        bytes.read(reinterpret_cast<char*>(&dnsMessage.hdr_flags), sizeof(dnsMessage.hdr_flags));

        uint16_t QdCount, AnCount, NsCount, ArCount;
        bytes.read(reinterpret_cast<char*>(&QdCount), sizeof(QdCount));
        bytes.read(reinterpret_cast<char*>(&AnCount), sizeof(AnCount));
        bytes.read(reinterpret_cast<char*>(&NsCount), sizeof(NsCount));
        bytes.read(reinterpret_cast<char*>(&ArCount), sizeof(ArCount));

        dnsMessage.id = ntohs(dnsMessage.id);
        dnsMessage.hdr_flags = ntohs(dnsMessage.hdr_flags);
        QdCount = ntohs(QdCount);
        AnCount = ntohs(AnCount);
        NsCount = ntohs(NsCount);
        ArCount = ntohs(ArCount);

        if (QdCount != FORCED_QUESTION_AMOUNT) {return std::nullopt;}

        std::optional QuestionResult = QuestionResourceRecord::fromBytes(bytes);
        if (QuestionResult == std::nullopt) {return std::nullopt;}
        dnsMessage.setQuestion(QuestionResult.value());

        for (int i = 0; i < AnCount; i++) {
            std::optional RrResult = ResourceRecord::fromBytes(bytes);
            if (RrResult == std::nullopt) {return std::nullopt;}
            dnsMessage.addAnswer(std::move(RrResult.value()));
        }

        for (int i = 0; i < NsCount; i++) {
            std::optional RrResult = ResourceRecord::fromBytes(bytes);
            if (RrResult == std::nullopt) {return std::nullopt;}
            dnsMessage.addAuthority(std::move(RrResult.value()));
        }

        for (int i = 0; i < ArCount; i++) {
            std::optional RrResult = ResourceRecord::fromBytes(bytes);
            if (RrResult == std::nullopt) {return std::nullopt;}
            dnsMessage.addAdditional(std::move(RrResult.value()));
        }

        return dnsMessage;
    }
}
