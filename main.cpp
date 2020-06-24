#include "ShareHolder.hpp"
#include <iostream>
#include <cassert>

using std::cout;
using std::endl;

int
main(int argc, char** argv)
{
    AutoSeededRandomPool prng;
    Integer secret{prng, Integer::One(), Integer{"999999999999999999999999999999999999999999999999999999"}};
    cout << secret << endl;
    unsigned k = 30, n = 100;
    Dealer* D = new Dealer(secret, k, n);
    vector<ShareHolder*> shareHolders;
    for (unsigned i = 0; i < n; ++i) {
        shareHolders.push_back(new ShareHolder());
    }
    D->Share(shareHolders);
    for (auto p: shareHolders) {
        assert(p->Verify());
    }
    cout << ShareHolder::Recover(shareHolders) << endl;
}