#include "GBNRdtSender.hh"

#include "DataStructure.h"
#include "Global.h"

#include <algorithm>
#include <iostream>

bool GBNRdtSender::send(const Message &message) {
    if (not getWaitingState()) {
        Packet packet{};
        packet.seqnum = seqNum++;
        packet.acknum = -1;
        std::copy_n(message.data, sizeof(message.data), packet.payload);

        packet.checksum = pUtils->calculateCheckSum(packet);

        cache.emplace(packet.seqnum, packet);

        pns->startTimer(SENDER, Configuration::TIME_OUT, packet.seqnum);
        pns->sendToNetworkLayer(RECEIVER, packet);
        pUtils->printPacket("发送报文", packet);
        return true;
    }

    return false;
}

void GBNRdtSender::receive(const Packet &packet) {
    pUtils->printPacket("收到ACK", packet);
    const auto checksum = pUtils->calculateCheckSum(packet);
    if (checksum != packet.checksum) {
        pUtils->printPacket("校验和不正确", packet);
        return;
    }

    const auto ack = packet.acknum;
    pns->stopTimer(SENDER, ack);
    if (ack < baseNum) {
        for (int i = baseNum; cache.count(i) != 0; i++) {
            pns->stopTimer(SENDER, i);

            const auto &packet = cache.at(i);
            pns->sendToNetworkLayer(RECEIVER, packet);
            pns->startTimer(SENDER, Configuration::TIME_OUT, i);
        }
        return;
    }

    for (int i = baseNum; i < ack; i++) {
        pns->stopTimer(SENDER, i);
    }

    std::cout << "滑动窗口移动: from [" << baseNum << ", " << endNum << ")";

    endNum += ack - baseNum + 1;
    baseNum = ack + 1;
    std::cout << " to：[" << baseNum << ", " << endNum << ")\n";
}

void GBNRdtSender::timeoutHandler(int seqNum) {
    if (seqNum < baseNum) {
        return;
    }
    pns->stopTimer(SENDER, seqNum);

    const auto &packet = cache.at(seqNum);

    pUtils->printPacket("发送方定时器时间到，重发上次发送的报文", packet);
    pns->startTimer(SENDER, Configuration::TIME_OUT, packet.seqnum);
    pns->sendToNetworkLayer(RECEIVER, packet);
}

bool GBNRdtSender::getWaitingState() { return seqNum == endNum; }
