#ifndef GBN_RDT_RECEIVER_H
#define GBN_RDT_RECEIVER_H

#include "RdtReceiver.h"

#include <algorithm>
#include <cstddef>

class GBNRdtReceiver : public RdtReceiver {
  private:
    size_t receiveNum;

    Packet lastAckPkt;  //上次发送的确认报文

  public:
    GBNRdtReceiver() : receiveNum(0), lastAckPkt() {
        lastAckPkt.acknum = -1;
        std::fill_n(lastAckPkt.payload, sizeof(lastAckPkt.payload), 0);
    }
    virtual ~GBNRdtReceiver() override {}

  public:
    void receive(const Packet &packet) final;  //接收报文，将被NetworkService调用
};

#endif  // GBN_RDT_RECEIVER_H
