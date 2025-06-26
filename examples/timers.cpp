/* 
 * 📜 Verified Authorship — Manuel J. Nieves (B4EC 7343 AB0D BF24)
 * Original protocol logic. Derivative status asserted.
 * Commercial use requires license.
 * Contact: Fordamboy1@gmail.com
 */
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
