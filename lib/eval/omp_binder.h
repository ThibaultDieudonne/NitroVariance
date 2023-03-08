#ifndef OMP_BINDER_H_INCLUDED
#define OMP_BINDER_H_INCLUDED

#include <vector>
#include "../OMPEval/omp/CardRange.h"
#include "../OMPEval/omp/EquityCalculator.h"

class omp_binder
{
    public:
        omp_binder();
        double evaluate(uint_fast32_t c0, uint_fast32_t c1, std::string const& board_str);
        double str_evaluate(std::string const& h0, std::string const& h1, std::string const& board_str);
        double get_pf_equity(std::string r0, std::string r1);
        uint16_t seven_cards_eval(uint_fast32_t c0, uint_fast32_t c1, uint_fast32_t c2, uint_fast32_t c3, uint_fast32_t c4, uint_fast32_t c5, uint_fast32_t c6);
        void player_eval(uint_fast32_t cs[11], std::string const& board_str, double equities[3], uint8_t player_id);

    private:
        std::vector<omp::CardRange> omp_hands;

        const std::string values = "AKQJT98765432";
        const std::string colors = "shdc";

        omp::EquityCalculator eq;
        omp::HandEvaluator eval;
        uint64_t dead = 0;
        double stdErrMargin = 1e-3; // stop when standard error below 0.1%
        double updateInterval = 0; // No callback
        unsigned threads = 1; // Vectorization is used at a higher level
        std::vector<omp::CardRange> ranges = std::vector<omp::CardRange>(2);

};

#endif // OMP_BINDER_H_INCLUDED
