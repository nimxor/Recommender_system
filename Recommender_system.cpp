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
int active_user;
ll a[MAXM];
map<ll,ll> m;
vector<vector<double> >init,normalize_init,training;
vector<vector<int> >Genre;  

// For Generating the initial matrix
vector<double> Generate()
{
	int x=0;
	vector<double> v;
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

double compute_max(vector<vector<double> > v)
{
	double maxm=0;
	for(int i=0;i<v.size();i++){
		for(int j=0;j<v[i].size();j++){
			maxm=max(maxm,v[i][25]);
		}
	}
	return maxm;
}

vector<vector<double> > normalize_init_matrix(){
	vector<vector<double> > norm;
	double maxi = compute_max(init);
	vector<double> nn;
	for(int i=0;i<init.size();i++){
		for(int j=0;j<init[i].size();j++){
			if(j<25){
				nn.pb(init[i][j]/5);
			}
			else if(j==25){
				nn.pb(init[i][j]/maxi);
			}else{
				nn.pb(init[i][j]);
			}
		}
		norm.pb(nn);
		nn.clear();
	}
	return norm;
}

vector<int> Create_genre()
{
	vector<int> v;
	for(int i=0;i<7;i++){
		int x = rand()%2;
		v.pb(x);
	}
	return v;
}

void print(vector<vector<int> > v)
{
	for(int i=0;i<v.size();i++){
		for(int j=0;j<v[i].size();j++){
			cout<<v[i][j]<<" ";
		}
		cout<<endl;
	}
}

vector<vector<double> > Generate_Training_data(vector<double> in,int a_u)
{
	vector<vector<double> > set,tset;
	vector<double> v;
	vector<pair<int,double> > vp;
	for(int i=0;i<in.size();i++){
		if(i==in.size()-1){
			vp.pb(mp(i,in[i]));
		}
		else if(in[i]!=0){
			vp.pb(mp(i,in[i]));
		}
	}

	int sz = vp.size()-2;
	int training_size = sz*(2.0/3); 

	for(int i=0;i<init.size();i++){
		if(i==a_u)	continue;
		for(int j=0;j<vp.size();j++){
			int pos = vp[j].fi;
			v.pb(init[i][pos]);
		}
		set.pb(v);
		v.clear();
	}

	// cout<<training_size<<endl;

	for(int i=0;i<set.size();i++){
		for(int j=0;j<training_size;j++){
			v.pb(set[i][j]);
		}
		tset.pb(v);
		v.clear();
	}

	for(int i=0;i<set.size();i++){
		for(int j=set[i].size()-2;j<set[i].size();j++){
			tset[i].pb(set[i][j]);
		}
	}

	return tset;

}

void print_double(vector<vector<double> > v)
{
	for(int i=0;i<v.size();i++){
		for(int j=0;j<v[i].size();j++){
			cout<<v[i][j]<<" ";
		}
		cout<<endl;
	}
}

void solve()
{
	int i,j,k,n,m;
	srand(time(NULL));

	// Considering initial population size of 40 users
	for(i=0;i<N;i++){
		vector<double> user = Generate();
		init.pb(user);
	}

	// Representation of initial population
	printf("\n*******Initial Matrix*******\n");
	print_double(init);

	// Creating genre matrix for each movie
	for(i=0;i<M;i++)
		Genre.pb(Create_genre());

	//nORMALISEZ matrix for rating dataset
	printf("\n*******Normalize Matrix*******\n");
	normalize_init = normalize_init_matrix();
	print_double(normalize_init);

	printf("\n*******Genre Matrix*******\n");
	print(Genre);
	printf("\n");

	printf("\n*******Training Matrix*******\n");
	for(i=0;i<N;i++){
		active_user = i;
		training = Generate_Training_data(init[active_user],active_user);
		printf("\n*******Training Matrix for user %d *******\n\n",i+1);
		print_double(training);
	}
}

int main()
{
	int i,j,k,n,m;
	srand(time(NULL));
	solve();
	return 0;
}