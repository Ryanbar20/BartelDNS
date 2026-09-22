//
// Created by ryan0 on 21-8-2026.
//
#include "../ResourceRecord.h"
#include "gtest/gtest.h"

TEST(ResourceRecordTest, TestToBytes) {
    const auto arData = new BartelDNS::Rdata::ARdata(0xAABBCCDD);
    const auto rr = BartelDNS::ResourceRecord(
        "abc",
        BartelDNS::RRType::A,
        BartelDNS::RRClass::CLASS_IN,
        10,
        std::make_unique<BartelDNS::Rdata::ARdata>(*arData)
        );
    const auto bytes = rr.toBytes()->str();
    EXPECT_EQ(bytes.size(), 19);
    EXPECT_EQ(static_cast<unsigned char>(bytes.at(bytes.size() -4)), 0xAA);
    EXPECT_EQ(static_cast<unsigned char>(bytes.at(bytes.size() -3)), 0xBB);
    EXPECT_EQ(static_cast<unsigned char>(bytes.at(bytes.size() -2)), 0xCC);
    EXPECT_EQ(static_cast<unsigned char>(bytes.at(bytes.size() -1)), 0xDD);
}


TEST(ResourceRecordTest, TestFromBytes) {
    const auto arData = new BartelDNS::Rdata::ARdata(0xAABBCCDD);
    const auto rr = BartelDNS::ResourceRecord(
        "www.test.com.",
        BartelDNS::RRType::A,
        BartelDNS::RRClass::CLASS_IN,
        10,
        std::make_unique<BartelDNS::Rdata::ARdata>(*arData)
        );
    std::stringstream wire;
    std::string bytes = rr.toBytes()->str();
    wire.write(bytes.data(), bytes.size());
    std::istringstream stream(wire.str());
    const auto result = BartelDNS::ResourceRecord::fromBytes(stream);
    if (result == std::nullopt) {FAIL();}
    const BartelDNS::ResourceRecord& Rrr = result.value();
    EXPECT_EQ(bytes, Rrr.toBytes()->str());
}
