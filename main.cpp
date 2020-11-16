#include "ShareHolder.hpp"
#include <iostream>
#include <cassert>
#include <sys/time.h>

using std::cout;
using std::endl;

const int M = 1e6;

int
main(int argc, char** argv)
{
    AutoSeededRandomPool prng;
    timeval begin, end;
    double shareTimeCost, verifyTimeCost = 0.0, recoverTimeCost;
    Integer MAX{"999999999999999999999999999999999999999999999999999999"}; cout << MAX.BitCount() << endl;
    Integer secret{prng, Integer::One(), Integer{"999999999999999999999999999999999999999999999999999999"}};
    unsigned k = 30, n = 100;
    if (argc > 1) {
        sscanf(argv[1], "%ud", &k);
    }
    Dealer* D = new Dealer(secret, k, n);
    vector<ShareHolder*> shareHolders;
    for (unsigned i = 0; i < n; ++i) {
        shareHolders.push_back(new ShareHolder());
    }
    gettimeofday(&begin, nullptr);
    D->Share(shareHolders);
    gettimeofday(&end, nullptr);
    shareTimeCost = (double) (end.tv_sec - begin.tv_sec)*M + (double) (end.tv_usec - begin.tv_usec);
    for (auto p: shareHolders) {
        gettimeofday(&begin, nullptr);
        auto b = p->Verify();
        gettimeofday(&end, nullptr);
        verifyTimeCost += (double) (end.tv_sec - begin.tv_sec)*M + (double) (end.tv_usec - begin.tv_usec);
        assert(b);
    }
    verifyTimeCost = verifyTimeCost / n;
    gettimeofday(&begin, nullptr);
    ShareHolder::Recover(shareHolders);
    gettimeofday(&end, nullptr);
    recoverTimeCost = (double) (end.tv_sec - begin.tv_sec)*M + (double) (end.tv_usec - begin.tv_usec);
    cout << ' ' << shareTimeCost << ' ' << verifyTimeCost << ' ' << recoverTimeCost << endl;
}