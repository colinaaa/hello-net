#include "TCPRdtSender.hh"

#include "DataStructure.h"
#include "Global.h"

#include <algorithm>
#include <spdlog/spdlog.h>

bool TCPRdtSender::send(const Message &message) {
    if (not getWaitingState()) {
        Packet packet{};
        packet.seqnum = seqNum++;
        packet.acknum = -1;
        std::copy_n(message.data, sizeof(message.data), packet.payload);

        packet.checksum = pUtils->calculateCheckSum(packet);

        cache.emplace(static_cast<std::size_t>(packet.seqnum), packet);

        if (not hasTimer) {
            pns->startTimer(SENDER, Configuration::TIME_OUT, baseNum);
            hasTimer = true;
        }

        pns->sendToNetworkLayer(RECEIVER, packet);
        pUtils->printPacket("发送报文", packet);
        return true;
    }

    return false;
}

void TCPRdtSender::receive(const Packet &packet) {
    pUtils->printPacket("收到ACK", packet);
    const auto checksum = pUtils->calculateCheckSum(packet);
    if (checksum != packet.checksum) {
        pUtils->printPacket("校验和不正确", packet);
        return;
    }

    const auto ack = packet.acknum;
    ackCache[ack]++;

    if (ack <= baseNum) {
        if (ackCache.at(ack) > 3 and cache.count(baseNum) != 0) {
            pUtils->printPacket("三个ACK", packet);

            // if (cache.count(baseNum) == 0) {
            //     return;
            // }
            const auto &pkt = cache.at(baseNum);
            pns->stopTimer(SENDER, baseNum);
            pns->startTimer(SENDER, Configuration::TIME_OUT, baseNum);
            hasTimer = true;

            pns->sendToNetworkLayer(RECEIVER, pkt);
        }
        return;
    }

    pns->stopTimer(SENDER, baseNum);
    hasTimer = false;

    spdlog::info("发送方滑动窗口移动: from [{}, {}) to [{}, {})", baseNum, endNum, ack, endNum + ack - baseNum);

    endNum += ack - baseNum;
    baseNum = ack;

    if (seqNum > baseNum) {
        pns->startTimer(SENDER, Configuration::TIME_OUT, baseNum);
        hasTimer = true;
    }
}

void TCPRdtSender::timeoutHandler(int seqNum) {
    if (seqNum < baseNum) {
        return;
    }
    pns->stopTimer(SENDER, seqNum);

    const auto &packet = cache.at(seqNum);

    pUtils->printPacket("发送方定时器时间到，重发上次发送的报文", packet);
    pns->startTimer(SENDER, Configuration::TIME_OUT, packet.seqnum);
    pns->sendToNetworkLayer(RECEIVER, packet);
}

bool TCPRdtSender::getWaitingState() { return seqNum == endNum; }
