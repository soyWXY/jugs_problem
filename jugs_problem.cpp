//  Created by hhf on 2021/8/28.

#include <iostream>
#include <vector>
#include <set>
#include <list>

using namespace std;

using Jugs = vector<int>;
using State = vector<int>;
using Path = list<State>;
using Paths = set<Path>;
    
ostream& operator<<(ostream& os, State s) {
    if (s.size() == 0) return os;
    
    os << "(" << s[0];
    s.erase(s.begin());
    for (int& c: s) {
        os << "," << c;
    }
    os << ")";

    return os;
};

class Pouring {
    Jugs    capacity_;
    Paths   solutions_;
    Paths   all_path_;
    set<State>  all_state_;
    
    State fill(int target, State s) {
        s[target] = capacity_[target];
        return s;
    }
    State empty(int target, State s) {
        s[target] = 0;
        return s;
    }
    State pour(int from, int to, State s) {
        if (s[from] + s[to] <= capacity_[to]) {
            s[to] += s[from];
            s[from] = 0;
        }
        else {
            s[from] -= capacity_[to] - s[to];
            s[to] = capacity_[to];
        }
        
        return s;
    }
    set<State> extend(State s) {
        set<State> new_state;
        
        for (int i = 0; i<s.size(); ++i) {
            new_state.insert(fill(i, s));
            new_state.insert(empty(i, s));
            for (int j = 0; j<i; ++j) {
                new_state.insert(pour(j, i, s));
                new_state.insert(pour(i, j, s));
            }
        }
        
        return new_state;
    }
    
public:
    Pouring(Jugs j): capacity_(j) {}
    bool found(int target, State s) {
        for (auto c: s) {
            if (c == target) return true;
        }
        return false;
    }
    
    void solve(int target, int steps) {
        
        State init(capacity_);
        Path p;
        p.push_back(init);
        all_path_.insert(p);
        
        while (steps>0) {
            // extend all the last state of paths
            Paths new_paths;
            for (auto p: all_path_) {
                set<State> new_state = extend(p.back());
                for (auto  s: new_state) {
                    // append solution
                    if (found(target, s)) {
                        Path new_path = p;
                        new_path.push_back(s);
                        solutions_.insert(new_path);
                    }
                    else {
                        auto index = all_state_.find(s);
                        if (index == all_state_.end()) {
                            // append path list
                            Path new_path = p;
                            new_path.push_back(s);
                            new_paths.insert(new_path);
                        }
                    }
                }
            }
            all_path_ = new_paths;
            
            // update state set
            for (auto p: all_path_) {
                all_state_.insert(p.back());
            }
            --steps;
        }
    }
    void show() {
        for (auto p: solutions_) {
            cout << p.front();
            p.pop_front();
            for (auto s: p) {
                cout << "->" << s;
            }
            cout << endl;
        }
    }
};

int main() {
    cout << "insert maximum capacities of jugs: ";
    
    Jugs j;
    int n;
    while (cin >> n) {
        j.push_back(n);
        if (cin.get()=='\n') break;
    }
    
    int target;
    int step;
    if (j.size()>0) {
        cout << "enter target capacity and maximum step: ";
        cin >> target;
        cin >> step;
        
        if (target>0 && step>0) {
            Pouring p{j};
            p.solve(target, step);
            p.show();
        }
    }
    
    if (!(j.size() & target & step)) {
        cout << "input error!!!" << endl;
    }
    
    return 0;
}
