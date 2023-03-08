#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "./lib/eval/omp_binder.h"

// TODO: use better rng, use bigger ints OR use maths instead of MC sim

long int mc_simulation(int BASE_WINRATE, int TARGET_WINRATE, int WINDOW, int SAMPLE)
{
    int rgn;
    int game_won;
    int hist[WINDOW];
    long int counter;
    long int result[SAMPLE];
    long int average = 0;
    int running_sum;
    int threshold = TARGET_WINRATE * WINDOW / 100;

    for (int i(0); i<SAMPLE; ++i)
    {
        counter = 0;
        running_sum = 0;
        while (1)
        {
            rgn = rand()%100;
            if (rgn < BASE_WINRATE)
            {
                game_won = 1;
            }
            else
            {
                game_won = 0;
            }
            if (counter < WINDOW)
            {
                hist[counter] = game_won;
                running_sum += game_won;
            }
            else
            {
                running_sum -= hist[counter%WINDOW];
                running_sum += game_won;
                if (running_sum < threshold)
                {
                    result[i] = counter - WINDOW;
                    // cout << "found after " << counter - WINDOW << endl;
                    break;
                }
                hist[counter%WINDOW] = game_won;
            }
            ++counter;
        }
    }
    for (int i(0); i<SAMPLE; ++i)
    {
        average += result[i] / SAMPLE;
    }
    std::cout << "found after " << average << std::endl;
    return average;
}

void calculate_equities_from_files()
{
    omp_binder evaluator;
    std::string line;
    std::vector<std::string> hero_hands;
    std::vector<std::string> vilain_hands;
    std::vector<std::string> boards;
    std::vector<std::string> final_boards;
    std::vector<double> real_equities;
    std::vector<double> adjusted_equities;

    std::ifstream hero_hands_file("./subfiles/hero_hands.txt");
    while (getline(hero_hands_file, line)) {
        hero_hands.push_back(line);
    }
    hero_hands_file.close();

    std::ifstream vilain_hands_file("./subfiles/vilain_hands.txt");
    while (getline(vilain_hands_file, line)) {
        vilain_hands.push_back(line);
    }
    vilain_hands_file.close();

    std::ifstream boards_file("./subfiles/boards.txt");
    while (getline(boards_file, line)) {
        if (line=="none"){boards.push_back("");}
        else {boards.push_back(line);}
    }
    boards_file.close();

    std::ifstream final_boards_file("./subfiles/final_boards.txt");
    while (getline(final_boards_file, line)) {
        final_boards.push_back(line);
    }
    final_boards_file.close();

    for (int i(0); i<hero_hands.size(); ++i)
    {
        real_equities.push_back(evaluator.str_evaluate(hero_hands[i], vilain_hands[i], final_boards[i]));
        adjusted_equities.push_back(evaluator.str_evaluate(hero_hands[i], vilain_hands[i], boards[i]));
    }

    std::fstream re_file;
    re_file.open("./subfiles/real_equities.txt", std::ios_base::out);
    for(int i(0); i<real_equities.size(); i++)
    {
        re_file << real_equities[i] << std::endl;
    }
    re_file.close();

    std::fstream ae_file;
    ae_file.open("./subfiles/adjusted_equities.txt", std::ios_base::out);
    for(int i(0); i<adjusted_equities.size(); i++)
    {
        ae_file << adjusted_equities[i] << std::endl;
    }
    ae_file.close();
}

int main()
{
    srand((unsigned) time(NULL));
    // mc_simulation(33, 22, 200, 10000);
    calculate_equities_from_files();
    return 0;
}
