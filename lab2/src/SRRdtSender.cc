#include "SRRdtSender.hh"

#include "DataStructure.h"
#include "Global.h"

#include <algorithm>
#include <spdlog/spdlog.h>

bool SRRdtSender::send(const Message &message) {
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

void SRRdtSender::receive(const Packet &packet) {
    pUtils->printPacket("收到ACK", packet);
    const auto checksum = pUtils->calculateCheckSum(packet);
    if (checksum != packet.checksum) {
        pUtils->printPacket("校验和不正确", packet);
        return;
    }

    const auto ack = packet.acknum;

    for (int i = baseNum; i <= ack; i++) {
        pns->stopTimer(SENDER, i);
    }

    spdlog::info("发送方滑动窗口移动: from [{}, {}) to [{}, {})", baseNum, endNum, ack + 1, endNum + ack - baseNum + 1);

    endNum += ack - baseNum + 1;
    baseNum = ack + 1;
}

void SRRdtSender::timeoutHandler(int seqNum) {
    if (seqNum < baseNum) {
        return;
    }
    pns->stopTimer(SENDER, seqNum);

    const auto &packet = cache.at(seqNum);

    pUtils->printPacket("发送方定时器时间到，重发上次发送的报文", packet);
    pns->startTimer(SENDER, Configuration::TIME_OUT, packet.seqnum);
    pns->sendToNetworkLayer(RECEIVER, packet);
}

bool SRRdtSender::getWaitingState() { return seqNum == endNum; }
