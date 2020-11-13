#include<iostream>
#include<vector>
#include<stdlib.h>
#include"solver.h"
using namespace std;

#define DEBUG

class CLAUSE{
	public:
		CLAUSE(int id){
			_id = id;
		}

		void push_li(int i) { _li.push_back(i);}
		int &operator[](int i){ return _li[i];}	
		const int &operator[](int i) const{ return _li[i];}	
		int size(void) const { return _li.size();}	
		int getid(void) {return _id;}
	private:
		vector<int> _li;
		int _id;
};

void ReadFromInput(vector< vector<int>>&  truth,int& clause_num,int& literal_num,vector<bool>& uni_or_exi){
	char c;
	string s;
	
	cin >> clause_num;
	cin >> literal_num;

	for(int i = 0;i < literal_num;i++){
		bool kk;
		cin >> kk;
		uni_or_exi.push_back(kk);
	}

	for(int i = 0;i < clause_num;i++){
		vector<int> cl;
		while(true){
			int literal;
			cin >> literal;
			if(literal == 0) break;
			cl.push_back(literal);
		}
		truth.push_back(cl);
	}
}


void initializeCNF(vector< vector<int>>&  truth,vector<CLAUSE*>& phi){
	int clause_num = truth.size();
	for(int i = 0;i < clause_num;i++){
		CLAUSE* obj = new CLAUSE(i+1);
		for(int j = 0;j < truth[i].size();j++){
			int num = truth[i][j];
			if(num > 0) num += clause_num;
			else num -= clause_num;
			obj->push_li(num);
		}	
		phi.push_back(obj);
	}
	

}


void printCNF(vector<CLAUSE*>& cnf){

	cout << "--------------------" << endl;
	cout << "phi" << endl;
	for(int i = 0;i < cnf.size();i++){
		for(int j = 0;j < cnf[i]->size();j++){
			cout << (*cnf[i])[j] << " ";
		}
		
		cout << "C" << cnf[i]->getid() << "  ";
		cout << endl;
	}	
}


void initialize_constrain(vector< vector<int> >& constrain,vector<CLAUSE*>& phi,bool* uni_exi){

	//global_clause: a least one clause must be choosed
	vector<int> global_clause;


	for(int i = 0;i < phi.size();i++){
		for(int j = 0;j < phi[i]->size();j++){
			int li = (*phi[i])[j];
			if(uni_exi[(abs(li))] == 1){
				vector<int> new_clause;
				new_clause.push_back(-phi[i]->getid());
				new_clause.push_back(-li);
				new_clause.push_back(0);
				constrain.push_back(new_clause);
			}
		}
		global_clause.push_back(phi[i]->getid());
	}
	global_clause.push_back(0);
	constrain.push_back(global_clause);
}

vector<int> solve_sat(vector< vector<int> >& in,int literal_num){
	SATSolverDPLL solver;
	solver.initialize(in,literal_num);	
	solver.solve();
	return solver.getu();

}

int main(void){

	//clause number
	int clause_num = 0;
	int literal_num = 0;

	//univeral or existential literal table
	vector<bool> uni_or_exi;

	vector< vector<int>> truth;	
	ReadFromInput(truth,clause_num,literal_num,uni_or_exi);


	//if arr[i] = 1, the literal i is a universal literal	
	bool* uni_or_exi_modify = new bool(literal_num+clause_num+1);

	#ifdef DEBUG
	cout << "--------------------------------" << endl;
	cout << "univeral or existential" << endl;
	#endif	

	for(int i = 0;i < literal_num+clause_num+ 1;i++){
		if(i <= clause_num){
			uni_or_exi_modify[i] = 0; 	
		}
		else{		
			uni_or_exi_modify[i] = uni_or_exi[i-clause_num-1]; 	
		}
		
		#ifdef DEBUG	
		cout << uni_or_exi_modify[i] << " ";
		#endif
	}
	#ifdef DEBUG
	cout << endl;
	#endif
	
	
	//initial phi, where phi is the global CNF that has to be calculated QBF result

	vector<CLAUSE*> phi;
	initializeCNF(truth,phi);

	#ifdef DEBUG
	printCNF(phi);
	#endif

	vector< vector<int> > constrain;

	// turn phi into constrain
	initialize_constrain(constrain,phi,uni_or_exi_modify);

	int round = 1;	

	while(true){
		#ifdef DEBUG
		cout << "******************************" << endl;
		cout << "Round " << round << endl;
		cout << "******************************" << endl;
		#endif

		round++;

		//solve tao that satisfy constrain
		vector<int> tao;
		tao = solve_sat(constrain,clause_num+literal_num);

		#ifdef DEBUG
		cout << "-------------------" << endl;
		cout << "tao" << endl;
		for(int i = 0;i < tao.size();i++){
			cout << tao[i] << " ";
		} 
		cout << endl;
		#endif
	
	        if(tao[0] == 0){
			cout << "******************************" << endl;
			cout << "Final result" << endl;
			cout << "******************************" << endl;
			cout << "QBF Result: true"  << endl;
			return 0;
		}

		vector<bool> S;
		for(int i = 0;i < clause_num;i++){
			bool selected = 1;
			for(int j = 0;j < phi[i]->size();j++){
				int li = (*phi[i])[j];
				if(uni_or_exi_modify[abs(li)] == 1 && li*tao[abs(li)-1] > 0)
					selected = false;
			}
			S.push_back(selected);
		}

		#ifdef DEBUG
		cout << "-----------------------" << endl;
		cout << "S" << endl;
		for(int i = 0;i < S.size();i++) cout << S[i] << " ";
		cout << endl;
		#endif

		//find phi_prime(existential part)
		vector< vector<int>> phi_prime;
		for(int i = 0;i < clause_num;i++){
			vector<int> cl;
			int id = phi[i]->getid();
			if(S[id-1] == 1){
				for(int j = 0;j < phi[i]->size();j++){
					int li = (*phi[i])[j];
					if(uni_or_exi_modify[abs(li)] == 0){
						cl.push_back(li);
					}
				}
				cl.push_back(0);
			}
			if(cl.size() > 1)
				phi_prime.push_back(cl);
						
		}		
		
		#ifdef DEBUG			 
		cout << "-----------------" << endl;
		cout << "phi_prime" << endl;
		for(int i = 0;i < phi_prime.size();i++){
			for(int j = 0;j < phi_prime[i].size();j++){
				cout << phi_prime[i][j] << " ";
			}
			cout << endl;
		}
		#endif

		//solve u that satisfy phi_prime
		vector<int> u;
		u = solve_sat(phi_prime,clause_num+literal_num);

		#ifdef DEBUG
		cout << "-------------------" << endl;
		cout << "u" << endl;
		for(int i = 0;i < u.size();i++){
			cout << u[i] << " ";
		} 
		cout << endl;
		#endif

		if(u[0] == 0){
			cout << "******************************" << endl;
			cout << "Final result" << endl;
			cout << "******************************" << endl;
			cout << "QBF Result: false"  << endl;
			return 0;		
		}


		// find S_prime
		bool S_prime[clause_num+1] = {0};
		
		for(int i = 0;i < clause_num;i++){
			bool choose = false;
			int id = phi[i]->getid();
			for(int j = 0;j < phi[i]->size();j++){
				int li = (*phi[i])[j];
				if(li* u[abs(li)-1] > 0) choose = true; 	
			}
			if(choose){
				S_prime[id] = 1;			
			}
		}

		#ifdef DEBUG		
		cout << "---------------------" << endl;
		cout << "S_prime" << endl;
		for(int i = 0;i < clause_num+1;i++) cout << S_prime[i] << " ";
		cout << endl;
		#endif 

		// find the new clause that has to be added to constrain
		vector<int> new_cl;
		for(int i = 0;i < clause_num;i++){
			int id = phi[i]->getid();
			if(S_prime[id] == 0){
				new_cl.push_back(id);
			}
		}
		if(new_cl.size() == 0){
			for(int i = 1;i < clause_num+literal_num+1;i++){
				if(i <= clause_num){
					new_cl.push_back(-tao[i-1]);
				}	
				else{
					if(uni_or_exi_modify[i] == 1){
						new_cl.push_back(-tao[i-1]);
					}
				}
			}	
		}

		new_cl.push_back(0);

		#ifdef DEBUG
		cout << "---------------" << endl;
		cout << "new clause" << endl;
		for(int i = 0;i < new_cl.size();i++) cout << new_cl[i] << " ";
		cout << endl;
		#endif
			
		constrain.push_back(new_cl);	
	}

	return 0;

} 
