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

inline
char lookup(const map_t& m, coord_t pos){
    return m[pos.row][pos.col];
}

map_t transpose(const map_t& m ){
    map_t out;
    auto orig_rowsize = m.size();
    auto orig_colsize = m[0].size();
    for (auto col = 0; col < orig_colsize; ++col){
        string s;
        for (auto row = 0; row < orig_rowsize; ++row){
            s.push_back(m[row][col]);
        }
        out.push_back(s);
    }
    return out;
}

map_t expand(const map_t& m){
    map_t out;
    auto orig_colsize = m[0].size();
    //string s(orig_colsize,'.');
    string s;

    for (const auto& line : m){
        out.push_back(line);
        if(line.find_first_of("#") == string::npos){
            out.push_back(line);
        }
    }
    return out;
}

vector<coord_t> find_galaxies(const map_t& m){
    int maxrow = (int)m.size();
    int maxcol = (int)m[0].size();
    vector<coord_t> out;
    for (int row = 0; row < maxrow; ++row){
        int col = 0;
        size_t offset = 0;
        offset = m[row].find("#",col);
        while (offset != string::npos){
            col = offset;
            out.push_back({row,col});
            cout <<"\t" <<out.back() <<endl;
            offset = m[row].find('#',col+1) ;
        }
    }
    return out; 
}

vector<vector<int>> get_combinations(int total_options, int selection_size){
    int n = total_options;
    int r = selection_size;
    std::vector<bool> v(n);
    std::fill(v.begin(), v.begin() + r, true);
    vector<vector<int>> out;
    do {
        vector<int> combo;
        for (int i = 0; i < n; ++i) {
            if (v[i]) {
                combo.push_back(i);
            }
        }
        out.push_back(combo);
    } while (std::prev_permutation(v.begin(), v.end()));
    return out;
}

vector<vector<int>> get_dist(const vector<coord_t>& pos, vector<vector<int>> combos, int64_t* sum){
    vector<vector<int>> out;
    for (int i = 0; i < pos.size() - 1; ++i){
        auto p1 = pos[i];
        vector<int> outi;
        for (int j = i+1; j < pos.size(); ++j){
            auto p2 = pos[j];
            auto d = abs(p2.row - p1.row) + abs(p2.col - p1.col); 
            *sum += d;
            outi.push_back(d);
        }
        out.push_back(outi);
    }
//    for(auto combo : combos){
//        vector<int> outi = combo;
//        auto p1 = pos[combo[0]];
//        auto p2 = pos[combo[1]];
//        auto d = abs(p2.row - p1.row) + abs(p2.col - p1.col); 
//        if (sum) *sum += d;
//        outi.push_back(d);
//        out.push_back(outi);
//    }
    return out;
}

vector<coord_t> find_galaxies2(const map_t& m){
    int maxrow = (int)m.size();
    int maxcol = (int)m[0].size();
    vector<coord_t> out;
    for (int row = 0; row < maxrow; ++row){
        int col = 0;
        size_t offset = 0;
        offset = m[row].find_first_of("#",col);
        while (offset != string::npos){
            col += offset;
            out.push_back({row,col});
            offset = m[row].find('#',col+1) ;
        }
    }
    return out; 
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
    map_t mapin;
    int rowc = 0;
    while(getline(in,line)){
        if (in && !line.empty()){
            mapin.push_back(line); 
            cout <<"Row: " <<rowc++ <<"\t|" <<line <<"|" <<endl;
        }
    }
    auto corig = find_galaxies(mapin);
    cout <<"Galaxies at: ";
    for (auto c : corig){
        cout <<"\t" <<c <<endl;
    }//cout <<"Coords: " <<coords <<endl;
    cout <<"Read File: ";
    cout <<"Map.size() : [" <<mapin.size() <<", " <<mapin[0].size() <<"]" <<endl;
    auto mapre = expand(mapin);
    cout <<"After row expansion: " ;
    cout <<"Map.size() : [" <<mapre.size() <<", " <<mapre[0].size() <<"]" <<endl;
    auto maptre = transpose(mapre);
    cout <<"After transpose: " ;
    cout <<"Map.size() : [" <<maptre.size() <<", " <<maptre[0].size() <<"]" <<endl;
    auto maptce = expand(maptre);
    cout <<"After expansion: " ;
    cout <<"Map.size() : [" <<maptce.size() <<", " <<maptce[0].size() <<"]" <<endl;
    auto mapt = transpose(maptce);
    cout <<"After transpose: " ;
    cout <<"Map.size() : [" <<mapt.size() <<", " <<mapt[0].size() <<"]" <<endl;
    ofstream of ("day11_orig_grid.txt");
    for (auto e : mapt){
        of <<e <<endl;
    }
    
    auto coords = find_galaxies(mapt);
    for (auto c : coords){
        cout <<"\t" <<c <<endl;
    }//cout <<"Coords: " <<coords <<endl;
    auto combos = get_combinations((int)coords.size(),2);
    //cout <<"Combos: " << combos <<endl;

    int64_t sum2 = 0;
    auto dist = get_dist(coords, combos, &sum2);
    cout <<"Dist: \n" ;
    int sum = 0; 
    for (auto d : dist){
        cout <<"\t" <<d <<endl;
        sum += d.back();
    }
    cout <<"Sum: " <<sum <<endl;
    cout <<"Sum2: " <<sum2 <<endl;


    return 0;
}




