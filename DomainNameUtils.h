//
// Created by ryan0 on 27-8-2026.
//

#ifndef DNS_DOMAINNAMEUTILS_H
#define DNS_DOMAINNAMEUTILS_H
#include <cstdint>
#include <optional>
#include <string>
#include <vector>


namespace BartelDNS::DomainNameUtils {

    constexpr uint8_t CompressionLabelBitMask = 0xC0;
    constexpr uint16_t CompressionOffsetBitMask = 0x3FFF;
    constexpr uint8_t CompressionLabelSize = 2;
    constexpr uint8_t EmptyLabel = 0;
    constexpr uint8_t MaxLabelSize = 63;
    constexpr char LabelSeparator = '.';

    std::optional<std::istringstream> toWireFormat(std::string_view domainName);

    std::optional<std::string> readDomainName(std::istringstream& dnsMessage);
}


#endif //DNS_DOMAINNAMEUTILS_H
