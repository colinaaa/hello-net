// StopWait.cpp : 定义控制台应用程序的入口点。
//

#include "Global.h"
#include "RdtReceiver.h"
#include "RdtSender.h"
#include "StopWaitRdtReceiver.h"
#include "StopWaitRdtSender.h"

#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
    const auto input = fs::relative("../input.txt");
    const auto output = fs::relative("../output.txt");

    RdtSender *ps = new StopWaitRdtSender();
    RdtReceiver *pr = new StopWaitRdtReceiver();
    pns->setRunMode(0);  // VERBOS模式
    //	pns->setRunMode(1);  //安静模式
    pns->init();
    pns->setRtdSender(ps);
    pns->setRtdReceiver(pr);
    pns->setInputFile(input.c_str());
    pns->setOutputFile(output.c_str());

    pns->start();

    delete ps;
    delete pr;
    delete pUtils;  //指向唯一的工具类实例，只在main函数结束前delete
    delete pns;     //指向唯一的模拟网络环境类实例，只在main函数结束前delete

    return 0;
}
