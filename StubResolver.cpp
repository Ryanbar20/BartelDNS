//
// Created by ryan0 on 21-8-2026.
//

#include "StubResolver.h"

#include <iostream>
#include <sstream>
#include <winsock2.h>


namespace BartelDNS {
    void StubResolver::resolve(DnsMessage &message, const uint32_t h_ns_ip_addr, const uint32_t h_ns_port, const uint16_t timeout_s) {

        const std::optional<std::istringstream> bytesStream = message.toBytes();
        if (!bytesStream.has_value()) return;
        const std::string bytes = bytesStream->str();
        std::vector<unsigned char> buffer = {};
        buffer.resize(1024);
        const SOCKET sock = static_cast<int>(socket(AF_INET, SOCK_DGRAM, 0));
        if (sock == INVALID_SOCKET) {
            this->error_code = 1;
            std::cerr << "socket failed: " << WSAGetLastError() << '\n';
            return;
        }

        DWORD timeoutMs = timeout_s * 1000;
        setsockopt(sock, SOL_SOCKET,
                SO_RCVTIMEO, reinterpret_cast<const char *>(&timeoutMs), sizeof(timeoutMs));

        std::cout << std::endl;
        sockaddr_in serv_address = {};
        serv_address.sin_family = AF_INET;
        serv_address.sin_port = htons(h_ns_port);
        serv_address.sin_addr.s_addr = htonl(h_ns_ip_addr);
        int serv_address_len = sizeof(serv_address);
        int _ = sendto(sock,
            bytes.data(),
            static_cast<int>(bytes.size()),
            0,
            reinterpret_cast<sockaddr *>(&serv_address),
            serv_address_len
            );
        const int n = recvfrom(sock,
            reinterpret_cast<char *>(buffer.data()),
            static_cast<int>(buffer.size()),
            0,
            reinterpret_cast<sockaddr *>(&serv_address),
            &serv_address_len
            );
        if (n < 0) {
            this->error_code = 2;
            return;
        }

        buffer.resize(n);
        std::stringstream stream;
        stream.write(reinterpret_cast<char *>(buffer.data()), buffer.size());
        std::istringstream istream(stream.str());
        std::cout << std::endl;
        std::optional<DnsMessage> msg = DnsMessage::fromBytes(std::move(istream));
        if (!msg.has_value()) {
            this->error_code = 3;
            return;
        }
        message = std::move(msg.value());
        closesocket(sock);
    }

    uint16_t StubResolver::getErrorCode() const {return this->error_code;}
}
