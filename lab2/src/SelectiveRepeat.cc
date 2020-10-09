#include "Global.h"
#include "RdtReceiver.h"
#include "RdtSender.h"
#include "SRRdtReceiver.hh"
#include "SRRdtSender.hh"

#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
    const auto input = fs::absolute(fs::relative("./input.txt"));
    const auto output = fs::absolute(fs::relative("./output.txt"));

    RdtSender *sender = new SRRdtSender();
    RdtReceiver *receiver = new SRRdtReceiver();
    // pns->setRunMode(0);  // VERBOS模式
    pns->setRunMode(1);  //安静模式
    pns->init();
    pns->setRtdSender(sender);
    pns->setRtdReceiver(receiver);
    pns->setInputFile(input.c_str());
    pns->setOutputFile(output.c_str());

    pns->start();

    delete sender;
    delete receiver;
    delete pUtils;  //指向唯一的工具类实例，只在main函数结束前delete
    delete pns;     //指向唯一的模拟网络环境类实例，只在main函数结束前delete

    return 0;
}
