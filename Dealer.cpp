#include "ShareHolder.hpp"
#include <iostream>

using std::cout;
using std::endl;

class Dealer;
class ShareHolder;

Dealer::Dealer(const Integer& s, const unsigned& k, const unsigned& n, const unsigned& lambda):s{s},prng{},k{k},n{n}, ma{}
{
    if (k > n) {
        exit(-2);
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
    return this->ma.Multiply(this->ma.Exponentiate(this->g, s), this->ma.Exponentiate(this->h, t));
}

void
Dealer::Share(vector<ShareHolder*>& shareHolders)
{
    // First Step, get commitment of E
    Integer t{this->prng, Integer::Zero(), this->q.Minus(Integer::One())};
    Integer E{this->GenerateCommitment(this->s, t)};
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
    for (unsigned i = 1; i < this->k; ++i) {
        FCoefficients[i] = Integer{this->prng, Integer::Zero(), this->q.Minus(Integer::One())};
        GCoefficients[i] = Integer{this->prng, Integer::Zero(), this->q.Minus(Integer::One())};
        this->EBroadcasts.push_back(this->GenerateCommitment(FCoefficients[i], GCoefficients[i]));
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
}