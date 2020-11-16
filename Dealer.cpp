#include "ShareHolder.hpp"
#include <iostream>
#include <sys/time.h>

using std::cout;
using std::endl;

class Dealer;
class ShareHolder;

Dealer::Dealer(const Integer& s, const unsigned& k, const unsigned& n, const unsigned& lambda):s{s},prng{},k{k},n{n}, ma{}
{
    if (k > n) {
        throw "The recover threshold k must not greater than n";
    }
    PrimeAndGenerator pq{this->delta, this->prng, lambda};
    this->q = pq.Prime();
    this->g = pq.Generator();
    this->h = Integer{prng, 1, this->q-1};
    ma.SetModulus(q);
}

Integer
Dealer::GenerateCommitment(const Integer& s, const Integer& t)
{
    Integer res{this->ma.Multiply(this->ma.Exponentiate(this->g, s), this->ma.Exponentiate(this->h, t))};
    return res;
}

void
Dealer::Share(vector<ShareHolder*>& shareHolders)
{
    // First Step, get commitment of E
    Integer t{this->prng, Integer::Zero(), this->q.Minus(Integer::One())};
    timeval begin, end;
    unsigned commitmentBitSize = 0;
    gettimeofday(&begin, nullptr);
    Integer E{this->GenerateCommitment(this->s, t)};
    gettimeofday(&end, nullptr);
    commitmentBitSize += E.BitCount();
    double commitTimeCost = (double) (end.tv_sec - begin.tv_sec)*1e6 + (double) (end.tv_usec - begin.tv_usec);
    // Second Step, Choose Polynomial of Z_p[x]
    vector<Integer> FCoefficients(this->k); FCoefficients[0] = this->s;
    vector<Integer> GCoefficients(this->k); GCoefficients[0] = t;
    vector<vector<Integer>> Xs(this->n, vector<Integer>(this->k, Integer::One()));
    // Now generating the coffiencis
    for (unsigned i = 0; i < this->n; ++i) {
        for (unsigned j = 1; j < this->k; ++j) {
            Xs[i][j] = EuclideanDomainOf<Integer>().Exponentiate(Integer{i+1}, Integer{j});
        }
    }
    this->EBroadcasts.push_back(E);
    unsigned EBroadcastBitSize = 0;
    unsigned shareBitSize = 0, tBitSize = 0, gBitSize = 0, hBitSize = 0, kBitSize = 0, indBitSize = 0;
    for (unsigned i = 1; i < this->k; ++i) {
        FCoefficients[i] = Integer{this->prng, Integer::Zero(), this->q.Minus(Integer::One())};
        GCoefficients[i] = Integer{this->prng, Integer::Zero(), this->q.Minus(Integer::One())};
        this->EBroadcasts.push_back(this->GenerateCommitment(FCoefficients[i], GCoefficients[i]));
    }
    for (auto &e: EBroadcasts) {
        EBroadcastBitSize += e.BitCount();
    }
    // Sending the Information
    for (unsigned i = 0; i < this->n; ++i) {
        shareHolders[i]->EBroadcasts = this->EBroadcasts;
        shareHolders[i]->share = this->DotProduct(FCoefficients, Xs[i]);
        shareHolders[i]->t = this->DotProduct(GCoefficients, Xs[i]);
        shareHolders[i]->g = this->g;
        shareHolders[i]->h = this->h;
        shareHolders[i]->k = this->k;
        shareHolders[i]->ind = i+1;
        shareHolders[i]->ma.SetModulus(this->q);
    }
    shareBitSize += shareHolders.front()->share.BitCount();
    tBitSize += shareHolders.front()->t.BitCount();
    gBitSize += shareHolders.front()->g.BitCount();
    hBitSize += shareHolders.front()->h.BitCount();
    kBitSize += shareHolders.front()->k.BitCount();
    indBitSize += shareHolders.front()->ind.BitCount();
    cout << commitmentBitSize << ' ' << EBroadcastBitSize << ' ' << shareBitSize << ' ' << tBitSize << ' ' << gBitSize << ' ' << hBitSize << ' ' << kBitSize << ' ' << indBitSize << ' ' << commitTimeCost;
}