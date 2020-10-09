#include "GBNRdtReceiver.hh"

#include "Global.h"

#include <algorithm>
#include <iostream>

void GBNRdtReceiver::receive(const Packet &packet) {
    const auto seq = packet.seqnum;
    const auto checksum = pUtils->calculateCheckSum(packet);

    if (checksum != packet.checksum) {
        pUtils->printPacket("校验和错误", packet);
        pns->sendToNetworkLayer(SENDER, lastAckPkt);
        return;
    }

    if (seq != receiveNum) {
        pUtils->printPacket("序列号错误", packet);
        pns->sendToNetworkLayer(SENDER, lastAckPkt);
        return;
    }

    Message msg;
    std::copy_n(packet.payload, sizeof(packet.payload), msg.data);
    pns->delivertoAppLayer(RECEIVER, msg);

    lastAckPkt.acknum = receiveNum++;
    lastAckPkt.seqnum = -1;
    lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);

    pUtils->printPacket("发送ACK", lastAckPkt);
    pns->sendToNetworkLayer(SENDER, lastAckPkt);

    return;
}