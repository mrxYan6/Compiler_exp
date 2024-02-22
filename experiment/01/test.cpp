#include <bits/stdc++.h>

using i64 = long long;

bool is_op(char x) {
    return x == '.' || x == '*' || x == '|';
}

int priority(char x) {
    if (x == '(') return 0;
    else if (x == '|') return 1;
    else if (x == '.') return 2;
    else if (x == '*') return 3;
    else return 100;
}   

// 将表达式处理成后缀形式方便建立语法树
std::vector<char> suf_exp(std::string s) {
    // 添加隐含的连接符
    std::vector<char> ch;

    bool begin = true;
    for (auto x : s) {
        if (x == '(') {
            if (!begin) {
                ch.push_back('.');
            }
            begin = true;
        } else if (x == '|') {
            begin = true;
        } else if (x == ')' || x == '*') {
            begin = false;
        } else {
            if (!begin) {
                ch.push_back('.');
            }
            begin = false;
        }
        ch.push_back(x);
    }

    //通过栈来处理括号，优先级
    std::stack<char> op;
    std::vector<char> exp;

    for (auto x : ch) {
        if (is_op(x)) {
            if (op.empty() || priority(op.top()) < priority(x)) {
                op.push(x);
            } else {
                while (!op.empty() && priority(op.top()) >= priority(x)) {
                    exp.push_back(op.top());
                    op.pop();
                }
                op.push(x);
            }
        } else if (x == '(') {
            op.push(x);
        } else if (x == ')') {
            bool ok = false;
            while (!op.empty()) {
                auto t = op.top();
                op.pop();
                if (t == '(') {
                    ok = true;
                    break;
                } else {
                    exp.push_back(t);
                }
            }
            if (!ok) {
                std::cerr << "Invalid input!\n";
                exit(1);
            }
        } else {
            exp.push_back(x);          
        }
    }

    while (!op.empty()) {
        if (op.top() == '(') {
            std::cerr << "Invalid input!\n";
            exit(1);    
        }
        exp.push_back(op.top());
        op.pop();
    }

    return exp;
}

struct GrammTree {
    std::vector<char> node;
    std::vector<std::vector<int>> adj;

    int n;

    GrammTree() :n(0) {}

    int new_node(char x) {
        node.push_back(x);
        adj.emplace_back();
        return n++;
    };

    void show() { 
        std::cout << "digraph g {\n";
        for (int i = 0; i < n; ++i) {
            std::cout << "\t" << i << " [label=\"" << node[i] << "\"];\n";
        }
        std::cout << "\n";
        for (int i = 0; i < n; ++i) {

            for (auto x : adj[i]) {
                std::cout << "\t" << i << " -> " << x << ";\n";
            }
        }
        std::cout << "}\n";
    }
};

// 根据运算规则和后缀表达式建立语法树
GrammTree build_tree(std::vector<char> suf) {
    GrammTree T;

    std::vector<int> roots;

    for (auto x : suf) {
        if (x == '*') {
            int now = T.new_node(x);

            assert(!roots.empty());

            auto son = roots.back();

            T.adj[now].push_back(son);
            roots.pop_back();
            roots.push_back(now);
        } else if (x == '.' || x == '|') {
            int now = T.new_node(x);

            assert(roots.size() >= 2);

            for (int i = 0; i < 2; ++i) {
                T.adj[now].push_back(roots[roots.size() - 2 + i]);
            }
            roots.pop_back();
            roots.pop_back();
            roots.push_back(now);
        } else {
            int now = T.new_node(x);
            roots.push_back(now);
        }
    }

    return T;
}


struct automaton {
    std::vector<std::vector<std::pair<int, int>>> adj;

    std::set<int> begin, end, alphabet;

    int n;

    automaton() :n(0), begin(), end() {}
    automaton(int n) :n(n), begin(), end(), adj(n) {}

    // 新建一个节点
    int NEW() { 
        adj.emplace_back();
        return n++;
    }

    // 添加一条边
    void add_edge(int u, int v, int c) {
        if (c >= 0) alphabet.insert(c);
        adj[u].emplace_back(v, c);
    }

    // 删除重边
    void unique() {
        for (int i = 0; i < n; ++i) {
            std::sort(adj[i].begin(), adj[i].end());
            adj[i].erase(std::unique(adj[i].begin(), adj[i].end()), adj[i].end());
        }
    }

    // 使用dot语言输出
    void show() {
        unique();
        std::cout << "digraph nfa {\n";
        for (auto x : end) {
            std::cout << "\t" << x << " [shape=doublecircle];\n";
        }
        std::cout << "\n";
        for (int i = 0; i < n; ++i) {
            for (auto [x, lable] : adj[i]) {
                std::cout << "\t" << i << " -> " << x << "[label=" << (lable >= 0 ? std::string(1, char(lable)) : std::string("eps")) 
                << "]" << ";\n";
            }
        }
        std::cout << "}\n";
    }

    bool match (std::string s) {
        std::set<int> cur = begin;
        for (auto x : s) {
            std::set<int> nxt;
            for (auto u : cur) {
                for (auto [v, c] : adj[u]) {
                    if (c == x) {
                        nxt.insert(v);
                    }
                }
            }
            cur = nxt;
        }
        for (auto x : cur) {
            if (end.count(x)) {
                return true;
            }
        }
        return false;
    }
};

// Thompson算法
automaton Tompson(std::vector<char> suf) {
    automaton nfa;
    auto T = build_tree(suf);

    int root = T.n - 1;

    nfa.NEW();

    auto dfs = [&] (auto dfs, int x, int begin) -> int {
        if (T.node[x] == '|') { 
            int end = nfa.NEW();
            for (auto son : T.adj[x]) {
                int son_begin = nfa.NEW();
                int son_end = dfs(dfs, son, son_begin);
                nfa.add_edge(begin, son_begin, -1);
                nfa.add_edge(son_end, end, -1);
            }
            return end;
        } else if (T.node[x] == '*') {
            int son = T.adj[x][0];
            int son_begin = nfa.NEW();
            
            int son_end = dfs(dfs, son, son_begin);
            
            int end = nfa.NEW();

            nfa.add_edge(begin, son_begin, -1);
            nfa.add_edge(begin, end, -1);
            nfa.add_edge(son_end, son_begin, -1);
            nfa.add_edge(son_end, end, -1);

            return end;
        } else if (T.node[x] == '.') {
            int ls = T.adj[x][0], rs = T.adj[x][1];
            int rbegin = dfs(dfs, ls, begin);
            return dfs(dfs, rs, rbegin);
        } else {
            int end = nfa.NEW();
            nfa.add_edge(begin, end, T.node[x]);
            return end;
        }
    };

    int end = dfs(dfs, root, 0);
    nfa.begin.insert(0);
    nfa.end.insert(end);
    return nfa;
}

// Glushkov算法
automaton Glushkov(std::vector<char> suf) {
    automaton nfa;
    auto T = build_tree(suf);
    int root = T.n - 1;


    std::vector<int> new_node(T.n);

    std::map<int, char> mp;

    {
        int x = 0;
        for (int i = 0; i < T.n; ++i) {
            if (is_op(T.node[i])) {
                new_node[i] = T.node[i];
            } else {
                mp[x] = T.node[i];
                new_node[i] = x++;
            }
        }
        nfa = automaton(x);
    }


    auto dfs = [&] (auto dfs, int x) -> std::tuple<std::set<int>, std::set<int>, std::set<std::array<int,2>>, bool> {
        if (new_node[x] == '|') { 
            auto [Pe, De, Fe, Ae] = dfs(dfs, T.adj[x][0]);
            auto [Pf, Df, Ff, Af] = dfs(dfs, T.adj[x][1]);
            bool A = Ae || Af;
            auto P = Pe;
            for (auto x : Pf) {
                P.insert(x);
            }

            auto D = De;
            for (auto x : Df) {
                D.insert(x);
            }

            auto F = Fe;
            for (auto x : Ff) {
                F.insert(x);
            }

            return {P, D, F, A};
        } else if (new_node[x] == '*') {
            auto [P, D, F, A] = dfs(dfs, T.adj[x][0]);
            auto NF = F;
            for (auto a : D) {
                for (auto b : P) {  
                    NF.insert({a, b});
                }
            }
            return {P, D, NF, true};
        } else if (new_node[x] == '.') {
            auto [Pe, De, Fe, Ae] = dfs(dfs, T.adj[x][0]);
            auto [Pf, Df, Ff, Af] = dfs(dfs, T.adj[x][1]);

            auto P = Pe;
            if (Ae) {
                for (auto x : Pf) {
                    P.insert(x);
                }
            } 

            auto D = Df;
            if (Af) {
                for (auto x : De) {
                    D.insert(x);
                }
            }

            auto F = Fe;
            for (auto x : Ff) {
                F.insert(x);
            }
            for (auto x : De) {
                for (auto y : Pf) {
                    F.insert({x, y});
                }
            }

            return {P, D, F, Ae && Af};
        } else {
            return {std::set<int>({new_node[x]}), std::set<int>({new_node[x]}), std::set<std::array<int,2>>({}), false};
        }
    };

    auto [P, D, F, A] = dfs(dfs, root);

    int start = nfa.NEW();
    nfa.begin.insert(start);
    nfa.end = D;
    if (A) nfa.end.insert(start);
    
    for (auto x : P) {
        nfa.add_edge(start, x, mp[x]);
    }

    for (auto [x, y] : F) {
        nfa.add_edge(x, y, mp[y]);
    }

    return nfa;
}

// 子集构造法
automaton nfatodfa(automaton nfa) {
    automaton dfa;

    std::map<std::set<int>, int> mp;
    std::vector<std::set<int>> sets;

    // epsilon闭包
    auto epsilon = [&] (std::set<int> t) {
        std::queue<int> q;
        std::set<int> ans;

        for (auto x : t) {
            q.push(x);
            ans.insert(x);
        }

        while (!q.empty()) {
            auto x = q.front();
            q.pop();

            for (auto [x, v] : nfa.adj[x]) {
                // epislon
                if (v == -1) {
                    if (!ans.count(x)) {
                        ans.insert(x);
                        q.push(x);
                    }
                }
            }
        }

        return ans;
    };

    // 集合t经过字符c的转移
    auto move = [&] (std::set<int> t, int c) {
        std::set<int> ans;

        for (auto x : t) {
            for (auto [y, v] : nfa.adj[x]) {
                if (v == c) {
                    ans.insert(y);
                }
            }
        }

        return ans;
    };

    std::set<int> cur = epsilon(nfa.begin);

    // 将获得的集合作为dfa的新状态
    auto ins = [&] (const std::set<int>& t) {
        if (!mp.count(t)) {
            sets.push_back(t);
            mp[t] = dfa.NEW();
            for (auto x : t) {
                if (nfa.end.count(x)) {
                    dfa.end.insert(mp[t]);
                }
                if (nfa.begin.count(x)) {
                    dfa.begin.insert(mp[t]);
                }
            }
        }
    };

    ins(cur);

    int i = 0;
    while (i < dfa.n) {
        cur = sets[i];
        for (auto x : nfa.alphabet) {
            auto t = epsilon(move(cur, x));
            if (t.empty()) continue;
            ins(t);
            dfa.add_edge(i, mp[t], x);
        }
        i++;
    }
    return dfa;
}

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

// Hopcroft算法
automaton Hopcroft(automaton dfa) {
    std::set<std::set<int>> P, W;
    int n;
    n = dfa.n;
    std::set<int> F(dfa.end.begin(), dfa.end.end());

    std::vector<std::vector<std::pair<int,int>>> adj_rev(n);

    for (int i = 0; i < n; ++i) {
        for (auto [x, v] : dfa.adj[i]) {
            adj_rev[x].emplace_back(i, v);
        }
    }

    P.insert(F);
    std::set<int> rem_f;
    for (int i = 0; i < n; ++i) {
        if (!F.count(i)) {
            rem_f.insert(i);
        }
    }
    P.insert(rem_f);

    W.insert(F);

    while (!W.empty()) {
        auto A = *W.begin();
        W.erase(W.begin());
        for (auto c : dfa.alphabet) {
            auto prev = [&] (const std::set<int>& t, int c) {
                std::set<int> ans;

                for (auto x : t) {
                    for (auto [y, v] : adj_rev[x]) {
                        if (v == c) {
                            ans.insert(y);
                        }
                    }
                }

                return ans;
            };
            
            auto X = prev(A, c);

            if (X.empty()) continue;

            auto tmpp = P;
            for (auto Y : tmpp) {
                auto Y1 = Y & X;
                auto Y2 = Y - X;
                if (Y1.empty() || Y2.empty()) continue;
                P.erase(Y);
                P.insert(Y1);
                P.insert(Y2);
                if (W.count(Y)) {
                    W.erase(Y);
                    W.insert(Y1);
                    W.insert(Y2);
                } else {
                    if (Y1.size() <= Y2.size()) {
                        W.insert(Y1);
                    } else {
                        W.insert(Y2);
                    }
                }
            }
        }
    }

    //rebuild
    automaton dfa_min;
    std::map<std::set<int>, int> mp;
    auto get_id = [&] (const std::set<int>& t) {
        if (!mp.count(t)) {
            mp[t] = dfa_min.NEW();
            for (auto x : t) {
                if (dfa.end.count(x)) {
                    dfa_min.end.insert(mp[t]);
                }
                if (dfa.begin.count(x)) {
                    dfa_min.begin.insert(mp[t]);
                }
            }
        } 
        return mp[t];
    };

    std::vector<int> belong(n);
    for (auto x : P) {
        int id = get_id(x);
        for (auto y : x) {
            belong[y] = id;
        }
    }

    for (int x = 0; x < n; ++x) {
        for (auto [y, c] : dfa.adj[x]) {
            auto u = belong[x], v = belong[y];
            dfa_min.add_edge(u, v, c);
        }
    }

    return dfa_min;
}

automaton exptoam(std::string s) {

    auto nfa = Tompson(suf_exp(s));

    auto dfa = nfatodfa(nfa);

    return Hopcroft(dfa);
}

int main() {
    std::string s;
    std::cin >> s;
    auto dfa_min = exptoam(s);
   // dfa_min.show();    

    int t;
    std::cin >> t;
    std::cin.get();
    while (t--) {
        std::string s;
        std::getline(std::cin, s);
        std::cout << (dfa_min.match(s) ? "YES" : "NO") << "\n";
    }

    return 0;
} 