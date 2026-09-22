//
// Created by ryan0 on 27-8-2026.
//


#include "DomainNameUtils.h"
#include "gtest/gtest.h"

TEST(domainNameUtilsTest, testToWire1) {
    const std::string domain = "www.test.com";
    const std::string expected("\03www\04test\03com\0", 14);
    EXPECT_EQ(expected, BartelDNS::DomainNameUtils::toWireFormat(domain)->str());
}
TEST(domainNameUtilsTest, testToWire2) {
    const std::string domain = "www.test.abc.def.com";
    const std::string expected("\03www\04test\03abc\03def\03com\00", 22);
    EXPECT_EQ(expected, BartelDNS::DomainNameUtils::toWireFormat(domain)->str());
}
TEST(domainNameUtilsTest, testToWire3) {
    const std::string domain = "www.test.com.";
    const std::string expected("\03www\04test\03com\0", 14);
    EXPECT_EQ(expected, BartelDNS::DomainNameUtils::toWireFormat(domain)->str());
}

TEST(domainNameUtilsTest, testToWireInvalid1) {
    const std::string domain = "www..com";
    EXPECT_EQ(BartelDNS::DomainNameUtils::toWireFormat(domain), std::nullopt);
}
TEST(domainNameUtilsTest, testToWireInvalid2) {
    const std::string domain = "www.1234567890123456789012345678901234567890123456789012345678901234";
    EXPECT_EQ(BartelDNS::DomainNameUtils::toWireFormat(domain), std::nullopt);
}
TEST(domainNameUtilsTest, testToWireInvalid3) {
    const std::string domain = ".www.test.com.";
    EXPECT_EQ(BartelDNS::DomainNameUtils::toWireFormat(domain), std::nullopt);
}

TEST(domainNameUtilsTest, testFromWire1) {
    std::stringstream  wire;
    wire.write("\03www\04test\03com", 13);
    wire.put('\0');
    std::istringstream stream(wire.str());
    const std::string domain = BartelDNS::DomainNameUtils::readDomainName(stream).value();
    EXPECT_EQ(domain ,"www.test.com.");
}
TEST(domainNameUtilsTest, testFromWire2) {
    std::stringstream  wire;
    wire.write("\03www\04test\05abcde\03com", 19);
    wire.put('\0');
    std::istringstream stream(wire.str());
    const std::string domain = BartelDNS::DomainNameUtils::readDomainName(stream).value();
    EXPECT_EQ(domain ,"www.test.abcde.com.");
}


TEST(domainNameUtilsTest, testFromWireInv1) {
    std::stringstream  wire;
    wire.write("\03www\255test\05abcde\03com", 19);
    wire.put('\0');
    std::istringstream stream(wire.str());
    EXPECT_FALSE(BartelDNS::DomainNameUtils::readDomainName(stream).has_value());
}

TEST(domainNameUtilsTest, testFromWireInv2) {
    std::stringstream  wire;
    wire.write("\02www\04test\05abcde\03com", 18);
    wire.put('\0');
    std::istringstream stream(wire.str());
    EXPECT_FALSE(BartelDNS::DomainNameUtils::readDomainName(stream).has_value());
}

TEST(domainNameUtilsTest, testFromWireInv3) {
    std::stringstream  wire;
    wire.write("\02www\x08test\05abcde\03com", 18);
    wire.put('\0');
    std::istringstream stream(wire.str());
    EXPECT_FALSE(BartelDNS::DomainNameUtils::readDomainName(stream).has_value());
}