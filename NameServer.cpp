//
// Created by ryan0 on 17-9-2026.
//

#include "Nameserver.h"

#include <atomic>
#include <future>
#include <iostream>
#include <ostream>
#include <sstream>
#include <winsock2.h>


namespace BartelDNS {
    void NameServer::start(std::optional<std::promise<void>> started, const std::optional<std::stop_token>& stopToken) const {
        const SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock == INVALID_SOCKET) {
            std::cerr << "Error creating socket" << std::endl;
            return;
        }

        sockaddr_in serv_address = {};
        serv_address.sin_family = AF_INET;
        serv_address.sin_port = htons(this->h_port);
        serv_address.sin_addr.s_addr = htonl(this->h_ip4_addr);
        if (int serv_address_len = sizeof(serv_address);
            bind(sock, reinterpret_cast<sockaddr *>(&serv_address), serv_address_len)) {
            std::cerr << "bind failed: " << WSAGetLastError() << "\n";
            return;
        }
        std::vector<unsigned char> buffer = {};
        buffer.resize(1024);
        sockaddr_in sender{};
        int sender_len = sizeof(sender);
        if (started.has_value()) {
            started.value().set_value();
        }
        while (!stopToken.has_value() || !stopToken->stop_requested()) {
            const int n = recvfrom(sock,
                        reinterpret_cast<char *>(buffer.data()),
                        static_cast<int>(buffer.size()),
                        0,
                        reinterpret_cast<sockaddr *>(&sender),
                        &sender_len
                        );
            if (n == SOCKET_ERROR) {
                std::cerr << "recvfrom failed: " << WSAGetLastError() << "\n";
                break;
            }
            std::stringstream wire;
            wire.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
            std::istringstream stream(wire.str());

            if (auto message = DnsMessage::fromBytes(std::move(stream)); message.has_value()) {
                DnsMessage returnMessage = std::move(message.value());
                this->handle(returnMessage);
                std::optional<std::istringstream> ans_bytes = returnMessage.toBytes();
                if (!ans_bytes.has_value()) continue;

                int _ = sendto(sock,
                    ans_bytes->view().data(),
                    static_cast<int>(ans_bytes->view().size()),
                    0,
                    reinterpret_cast<sockaddr *>(&sender),
                    sender_len
                    );
            } else {
                std::cerr << "Invalid dnsMessage received\n";
            }
        }
    }
}
