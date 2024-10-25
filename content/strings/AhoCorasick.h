/**
 * Author: Iván Renison
 * Date: 2024-10-23
 * License: CC0
 * Source: notebook el vasito
 * Description: AhoCorasick automaton. It consists of a trie in with each node except the root has
 * a link to the longest suffix that is also a node in the trie. That link is used for
 * transitions that are not defined in the trie.
 * Works with vectors, but for lower case latin strings you can to convert it to
 * \texttt{vi} and subtract 'a' for each character.
 * Use the function \texttt{go} to get the next state of the automaton given the current state.
 * Use \texttt{t[v].leaf} to know witch strings end in the state \texttt{v}.
 * Time: O(N) for constructing where N is the sum of lengths of the words and O(1) for each
 * transition query.
 * Status: Tested on codeforces
 */
#pragma once

struct AhoCorasick {
	static const ll alpha = 26; // Size of the alphabet
	struct Node {
		array<ll, alpha> next, go; vi leaf; ll p, link, pch;
		Node(ll p = -1, ll pch = -1) : p(p), link(-1), pch(pch) {
			next.fill(-1), go = next;
		}
	};
	vector<Node> t;
	AhoCorasick(vector<vi>& words) : t(1) {
		fore(i, 0, SZ(words)) {
			ll v = 0;
			for (ll c : words[i]) {
				if (t[v].next[c]<0) t[v].next[c]=SZ(t),t.pb(Node(v,c));
				v = t[v].next[c];
			}
			t[v].leaf.pb(i);
		}
	}
	ll getLink(ll v) {
		if (t[v].link < 0) t[v].link = v && t[v].p ?
			go(getLink(t[v].p), t[v].pch) : 0;
		return t[v].link;
	}
	ll go(ll v, ll c) {
		if (t[v].go[c] < 0) t[v].go[c] = t[v].next[c] >= 0 ?
			t[v].next[c] : v ? go(getLink(v),c) : 0;
		return t[v].go[c];
	}
};
