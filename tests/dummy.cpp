#include "gtest/gtest.h"
#include "glog/logging.h"

TEST(SanityTest, test) {
    FLAGS_log_dir = "./";
    google::InitGoogleLogging("runAlltests");
    LOG(INFO) << "It works!";
}