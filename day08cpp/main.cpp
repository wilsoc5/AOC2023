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

typedef string node_name_t;

struct node_t{
    node_name_t name;
    map<char,node_name_t> next;
};

typedef map<node_name_t, node_t> maplist_t;

struct puzzle_t {
    string directions;
    maplist_t maplist;
};

istream& operator>>(istream& in, node_t& n){
//AAA = (BBB, CCC)
    char c;
    //    AAA      =   (   BBB,          CCC)
    in >> n.name >>c >>c >>n.next['L'] >>n.next['R'];;
    n.next['L'].pop_back(); //Remove ',';
    n.next['R'].pop_back(); //Remove ')';
    return in;
}
ostream& operator<<(ostream& os, const node_t& n){
    os <<n.name <<" = (" <<n.next.at('L') <<", " <<n.next.at('R') <<")\n";
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

const node_name_t& walk(const maplist_t& maplist, const node_name_t& cur_node, char dir){
    const auto& next_node = maplist.at(cur_node).next;
    return next_node.at(dir);
}

puzzle_t parse_file(istream& in){
    puzzle_t p;
    string dummy;
    in >>p.directions;
    while (in){
        node_t n;
        in >> n;
        if (in)
        p.maplist[n.name] = n;
    }
    return p;
}
ostream& operator<<(ostream& os, const puzzle_t& p){
    os <<p.directions <<endl;
    for (auto it = p.maplist.cbegin(); it != p.maplist.cend(); ++it){
        os <<it->second;
    }
    return os;
}

bool alldone(const vector<node_name_t>& v){
    bool equal = true;
    for (int i = 0; i < v.size() && equal; ++i){
        equal = equal && (v[i].back() == 'Z');
    }
    return equal;
}
int main(int argc, char* argv[]){
    using std::begin, std::end;
    if (argc < 2){
        cout <<"Usage " <<argv[0] <<" <input file>" <<endl;
        exit(1);
    }
    std::ifstream in(argv[1]);
    uint64_t count = 0;
    uint64_t direction_length;
    uint64_t direction_count = 0;

    auto p = parse_file(in);
    direction_length = p.directions.size();
    cout <<"Puzzle: " <<p<<endl;
//    node_name_t curname = "AAA";
//
//    uint64_t loop_count = 100000;
//    while((curname != "ZZZ")&&(loop_count-- > 0)){
//        char d = p.directions[direction_count++];
//        curname = walk(p.maplist, curname, d);
//        ++count;
//        direction_count = direction_count % direction_length;
//    }
//    cout <<"P1: Walked " <<count <<" steps from AAA to ZZZ"<<endl;

    std::vector<node_name_t> starts;
    for (auto it = p.maplist.cbegin(); it != p.maplist.cend(); ++it){
        if (it->first.back() == 'A')
            starts.push_back(it->first);
    }
    auto num_ghosts = starts.size();
    cout <<"P2: # Ghosts: " <<num_ghosts <<endl;

    uint64_t p2count = 0;
    uint64_t p2direction_count = 0;

    uint64_t p2loop_count = 100000000;
    vector<uint64_t> counts;
    for (auto& s : starts){
        node_name_t curname = s;
        p2direction_count = 0;
        p2count = 0;
        
        while((curname.back() != 'Z')&&(p2loop_count-- > 0)){
            char d = p.directions[p2direction_count++];
            curname = walk(p.maplist,curname, d);
            ++p2count;
            p2direction_count = p2direction_count % direction_length;
        }
        counts.push_back(p2count);
    }
    cout <<"Counts: " <<counts <<endl;
    auto prod = 1ULL;
    for (auto c : counts){
        prod = std::lcm(prod,c);
    }
    cout <<"Product: " <<prod <<endl;
    return 0;
}




