#include "serverthread.h"
#include <csignal>

ServerThread::ServerThread()
    : io_context_(), signals_(io_context_), server_(io_context_, 12345) {
    signals_.add(SIGINT);
    signals_.async_wait([this](const boost::system::error_code& /*error*/, int /*signal_number*/) {
        io_context_.stop();
    });
}

ServerThread::~ServerThread() {
    quit();
    wait();
}

void ServerThread::run() {
    io_context_.run();
}
