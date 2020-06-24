#include "cryptopp/nbtheory.h"
#include "cryptopp/integer.h"
#include "cryptopp/modarith.h"
#include "cryptopp/algebra.h"
#include "Dealer.hpp"
#include <vector>

using CryptoPP::Integer;
using CryptoPP::ModularArithmetic;
using CryptoPP::EuclideanDomainOf;
using std::vector;

class Dealer;

class ShareHolder
{
public:
    ShareHolder();
    bool Verify();
    static Integer Recover(vector<ShareHolder*>&);
private:
    vector<Integer> EBroadcasts;
    Integer share;
    Integer t;
    Integer g;
    Integer k;
    Integer h;
    Integer ind;
    ModularArithmetic ma;
    friend void Dealer::Share(vector<ShareHolder*>&);
    friend Integer ShareHolder::Recover(vector<ShareHolder*>&);
};