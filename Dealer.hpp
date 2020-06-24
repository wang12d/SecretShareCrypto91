#include "cryptopp/nbtheory.h"
#include "cryptopp/integer.h"
#include "cryptopp/osrng.h"
#include "cryptopp/modarith.h"
#include <vector>

using CryptoPP::PrimeAndGenerator;
using CryptoPP::Integer;
using CryptoPP::AutoSeededRandomPool;
using CryptoPP::ModularArithmetic;
using std::vector;

class ShareHolder;

class Dealer
{
public:
    Dealer(const Integer&, const unsigned&, const unsigned&, const unsigned& = 256);
    void Share(vector<ShareHolder*>&);
    Integer GenerateCommitment(const Integer&, const Integer&);
private:
    const int delta = 1;
    Integer s;
    AutoSeededRandomPool prng;
    unsigned k;
    unsigned n;
    Integer q;
    Integer g;
    Integer h;
    ModularArithmetic ma;
    vector<Integer> EBroadcasts;
    inline Integer DotProduct(const vector<Integer>&, const vector<Integer>&);
};

inline Integer
Dealer::DotProduct(const vector<Integer>& a, const vector<Integer>& b)
{
    if (a.size() != b.size()) {
        exit(-2);
    }
    unsigned size = a.size();
    Integer res{Integer::Zero()};
    for (unsigned i = 0; i < size; ++i) {
        res += a[i].Times(b[i]);
    }
    return res;
}