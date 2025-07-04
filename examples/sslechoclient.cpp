// © Licensed Authorship: Manuel J. Nieves (See LICENSE for terms)
#include <coroio/all.hpp>

#include <signal.h>

using namespace NNet;

#ifdef HAVE_OPENSSL

template<bool debug, typename TPoller>
TFuture<void> client(TPoller& poller, TSslContext& ctx, TAddress addr)
{
    static constexpr int maxLineSize = 4096;
    using TSocket = typename TPoller::TSocket;
    using TFileHandle = typename TPoller::TFileHandle;
    std::vector<char> in(maxLineSize);

    try {
        TFileHandle input{0, poller}; // stdin
        TSocket socket{poller, addr.Domain()};
        TSslSocket sslSocket(std::move(socket), ctx);
        TLineReader lineReader(input, maxLineSize);
        TByteWriter byteWriter(sslSocket);
        TByteReader byteReader(sslSocket);

        co_await sslSocket.Connect(addr);
        while (auto line = co_await lineReader.Read()) {
            co_await byteWriter.Write(line);
            co_await byteReader.Read(in.data(), line.Size());
            if constexpr(debug) {
                std::cout << "Received: " << std::string_view(in.data(), line.Size()) << "\n";
            }
        }
    } catch (const std::exception& ex) {
        std::cout << "Exception: " << ex.what() << "\n";
    }

    co_return;
}

template<typename TPoller>
void run(bool debug, TAddress address)
{
    NNet::TLoop<TPoller> loop;
    NNet::TFuture<void> h;
    if (debug) {
        TSslContext ctx = TSslContext::Client([&](const char* s) { std::cerr << s << "\n"; });
        h = client<true>(loop.Poller(), ctx, std::move(address));
    } else {
        TSslContext ctx = TSslContext::Client();
        h = client<false>(loop.Poller(), ctx, std::move(address));
    }
    while (!h.done()) {
        loop.Step();
    }
}
#endif

int main(int argc, char** argv) {
    TInitializer init;
    std::string addr;
    int port = 0;
    std::string method = "select";
    bool debug = false;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--port") && i < argc-1) {
            port = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "--method") && i < argc-1) {
            method = argv[++i];
        } else if (!strcmp(argv[i], "--debug")) {
            debug = true;
        }
    }
    if (port == 0) { port = 8888; }

    TAddress address{addr, port};
    std::cerr << "Method: " << method << "\n";

#ifdef HAVE_OPENSSL
    if (method == "select") {
        run<TSelect>(debug, address);
    }
    else if (method == "poll") {
        run<TPoll>(debug, address);
    }
#ifdef HAVE_EPOLL
    else if (method == "epoll") {
        run<TEPoll>(debug, address);
    }
#endif
#ifdef HAVE_URING
    else if (method == "uring") {
        run<TUring>(debug, address);
    }
#endif
#ifdef HAVE_KQUEUE
    else if (method == "kqueue") {
        run<TKqueue>(debug, address);
    }
#endif
#ifdef HAVE_IOCP
    else if (method == "iocp") {
        run<TIOCp>(debug, address);
    }
#endif
    else {
        std::cerr << "Unknown method\n";
    }
#else
    std::cerr << "coroio compiled without openssl support\n";
#endif

    return 0;
}
