#include<bits/stdc++.h>
using namespace std;
size_t A = 1101984, B = 1364128;
vector<long long> cache(32, -1);
void use(size_t adr, int &miss){
    size_t b = adr / 32;
    size_t s = b % 32;
    size_t tag = b / 32;
    if(cache[s] != tag) miss++;
    cache[s] = tag;
}

int calc(int n, int m){
    int miss = 0;
    for(int i = 0; i < 61; i += n){
        for(int j = 0; j < 67; j += m){
            for(int l = i; l < i + n && l < 61; l++){
                for(int r = j; r < j + m && r < 67; r++){
                    use(A + (l * 67 + r) * 4, miss);
                    use(B + (r * 61 + l) * 4, miss);
                }
            }
        }
    }
    return miss;
}



int main(){
    vector<tuple<int, int, int>> v;
    for(int i = 1; i <= 61; i++){
        for(int j = 1; j <= 67; j++){
            v.emplace_back(calc(i, j), i, j);
        }
    }

    sort(v.begin(), v.end());
    for(int i = 0; i < 10; i++){
        auto [cost, n, m] = v[i];
        cout << cost << " " <<n << ' ' << m << endl;
    }
}
