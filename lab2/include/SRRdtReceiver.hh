#ifndef SR_RDT_RECEIVER_H
#define SR_RDT_RECEIVER_H

#include "RdtReceiver.h"

#include <algorithm>
#include <unordered_map>

class SRRdtReceiver : public RdtReceiver {
  private:
    std::size_t baseNum;
    std::size_t endNum;

    Packet lastAckPkt;  // 上次发送的确认报文

    std::unordered_map<std::size_t, const Packet> cache{};

  public:
    SRRdtReceiver() : baseNum(0), endNum(Configuration::WIN_SIZE), lastAckPkt() {
        lastAckPkt.acknum = 0;
        std::fill_n(lastAckPkt.payload, sizeof(lastAckPkt.payload), 'x');
    }
    virtual ~SRRdtReceiver() override {}

  public:
    void receive(const Packet &packet) final;  //接收报文，将被NetworkService调用
};

#endif  // SR_RDT_RECEIVER_H
