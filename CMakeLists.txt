cmake_minimum_required(VERSION 4.3)
project(BartelDNS)

set(CMAKE_CXX_STANDARD 20)

add_library(BartelDNS SHARED
        ResourceRecord.cpp
        ResourceRecord.h
        RRType.h
        RRClass.h
        RDATA/Rdata.cpp
        RDATA/Rdata.h
        RDATA/ARdata.cpp
        RDATA/ARdata.h
        DnsMessage.cpp
        DnsMessage.h
        MessageHeaderFlags.h
        QuestionResourceRecord.cpp
        QuestionResourceRecord.h
        StubResolver.cpp
        StubResolver.h
        DomainNameUtils.cpp
        DomainNameUtils.h
        RDATA/NSRData.cpp
        RDATA/NSRdata.h
        Nameserver.cpp
        Nameserver.h
        RDATA/AAAARdata.cpp
        RDATA/AAAARdata.h
)
target_link_libraries(BartelDNS PRIVATE ws2_32)
target_include_directories(BartelDNS PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(Test)