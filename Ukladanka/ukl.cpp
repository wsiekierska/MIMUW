#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

using namespace std;


struct Block {
    vector<pair<int, int>> area;
    vector<pair<int, int>> area2; //trzymam wektory wspolrzednych klocka po obrotach o 90 st.
    vector<pair<int, int>> area3;
    vector<pair<int, int>> area4;
    char letter;

    //przesuwam wspołrzędne klocka do "lewego górnego" rogu,
    //żeby łatwiej było je umieszczać na planszy
    void level(vector<pair<int, int>> &points) {
        int x = INT_MAX;
        int y = INT_MAX;
        for (auto [a, b]: points) {
            x = min(x, a);
            y = min(y, b);
        }
        for (auto &[a, b]: points) {
            a -= x;
            b -= y;
        }
    }

    Block(vector<vector<char>> elem) {
        for (int i = 0; i < (int) elem.size(); i++) {
            for (int j = 0; j < (int) elem[i].size(); j++) {
                if (elem[i][j] != '.') {
                    letter = elem[i][j];
                    area.emplace_back(pair(i, j));
                    area2.emplace_back(pair(j, -i));
                    area3.emplace_back(pair(-i, -j));
                    area4.emplace_back(pair(-j, i));
                }
            }
        }
        level(area);
        level(area2);
        level(area3);
        level(area4);
    }
};


struct Board {
    vector<Block> blocks;
    int n, m, k;
    vector<vector<char>> state;
    bool possible = false;

    Board(int a, int b) {
        state.resize(a, vector<char>(b, '.'));
        n = a;
        m = b;
    }

    void add(Block b) {
        blocks.push_back(b);
    }

    //sortuje wektor klocków po najwiekszym rozmiarze
    void sorting() {
        k = int(blocks.size());
        sort(blocks.begin(), blocks.end(), [](Block &a, Block &b) {
            return (a.area).size() > (b.area).size();
        });
    }


    //sprawdzam, czy na danym polu moge położyć klocek
    bool can_put(int x, int y, vector<pair<int, int>> &points) {
        for (auto [a, b]: points) {
            if (a + x >= n || b + y >= m || state[a + x][b + y] != '.') {
                return false;
            }
        }
        return true;
    }

    //umieszczam klocek na polu (x,y)
    void put(int x, int y, vector<pair<int, int>> &points, char letter) {
        for (auto [a, b]: points) {
            state[a + x][b + y] = letter;
        }
    }

    //usuwam klocek z pola(x, y)
    void remove(int x, int y, vector<pair<int, int>> &points) {
        for (auto [a, b]: points) {
            state[a + x][b + y] = '.';
        }
    }

    //sprawdzam,
    void check(vector<int> &IDs, int i) {
        //patrzę czy przejrzałam wszystkie klocki z IDs
        //(których łączny rozmiar sumuje się do rozmiaru planszy
        if (i == int(IDs.size())) {
            possible = true;
            return;
        }

        for (int j = 0; j < n; j++) {
            for (int l = 0; l < m; l++) {
                //sprawdzam czy mogę w danym polu położyc i-ty klocek, jeżeli tak,
                //to kładę go, wywołuję funkcję check dla pozostałych klocków
                //na koniec zdejmuję klocek dla dalszych rozważań
                if (can_put(j, l, blocks[IDs[i]].area)) {
                    put(j, l, blocks[IDs[i]].area, blocks[IDs[i]].letter);
                    check(IDs, i + 1);
                    if (possible)return;
                    remove(j, l, blocks[IDs[i]].area);
                }
                //to samo robię dla tego i-tego klocka obróconego o 90, 180 i 270 st.
                if (can_put(j, l, blocks[IDs[i]].area2)) {
                    put(j, l, blocks[IDs[i]].area2, blocks[IDs[i]].letter);
                    check(IDs, i + 1);
                    if (possible)return;
                    remove(j, l, blocks[IDs[i]].area2);
                }

                if (can_put(j, l, blocks[IDs[i]].area3)) {
                    put(j, l, blocks[IDs[i]].area3, blocks[IDs[i]].letter);
                    check(IDs, i + 1);
                    if (possible)return;
                    remove(j, l, blocks[IDs[i]].area3);
                }

                if (can_put(j, l, blocks[IDs[i]].area4)) {
                    put(j, l, blocks[IDs[i]].area4, blocks[IDs[i]].letter);
                    check(IDs, i + 1);
                    if (possible)return;
                    remove(j, l, blocks[IDs[i]].area4);
                }
            }
        }
    }


    void solve(int i, vector<bool> &v) {
        int s = 0;
        //jeśli mam kombinację dla wszystkich klocków
        if (i == k) {
            for (int l = 0; l < k; l++) {
                //jeśli l-ty klocek znajduje sie w kombinacji
                if (v[l]) {
                    s += int(blocks[l].area.size()); //dodaje jego rozmiar do aktualnej sumy
                }
            }
            if (s == n * m) { //jeśli suma pól ze wszytskich klocków sumuje się do rozmiaru planszy
                vector<int> IDs; //tworzę wektor mówiący, ktore klocki dają dobrą sumę pól
                for (int l = 0; l < k; l++) {
                    if (v[l])IDs.push_back(l);
                }
                check(IDs, 0);
            }
            return;
        }
        for (int j = 0; j <= 1; j++) { //tworzę kolejne kombinacje
            v.push_back(j);
            solve(i + 1, v);
            v.pop_back();
        }
    }

    void result() {
        sorting();
        vector<bool> v;
        solve(0, v);
        if (state[0][0] == '.') {
            cout << "NIE" << endl;
            return;
        }
        cout << "TAK" << endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                cout << state[i][j];
            }
            cout << endl;
        }
        return;
    }
};


int main() {
    int n, m, k;
    cin >> n >> m >> k;
    Board board(n, m);
    for (int i = 0; i < k; i++) {
        vector<vector<char>> blocks(n, vector<char>(m));
        for (int j = 0; j < n; j++) {
            for (int l = 0; l < m; l++) {
                cin >> blocks[j][l];
            }
        }
        board.add(Block(blocks));
    }
    board.result();
    return 0;
}
