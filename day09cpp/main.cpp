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

int main(int argc, char* argv[]){
    using std::begin, std::end;
    if (argc < 2){
        cout <<"Usage " <<argv[0] <<" <input file>" <<endl;
        exit(1);
    }
    std::ifstream in(argv[1]);
    string line;
    vector<int64_t> next_vals; 
    while(getline(in,line)){
        vector<vector<int64_t>> arrays;
        vector<int64_t> inputvals;
        uint64_t lc = 100;
        stringstream ss(line);
        int iv;
        while (ss){
            ss >>iv;
            if (ss) inputvals.push_back(iv);
        }
        uint64_t i = 0;
        arrays.push_back(inputvals);
        bool allzero = true;
        do{
            inputvals.clear();
            std::adjacent_difference(arrays[i].begin(), arrays[i].end(), std::back_inserter(inputvals));
            arrays.push_back(inputvals);
            ++i; //before all_of
            allzero = std::all_of(inputvals.begin()+i, inputvals.end(), [](auto a){ return a == 0;});
        }while((!allzero) && (--lc > 0));

        
        cout <<"Product0: " <<arrays<<endl;
        auto old_it = arrays.rbegin();
        for (auto it = arrays.rbegin()+1; it != arrays.rend(); ++it){
            cout <<"it: " <<*it <<endl; 
            auto val = it->back() + old_it->back();
            cout <<"Val: " <<val <<endl;
            it->push_back(it->back() + old_it->back());
            ++old_it;
        }
        next_vals.push_back(arrays[0].back());
    }
    cout <<"Product: " <<next_vals<<endl;
    cout <<"Sum: " <<std::accumulate(next_vals.begin(), next_vals.end(), 0) <<endl;
    return 0;
}




