//
// Created by ryan0 on 18-9-2026.
//


#include <thread>

#include "MessageHeaderFlags.h"
#include "Nameserver.h"
#include "StubResolver.h"
#include "gtest/gtest.h"
#include <winsock2.h>

static void handle(BartelDNS::DnsMessage& msg) {
    msg.setHdrFlags(10);
}
static std::promise<void> promise{};

TEST(NameServerTest, handleTest) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    // start the NameServer
    BartelDNS::NameServer ns{handle, INADDR_ANY, 2000};
    const auto future = promise.get_future();
    const std::stop_source stopSource{};
    std::jthread ns_thread(&BartelDNS::NameServer::start, &ns, std::move(promise), stopSource.get_token());
    // create a BartelDNS Message
    auto* message = new BartelDNS::DnsMessage(1, 10);
    const auto* question = new BartelDNS::QuestionResourceRecord("google.nl.", BartelDNS::RRType::NS, BartelDNS::RRClass::CLASS_IN);
    message->setQuestion(*question);
    // create an array with the expected result byts
    const std::string bytes = message->toBytes()->str();
    // resolve the Question in the Message using the NameServer
    BartelDNS::StubResolver resolver{};
    future.wait();
    message->setHdrFlags(0);
    resolver.resolve(*message, (127 << 24) + 1, 2000);
    // Perform assertions.
    if (bytes == message->toBytes()->str()) {
        GTEST_SUCCEED();
    }
    auto _ = stopSource.request_stop();
    WSACleanup();
}
