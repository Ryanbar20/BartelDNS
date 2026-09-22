cmake_minimum_required(VERSION 4.3)
project(DnsTest)

include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
)

FetchContent_MakeAvailable(googletest)

add_executable(DnsTest
        ResourceRecordTest.cpp
        StubResolverTest.cpp
        DomainNameUtilsTest.cpp
        NameServerTest.cpp
)

target_link_libraries(DnsTest
        PRIVATE
        BartelDNS
        ws2_32
        GTest::gtest_main
)

add_custom_command(
        TARGET DnsTest
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:BartelDNS>"
        "$<TARGET_FILE_DIR:DnsTest>"
        VERBATIM
)

include(GoogleTest)
gtest_discover_tests(DnsTest PROPERTIES DISCOVERY_TIMEOUT 15)