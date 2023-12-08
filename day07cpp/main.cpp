#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <sstream>
#include <map>

using namespace std;

typedef string hand_t;

enum class hand_type_t {HIGHCARD = 0, ONEPAIR, TWOPAIR, THREEOAK, FULLHOUSE, FOUROAK, FIVEOAK};

const std::map<char, int> cards = {{'2',0},
                             {'3',1},
                             {'4',2},
                             {'5',3},
                             {'6',4},
                             {'7',5},
                             {'8',6},
                             {'9',7},
                             {'T',8},
                             {'J',-9},
                             {'Q',10},
                             {'K',11},
                             {'A',12}};

struct player_t{
    hand_t hand;
    int bid;
    hand_type_t type;
    hand_type_t p2type;
};

hand_type_t operator+(const hand_type_t h, const int i){
    switch(h){
        case hand_type_t::HIGHCARD:{
            switch(i){
                case 0: return hand_type_t::HIGHCARD;
                default: return hand_type_t::ONEPAIR;
            }
            break;
        }
        case hand_type_t::ONEPAIR:{
            switch(i){
                default: 
                case 0: return hand_type_t::ONEPAIR;
                case 1: return hand_type_t::THREEOAK;
            }
            break;
        }
        case hand_type_t::TWOPAIR:{
            switch(i){
                default: 
                case 0: return hand_type_t::TWOPAIR;
                case 1: return hand_type_t::FULLHOUSE;
                case 2: return hand_type_t::FOUROAK;
                case 3: return hand_type_t::FIVEOAK;
            }
            break;
        }
        case hand_type_t::THREEOAK:{
            switch(i){
                default:
                case 0: return hand_type_t::THREEOAK;
                case 1: return hand_type_t::FOUROAK;
                case 2: return hand_type_t::FIVEOAK;
                case 3: return hand_type_t::FOUROAK;
            }
            break;
        }
        case hand_type_t::FULLHOUSE:{
            switch(i){
                default:
                case 0: return hand_type_t::FULLHOUSE;
                case 1: return hand_type_t::FOUROAK; //shouldn't be possible.
                case 2: return hand_type_t::FIVEOAK;
                case 3: return hand_type_t::FIVEOAK;
            }
            break;
        }
        case hand_type_t::FOUROAK:{
            switch(i){
                default: 
                case 0: return hand_type_t::FOUROAK;
                case 1: return hand_type_t::FIVEOAK; 
                case 2: return hand_type_t::FIVEOAK; 
                case 3: return hand_type_t::FIVEOAK; 
                case 4: return hand_type_t::FIVEOAK; 
            }
            break;
        }
        case hand_type_t::FIVEOAK:
            return hand_type_t::FIVEOAK;
    }
}
hand_type_t classify(hand_t hand){
    std::map<char, int> m;
    for (auto c : hand){
        if(m.count(c) > 0){
            m[c] += 1;
        } else {
            m[c] = 1;
        }
    }
    if (m.size() == 1) return hand_type_t::FIVEOAK;
    if (m.size() == 2){
        if (m.begin()->second == 4 || m.begin()->second == 1){
            return hand_type_t::FOUROAK ;
        }
        return hand_type_t::FULLHOUSE; 
    }
    if (m.size() == 3){
        for (auto mi : m){
            if (mi.second == 2){
                return hand_type_t::TWOPAIR;
            }
        }
        return hand_type_t::THREEOAK;
    }
    if (m.size() == 4){
        return hand_type_t::ONEPAIR ;
    }
    return hand_type_t::HIGHCARD ;
}

hand_type_t p2classify(hand_t hand){
    std::map<char, int> m;
    for (auto c : hand){
        if(m.count(c) > 0){
            m[c] += 1;
        } else {
            m[c] = 1;
        }
    }
    if (m.size() == 1) return hand_type_t::FIVEOAK;
    if (m.size() == 2){
        if (m.begin()->second == 4 || m.begin()->second == 1){
            return hand_type_t::FOUROAK + m['J'];
        }
        return hand_type_t::FULLHOUSE + m['J'];
    }
    if (m.size() == 3){
        for (auto mi : m){
            if (mi.second == 2){
                return hand_type_t::TWOPAIR+m['J'];
            }
        }
        return hand_type_t::THREEOAK + m['J'];
    }
    if (m.size() == 4){
        if (m.count('J')){
            if (m['J'] == 2) return hand_type_t::THREEOAK;
        }
        return hand_type_t::ONEPAIR + m['J'];
    }
    return hand_type_t::HIGHCARD + m['J'];;
}

istream& operator>>(istream& in, player_t& p){
    in >> p.hand >>p.bid;
    p.type = classify(p.hand);
    p.p2type = p2classify(p.hand);
    return in;
}
ostream& operator<<(ostream& os, const hand_type_t h){
    switch(h){
        case hand_type_t::HIGHCARD:
            os <<"High Card";
            break;
        case hand_type_t::ONEPAIR:
            os <<"One Pair";
            break;
        case hand_type_t::TWOPAIR:
            os <<"Two Pair";
            break;
        case hand_type_t::THREEOAK:
            os <<"3 of a Kind";
            break;
        case hand_type_t::FULLHOUSE:
            os <<"Full House";
            break;
        case hand_type_t::FOUROAK:
            os <<"4 of a Kind";
            break;
        case hand_type_t::FIVEOAK:
            os <<"5 of a Kind";
            break;
    }
    return os;
}
ostream& operator<<(ostream& os, const player_t p){
    os << "{ " <<p.hand <<", " <<p.bid <<", " <<p.type <<", " <<p.p2type <<" }";
    return os;
}

template<typename T>
ostream& operator<<(ostream& os, const vector<T>& v){
    bool first = true;
    for (const auto& vi : v){
        if (!first) os <<", ";
        os <<vi <<"\n"; 
        first = false;
    }
    return os;
}

bool compare_hand(hand_t a, hand_t b){
    for (int i = 0; i < 5; ++i){
        if( cards.at(a[i]) < cards.at(b[i]) )
            return true;
        if( cards.at(a[i]) > cards.at(b[i]) )
            return false;
    }
    return false;
}

bool sort_players(player_t a, player_t b){
    if (a.type < b.type)
        return true;
    if (a.type > b.type)
        return false;
    return compare_hand(a.hand,b.hand);
}

bool p2sort_players(player_t a, player_t b){
    if (a.p2type < b.p2type)
        return true;
    if (a.p2type > b.p2type)
        return false;
    return compare_hand(a.hand,b.hand);
}

std::vector<player_t> parse_file(istream& in){
    std::vector<player_t> res;
    player_t p;
    while (in >> p){
        res.push_back(p);
    }
    return res;
}

int main(int argc, char* argv[]){
    using std::begin, std::end;
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

    auto a = parse_file(in);
    cout <<"Players: " <<a<<endl;

    std::sort(a.begin(), a.end(), sort_players);
    cout <<"Sorted Players: " <<a<<endl;

    uint64_t total_winnings = 0;
    for (auto rank = 0; rank < a.size(); ++rank){
       const auto& p = a[rank];
       uint64_t winnings = p.bid * (rank + 1);
       total_winnings += winnings;
    }
    cout <<"total winnings: " <<total_winnings <<endl;

    std::sort(a.begin(), a.end(), p2sort_players);
    total_winnings = 0;
    for (auto rank = 0; rank < a.size(); ++rank){
       const auto& p = a[rank];
       uint64_t winnings = p.bid * (rank + 1);
       total_winnings += winnings;
    }
    cout <<"Sorted Players: " <<a<<endl;
    cout <<"P2 total winnings: " <<total_winnings <<endl;

    cout <<"J hands: \n";
    for (auto p : a){
        if (p.hand.find('J') != string::npos){
            cout <<p <<endl;
        }
    }
     cout <<"=============\n\n\n";
    for (auto p : a){
        if (p.hand.find('J') != string::npos){
            if (p.type == p.p2type)
                cout <<p <<endl;
        }
    }

    return 0;
}
