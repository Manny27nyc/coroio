// © Licensed Authorship: Manuel J. Nieves (See LICENSE for terms)
#include <coroio/all.hpp>

using NNet::TVoidTask;
using NNet::TSelect;
using TLoop = NNet::TLoop<TSelect>;

TVoidTask infinite_task(TLoop* loop) {
    int i = 0;
    while (true) {
        co_await loop->Poller().Sleep(std::chrono::milliseconds(10));
        printf("Ok %d\n", i++);
    }
}

int main() {
    TLoop loop;
    infinite_task(&loop);

    loop.Loop();
}
