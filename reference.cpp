#include <bits/stdc++.h>

/*--------removing warning flags----------------*/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wc++11-extensions"
#pragma GCC diagnostic ignored "-Wunused-const-variable"

using namespace std;
#define deb(x) cout << #x << "=" << x << endl
#define debb(x, y) cout << #x << "=" << x << "," << #y << "=" << y << endl
#define print(x) cout<<x<<endl;
#define printt(x, y) cout<<x<<" "<<y<<endl;

#define int long long
#define int long long
#define ii pair<int, int>

#define F first
#define S second
#define pb push_back
#define vi vector<int>
#define vii vector<ii>
#define vvi vector<vi>
#define vvii vector<vii>
#define MP make_pair
int max(int a, int b) { return (a > b) ? a : b; }
int min(int a, int b) { return (a < b) ? a : b; }
double max(double a, double b) { return (a > b) ? a : b; }
double min(double a, double b) { return (a < b) ? a : b; }
void inpa(vi& a) { for (int i = 0;i < a.size();i++) cin >> a[i]; }
int Ceil(double d) { return ((int)d == d) ? d : d + 1; }
int gcd(int a, int b) { if (b > a) { return gcd(b, a); } if (b == 0) { return a; } return gcd(b, a % b); }
int expo(int a, int b, int mod) { int res = 1; while (b > 0) { if (b & 1)res = (res * a) % mod; a = (a * a) % mod; b = b >> 1; } return res; }
void swap(int& x, int& y) { int temp = x; x = y; y = temp; }
int combination(int n, int r, int m, int* fact, int* ifact) { int val1 = fact[n]; int val2 = ifact[n - r]; int val3 = ifact[r]; return (((val1 * val2) % m) * val3) % m; }
int mod_add(int a, int b, int m) { a = a % m; b = b % m; return (((a + b) % m) + m) % m; }
int mod_mul(int a, int b, int m) { a = a % m; b = b % m; return (((a * b) % m) + m) % m; }
int mod_sub(int a, int b, int m) { a = a % m; b = b % m; return (((a - b) % m) + m) % m; }

template<class T, class V> ostream& operator<< (ostream& out, const pair<T, V>& p) { out << p.first << ", " << p.second; return out; }
template<class T> ostream& operator<< (ostream& out, const vector<T>& c) { for (int i : c) out << i << ' '; return out; }
template<class T, class V> ostream& operator<< (ostream& out, const map<T, V> m) { out << "{"; for (auto p : m) out << p.first << ": " << p.second << ", "; out << "}"; return out; }
template<class T, class V> ostream& operator<< (ostream& out, const unordered_map<T, V> m) { out << "{"; for (auto p : m) out << p.first << ": " << p.second << ", "; out << "}"; return out; }
template<class T> ostream& operator<< (ostream& out, const set<T> m) { out << "{"; for (T p : m) out << p << ", "; out << "}"; return out; }

void solve();
int32_t main() {
    #ifndef ONLINE_JUDGE
        freopen("input.txt", "r", stdin);
        freopen("output.txt", "w", stdout);
    #endif
        ios_base::sync_with_stdio(false);
        cin.tie(NULL);
        cout.tie(NULL);
        int t = 1;
        cin >> t;
        while (t--)
        {
            solve();
        }
        return 0;
    
}
void solve(){
}
#pragma GCC diagnostic pop