// #define _GLIBCXX_DEBUG
#include <bits/stdc++.h>
using namespace std;
#include <atcoder/modint>

// Faster input/output
struct Init{Init(){ios::sync_with_stdio(0);cin.tie(0);cout<<fixed<<setprecision(15);}}_atcoder_init;

// Simplify type notation
using ll = long long; using ull = unsigned long long;
#define int ll
using vi = vector<int>; using vvi = vector<vi>; using vvvi = vector<vvi>;
using pii = pair<int,int>;
using mint = atcoder::modint998244353;

// Macro
#define OVERLOAD_REP(_1, _2, _3, name, ...) name
#define REP1(i, n) for (auto i = std::decay_t<decltype(n)>{}; (i) != (n); ++(i))
#define REP2(i, l, r) for (auto i = (l); (i) != (r); ++(i))
#define rep(...) OVERLOAD_REP(__VA_ARGS__, REP2, REP1)(__VA_ARGS__)

#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()
#define el '\n'
#define Yes cout << "Yes\n"
#define No cout << "No\n"
#define debug(x) cerr << #x << " = " << x << el
#define sz(c) ((ll)(c).size())

// Constants
const int inf = 1001001001001001001LL;
const string ABC = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const string abc = "abcdefghijklmnopqrstuvwxyz";
const int mod = 998244353;
const int dh[] = {-1,1,0,0};
const int dw[] = {0,0,-1,1};

// In-line chmin/chmax
template<typename T> inline bool chmax(T &a, T b) { return ((a < b) ? (a = b, true) : (false)); }
template<typename T> inline bool chmin(T &a, T b) { return ((a > b) ? (a = b, true) : (false)); }

// Concise notation of inputs/outputs
template<typename T1,typename T2>ostream&operator<<(ostream&os,pair<T1,T2>p){os<<p.first<<" "<<p.second;return os;}
template<typename T1,typename T2>istream&operator>>(istream&is,pair<T1,T2>&p){is>>p.first>>p.second;return is;}
template<typename T>ostream&operator<<(ostream&os,vector<T>v){rep(i,sz(v))os<<v[i]<<(i+1!=sz(v)?" ":"");return os;}
template<typename T>istream&operator>>(istream&is,vector<T>&v){for(T&in:v)is>>in;return is;}
ostream&operator<<(ostream&os,mint a){os<<a.val();return os;}
istream&operator>>(istream&is,mint&a){int x;is>>x;a=mint(x);return is;}



////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////



signed main(void){
}
