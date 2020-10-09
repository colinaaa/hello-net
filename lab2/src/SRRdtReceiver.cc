#include "SRRdtReceiver.hh"

#include "Global.h"

#include <algorithm>
#include <iostream>
#include <spdlog/spdlog.h>

void SRRdtReceiver::receive(const Packet &packet) {
    const auto seq = packet.seqnum;
    const auto checksum = pUtils->calculateCheckSum(packet);

    if (checksum != packet.checksum) {
        pUtils->printPacket("校验和错误", packet);
        pns->sendToNetworkLayer(SENDER, lastAckPkt);
        return;
    }

    cache.emplace(seq, packet);

    if (seq != baseNum) {
        // std::cout << seq << ' ' << baseNum << std::endl;
        pUtils->printPacket("非最小报文", packet);
        pns->sendToNetworkLayer(SENDER, lastAckPkt);
        return;
    }

    Message msg;

    for (; cache.count(baseNum) != 0; baseNum++, endNum++) {
        const auto &p = cache.at(baseNum);
        std::copy_n(p.payload, sizeof(p.payload), msg.data);

        pns->delivertoAppLayer(RECEIVER, msg);
        cache.erase(baseNum);

        lastAckPkt.acknum = baseNum;
        lastAckPkt.seqnum = -1;
        lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
        pUtils->printPacket("发送ACK", lastAckPkt);
        pns->sendToNetworkLayer(SENDER, lastAckPkt);

        spdlog::info("接收方滑动窗口移动: from [{}, {}) to [{}, {})", baseNum, endNum, baseNum + 1, endNum + 1);
    }

    return;
}