#ifndef GBN_RDT_RECEIVER_HH
#define GBN_RDT_RECEIVER_HH

#include "RdtReceiver.h"

#include <algorithm>

class GBNRdtReceiver : public RdtReceiver {
  private:
    std::size_t receiveNum;

    Packet lastAckPkt;  //上次发送的确认报文

  public:
    GBNRdtReceiver() : receiveNum(0), lastAckPkt() {
        lastAckPkt.acknum = -1;
        std::fill_n(lastAckPkt.payload, sizeof(lastAckPkt.payload), 'x');
    }
    virtual ~GBNRdtReceiver() override {}

  public:
    //接收报文，将被NetworkService调用
    void receive(const Packet &packet) final;
};  // class GBNRdtReceiver

#endif  // GBN_RDT_RECEIVER_HH
