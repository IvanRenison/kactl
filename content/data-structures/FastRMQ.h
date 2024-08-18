/**
 * Author: Iván Renison
 * Date: 2024-08-18
 * License: CC0
 * Source: notebook el vasito
 * Description: Like RMQ but with construction O(|V|).
 * Time: $O(|V| + Q)$
 * Status:
 */
#pragma once

typedef uint64_t ull;

template<class T>
struct RMQ {
	ll n; T inf=1e9;	//change sign of inf for maximum
	vector<ull> mk; vector<T> bk,v;
	T op(T a, T b){return min(a,b);}	//change for maximum
	ll f(ll x){return x>>5;}
	RMQ(vector<T> &V):n(SZ(V)),mk(n),bk(n,inf),v(V){
		ull lst=0;
		for(ll i=0;i<SZ(v);i++,lst<<=1){
			bk[f(i)]=op(bk[f(i)],v[i]);
			while(lst&&v[i-__builtin_ctzll(lst)]>v[i]) lst^=lst&-lst;		//MIN
			//while(lst&&v[i-__builtin_ctzll(lst)]<v[i]) lst^=lst&-lst;	//MAX
			mk[i]=++lst;
		}
		for(ll k=1,top=f(n);(1<<k)<=top;k++)fore(i,0,top)if(i+(1<<k)<=top)
			bk[top*k+i]=op(bk[top*(k-1)+i], bk[top*(k-1)+i+(1<<k-1)]);
	}
	T get(ll st, ll en){
		return v[en-63+__builtin_clzll(mk[en]&((1ll<<en-st+1)-1))];
	}
	T query(ll s, ll e){	//[s,e]
		ll b1=f(s),b2=f(e),top=f(n);
		if(b1==b2) return get(s,e);
		T ans=op(get(s,(b1+1)*32-1), get(b2*32,e)); s=(b1+1)*32; e=b2*32-1;
		if(s<=e){
			ll k=63-__builtin_clzll(f(e-s+1));
			ans=op(ans,op(bk[top*k+f(s)],bk[top*k+f(e)-(1<<k)+1]));
		}
		return ans;
	}
};
