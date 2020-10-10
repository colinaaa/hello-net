#ifndef TCP_RDT_RECEIVER_HH
#define TCP_RDT_RECEIVER_HH

#include "DataStructure.h"
#include "RdtReceiver.h"

#include <algorithm>

class TCPRdtReceiver : public RdtReceiver {
  private:
    std::unordered_map<std::size_t, const Packet> cache;
    std::size_t receiveNum;
    std::size_t baseNum;
    std::size_t endNum;

    Packet lastAckPkt;  //上次发送的确认报文

  public:
    TCPRdtReceiver() : receiveNum(0), baseNum(0), endNum(Configuration::WIN_SIZE), lastAckPkt() {
        lastAckPkt.acknum = -1;
        std::fill_n(lastAckPkt.payload, sizeof(lastAckPkt.payload), 'x');
    }
    virtual ~TCPRdtReceiver() override {}

  public:
    //接收报文，将被NetworkService调用
    void receive(const Packet &packet) final;
};  // class TCPRdtReceiver

#endif  // TCP_RDT_RECEIVER_HH
