#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <sstream>

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

std::vector<int64_t> parse_line(istream& in){
    string time;
    int64_t val;
    std::vector<int64_t> times;
    in >>time;
    while (in >>val){
        times.push_back(val);
    }
    return times;
}

std::pair<vector<int64_t>, vector<int64_t>> parse_file(istream& in){
    std::pair<vector<int64_t>, vector<int64_t>> res;
    string line;
    {
        getline(in,line);
        stringstream ss(line);
        res.first = parse_line(ss);
    }
    {
        getline(in,line);
        stringstream ss(line);
        res.second= parse_line(ss);
    }
    return res;
}
uint64_t convert(vector<int64_t> v){
    uint64_t correct= 0;
    auto digits = 0;
    cout <<"convert: " <<v <<"\n";
    for (auto it = v.rbegin(); it != v.rend(); ++it){
        auto d = uint64_t(ceil(log10(*it)));
        correct += *it * pow(10,digits);
        cout <<*it <<"\t" <<d  <<"\t" <<correct<<"\n";
        digits += d;
    }
    return correct;
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
    cout <<"Times: " <<a.first <<endl;
    cout <<"Distances: " <<a.second<<endl;


    vector<int> t(20);
    cout <<"Time\tDistance\n";
    vector<int> counts;
    for (auto i = 0; i < a.first.size(); ++i){
        int count = 0;
        t.resize(a.first[i]);
        auto max_time = a.first[i];
        auto max_distance = a.second[i];
        std::iota(t.begin(), t.end(), 0);
        auto peak = max_time / 2;
        auto best_time_min = int(ceil((max_time - sqrt(max_time*max_time - 4 * max_distance))/2.0));
        auto best_time_max = int(floor((max_time + sqrt(max_time*max_time - 4 * max_distance))/2.0));
        auto ways2win = best_time_max - best_time_min ;
        cout <<"Peak time: " <<peak<<endl;;
        cout <<"Best time: " <<best_time_min <<" (" <<best_time_max <<")"<<endl;
        cout <<"Ways to win: " <<ways2win<<endl;;
        
        for (auto thold : t){
            auto distance = thold * (max_time - thold);
            cout <<thold <<"\t" <<distance <<"\n";
            if (distance > a.second[i]){
                ++count;
            }
        }
        counts.push_back(count);
    }
    int prod = 1;
    for (auto c : counts)
        prod *= c;
    cout <<"Product: " <<prod <<endl;

    //p2: 
    auto p2_time= convert(a.first);
    uint64_t p2_distance= convert(a.second);
    cout <<"\nCorrect time:"<<p2_time<<endl;
    cout <<"\nCorrect distance:"<<p2_distance<<endl;
    t.resize(p2_time);
    auto p2c = 0;
    std::iota(t.begin(), t.end(), 0);
        for (auto thold : t){
            auto distance = thold * (p2_time - thold);
            if (distance > p2_distance){
                ++p2c;
            }
        }
    cout <<"P2 Counts: " <<p2c <<endl;

    return 0;
}
