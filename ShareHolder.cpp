#include "ShareHolder.hpp"
#include <iostream>

using std::cout;
using std::endl;

class Dealer;
class ShareHolder;

ShareHolder::ShareHolder() {}

bool
ShareHolder::Verify()
{
    Integer right{Integer::One()};
    for (unsigned i = 0; i < this->k; ++i) {
        right = ma.Multiply(right, ma.Exponentiate(this->EBroadcasts[i], EuclideanDomainOf<Integer>().Exponentiate(this->ind, i)));
    }
    Integer left{ma.Multiply(ma.Exponentiate(this->g, this->share), ma.Exponentiate(this->h, this->t))};
    return left == right;
}

Integer
ShareHolder::Recover(vector<ShareHolder*>& shareHolders)
{
    Integer res{Integer::Zero()};
    vector<Integer> denos, numers;
    unsigned k = shareHolders.front()->k.ConvertToLong();
    if (shareHolders.size() < k) {
        throw "Not enough secret to recover the secret.";
    }
    Integer finalDenominator{Integer::One()};
    Integer finalNumerator{Integer::Zero()};
    for (unsigned i = 0; i < k; ++i) {
        auto &s = shareHolders[i];
        Integer numerator{Integer::One()};
        Integer denominator{Integer::One()};
        numerator = numerator.Times(s->share);
        for (unsigned j = 0; j < k; ++j) {
            auto &p = shareHolders[j];
            if (s->ind != p->ind) {
                denominator = denominator.Times(s->ind.Minus(p->ind));
                numerator = numerator.Times(p->ind);
            }
        }
        finalDenominator *= denominator;
        denos.push_back(denominator);
        numers.push_back(numerator);
    }
    unsigned size = denos.size();
    for (unsigned i = 0; i < size; ++i) {
        finalNumerator += finalDenominator.DividedBy(denos[i]).Times(numers[i]);
    }
    if (finalNumerator.Modulo(finalDenominator).IsZero()) {
        res = finalNumerator.DividedBy(finalDenominator); res.SetPositive();
        return res;
    }
    else {
        throw "Cannot recover secret as an integer.";
    }
}