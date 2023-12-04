#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <sstream>

using namespace std;

struct card_t{ 
    uint64_t id = 0;
    uint64_t winning_count = 0;
    uint64_t instances = 0;
};

vector<card_t> cards;

int parse_game(std::string line, uint64_t& id){
    auto colon_pos = line.find(":");
    auto game_id_str = line.substr(line.find(" "),colon_pos); 
    id = stoi(game_id_str);
    vector<string> draws_sv;
    auto start_it = colon_pos+1;
    auto bar_it= line.find("|", start_it+1);
    auto winning_numbers_string = line.substr(start_it,bar_it-start_it);
    stringstream ss(winning_numbers_string);
    std::vector<int> winning_numbers;
    int wn = -1;
    while (ss){
        ss >> wn;
        if (ss){
            winning_numbers.push_back(wn);
        }
    }
    cout <<"\tWinning Numbers:\n";
    for (auto cn : winning_numbers){
        cout <<"\t\t" <<cn <<endl;
    }
    auto card_nums = line.substr(bar_it+1);
    std::vector<int> current_numbers;
    stringstream csn(card_nums);
    while (csn){
        csn >> wn;
        if (csn){
            current_numbers.push_back(wn);
        }
    }
    cout <<"\tCurrent Numbers:\n";
    for (auto cn : current_numbers){
        cout <<"\t\t" <<cn <<endl;
    }
    sort(winning_numbers.begin(), winning_numbers.end());
    sort(current_numbers.begin(), current_numbers.end());
    std::vector<int> matches;
    std::set_intersection(winning_numbers.begin(), winning_numbers.end(), 
                        current_numbers.begin(), current_numbers.end(),
                        std::back_inserter(matches));
    cout <<"Found " <<matches.size() <<" matches\n";
    int points = 0;
    if (!matches.empty()){
        points = pow(2,matches.size() - 1);
    }
    cards.push_back({ id, matches.size(), 1 });
    return points;

}

int main(int argc, char* argv[]){
    if (argc < 2){
        cout <<"Usage " <<argv[0] <<" <input file>" <<endl;
        exit(1);
    }
    std::ifstream in(argv[1]);
    string line;
    uint64_t line_count = 0;
    uint64_t game_number;
    uint64_t sum = 0;
    uint64_t power_sum = 0;
    while(getline(in,line)){
        if (line.empty()){
            ++line_count;
            continue;
        }
        cout <<"========\n" <<line <<endl;
        auto max_game = parse_game(line, game_number);
        cout <<"\nCard Id: " <<game_number  <<" points: "<<max_game;
        sum += max_game;
        cout <<endl;
    }
    cout <<"Sum of game ids is: " <<sum<<endl;

    for (auto idx = 0; idx < cards.size(); ++idx){
        auto& card = cards[idx];
        const int count = card.winning_count;
        for (int instance_idx = 0; instance_idx < card.instances; ++instance_idx){
            for (int count_idx = 1; count_idx <= count; ++count_idx){
                ++cards[idx + count_idx].instances;
            }
        }
        power_sum += card.instances;
    }
    cout <<"P2: " <<power_sum<<endl;

    return 0;
}
