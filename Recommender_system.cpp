#include<bits/stdc++.h>
#include<time.h>
using namespace std;

#define fi                    first
#define se                    second
#define ll                    long long
#define all(a)                a.begin(),a.end()
#define rall(a)               a.rbegin(),a.rend()  
#define mod                  1000000007
#define mp                    make_pair
#define pb                    push_back
#define mset(m,v)             memset(m,v,sizeof(m))

const int MAXM = 1e5+3;
const int N = 4e1;
const int M = 25; // For 25 movies
ll a[MAXM];
map<ll,ll> m;
vector<vector<int> >init;

// For Generating the initial matrix
vector<int> Generate()
{
	int x=0;
	vector<int> v;
	for(int i=1;i<=M;i++){
		x = rand()%6; // Rating by user for each movie
		v.pb(x);	
	}

	x = rand()%100+1; // Considering maximum age to be 100
	v.pb(x);

	x = rand()%2;
	v.pb(x);

	return v;

}

int main()
{
	int i,j,k,n,m;
	srand(time(NULL));

	// Considering initial population size of 40 users
	for(i=0;i<N;i++){
		vector<int> user = Generate();
		init.pb(user);
	}

	for(i=0;i<init.size();i++){
		for(j=0;j<init[i].size();j++){
			cout<<init[i][j]<<" ";
		}
		cout<<endl;
	}
	return 0;
}