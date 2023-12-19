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
    size_t row = string::npos;
    size_t col = string::npos;
    coord_t& operator =(const coord_t& b){
        this->row = b.row;
        this->col = b.col;
        return *this;
    };
};

bool operator<(const coord_t& a, const coord_t& b){
    return a.row < b.row || ((a.row == b.row ) && (a.col < b.col));
}

inline
bool operator==(const coord_t& a, const coord_t& b){
    return (a.row == b.row) && (a.col == b.col);
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
vector<coord_t> transpose (vector<coord_t> in){
    std::transform(in.begin(), in.end(), in.begin(), [](auto c){
        swap(c.row, c.col);
        return c;
    });
    return in;
}

vector<coord_t> expand(const map_t& m, vector<coord_t> coords, size_t expansion_val){
    size_t r = 0;
    //for (int r = 0; r < m.size(); ++r){
    for (const auto& line : m){
        //const auto& line = m[r];
        if(line.find_first_of("#") == string::npos){
            cout <<"Row: " <<r <<endl;
            auto it = stable_partition(coords.begin(), coords.end(),[r](auto c){
                return c.row < r;
            });
            std::transform(it, coords.end(), it, [expansion_val](auto c){
                c.row += expansion_val;
                return c;
            });
            r += expansion_val;
        }
        ++r;
    }
    return coords;
}

vector<coord_t> find_galaxies(const map_t& m){
    int maxrow = (int)m.size();
    int maxcol = (int)m[0].size();
    vector<coord_t> out;
    for (size_t row = 0; row < maxrow; ++row){
        size_t col = 0;
        size_t offset = 0;
        offset = m[row].find("#",col);
        while (offset != string::npos){
            col = offset;
            out.push_back({row,col});
            //cout <<"\t" <<out.back() <<endl;
            offset = m[row].find('#',col+1) ;
        }
    }
    return out; 
}

vector<vector<size_t>> get_dist(const vector<coord_t>& pos, int64_t* sum){
    vector<vector<size_t>> out;
    for (int i = 0; i < pos.size() - 1; ++i){
        auto p1 = pos[i];
        vector<size_t> outi;
        for (int j = i+1; j < pos.size(); ++j){
            auto p2 = pos[j];
            auto d = (size_t)abs((long long)(p2.row) - (long long) p1.row) + abs((long long)p2.col - (long long)p1.col);
            *sum += d;
            outi.push_back(d);
        }
        out.push_back(outi);
    }
    return out;
}

int main(int argc, char* argv[]){
    using std::begin, std::end;
    size_t expansion_value = 1;
    if (argc < 2){
        cout <<"Usage " <<argv[0] <<" <input file> <expansion value = 1> " <<endl;
        exit(1);
    }
    cout <<"Running: " ;
    for (int i = 0; i < argc; ++i){
        cout << argv[i] <<" ";
    }
    cout <<endl;
    std::ifstream in(argv[1]);
    if (argc > 2){
        try{
            size_t ev = stoull(argv[2]);
            expansion_value = ev;
        } catch (...){
            cerr <<"Couldn't parse expansion value: " <<argv[2] <<endl;
            return EINVAL;
        }
    }
    cout <<"Expansion Value = " <<expansion_value <<endl;
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
    vector<coord_t> row_galaxies = expand(mapin, corig, expansion_value);
    auto mapint = transpose(mapin);
    vector<coord_t> rgt = transpose(row_galaxies);
    vector<coord_t> col_galaxies = expand(mapint, rgt, expansion_value);
    sort(col_galaxies.begin(), col_galaxies.end());
    auto expanded_galaxies = transpose(col_galaxies);
    sort(expanded_galaxies.begin(), expanded_galaxies.end());
    int64_t sum3 = 0;
    auto dist2 = get_dist(expanded_galaxies, &sum3);
    cout <<"Sum3: " <<sum3 <<endl;


    return 0;
}




