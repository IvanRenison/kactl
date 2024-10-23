/**
 * Author: Iván Renison
 * Date: 2024-10-23
 * License: CC0
 * Source: notebook el vasito
 * Description:
 * Time:
 * Status: stress-tested
 */
#pragma once

struct AhoCorasick {
	enum {alpha = 26, first = 'A'}; // change this!
	struct vertex {
		vi next = vi(alpha, -1), go = vi(alpha, -1);
		ll p, link, pch;
		vi leaf;
		vertex(ll p = -1, ll pch = -1) : p(p), link(-1), pch(pch) {}
	};
	vector<vertex> t;
	AhoCorasick() : t(1) {}
	ll add_string(string& s, ll id) {
		ll v = 0;
		for (char c : s) {
			if (t[v].next[c - first] == -1) {
				t[v].next[c - first] = SZ(t);
				t.pb(vertex(v, c - first));
			}
			v = t[v].next[c - first];
		}
		t[v].leaf.pb(id);
		return v;
	}
	ll get_link(ll v) {
		if (t[v].link < 0) {
			if (!v || !t[v].p) t[v].link = 0;
			else t[v].link = go(get_link(t[v].p), t[v].pch + first);
		}
		return t[v].link;
	}
	ll go(ll v, char c) {
		if (t[v].go[c - first] == -1) {
			if (t[v].next[c - first] != -1) t[v].go[c - first] = t[v].next[c - first];
			else t[v].go[c - first] = v == 0 ? 0 : go(get_link(v), c);
		}
		return t[v].go[c - first];
	}
};