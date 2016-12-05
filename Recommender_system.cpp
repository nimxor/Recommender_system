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
int PC = 1.0;
int MP = 0.01;
int active_user;
ll a[MAXM];
map<ll,ll> m;
vector<vector<double> >init,normalize_init,training,testing;
vector<vector<int> >Genre;  
vector<vector<int> > matrix;
double w[13];
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

void print_double(vector<vector<double> > v)
{
	for(int i=0;i<v.size();i++){
		for(int j=0;j<v[i].size();j++){
			cout<<v[i][j]<<" ";
		}
		cout<<endl;
	}
}


pair<vector<vector<double> >,vector<vector<double> > > Generate_Training_data(vector<double> in,int a_u)
{
	vector<vector<double> > set,tset,testset;
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

	for(int i=0;i<normalize_init.size();i++){
		if(i==a_u)	continue;
		for(int j=0;j<vp.size();j++){
			int pos = vp[j].fi;
			v.pb(normalize_init[i][pos]);
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
		for(int j=training_size;j<set[i].size();j++){
			v.pb(set[i][j]);
		}
		testset.pb(v);
		v.clear();
	}

	for(int i=0;i<set.size();i++){
		for(int j=set[i].size()-2;j<set[i].size();j++){
			tset[i].pb(set[i][j]);
		}
	}

	pair<vector<vector<double> >,vector<vector<double> > > vv;
	vv = mp(tset,testset);

	return vv;

}

vector<vector<int> > Generate_Matrix()
{
	vector<vector<int> > v;	
	vector<int> vv;
	for(int i=0;i<100;i++){
		for(int j=0;j<40;j++){
			int x = rand()%2;
			vv.pb(x);
		}
		v.pb(vv);
		vv.clear();
	}
	return v;
}

double calculate_Rating_Distance(int active,int subactive)
{
	double dist = 0.0;
	// cout<<training[active].size()<<" "<<training[subactive].size()<<endl;
	for(int i=0;i<training[active].size()-2;i++){
		dist += abs(training[active][i]-training[subactive][i]);
	}		
	return dist;
}

double calculate_Age_Distance(int active,int subactive)
{
	double dist = 0;
	dist += abs(training[active][training[active].size()-2] - training[subactive][training[subactive].size()-2]);
	return dist;
}

double calculate_Gender_Distance(int active,int subactive)
{
	double dist = 0;
	dist += abs(training[active][training[active].size()-1] - training[subactive][training[subactive].size()-1]);
	return dist;
}

// Applying single point crossover
void applycrossover(int x,int y)
{
	int size = matrix[x].size();
	int ra = rand()%size;
	int pc = (rand()%99)+1/100;
	if(pc<=PC){
		for(int i=ra;i<size;i++){
			swap(matrix[x][i],matrix[y][i]);
		}
	}
}

// Applying bit flipping
void applymutation(int x)
{
	int size = matrix[x].size();
	int ra = rand()%size;
	int pr = (rand()%99)+1/100;
	if(pr<=MP){
		matrix[x][ra] = 1 - matrix[x][ra];
	}
}

int sz = matrix.size()*4.0/5;
void apply_singlepoint_crossover()
{
	// On 80% of the data
	for(int i=0;i<sz;i+=2){
		applycrossover(i,i+1);
	}
}

void apply_singlepoint_mutation()
{
	// On 20% of the data
	for(int i=sz;i<matrix.size();i++){
		applymutation(i);
	}
}

bool cmp(pair<int,vector<int> > l,pair<int,vector<int> > r){
	return l.fi>r.fi;
}

vector<pair<int,vector<int> > >vpp;

void sort_operation()
{
	int i,j,k;
	int sum=0;
	for(i=0;i<40;i++){
		int su=0;
		for(j=0;j<40;j+=4){
			su+=(matrix[i][j]*(1<<3));
			su+=(matrix[i][j+1]*(1<<2));
			su+=(matrix[i][j+2]*(1<<1));
			su+=(matrix[i][j]*(1<<0));
			sum+=su;
		}
		vpp.pb(mp(sum,matrix[i]));
	}
	sort(vpp.begin(),vpp.end(),cmp);
	vector<int> vv = vpp[0].se;
	double su=0.0;
	int pos=1;
	for(i=0;i<40;i+=4){
		w[pos]+=(vv[i]*(1<<3));
		w[pos]+=(vv[i+1]*(1<<2));
		w[pos]+=(vv[i+2]*(1<<1));
		w[pos]+=(vv[i+3]*(1<<0));
		su+=w[pos];
		pos+=1;
	}
	for(i=1;i<11;i++){
		w[i]/=su;
	}
}

void apply_operations()
{
	apply_singlepoint_crossover();
	apply_singlepoint_mutation();
	sort_operation();
}

// For testing data
double for_testing_data()
{
	int i,j,k,kk;
	double rating_dist = 0,Gender_dist = 0,Age_dist = 0;
	vector<pair<double,int> > similarity;

	matrix = Generate_Matrix();
	
	// Applying mutaion and crossover on wi matrix
	for(j=0;j<30;j++)
		apply_operations();

	for(j=0;j<N-1;j++){
		if(j==active_user)	continue;
		rating_dist = calculate_Rating_Distance(active_user,j);
		Gender_dist = calculate_Gender_Distance(active_user,j);
		Age_dist = calculate_Age_Distance(active_user,j); 
		similarity.pb(mp((rating_dist*w[1]+Gender_dist*w[2]+Age_dist*w[3]),j));// Rest all are 0;
	}

	sort(similarity.begin(),similarity.end());
	reverse(similarity.begin(),similarity.end());

	// cout<<similarity.size()<<endl;

	vector<int> select_user;
	for(j=0;j<10;j++){
		select_user.pb(similarity[j].se);
	}

	printf("\nNabhourhood set of 10 individuals\n\n");
	for(j=0;j<10;j++){
		cout<<select_user[j]<<" ";
	}
	cout<<endl;

	vector<double> movies_predict;
	double suu=0.0,su=0.0,dis=0.0;
	// Now predicting the values for all the movies
	for(j=0;j<25;j++){
		suu = dis = su = 0.0;
		for(k=0;k<select_user.size();k++){
			for(kk=0;kk<similarity.size();kk++){
				if(kk==select_user[k])	break;
			}
			su+=((training[j][kk])*(similarity[kk].fi));
			dis+=(similarity[kk].fi);
		}
		suu=su/dis;
		movies_predict.pb(suu);
	}

	printf("\nPredicted value of each movie rating\n");
	for(j=0;j<movies_predict.size();j++){
		cout<<movies_predict[j]<<" ";
	}
	cout<<endl;

	printf("\nOverall value\n\n");
	double overallvalue = 1.0/training.size();

	su = 0.0;
	for(j=0;j<movies_predict.size();j++){
		su += abs(movies_predict[j]-matrix[i][j]);
	}
	return su;
}

void solve()
{
	int i,j,k,n,m,kk;
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
	for(i=0;i<N-1;i++){
		active_user = i;
		training = Generate_Training_data(init[active_user],active_user).fi;
		testing = Generate_Training_data(init[active_user],active_user).se;
		printf("\n*******Training Matrix for user %d *******\n\n",i+1);
		print_double(training);
		printf("\n*******Testing Matrix for user %d *******\n\n",i+1);
		print_double(testing);

		double rating_dist = 0,Gender_dist = 0,Age_dist = 0;
		vector<pair<double,int> > similarity;

		matrix = Generate_Matrix();
		
		// Applying mutaion and crossover on wi matrix
		for(j=0;j<30;j++)
			apply_operations();

		for(j=0;j<N-1;j++){
			if(j==active_user)	continue;
			rating_dist = calculate_Rating_Distance(active_user,j);
			Gender_dist = calculate_Gender_Distance(active_user,j);
			Age_dist = calculate_Age_Distance(active_user,j); 
			similarity.pb(mp((rating_dist*w[1]+Gender_dist*w[2]+Age_dist*w[3]),j));// Rest all are 0;
		}

		sort(similarity.begin(),similarity.end());
		reverse(similarity.begin(),similarity.end());

		// cout<<similarity.size()<<endl;

		vector<int> select_user;
		for(j=0;j<10;j++){
			select_user.pb(similarity[j].se);
		}

		printf("\nNabhourhood set of 10 individuals\n\n");
		for(j=0;j<10;j++){
			cout<<select_user[j]<<" ";
		}
		cout<<endl;

		vector<double> movies_predict;
		double suu=0.0,su=0.0,dis=0.0;
		// Now predicting the values for all the movies
		for(j=0;j<25;j++){
			suu = dis = su = 0.0;
			for(k=0;k<select_user.size();k++){
				for(kk=0;kk<similarity.size();kk++){
					if(kk==select_user[k])	break;
				}
				su+=((training[j][kk])*(similarity[kk].fi));
				dis+=(similarity[kk].fi);
			}
			suu=su/dis;
			movies_predict.pb(suu);
		}

		printf("\nPredicted value of each movie rating\n");
		for(j=0;j<movies_predict.size();j++){
			cout<<movies_predict[j]<<" ";
		}
		cout<<endl;

		printf("\nTraining value\n\n");
		double overallvalue = 1.0/training.size();

		double train = 0.0;
		for(j=0;j<movies_predict.size();j++){
			train += abs(movies_predict[j]-matrix[i][j]);
		}

		cout<<train<<endl;

		double test = for_testing_data();

		printf("\nTest value\n\n");
		cout<<test<<endl;

		printf("\ndifference in result!!\n");

		cout<<(abs(train-test)/test)*100<<endl;

		training.clear();
		testing.clear();
		matrix.clear();
		vpp.clear();
		similarity.clear();
		movies_predict.clear();
		select_user.clear();
	}
}

int main()
{
	int i,j,k,n,m;
	srand(time(NULL));
	solve();
	return 0;
}