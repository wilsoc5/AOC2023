#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <sstream>

using namespace std;

struct draw_t {
    uint64_t red = 0;
    uint64_t blue = 0;
    uint64_t green = 0;
};

std::ostream& operator<<(std::ostream& os, draw_t d){
    os <<"{red: " <<d.red <<", blue: " <<d.blue <<", green: " <<d.green <<"}";
    return os;
}

draw_t parse_draw(std::string line){
    stringstream ssl ( line );
    draw_t d;
    cout <<"PD: " <<endl;
    cout <<"\t" <<line;
    while (ssl){
        int num = 0;
        string color;
        char comma;
        ssl >> num;
        cout <<"\n\t\t" <<num;
        ssl >> color;
        cout <<"\n\t\t" <<color;
        auto commait = color.find(',');
        if (commait != string::npos)
            color.erase(commait);
        cout <<"\n\t\t" <<color;
        if (color == "red"){
            d.red= num;
        } else if (color == "blue"){
            d.blue = num;
        } else if (color == "green") {
            d.green = num;
        }
    }
    cout <<"\t\t\t" <<d <<endl;
    return d;
}

draw_t parse_game(std::string line, uint64_t& id){
    auto colon_pos = line.find(":");
    auto game_id_str = line.substr(line.find(" "),colon_pos); 
    id = stoi(game_id_str);
    vector<string> draws_sv;
    auto start_it = colon_pos+1;
    auto semicolon_it = line.find(";", start_it+1);
    while (semicolon_it != string::npos){
        auto drawstring = line.substr(start_it,semicolon_it - start_it);
        cout <<"\t"<<drawstring <<"\n";
        draws_sv.push_back(drawstring);
        start_it = semicolon_it+1;
        semicolon_it = line.find(";", start_it+1);
    }
    draws_sv.push_back(line.substr(start_it));
    cout <<"\t"<<draws_sv.back()<<"\n";
    draw_t max_game;
    for (const auto& draw_s : draws_sv){
        auto d = parse_draw(draw_s);
        max_game.red = std::max(d.red, max_game.red);
        max_game.blue= std::max(d.blue, max_game.blue);
        max_game.green= std::max(d.green, max_game.green);
    }
    cout <<"PG: Max Game: " <<max_game <<endl;
    return max_game;
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
        bool possible = (max_game.red <= 12) &&
                        (max_game.blue <= 14) &&
                        (max_game.green <= 13);
        auto power = max_game.red * max_game.blue * max_game.green;
        power_sum += power;
        cout <<"\nGame Id: " <<game_number ;
        if (possible){
            cout <<" is possible";
            sum += game_number;
        }
        cout <<endl;
    }
    cout <<"Sum of game ids is: " <<sum<<endl;
    cout <<"Sum of power is: " <<power_sum <<endl;

    return 0;
}
