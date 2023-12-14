#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <sstream>
#include <map>
#include <valarray>

using namespace std;

enum {
    NS = '|',
    EW = '-',
    NE = 'L',
    NW = 'J',
    SW = '7',
    SE = 'F',
    G = '.',
    START = 'S'
};

template<typename T>
ostream& operator<<(ostream& os, const vector<T>& v){
    bool first = true;
    for (const auto& vi : v){
        if (!first) os <<", ";
        os <<vi ;
        first = false;
    }
    return os;
}

template <typename T>
ostream& operator<<(ostream& os, const std::valarray<T>& v){
    bool first = true;
    for (const auto& vi : v){
        if (!first) os <<", ";
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
void print(vector<vector<T>> a){
    for (auto& b: a){
        cout <<b <<endl;
    }
}

typedef vector<string> map_t;
struct coord_t{
    int row = -1;
    int col = -1;
    coord_t& operator =(const coord_t& b){
        this->row = b.row;
        this->col = b.col;
        return *this;
    };
};
bool operator<(const coord_t& a, const coord_t& b){
    return a.row < b.row || ((a.row == b.row ) && (a.col < b.col));
}
bool operator==(const coord_t& a, const coord_t& b){
    return a.row == b.row && a.col == b.col;
}
ostream& operator<<(ostream& os, const coord_t& c){
    os <<"{ "<< c.row <<", " <<c.col <<" }";
    return os;
}
void swap(coord_t& lhs, coord_t& rhs)
{
    std::swap(lhs.row, rhs.row);
    std::swap(lhs.col, rhs.col);
}
struct pipe_item_t{
    int dist = -1;
    coord_t pos ;
    char type = '0';
    pipe_item_t& operator=(const pipe_item_t& b) = default;
};

void swap(pipe_item_t& lhs, pipe_item_t& rhs)
{
    std::swap(lhs.dist, rhs.dist);
    std::swap(lhs.pos, rhs.pos);
    std::swap(lhs.type, rhs.type);
}


bool operator<(const pipe_item_t& a, const pipe_item_t& b){
    return a.pos < b.pos;
}

ostream& operator<<(ostream& os, const pipe_item_t& p){
    os <<"{ " <<p.dist <<", " <<p.pos <<", " <<p.type <<" }";
    return os;
}

typedef vector<pipe_item_t> pipe_t;

coord_t find_start(const map_t& m){
    coord_t s; 
    for (int rowi = 0; rowi != m.size(); rowi++){
        auto res = m[rowi].find(START);
        if (res != string::npos){
            s.row = rowi;
            s.col = res;
            return s;
        }
    }
    return {};
}

inline
char lookup(const map_t& m, coord_t pos){
    return m[pos.row][pos.col];
}

char convert_start(const map_t& m, coord_t start){
    char top = G;
    char bot = G;
    char left = G;
    char right = G;
    char val = G;
    bool connects_top = false;
    bool connects_left = false;
    bool connects_right = false;
    bool connects_bot = false;
    if (start.row != 0){
        top = lookup(m,{start.row-1,start.col});
    }
    if (start.row != m.size() - 1){
        bot = lookup(m, {start.row+1, start.col});
    }
    if (start.col != 0){
        left = lookup(m, {start.row, start.col-1});
    }
    if (start.col != m[start.row].size() - 1){
        right = lookup(m, {start.row, start.col+1});
    }
    switch (top){
        case NS:
        case SE:
        case SW:{
            connects_top = true;
            break;
        };
        default: ;
    }
    switch (bot){
        case NS:
        case NE:
        case NW: {
            connects_bot = true;
            break;
        }
        default: ;
    }
    switch (left){
        case NE:
        case EW:
        case SE:{
            connects_left = true;
            break;
        }
        default: ;
    }
    switch (right){
        case NW:
        case EW: 
        case SW:{
            connects_right = true;
            break;
        }
        default: ;
    }
    if (connects_top){
        if (connects_left) return NW;
        if (connects_bot) return NS;
        if (connects_right) return NE;
    }
    if (connects_bot){
        if (connects_left) return SW;
        if (connects_right) return SE;
    }
    return EW;
}

/// Given the current position and the previous node, return the next pipe item in the path.
pipe_item_t travel_next(const map_t& m, const pipe_item_t& current, const pipe_item_t& previous){
    pipe_item_t next;
    next.pos = current.pos;
    next.dist = current.dist + 1;
    switch (current.type){
        case NE:{
            if (current.pos.row == previous.pos.row)
                next.pos.row--;
            else
                next.pos.col++;
            break;
        }
        case NW:{
            if (current.pos.row == previous.pos.row)
                next.pos.row--;
            else
                next.pos.col--;
            break;
        }
        case NS:{
            if (current.pos.row-1 == previous.pos.row)
                next.pos.row++;
            else
                next.pos.row--;
            break;
        }
        case EW:{
            if (current.pos.col - 1 == previous.pos.col)
                next.pos.col++;
            else 
                next.pos.col--;
            break;
        }
        case SE:{
            if (current.pos.row == previous.pos.row)
                next.pos.row++;
            else
                next.pos.col++;
            break;
        }
        case SW:{
            if (current.pos.row == previous.pos.row)
                next.pos.row++;
            else
                next.pos.col--;
            break;
        }
        default: 
            cout <<"Error: " <<current <<endl;
            break;
    }
    next.type= lookup(m, next.pos);
    return next;
}

pipe_t travel_loop(const map_t& m, coord_t start_pos, int64_t* max_dist){
    pipe_t pNW;
    pipe_t pSE;
    pipe_item_t piNW;
    pipe_item_t piSE;
    piNW.dist = 0;
    piNW.pos = start_pos;
    piNW.type = convert_start(m, piNW.pos);
    pNW.push_back(piNW);
    
    //North beats West, West beats East, East beats South.
    pipe_item_t forwardNW;
    pipe_item_t forwardSE;
    forwardNW.dist = 1;
    forwardSE.dist = 1;
    switch(piNW.type){
        case NE:
            forwardSE.pos = {piNW.pos.row, piNW.pos.col + 1};
            forwardNW.pos = {piNW.pos.row-1, piNW.pos.col};
            break;
        case NW:
            forwardNW.pos = {piNW.pos.row-1, piNW.pos.col};
            forwardSE.pos = {piNW.pos.row, piNW.pos.col - 1};
            break;
        case NS:
            forwardNW.pos = {piNW.pos.row+1, piNW.pos.col};
            forwardSE.pos = {piNW.pos.row-1, piNW.pos.col};
            break;
        case EW:
            forwardNW.pos = {piNW.pos.row, piNW.pos.col - 1};
            forwardSE.pos = {piNW.pos.row, piNW.pos.col + 1};
            break;
        case SE:
            forwardNW.pos = {piNW.pos.row, piNW.pos.col + 1};
            forwardSE.pos = {piNW.pos.row+1, piNW.pos.col};
            break;
        case SW:
            forwardNW.pos = {piNW.pos.row, piNW.pos.col - 1};
            forwardSE.pos = {piNW.pos.row+1, piNW.pos.col};
            break;
        default:
            cout <<"Error: " <<piNW.type <<endl;
            break;
    }
    forwardSE.type = lookup(m, forwardSE.pos);
    forwardNW.type = lookup(m, forwardNW.pos);
    cout <<"TL::forwardSE: " << forwardSE  <<endl;
    cout <<"TL::forwardNW: " << forwardNW  <<endl;
    pNW.push_back(forwardNW);
    piSE = piNW;

    for (int i = 0; i < 10000; ++i){
        piNW = travel_next(m, forwardNW, piNW) ;
        piSE = travel_next(m, forwardSE, piSE) ;
        
        cout <<"TL::NW: " << piNW <<endl;
        cout <<"TL::SE: " << piSE <<endl;
         
        pNW.push_back(piNW);
        pSE.push_back(piSE);
        if (piNW.pos == piSE.pos)
            break;
        swap(piNW, forwardNW);
        swap(piSE, forwardSE);
    }
    if (max_dist)
        *max_dist = piNW.dist;

    std::copy(pSE.begin(), pSE.end(), std::back_inserter(pNW));
    return pNW;
}

int p2(pipe_t& loop){
    int inside = 0;
    vector<pipe_t> rows;
    vector<int> row_index = {loop.front().pos.row};
    vector<int> col_index = {loop.front().pos.col};
    std::sort(loop.begin(), loop.end());
    for (auto it = loop.cbegin(); it != loop.cend(); ++it){
        row_index.push_back(it->pos.row);
        col_index.push_back(it->pos.col);
    }
    std::sort(row_index.begin(), row_index.end());
    auto rlast = std::unique(row_index.begin(), row_index.end());
    row_index.erase(rlast, row_index.end());
    std::sort(col_index.begin(), col_index.end());
    auto clast = std::unique(col_index.begin(), col_index.end());
    col_index.erase(clast, col_index.end());
    cout <<row_index.size() <<" Unique rows: " <<row_index <<endl;
    cout <<col_index.size() <<" Unique cols: " <<col_index <<endl;
    int min_col = 0;
    int max_col = col_index.back();
    cout <<"Max col: " <<max_col <<endl;
    auto lit = loop.begin();
    for (auto rit = row_index.begin(); rit != row_index.end(); ++rit){
        pipe_t pi;
        while (lit->pos.row == *rit && lit != loop.end()){
            pi.push_back(*lit++);
        }
        rows.push_back(pi);
    }
    cout <<"Rows: " <<endl;
    for (auto ri = rows.cbegin(); ri != rows.cend(); ++ri){
        vector<int> cols;
        std::transform(ri->cbegin(), ri->cend(), std::back_inserter(cols),[](auto a){ return a.pos.col;});
        vector<int> diff;
        cout <<"\t" <<*ri <<endl;
        auto it = std::adjacent_difference(cols.cbegin(), cols.cend(), back_inserter(diff));
        cout <<"Diff: " <<diff<<endl;
        bool even = false;
        auto removeit = std::remove_if(diff.begin(), diff.end(),[&even](const auto& i){
            even = !even;
            return !even;
        });
        diff.erase(removeit, diff.end());
        cout <<"\tDiff slice: " <<diff <<endl;
        for (auto& di : diff){
            if (di > 0) --di;
        }
        cout <<"\tDiff slice 2: " <<diff <<endl;
        inside += std::accumulate(diff.begin(), diff.end(), 0);
    }
    
    return inside;
}

int main(int argc, char* argv[]){
    using std::begin, std::end;
    if (argc < 2){
        cout <<"Usage " <<argv[0] <<" <input file>" <<endl;
        exit(1);
    }
    cout <<"Running: " ;
    for (int i = 0; i < argc; ++i){
        cout << argv[i] <<" ";
    }
    cout <<endl;
    std::ifstream in(argv[1]);
    string line;
    map_t map;
    while(getline(in,line)){
        map.push_back(line); 
    }
    coord_t start_pos = find_start(map);
    cout <<"Start pos: " <<start_pos<<endl;
    cout <<"Start type: " <<convert_start(map,start_pos) <<endl;
    int64_t max_dist = -1;
    auto loop = travel_loop(map, start_pos, &max_dist);
    cout <<"Travel Loop:\n" <<loop <<endl;
    cout <<"Max Dist: " <<max_dist <<endl;
    
    cout <<"Loop size: " <<loop.size() <<endl;
    int p2res = p2(loop);
    cout <<"Travel Loop:\n" <<loop <<endl;
    cout << "P2 Result: " <<p2res <<endl;
    return 0;
}




