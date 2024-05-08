#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <sstream>
#include <map>
#include <iterator>
#include <set>
#include <deque>

using namespace std;

enum optic_type_t {
    EMPTY = (uint8_t)'.',
    LD  = (uint8_t)'\\',
    RD  = (uint8_t)'/',
    VV = (uint8_t)'|',
    HH = (uint8_t)'-',
};

enum direction_t {
    NORTH,
    SOUTH,
    EAST, 
    WEST
};

//struct coord_t{
//    size_t row = string::npos;
//    size_t col = string::npos;
//    coord_t& operator =(const coord_t& b){
//        this->row = b.row;
//        this->col = b.col;
//        return *this;
//    };
//};

using coord_t = std::pair<int,int>;

using mapping_t = std::pair<coord_t, direction_t>;
using coord_set_t = std::set<coord_t>;

typedef vector<string> map_t;

ostream& operator<<(ostream& os, const optic_type_t& optic){
    switch(optic){
    case EMPTY:{
        os <<" EM ";
        break;
        }
    case LD:{
        os <<" LD ";
        break;
        }
    case RD:{
        os <<" RD ";
        break;
        }
    case VV:{
        os <<" VV ";
        break;
        }
    case HH:{
        os <<" HH ";
        break;
        }
    }
    return os;
}

ostream& operator<<(ostream& os, const direction_t& d){
    switch(d){
    case NORTH:{
        os <<"N";
        break;
    }
    case SOUTH:{
        os <<"S";
        break;
    }
    case EAST:{
        os <<"E";
        break;
    }
    case WEST:{
        os <<"W";
        break;
    }
    }
    return os;
}
template<typename T>
ostream& operator<<(ostream& os, const vector<T>& v){
    bool first = true;
    for (const auto& vi : v){
        if (!first) os <<"\n";
        os <<vi ;
        first = false;
    }
    return os;
}
template<typename T, typename U>
ostream& operator<<(ostream& os, const pair<T,U>& p){
    os <<"{ " <<p.first <<", " <<p.second <<" }";
    return os;
}
template<typename T>
ostream& operator<<(ostream& os, const set<T>& v){
    bool first = true;
    for (auto si = v.cbegin(); si != v.cend(); ++si){
        if (!first) os <<"\n";
        os << *si ;
        first = false;
    }
    return os;
}


template<typename T>
void print(vector<vector<T>> a){
    for (auto& b: a){
        cout <<b <<endl;
    }
}



bool operator<(const coord_t& a, const coord_t& b){
    return a.first < b.first || ((a.first == b.first) && (a.second < b.second));
}

inline
bool operator==(const coord_t& a, const coord_t& b){
    return (a.first == b.first) && (a.second == b.second);
}

ostream& operator<<(ostream& os, const coord_t& c){
    os <<"{ "<< c.first <<", " <<c.second <<" }";
    return os;
}
void swap(coord_t& lhs, coord_t& rhs)
{
    std::swap(lhs.first, rhs.first);
    std::swap(lhs.second, rhs.second);
}

inline
char lookup(const map_t& m, coord_t pos){
    return m[pos.first][pos.second];
}

map_t parse_input(ifstream& in){
    string line;
    map_t m;
    while(getline(in,line)){
        if (!line.empty()){
            if (line[0] == '\n')
                continue;
            m.push_back(line);
        }
    }
    return m;
}

coord_t getNewPos(const map_t& map, coord_t curpos, const direction_t d, bool& edge){
    switch (d){
        case NORTH:{
            edge = curpos.first == 0;
            if (!edge) --curpos.first;
            return curpos;
            }
        case EAST:{
            edge = curpos.second == map[curpos.first].size() - 1;  
            if (!edge) ++curpos.second;
            return curpos;
            }
        case SOUTH:{
            edge = curpos.first == map.size() - 1;
            if (!edge) ++curpos.first;
            return curpos;
            }
        case WEST: {
            edge = curpos.second == 0;
            if (!edge) --curpos.second;
            return curpos;
            }
        default:
            break;
    }
    return curpos;
}

bool walk(const map_t& map, coord_t oldpos, direction_t d, map_t& emap, set<coord_t>& energized, std::set<mapping_t>& map_set, std::deque<mapping_t>& pending_list){
    bool edge = false;
    coord_t pos = getNewPos(map, oldpos, d, edge);
    uint8_t c = map[pos.first][pos.second];
    std::cout <<"Walking " <<d <<" from " <<oldpos << " takes us to " <<pos <<" [ " <<(edge ? "true" : "false") <<" ] = " <<c<<endl ;
    if(edge) return false;
    emap[pos.first][pos.second] = '#';

    auto ee = energized.emplace(pos);
    auto mse = map_set.emplace(std::make_pair(pos, d));
    if (!mse.second){
    //Insert occurred into mapset. Need to continue walking.
        cout <<"Pos " <<pos <<" at direction " <<d <<" already hit. Stopping." <<endl;
        return false;
    }
    switch(c){
    case EMPTY:{
        cout <<"Pos " <<pos <<" is EMPTY."<<endl;
        pending_list.emplace_back(std::make_pair(pos, d));
        cout <<"Adding " <<pos <<", " <<d <<" to list."<<endl;
        break;
        }
    case LD:{
        cout <<"Pos " <<pos <<" is LD."<<endl;
        switch(d){
        case NORTH:
            pending_list.emplace_back(std::make_pair(pos, WEST));
            cout <<"Adding " <<pos <<", " <<WEST <<" to list."<<endl;
            break;
        case SOUTH:
            pending_list.emplace_back(std::make_pair(pos, EAST));
            cout <<"Adding " <<pos <<", " <<EAST <<" to list."<<endl;
            break;
        case EAST:
            pending_list.emplace_back(std::make_pair(pos, SOUTH));
            cout <<"Adding " <<pos <<", " <<SOUTH <<" to list."<<endl;
            break;
        case WEST:
            pending_list.emplace_back(std::make_pair(pos, NORTH));
            cout <<"Adding " <<pos <<", " <<NORTH <<" to list."<<endl;
            break;
        }
        break;
    }
    case RD:{
        cout <<"Pos " <<pos <<" is RD."<<endl;
        switch(d){
        case NORTH:
            pending_list.emplace_back(std::make_pair(pos, EAST));
            cout <<"Adding " <<pos <<", " <<EAST <<" to list."<<endl;
            break;
        case SOUTH:
            pending_list.emplace_back(std::make_pair(pos, WEST));
            cout <<"Adding " <<pos <<", " <<WEST <<" to list."<<endl;
            break;
        case EAST:
            pending_list.emplace_back(std::make_pair(pos, NORTH));
            cout <<"Adding " <<pos <<", " <<NORTH <<" to list."<<endl;
            break;
        case WEST:
            pending_list.emplace_back(std::make_pair(pos, SOUTH));
            cout <<"Adding " <<pos <<", " <<SOUTH <<" to list."<<endl;
            break;
        }
        break;
    }
    case VV:{
        cout <<"Pos " <<pos <<" is VV."<<endl;
        switch(d){
        case NORTH: 
        case SOUTH: 
            pending_list.emplace_back(std::make_pair(pos, d));
            cout <<"Adding " <<pos <<", " <<d <<" to list."<<endl;
            break;
        case EAST:
        case WEST:
            pending_list.emplace_back(std::make_pair(pos, NORTH));
            cout <<"Adding " <<pos <<", " <<NORTH <<" to list."<<endl;
            pending_list.emplace_back(std::make_pair(pos, SOUTH));
            cout <<"Adding " <<pos <<", " <<SOUTH <<" to list."<<endl;
        }
        break;
    }
    case HH:{
        cout <<"Pos " <<pos <<" is HH."<<endl;
        switch(d){
        case EAST: 
        case WEST: 
            pending_list.emplace_back(std::make_pair(pos, d));
            cout <<"Adding " <<pos <<", " <<d <<" to list."<<endl;
            break;
        case NORTH: 
        case SOUTH:
            pending_list.emplace_back(std::make_pair(pos, EAST));
            cout <<"Adding " <<pos <<", " <<EAST <<" to list."<<endl;
            pending_list.emplace_back(std::make_pair(pos, WEST));
            cout <<"Adding " <<pos <<", " <<WEST <<" to list."<<endl;
        }
        break;
    }
    default:{
        cout <<"Pos " <<pos <<" = '" <<c <<"' [" <<(uint16_t) c <<"]"<<endl;
    }//default

    } //switch()
    return true;
}


int main(int argc, char* argv[]){
    using std::begin, std::end;
    if (argc < 2){
        cout <<"Usage " <<argv[0] <<" <input file> " <<endl;
        exit(1);
    }
    cout <<"Running: " ;
    for (int i = 0; i < argc; ++i){
        cout << argv[i] <<" ";
    }
    cout <<endl;
    size_t p2count = 3;
    if (argc > 2)
        p2count = stoi(argv[2]);
    std::ifstream in(argv[1]);
    auto map = parse_input(in);
    cout <<"Read file: " <<endl;
    size_t sum = 0;
    cout <<map <<endl;
    cout <<"-====-=-=-=-=-=" <<endl;
    for (auto n : map){
       cout <<n <<endl;
    }

    std::set<coord_t> energized;
    std::set<mapping_t> map_set;
    std::deque<mapping_t> pending_list;
    auto emap = map;
    coord_t pos = std::make_pair(0,-1);
    direction_t d = EAST;
    bool cont = false;
    pending_list.emplace_back(std::make_pair(pos,d));
    while (!pending_list.empty()){
        auto mapping = pending_list.front();
        pos = mapping.first;
        d = mapping.second;
        pending_list.pop_front();
        std::cout <<"Walking " <<d <<" from " <<pos <<endl ;
        walk(map, pos, d, emap, energized, map_set, pending_list);

    }
    auto count = 0; 
    for (auto n : emap){
        count += std::count(n.begin(), n.end(), '#');
    }
    cout <<"\n==========\n";
    cout <<"Energized count: " <<count<<endl;
    cout <<"Energized: " <<energized.size()<<endl;
    //cout <<"Energized: " <<energized<<endl;

    //for (auto n : emap){
    //   cout <<n <<endl;
    //}
    
    return 0;
}




