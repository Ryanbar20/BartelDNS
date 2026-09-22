//
// Created by ryan0 on 27-8-2026.
//

#include "DomainNameUtils.h"

#include <cstdint>
#include <sstream>

namespace BartelDNS::DomainNameUtils {
    std::optional<std::istringstream> toWireFormat(const std::string_view domainName) {
        std::ostringstream result;
        std::istringstream ss((domainName.data()));
        std::string token;

        while (getline(ss, token, LabelSeparator)) {
            if (token.length() > MaxLabelSize || token.empty()) {
                return std::nullopt;
            }
            result << static_cast<unsigned char>(token.length());
            result << token;
        }
        result << EmptyLabel;
        return std::istringstream(result.str());
    }


    std::optional<std::string> readDomainName(std::istringstream& dnsMessage) {
        std::string result {};
        const std::streampos init_pos = dnsMessage.tellg();
        if (dnsMessage.peek() == std::char_traits<char>::eof()) {
            return {};
        }
        if (const uint8_t first_byte = dnsMessage.peek();
            (first_byte & CompressionLabelBitMask) == CompressionLabelBitMask) {

            // message compression is used. Thus go to the appropriate location of the message
            const uint16_t offset_field = dnsMessage.get() << 8 | dnsMessage.get();
            dnsMessage.seekg(offset_field & CompressionOffsetBitMask);
            if (const auto rest = readDomainName(dnsMessage); rest.has_value()) {
                result = rest.value();
            } else {
                return std::nullopt;
            }
            dnsMessage.seekg(init_pos + static_cast<std::streamoff>(CompressionLabelSize));
            return result;
        }
        if (const uint8_t label_size = dnsMessage.get(); label_size <= MaxLabelSize && dnsMessage && label_size > 0) {
            std::vector<char> label;
            label.resize(label_size);
            dnsMessage.read(label.data(), label_size);
            if (dnsMessage.fail()) {
                result.clear();
                return result;
            }
            result.insert(result.end(), label.begin(), label.end());
            result.push_back(LabelSeparator);
            if (const auto rest = readDomainName(dnsMessage); rest.has_value()) {
                result += rest.value();
            } else {
                return std::nullopt;
            }
        } else if (label_size != 0) {
            return std::nullopt;
        }
        return result;
    }
}
