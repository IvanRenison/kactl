#include "../utilities/template.h"

#include "../../content/data-structures/FenwickTree2d.h"

int main() {
	fore(it,0,1000000) {
		FT2 ft(12);
		vector<tuple<ll, ll, ll>> upd;
		ll c = rand() % 20;
		fore(i,0,c) {
			upd.pb({rand() % 12, rand() % 12, rand() % 10 - 5});
		}

		vector<vi> grid(12, vi(12)), sumto(13, vi(13));
		for (auto &pa : upd)
			ft.fakeUpd(get<0>(pa), get<1>(pa));
		ft.init();
		for (auto &pa : upd) {
			grid[get<0>(pa)][get<1>(pa)] += get<2>(pa);
			ft.upd(get<0>(pa), get<1>(pa), get<2>(pa));
		}

		fore(i,0,13) {
			fore(j,0,13) {
				ll v = ft.query(i, j);
				if (i == 0 || j == 0) assert(v == 0);
				else {
					sumto[i][j] = grid[i-1][j-1] + sumto[i-1][j] + sumto[i][j-1] - sumto[i-1][j-1];
					assert(v == sumto[i][j]);
				}
			}
		}
	}
	cout << "Tests passed!" << endl;
}
