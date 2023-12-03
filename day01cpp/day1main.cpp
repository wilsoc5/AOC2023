#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>

using namespace std;

int main(int argc, char* argv[]){
    if (argc < 2){
        cout <<"Usage " <<argv[0] <<" <input file>" <<endl;
        exit(1);
    }
    std::ifstream in(argv[1]);
    string line;
    uint64_t value(0);
    uint64_t line_count = 0;
    std::vector<std::pair<std::string,std::string>> digits = { {"zero","0"}, 
        {"one", "1"},
        {"two", "2"},
        {"three","3"},
        {"four", "4"},
        {"five", "5"},
        {"six", "6"},
        {"seven", "7"},
        {"eight", "8"},
        {"nine", "9"},
        {"0","0"},
        {"1","1"},
        {"2","2"},
        {"3","3"},
        {"4","4"},
        {"5","5"},
        {"6","6"},
        {"7","7"},
        {"8","8"},
        {"9","9"}};
    while(getline(in,line)){
        char left, right;
        char left_p2, right_p2;
        if (line.empty()){
            ++line_count;
            continue;
        }
        //p2
        cout <<"==============\n"<<line <<endl;
        vector<std::pair<int, size_t>> loc; 
        int c = 0;
        for (const auto& d : digits){
            auto it = line.find(d.first,0);
            while (it != string::npos){
                loc.push_back({c,it});
                it = line.find(d.first,it+1);
            }
            ++c;
        }
        std::sort(loc.begin(), loc.end(), [](auto a, auto b){return a.second < b.second;});
        for (const auto& l : loc){
            if (l.second != string::npos)
                cout <<"\t" <<l.first <<"\t" <<digits[l.first].first <<"\t" <<digits[l.first].second <<"\t" <<l.second <<endl;
        }
        cout <<line <<endl;
        //end p2
        left = digits[loc.front().first].second[0];
        right = digits[loc.back().first].second[0];
        cout <<"Left: Loc.front: " <<loc.front().first <<", "<<loc.front().second 
            <<"\t" <<digits[loc.front().first].first <<", " <<digits[loc.front().first].second<<endl;
        cout <<"Right: Loc.back: " <<loc.back().first <<", "<<loc.back().second 
            <<"\t" <<digits[loc.back().first].first <<", " <<digits[loc.back().first].second<<endl;
        cout <<left <<"\t Right: " <<right <<endl;
        int left_val = left - '0';
        int right_val = right - '0';
        int cal_value = left_val * 10 + right_val;
        std::cout <<"Line: " <<line_count <<"\t Value: "<<cal_value <<endl;
        value += cal_value;
        ++line_count;
    }
    cout <<"Sum of cal values is: " <<value <<endl;

    return 0;
}
