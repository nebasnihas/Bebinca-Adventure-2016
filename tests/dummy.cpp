#include "gtest/gtest.h"
#include "glog/logging.h"
#include "gsl/gsl"

TEST(SanityTest, TestExternalLibs) {
    FLAGS_log_dir = "./";
    google::InitGoogleLogging("runAlltests");
    std::string s ("It works!");
    gsl::string_span<> ss = s;
    LOG(INFO) << s;
}