/**
 * Author: Stanford
 * Source: Stanford Notebook
 * License: MIT
 * Description: Solves a general linear maximization problem: maximize $c^T x$ subject to $Ax \le b$, $x \ge 0$.
 * Returns -inf if there is no solution, inf if there are arbitrarily good solutions, or the maximum value of $c^T x$ otherwise.
 * The input vector is set to an optimal $x$ (or in the unbounded case, an arbitrary solution fulfilling the constraints).
 * Numerical stability is not guaranteed. For better performance, define variables such that $x = 0$ is viable.
 * Usage:
 * vvd A = {{1,-1}, {-1,1}, {-1,-2}};
 * vd b = {1,1,-4}, c = {-1,-1}, x;
 * T val = LPSolver(A, b, c).solve(x);
 * Time: O(NM * \#pivots), where a pivot may be e.g. an edge relaxation. O(2^n) in the general case.
 * Status: seems to work?
 */
#pragma once

const double eps = 1e-8, inf = 1/.0;
typedef vector<double> vd;

struct LPSolver {
	vi X,Y;
	vector<vd> A;
	vd b,c;
	double z;
	ll n,m;
	LPSolver(vector<vd> A, vd b, vd c) : // maximize c^T x s.t. Ax<=b, x>=0
			A(A), b(b), c(c), n(b.size()), m(c.size()), z(0) {}
	void pivot(ll x,ll y){
		swap(X[y],Y[x]);
		b[x]/=A[x][y];
		fore(i,0,m)if(i!=y)A[x][i]/=A[x][y];
		A[x][y]=1/A[x][y];
		fore(i,0,n)if(i!=x&&abs(A[i][y])>eps){
			b[i]-=A[i][y]*b[x];
			fore(j,0,m)if(j!=y)A[i][j]-=A[i][y]*A[x][j];
			A[i][y]=-A[i][y]*A[x][y];
		}
		z+=c[y]*b[x];
		fore(i,0,m)if(i!=y)c[i]-=c[y]*A[x][i];
		c[y]=-c[y]*A[x][y];
	}
	double solve(vd& r){
		X=vi(m);Y=vi(n);
		fore(i,0,m)X[i]=i;
		fore(i,0,n)Y[i]=i+m;
		while(1){
			ll x=-1,y=-1;
			double mn=-eps;
			fore(i,0,n)if(b[i]<mn)mn=b[i],x=i;
			if(x<0)break;
			fore(i,0,m)if(A[x][i]<-eps){y=i;break;}
			if (y < 0) return -inf; // no solution to Ax<=b
			pivot(x,y);
		}
		while(1){
			double mx=eps;
			ll x=-1,y=-1;
			fore(i,0,m)if(c[i]>mx)mx=c[i],y=i;
			if(y<0)break;
			double mn=1e200;
			fore(i,0,n)if(A[i][y]>eps&&b[i]/A[i][y]<mn)mn=b[i]/A[i][y],x=i;
			if (x < 0) return inf; // c^T x is unbounded
			pivot(x,y);
		}
		r = vd(m);
		fore(i,0,n)if(Y[i]<m)r[Y[i]]=b[i];
		return z;
	}
};
