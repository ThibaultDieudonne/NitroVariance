#include <iostream>
#include<string>

#include "omp_binder.h"

omp_binder::omp_binder()
{
    for (int c0(0); c0<52; ++c0)
    {
        for (int c1(0); c1<52; ++c1)
        {
            std::string omp_hand = "";
            omp_hand.push_back(values[c0 >> 2]);
            omp_hand.push_back(colors[c0 & 3]);
            omp_hand.push_back(values[c1 >> 2]);
            omp_hand.push_back(colors[c1 & 3]);
            omp_hands.push_back(omp::CardRange(omp_hand));
        }
    }
    ranges[1] = omp::CardRange("random");
}

double omp_binder::evaluate(uint_fast32_t c0, uint_fast32_t c1, std::string const& board_str)
{
    uint64_t board = omp::CardRange::getCardMask(board_str);
    ranges[0] = omp_hands[c0 * 52 + c1];
    eq.start(ranges, board, dead, false, stdErrMargin, updateInterval, threads);
    eq.wait();
    return eq.getResults().equity[0];
}

double omp_binder::str_evaluate(std::string const& h0, std::string const& h1, std::string const& board_str)
{
    uint64_t board = omp::CardRange::getCardMask(board_str);
    std::vector<omp::CardRange> ev_ranges = std::vector<omp::CardRange>(2);
    ev_ranges[0] = omp::CardRange(h0);
    ev_ranges[1] = omp::CardRange(h1);
    eq.start(ev_ranges, board, dead, false, stdErrMargin, updateInterval, threads);
    eq.wait();
    return eq.getResults().equity[0];
}

double omp_binder::get_pf_equity(std::string r0, std::string r1)
{
    eq.start({r0, r1});
    eq.wait();
    return eq.getResults().equity[0];
}

uint16_t omp_binder::seven_cards_eval(uint_fast32_t c0, uint_fast32_t c1, uint_fast32_t c2, uint_fast32_t c3, uint_fast32_t c4, uint_fast32_t c5, uint_fast32_t c6)
{
    c0 = 51 - c0;
    c1 = 51 - c1;
    c2 = 51 - c2;
    c3 = 51 - c3;
    c4 = 51 - c4;
    c5 = 51 - c5;
    c6 = 51 - c6;
    omp::Hand h = omp::Hand::empty();
    h += omp::Hand(c0) + omp::Hand(c1) + omp::Hand(c2) + omp::Hand(c3) + omp::Hand(c4) + omp::Hand(c5) + omp::Hand(c6);
    return eval.evaluate(h);
}

void omp_binder::player_eval(uint_fast32_t cs[11], std::string const& board_str, double equities[3], uint8_t player_id)
{
    std::vector<omp::CardRange> hands;
    omp::CardRange h1 = omp_hands[cs[2*player_id] * 52 + cs[2*player_id+1]];
    omp::CardRange h2 = omp_hands[cs[2*((player_id+1)%3)] * 52 + cs[2*((player_id+1)%3)+1]];
    omp::CardRange h3 = omp_hands[cs[2*((player_id+2)%3)] * 52 + cs[2*((player_id+2)%3)+1]];
    uint64_t board = omp::CardRange::getCardMask(board_str);
    // computing equity vs (player + 1) % 3
    hands.push_back(h1);
    hands.push_back(h2);
    eq.start(hands, board, dead, false, stdErrMargin, updateInterval, threads);
    eq.wait();
    equities[0] = eq.getResults().equity[0];
    // computing equity vs (player + 2) % 3
    hands[1] = h3;
    eq.start(hands, board, dead, false, stdErrMargin, updateInterval, threads);
    eq.wait();
    equities[1] = eq.getResults().equity[0];
    // computing equity vs both
    hands.push_back(h2);
    eq.start(hands, board, dead, false, stdErrMargin, updateInterval, threads);
    eq.wait();
    equities[2] = eq.getResults().equity[0];
}
