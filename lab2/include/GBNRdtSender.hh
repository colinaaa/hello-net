#ifndef GBN_RDT_SENDER_HH
#define GBN_RDT_SENDER_HH

#include "RdtSender.h"

#include <unordered_map>

class GBNRdtSender : public RdtSender {
  private:
    std::size_t baseNum;
    std::size_t endNum;
    std::size_t seqNum;
    std::unordered_map<std::size_t, Packet> cache;

  public:
    bool getWaitingState() final;

    //! \brief 发送应用层下来的Message，由NetworkServiceSimulator调用
    //! \note 如果发送方成功地将Message发送到网络层，返回true;
    //! \note 如果因为发送方处于等待正确确认状态而拒绝发送Message，则返回false
    bool send(const Message &message) final;

    //! \brief 接受确认Ack，将被 `NetworkServiceSimulator` 调用
    void receive(const Packet &ackPkt) final;

    //! \brief Timeout handler，将被 `NetworkServiceSimulator` 调用
    void timeoutHandler(int seqNum) final;

  public:
    GBNRdtSender() : baseNum(0), endNum(Configuration::WIN_SIZE), seqNum(0), cache() {}
    virtual ~GBNRdtSender() override {}
};  // class GBNRdtSender

#endif  // GBN_RDT_SENDER_HH
