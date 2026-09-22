//
// Created by ryan0 on 24-8-2026.
//

#include "winsock2.h"

#include "DnsMessage.h"
#include "MessageHeaderFlags.h"
#include "QuestionResourceRecord.h"
#include "StubResolver.h"
#include "gtest/gtest.h"


TEST(StubResolverTest, resolveTest) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    auto* message = new BartelDNS::DnsMessage(1, 0);
    {
        using namespace BartelDNS::MessageHeader;
        message->setHdrFlags(RD);
    }
    const auto* question = new BartelDNS::QuestionResourceRecord("google.nl.", BartelDNS::RRType::NS, BartelDNS::RRClass::CLASS_IN);
    message->setQuestion(*question);
    auto* resolver = new BartelDNS::StubResolver();
    resolver->resolve(*message,  16843009);
    EXPECT_GT(message->getAnswers().size(), 0);
    WSACleanup();
}