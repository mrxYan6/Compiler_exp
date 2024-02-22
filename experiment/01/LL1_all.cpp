#include <bits/stdc++.h>

using i64 = long long;

// 集合交集
std::set<int> operator & (const std::set<int> a, const std::set<int> b) {
    std::set<int> ans;
    for (auto x : a) {
        if (b.count(x)) {
            ans.insert(x);
        }
    }
    return ans;
}

// 集合并集
std::set<int> operator + (const std::set<int> a, const std::set<int> b) {
    std::set<int> ans;
    for (auto x : a) {
        ans.insert(x);
    }
    for (auto x : b) {
        ans.insert(x);
    }
    return ans;
}

// 集合减法
std::set<int> operator - (const std::set<int> a, const std::set<int> b) {
    std::set<int> ans(a);
    for (auto x : b) {
        ans.erase(x);
    }
    return ans;
}

struct rule {
    std::string L;
    std::vector<std::string> R;

    rule(const std::string s) {
        std::stringstream ss(s);
        ss >> L;
        std::string tmp, sep;
        while (ss >> sep >> tmp) {
            R.push_back(tmp);
        }
    }

    void show() {
        std::cout << L << " =: ";
        for (auto &s : R) {
            std::cout << s << " ";
        }
        std::cout << std::endl;
    }
};

struct rule_token {
    int L;
    std::vector<std::vector<int>> R;
};

struct trie{
    std::map<int, trie*> nxt;
    bool is_end = false;
    int count = 0;
    int id = -1;

    void insert(std::vector<int> &s, int pos) {
        if (pos == s.size()) {
            is_end = true;
            return;
        }
        if (!nxt.count(s[pos])) {
            nxt[s[pos]] = new trie();
        }
        nxt[s[pos]]->count++;
        nxt[s[pos]]->id = s[pos];
        nxt[s[pos]]->insert(s, pos + 1);
    }

    ~trie() {
        for (auto &[c, p] : nxt) {
            delete p;
        }
    }
};

struct gramma_token{ 
    std::map<int, std::vector<std::vector<int>>> G;
    std::vector<std::string> mp;
    std::map<std::string, int> id;
    int n = 0, tmp_cnt = 0;
    int start = 1;

    std::map<int, std::set<int>> first, fellow;
    
    std::map<int, std::vector<std::set<int>>> select;

    void add_rule(int l, std::vector<int> r) {
        G[l].push_back(r);
    }

    int new_rule() {
        return n++;
    }

    
    void add_tmp () {
        while (mp.size() < n) {
            id["tmp" + std::to_string(tmp_cnt)] = mp.size();
            mp.push_back("tmp" + std::to_string(tmp_cnt++));   
        }
    }

    void unique() {
        for (auto &[l, r] : G) {
            std::sort(r.begin(), r.end());
            r.erase(std::unique(r.begin(), r.end()), r.end());
        }
    }

    void remove_rep() {
        bool modify = true;
        while (modify) {
            modify = false;
            auto G_T = G;
            for (auto &[l, r] : G_T) {
                trie t;

                for (auto x : r) {
                    t.insert(x, 0);
                }

                auto dfs = [&] (auto dfs, trie* cur) -> std::vector<int> {
                    if (cur->count == 1 && cur->is_end) {
                        return std::vector<int> ();
                    }

                    std::vector<std::vector<int>> sons;
                    
                    if (cur->is_end) {
                        sons.push_back(std::vector<int> (1, 0));
                    }
                    
                    for (auto &[c, p] : cur->nxt) {
                        auto s = dfs(dfs, p);
                        s.push_back(c);
                        sons.push_back(s);
                    }

                    if (cur == &t) {
                        G[l].clear();
                        for (auto x : sons) {
                            std::reverse(x.begin(), x.end());
                            G[l].push_back(x);
                        }
                        return std::vector<int> ();
                    }
                    if (sons.size() > 1) {
                        modify = true;
                        int x = new_rule();
                        for (auto T : sons) {
                            std::reverse(T.begin(), T.end());
                            G[x].push_back(T);
                        }
                        return std::vector<int> (1, x);
                    } else {
                        return sons[0];
                    }
                };

                dfs(dfs, &t);
            }
        }
        unique();
    }

    void show() {
        unique();
        add_tmp();
        for (auto [l, r] : G) {
            std::cout << mp[l] << " -> ";
            int m = r.size();
            for (auto &x : r) {
                for (auto &y : x) {
                    std::cout << mp[y] << " ";
                }
                if (&x != &(*r.rbegin())) {
                    std::cout << "| ";
                }
            }
            std::cout << "\n";
        }
    }
    
    // 消去直接左递归
    void remove_direct_left_recursion(int l) {
        //remove left recursion
        add_tmp();
        auto r = G[l];
        std::vector<std::vector<int>> A, B;
        for (auto &x : r) {
            if (x[0] == l) {
                A.push_back(x);
            } else {
                B.push_back(x);
            }
        }
        if (A.size() == 0) {
            return;
        }

        if (B.empty()) {
            std::cerr << "Error: a -> a+\n";
            exit(0);
        }
        int x = new_rule();
        for (auto &y : B) {
            y.push_back(x);
        }
        for (auto &y : A) {
            y.erase(y.begin());
            y.push_back(x);
        }

        G[l] = B;
        G[x] = A;
        G[x].push_back(std::vector<int> (1, 0));
        unique();
    }

    // 消去间接左递归
    void remove_left_recursion() {
        //remove left recursion
        add_tmp();

        std::vector<int> not_end;
        for (auto &[l, r] : G) {
            if (r.size() == 0) {
                continue;
            }
            not_end.push_back(l);
        }
        int m = not_end.size();

        for (int ii = 0; ii < m; ++ii) {
            int i = not_end[ii];
            for (int jj = 0; jj < ii; ++jj) {
                int j = not_end[jj];
                auto A = G[i], B = G[j];
                for (auto y : A) {
                    if (y[0] == j) {
                        G[i].erase(std::find(G[i].begin(), G[i].end(), y));
                        for (auto z : B) {
                            std::vector<int> tmp = z;
                            tmp.insert(tmp.end(), y.begin() + 1, y.end());
                            G[i].push_back(tmp);
                        }
                    }
                }
            }
            remove_direct_left_recursion(i);
        }
        unique();
    }

    void get_first() {
        for (int i = 0; i < n; ++i) {
            if (!G.count(i) || G[i].size() == 0) {
                first[i].insert(i);
            }
        }

        std::vector<int> last_moify(n, 0);
        bool modify = true;
        for (int i = 0; modify; ++i) {
            // std::cerr << "i = " << i << std::endl;
            modify = false;
            for (auto [l, r] : G) {
                bool modify_l = false;
                for (auto y : r) {
                    bool empty = true;
                    for (auto yi : y) {
                        if (!empty) break;
                        if (last_moify[yi] >= last_moify[l]) {
                            for (auto x : first[yi]) {
                                if (x == 0) continue;
                                if (!first[l].count(x)) {
                                    first[l].insert(x);
                                    modify_l = true;
                                }
                            }
                        }
                        empty = first[yi].count(0);
                    }
                    if (empty) {
                        if (!first[l].count(0)) {
                            first[l].insert(0);
                            modify_l = true;
                        }
                    }
                }
                if (modify_l) {
                    modify = true;
                    last_moify[l] = i;
                }
            }
        }
    }

    void get_fellow() {
        get_first();

        std::set<int> end;

        for (int i = 0; i < n; ++i) {
            if (!G.count(i) || G[i].size() == 0) {
                end.insert(i);
            }
        }

        // -1 means $
        fellow[start].insert(-1);

        bool modify = true;
        for (int i = 0; modify; ++i) {
            modify = false;
            for (auto [l, r] : G) {
                for (auto y : r) {
                    for (int j = 0; j < y.size(); ++j) {
                        int yi = y[j];
                        if (end.count(yi))continue;
                        bool empty = true;
                        for (int k = j + 1; k < y.size(); ++k) {
                            int yk = y[k];
                            if (!empty) break;
                            for (auto x : first[yk]) {
                                if (x == 0) continue;
                                if (!fellow[yi].count(x)) {
                                    fellow[yi].insert(x);
                                    modify = true;
                                }
                            }
                            empty = first[yk].count(0);
                        }
                        if (empty) {
                            for (auto x : fellow[l]) {
                                if (!fellow[yi].count(x)) {
                                    fellow[yi].insert(x);
                                    modify = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    void get_select() {
        for (auto [l, r] : G) {
            int m = r.size();
            select[l].resize(m);
            for (int i = 0; i < m; ++i) {
                auto &y = r[i];
                bool empty = true;
                for (auto yi : y) {
                    if (!empty) break;
                    for (auto x : first[yi]) {
                        if (x == 0) continue;
                        select[l][i].insert(x);
                    }
                    empty = first[yi].count(0);
                }
                if (empty) {
                    for (auto x : fellow[l]) {
                        select[l][i].insert(x);
                    }
                }
            }
        }
    }

    bool check_ll1() {
        for (auto [l, r] : G) {
            int m = r.size();
            for (int i = 0; i < m; ++i) {
                for (int j = i + 1; j < m; ++j) {
                    if ((select[l][i] & select[l][j]).size()) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    std::vector<std::vector<int>> biao;
    std::vector<int> end, not_end;
    void get_graph() {
        end.clear();
        not_end.clear();
        for (int i = -1; i < n; ++i) {
            if (!G.count(i) || G[i].size() == 0) {
                end.push_back(i);
            } else {
                not_end.push_back(i);
            }
        }

        biao = std::vector<std::vector<int>> (not_end.size(), std::vector<int> (end.size(), -1));
        for (auto [l, r] : G) {
            int m = r.size();
            for (int i = 0; i < m; ++i) {
                for (auto x : select[l][i]) {
                    biao[std::find(not_end.begin(), not_end.end(), l) - not_end.begin()][std::find(end.begin(), end.end(), x) - end.begin()] = i;
                }
            }
        }
    }

    void put_graph() {
        // 用md格式打印表
        std::cout << "| | ";
        for (auto x : end) {
            std::cout << (x == -1 ? "$" : mp[x]) << " | ";
        }
        std::cout << "\n";

        std::cout << "| --- |";
        for (auto x : end) {
            std::cout << " --- |";
        }
        std::cout << "\n";

        for (int i = 0; i < not_end.size(); ++i) {
            std::cout << "| " << mp[not_end[i]] << " | ";
            for (int j = 0; j < end.size(); ++j) {
                if (biao[i][j] == -1) {
                    std::cout << " | ";
                } else {
                    std::cout << mp[not_end[i]] << " -> ";
                    for (auto x : G[not_end[i]][biao[i][j]]) {
                        std::cout << mp[x] << " ";
                    }
                    std::cout << " | ";
                }
            }
            std::cout << "\n";
        }
    }

    bool is_end(int x) {
        return !G.count(x) || G[x].size() == 0;
    }
};

gramma_token parse(std::vector<rule> rules) {
    std::vector<std::string> mp;
    std::map<std::string, int> id;
    std::string epislon = "ε";

    int n = rules.size();
    std::vector<rule_token> tokens(n);

    id[epislon] = 0;
    mp.push_back(epislon);

    auto get_id = [&] (std::string x) {
        if (!id.count(x)) {
            id[x] = id.size();
            mp.push_back(x);
        }
        return id[x];
    };
    for (int i = 0; i < n; ++i) {
        tokens[i].L = get_id(rules[i].L);
        tokens[i].R.resize(rules[i].R.size());
    }

    for (int i = 0; i < n; ++i) {
        int m = rules[i].R.size();
        for (int j = 0; j < m; ++j) {
            auto r = rules[i].R[j];
            while (r.length() > 0) {
                bool modify = false;
                // std::cerr << "!" << r << std::endl;
                for (auto [str, ID] : id) {
                    if (r.length() < str.length()) {
                        continue;
                    } else {
                        // std::cerr << str << " " << r.substr(0, str.length()) << "\n"; 
                        if (r.substr(0, str.length()) == str) {
                            r = r.substr(str.length());
                            tokens[i].R[j].push_back(get_id(str));
                            modify = true;
                            break;
                        }
                    }
                }
                if (!modify) {
                    auto tmp = r.substr(0, 1);
                    r = r.substr(1);
                    tokens[i].R[j].push_back(get_id(tmp));
                }
            }
        }
    }

    gramma_token g;
    g.mp = mp;
    g.id = id;

    for (auto x : tokens) {
        for (auto y : x.R) {
            g.G[x.L].push_back(y);
        }
    }

    g.n = g.mp.size();

    return g;
}

void parse_str(gramma_token &g, const std::string s) {
    std::vector<int> input, work;

    for (auto str : s) {
        input.push_back(g.id[std::string(1, str)]);
    }
    input.push_back(-1);

    work.push_back(-1);
    work.push_back(g.start);
    // std::cerr << "!!!\n";
    // return;

    int idx = 0;
    while (work.size() > 1) {
        auto top = work.back();
        work.pop_back();

        if (idx < input.size() && top == input[idx]) {
            idx++;
        } else if (g.is_end(top)) {
            std::cout << "Not matching\n";
            return;
        } else if (g.biao[std::find(g.not_end.begin(), g.not_end.end(), top) - g.not_end.begin()][std::find(g.end.begin(), g.end.end(), input[idx]) - g.end.begin()] == -1) {
            std::cout << "Not matching\n";
            return;
        } else {
            auto r = g.G[top][g.biao[std::find(g.not_end.begin(), g.not_end.end(), top) - g.not_end.begin()][std::find(g.end.begin(), g.end.end(), input[idx]) - g.end.begin()]];
            std::cout << g.mp[top] << " -> ";
            for (auto x : r) {
                std::cout << g.mp[x] << " ";
            }
            std::cout << "\n";

            if (r[0] == 0) continue;
            std::reverse(r.begin(), r.end());
            work.insert(work.end(), r.begin(), r.end());            
        }
    }
    std::cout << "Matching\n";
}

int main() {
    std::vector<rule> g;
    std::string line;
    int n;
    std::cin >> n;
    std::cin.get();
    while (n-- && std::getline(std::cin, line)) {
        g.push_back(rule(line));
    }

    auto g_token = parse(g);
    // g_token.show();
    g_token.remove_left_recursion();
    g_token.remove_rep();
    g_token.get_first();
    g_token.get_fellow();
    g_token.get_select();
    g_token.get_graph();
    g_token.show();
    // g_token.put_graph();


    int t;
    std::cin >> t;
    while (t--) {
        std::string s;
        std::cin >> s;
        parse_str(g_token, s);
    }

    return 0;
} 