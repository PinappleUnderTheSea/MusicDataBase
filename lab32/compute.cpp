#include<iostream>
#include<fstream>
#include<numeric>
#include<string>
#include<sstream>
#include<vector>
using namespace std;
int main(){
    string s,sp;
    ifstream fin("matrix.txt",ios::in);
    getline(fin,sp);
    getline(fin,s);
    stringstream ss(s);
    string t;
    double d;
    vector<double>vec;
    while(ss>>t){
        d=stod(t);
        vec.push_back(d);
    }
    double sum =accumulate(vec.begin(),vec.end(),0.0);
    cout<<sum<<endl;
    return 0;
}