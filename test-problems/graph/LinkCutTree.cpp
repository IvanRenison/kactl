// Problem: https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_path_sum
// Status:
// Submission:
#include <bits/stdc++.h>
using namespace std;

#define fst first
#define snd second
#define pb push_back
#define fore(i, a, b) for (ll i = a, gmat = b; i < gmat; i++)
#define ALL(x) begin(x), end(x)
#define SZ(x) (ll)(x).size()
#define mset(a, v) memset((a), (v), sizeof(a))
typedef long long ll;
typedef pair<ll, ll> ii;
typedef pair<ll, ll> pii;
typedef vector<ll> vi;

/// content/graph/LinkCutTree.h
const ll N_DEL = 0, N_VAL = 0; //delta, value
inline ll mOp(ll x, ll y){return x+y;}//modify
inline ll qOp(ll lval, ll rval){return lval + rval;}//query
inline ll dOnSeg(ll d, ll len){return d==N_DEL ? N_DEL : d*len;}
//mostly generic
inline ll joinD(ll d1, ll d2){
	if(d1==N_DEL)return d2;if(d2==N_DEL)return d1;return mOp(d1, d2);}
inline ll joinVD(ll v, ll d){return d==N_DEL ? v : mOp(v, d);}
struct Node_t{
	ll sz_, nVal, tVal, d;
	bool rev;
	Node_t *c[2], *p;
	Node_t(ll v) : sz_(1), nVal(v), tVal(v), d(N_DEL), rev(0), p(0){
		c[0]=c[1]=0;
	}
	bool isRoot(){return !p || (p->c[0] != this && p->c[1] != this);}
	void push(){
		if(rev){
			rev=0; swap(c[0], c[1]);
			fore(x,0,2)if(c[x])c[x]->rev^=1;
		}
		nVal=joinVD(nVal, d); tVal=joinVD(tVal, dOnSeg(d, sz_));
		fore(x,0,2)if(c[x])c[x]->d=joinD(c[x]->d, d);
		d=N_DEL;
	}
	void upd();
};
typedef Node_t* Node;
ll getSize(Node r){return r ? r->sz_ : 0;}
ll getPV(Node r){
	return r ? joinVD(r->tVal, dOnSeg(r->d,r->sz_)) : N_VAL;}
void Node_t::upd(){
	tVal = qOp(qOp(getPV(c[0]), joinVD(nVal, d)), getPV(c[1]));
	sz_ = 1 + getSize(c[0]) + getSize(c[1]);
}
void conn(Node c, Node p, ll il){if(c)c->p=p;if(il>=0)p->c[!il]=c;}
void rotate(Node x){
	Node p = x->p, g = p->p;
	bool gCh=p->isRoot(), isl = x==p->c[0];
	conn(x->c[isl],p,isl); conn(p,x,!isl);
	conn(x,g,gCh?-1:(p==g->c[0])); p->upd();
}
void spa(Node x){//splay
	while(!x->isRoot()){
		Node p = x->p, g = p->p;
		if(!p->isRoot())g->push();
		p->push(); x->push();
		if(!p->isRoot())rotate((x==p->c[0])==(p==g->c[0])? p : x);
		rotate(x);
	}
	x->push(); x->upd();
}
Node exv(Node x){//expose
	Node last=0;
	for(Node y=x; y; y=y->p)spa(y),y->c[0]=last,y->upd(),last=y;
	spa(x);
	return last;
}

struct LinkCutTree {
	ll n;
	vector<Node> nodes;

	LinkCutTree(ll n) : n(n), nodes(n) {
		fore(i, 0, n) {
			nodes[i] = new Node_t(N_VAL);
		}
	}
	LinkCutTree(vi& vals) : n(vals.size()), nodes(n) {
		fore(i, 0, n) {
			nodes[i] = new Node_t(vals[i]);
		}
	}

	void mkR(ll u){ // makeRoot
		exv(nodes[u]);
		nodes[u]->rev ^= 1;
	}
	Node getR(ll u){
		Node x = nodes[u];
		exv(x);
		while(x->c[1]) x = x->c[1];
		spa(x);
		return x;
	}
	Node lca(ll u, ll v) {
		exv(nodes[u]);
		return exv(nodes[v]);
	}
	bool connected(ll u, ll v) {
		exv(nodes[u]), exv(nodes[v]);
		return u == v ? true : nodes[u]->p != 0;
	}
	void link(ll u, ll v) {
		mkR(u);
		nodes[u]->p = nodes[v];
	}
	void cut(ll u, ll v) {
		mkR(u), exv(nodes[v]);
		nodes[v]->c[1]->p = 0, nodes[v]->c[1] = 0;
	}
	Node father(ll u){
		exv(nodes[u]);
		Node r = nodes[u]->c[1];
		if (!r) return 0;
		while(r->c[0]) r = r->c[0];
		return r;
	}
	void cut(ll u) { // cuts x from father keeping tree root
		exv(father(u));
		nodes[u]->p = 0;
	}
	ll query(ll u, ll v) {
		mkR(u), exv(nodes[v]);
		return getPV(nodes[v]);
	}
	void modify(ll u, ll v, ll d) {
		mkR(u), exv(nodes[v]);
		nodes[v]->d = joinD(nodes[v]->d, d);
	}
	ll depth(ll u) { // distance from x to its tree root
		exv(nodes[u]);
		return getSize(nodes[u]) - 1;
	}
};

void mkR(Node x){exv(x);x->rev^=1;}//makeRoot
//Node getR(Node x){exv(x);while(x->c[1])x=x->c[1];spa(x);return x;}
//Node lca(Node x, Node y){exv(x); return exv(y);}
//bool connected(Node x, Node y){exv(x);exv(y); return x==y?1:x->p!=0;}
//void link(Node x, Node y){mkR(x); x->p=y;}
//void cut(Node x, Node y){mkR(x); exv(y); y->c[1]->p=0; y->c[1]=0;}
// Node father(Node x){
// 	exv(x);
// 	Node r=x->c[1];
// 	if(!r)return 0;
// 	while(r->c[0])r=r->c[0];
// 	return r;
// }
// void cut(Node x){ // cuts x from father keeping tree root
// 	exv(father(x));x->p=0;}
//ll query(Node x, Node y){mkR(x); exv(y); return getPV(y);}
//void modify(Node x, Node y, ll d){mkR(x);exv(y);y->d=joinD(y->d,d);}
Node lift_rec(Node x, ll t){
	if(!x)return 0;
	if(t==getSize(x->c[0])){spa(x);return x;}
	if(t<getSize(x->c[0]))return lift_rec(x->c[0],t);
	return lift_rec(x->c[1],t-getSize(x->c[0])-1);
}
Node lift(Node x, ll t){ // t-th ancestor of x (lift(x,1) is x's father)
	exv(x);return lift_rec(x,t);}
// ll depth(Node x){ // distance from x to its tree root
// 	exv(x);return getSize(x)-1;}
///END content

int main() {
	cin.tie(0)->sync_with_stdio(0);

	ll N, Q;
	cin >> N >> Q;

	LinkCutTree lct(N);
	fore(i, 0, N) {
		ll a;
		cin >> a;
		lct.modify(i, i, a);
	}
	fore(i, 0, N - 1) {
		ll a, b;
		cin >> a >> b;
		lct.link(a, b);
	}

	while (Q--) {
		ll t;
		cin >> t;
		if (t == 0) {
			ll u, v, w, x;
			cin >> u >> v >> w >> x;
			lct.cut(u, v);
			lct.link(w, x);
		} else if (t == 1) {
			ll p, x;
			cin >> p >> x;
			lct.modify(p, p, x);
		} else {
			ll u, v;
			cin >> u >> v;
			ll val = lct.query(u, v);
			cout << val << '\n';
		}
	}
}
