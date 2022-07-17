#include "grams.h"
#include "util.h"
#include <iostream>
#include <regex>
#include <set>
#include <numeric>
#include <iomanip>
#include <cmath>

using namespace std;
double similarity(const vector<vector<double>>& matrix, size_t i, size_t j) {
    // TODO: implement this function
    /* Your code here */
    double res;
    double a1=0,a2=0,a3=0;
    for(size_t k=0;k<matrix.size();k++){
        a1+=matrix[i][k]*matrix[j][k];
        a2+=matrix[i][k]*matrix[i][k];
        a3+=matrix[j][k]*matrix[j][k];
    }
    res=a1/(sqrt(a2)*sqrt(a3));
    return res == 0 ? 0 : res;
}
bool rankd(pair<string,double>a,pair<string,double>b){
    if((a.second)!=(b.second)) return a.second>b.second;
    else return a.first >b.first;
}
bool compare(pair<int,string>a,pair<int,string>b){
    return a.first<b.first;
}
void split_string(const string& s, vector<string>& ret) {
    // hint: you can use function `substr` to get a substring of the string `s`
    //       if you know what regular expression is, you can use `regex` to easily split the string `s`
    //       stringstream may be useful, too
    // TODO: implement this function
    /* Your code here */
    stringstream ss(s);
    string t;
    while(ss>>t){
        ret.push_back(t);
    }
    return;
}
size_t record_freq(const vector<string>& words, vector<pair<string, int>>& freq) {
    // hint: you may find the `map` data structure useful
    // TODO: impelement this function.
    /* Your code here */
    auto num=words.begin();
    map<string,int> map1;
    for(size_t i=0;i<words.size();i++){
        auto vec_num=freq.begin();
        for(size_t j=0;j<freq.size();j++){
            if(*num==vec_num->first){ 
                ++vec_num->second;
                break;
            }
            ++vec_num;
        }    
 
        if (vec_num==freq.end()){
            pair<string,int> pair1;
            pair1.first=*num;
            pair1.second=1;
            freq.push_back(pair1);

        }
        ++num;
    }
    return freq.size();
}
bool compare(pair<string,int>&a, pair<string,int>&b){
        return a.second>b.second;
}
void filter_words(vector<pair<string, int>>& freq, vector<string>& words) {
    // hint: you can use function `sort` to sort the vector `freq` by modify the compare function.
    // if you want to write a compare function more gracefully, you can try to use `lambda expression`(It's a new feature in C++11.).
    // TODO: implement this function.
    /* Your code here */
    bool compare(pair<string,int>&a, pair<string,int>&b);
    sort(freq.begin(),freq.end(),compare);
    auto itr=freq.begin();
    auto word=words.begin();
    set<string> s;
    for(int i =0;i<5;i++){
        s.insert(itr->first);
        ++itr;
    }
    for(size_t j=0;j<words.size();j++){
        int count=s.count(*word);
        if(count==0) *word="<unk>";
        ++word;
    }
    cout<<"============="<<endl;
    for(auto i=s.begin();i!=s.end();i++){
        cout<<*i<<" ";
    }
    cout<<endl;
    cout<<"============="<<endl;


    return;
}
void save_matrix(const string& filename, const vector<vector<double>>& matrix, const map<string, int>& word_index) {
    // TODO: save the normalized_matrix to file `result.txt`. Notice that the first row should be the words.
    /* Your code here */
    ofstream f(filename,ios::out);
    if(f.is_open()){
        for(auto itr=word_index.begin();itr!=word_index.end();itr++){
            f<<itr->first<<" ";
        }
        f<<endl;
        for(size_t i=0;i<matrix.size();i++){
            for(size_t j=0;j<matrix.size();j++){
                f<<matrix[i][j]<<" ";
            }
            f<<endl;
        }
        f.close();
    }    
}
size_t set_word_index(const vector<string>& words, map<string, int>& word_index) {
    // TODO: implement this function
    /* Your code here */
    vector<string> words_store;
    for(auto j=words.begin();j!=words.end();j++){
        words_store.push_back(*j);
    }
    sort(words_store.begin(),words_store.end());
    unique(words_store.begin(),words_store.end());  

    auto itr=words_store.begin();
    for(size_t i=0;i<words_store.size();i++){
        pair<string,int> pair_word;
        pair_word.first=*itr;
        pair_word.second=i;
        word_index.insert(pair_word);
        ++itr;
    }
    return word_index.size();
}

void normalize_matrix(const vector<vector<int>>& coocur_matrix, vector<vector<double>>& normalized_matrix) {
    // TODO: implement this function
    /* Your code here */
    vector<vector<double>>vec(coocur_matrix.size(),vector<double>(coocur_matrix.size(),0));
    vector<vector<double>>trans;

    for(size_t i=0;i<coocur_matrix.size();i++){
        for(size_t j=0;j<coocur_matrix.size();j++){
            vec[i][j]=coocur_matrix[j][i];
        }
    }
    for(size_t i=0;i<coocur_matrix.size();i++){
        int sum = accumulate(vec[i].begin(),vec[i].end(),0.0);
        vector<double>transline;
        for(size_t j=0;j<coocur_matrix.size();j++){
            double nor = (sum==0)?0:vec[i][j]/(sum*1.0);
            transline.push_back(nor);
        }
        trans.push_back(transline);
    }
    for(size_t i=0;i<coocur_matrix.size();i++){
        vector<double> line;
        for(size_t j=0;j<coocur_matrix.size();j++){
            line.push_back(trans[j][i]);
        }
        normalized_matrix.push_back(line);
    }
    return;
}
void set_coocur_matrix(const vector<string>& words, const map<string, int>& word_index, vector<vector<int>>& coocur_matrix, int n) {
    int size = words.size();
    coocur_matrix = vector<vector<int>>(word_index.size(), vector<int>(word_index.size(), 0));
    for (int left = 0, right = n, center = (n + 1) / 2; right <= size; left++, right++, center++) {
        if (center % 100 == 0) {
            cout << setprecision(5) << (double)center/size << '\r';     // This is to show the progress of building the co-occurrence matrix.
        }
        // If you want, you can do the similar thing to print the progress of other parts.
        // TODO: impelement this function. Store the co-occurrence matrix in the `coocur_matrix` vector.
        /* Your code here */
        int index_x=0;

        for(auto itr_index=word_index.begin();itr_index!=word_index.end();++itr_index){
            if(*(words.begin()+center)==itr_index->first){
                index_x=itr_index->second;
                break;
            }
        }
            
           
        for(auto itr_word=words.begin()+left;itr_word!=words.begin()+right;++itr_word){
            if(itr_word!=words.begin()+center){
                int index_y=0;
                for(auto itr_index=word_index.begin();itr_index!=word_index.end();++itr_index){
                    if(*itr_word==itr_index->first)
                        index_y=itr_index->second;
                }
                ++coocur_matrix[index_x][index_y];
            }

        }    
    }
    return;
}

void restore_matrix(const string& filename, vector<vector<double>>& matrix, map<int, string>& index_word) {
    // hint: you can refer to the `set_word_index` function and `split_string` function.
    // TODO: restore the normalized_matrix from file `result.txt`.
    /* Your code here */
    ifstream f(filename,ios::in);
    string word_line;
    getline(f,word_line);
    vector<string> word;
    split_string(word_line,word);
    
    map<string,int>wordtoindex;
    set_word_index(word,wordtoindex);
    auto i=wordtoindex.begin();
    for(;i!=wordtoindex.end();i++){
        pair<int,string>p;
        p.first=i->second;
        p.second=i->first;
        index_word.insert(p);
    }
    while(getline(f,word_line)){
        vector<double>line;
        stringstream ss(word_line);
        string str;
        double dou;
        while(ss>>str){
            dou=stod(str);
            line.push_back(dou);
        }
        matrix.push_back(line);
    }

    return;
}

vector<string> most_similar(const string& word, const vector<vector<double>>& matrix, const map<int, string>& index_word) {
    // TODO: implement this function. 
    /* Your code here */
    int index;
    int valid=1;
    auto itr=index_word.begin();
    bool rankd(pair<string,double>a,pair<string,double>b);
    vector<string> ans;
    for(;itr!=index_word.end();itr++){
        if(word==itr->second) {
            index=itr->first;
            break;
        }
    }
    if (itr==index_word.end()) valid=0;
    auto word_itr=index_word.begin();
    vector<pair<string,double>> sim_rank;
    if(valid){
        for(size_t i=0;i<index_word.size();i++,word_itr++){
            double sim=similarity(matrix,index,word_itr->first);
            cout<< (word_itr->second)<<" sim="<<sim<<endl;
            pair<string,double>p;
            p.first=word_itr->second;
            p.second=sim;
            sim_rank.push_back(p);
        }
        sort(sim_rank.begin(),sim_rank.end(),rankd);
        for(size_t k=0,x=0;k<sim_rank.size()&&x<5;k++){
            if(sim_rank[k].second!=0&&sim_rank[k].first!=word) {
                ans.push_back(sim_rank[k].first);
                x++;
            }
        }
    }
    else{
    cerr << "Error: cannot find word " << word << endl;
    } 
    return ans;
}
int main(){
    string s("a a a a a b b b b b c c c c c d d d d d e e e e e sd dgf sg sdgf sdgf sdfg sdfg sdfg sdgf v bv hj jh jh nfg y");

    vector<string> ret;
    split_string(s,ret);
    for (auto i = 0; i < 2 - 1; i++) {
        ret.insert(ret.begin(), "<unk>");
        ret.insert(ret.end(),"<unk>");
    }
    vector<pair<string,int>>freq;
    size_t t=(record_freq(ret,freq));
    vector<vector<double>> matrix;
    map<string,int>mp;
    vector<vector<double>> nmatrix;
    vector<vector<double>> rmatrix;
    map<int,string> rmp;
    vector<vector<int>> coo;

    for(auto i=ret.begin();i!=ret.end();i++){
        cout<<*i<<endl;
        
    }
    for(auto j=freq.begin();j!=freq.end();j++){
        cout<<j->first<<"--"<<j->second<<endl;
    }
    
    //filter
    filter_words(freq,ret);

    cout<<t<<endl;
    cout<<"-----------------"<<endl;
    for(auto i=ret.begin();i!=ret.end();i++){
        cout<<*i<<endl;
        
    }
    //set index
    
    set_word_index(ret,mp);
    
    
    
    //save
    for(size_t m=0;m<mp.size();m++){
        vector<double>line;
        for(size_t n=0;n<mp.size();n++){
            line.push_back(n);
        }
        matrix.push_back(line);
    }
    cout<<"--------------------"<<endl;
    //set coo
    set_coocur_matrix(ret,mp,coo,2);

    for(size_t i=0;i<mp.size();i++){
        for(size_t j=0;j<mp.size();j++){
            cout<<coo[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<"--------------------"<<endl;
   
    //normalize
    ++coo[4][0];
    normalize_matrix(coo,nmatrix);
    for(size_t i=0;i<mp.size();i++){
        for(size_t j=0;j<mp.size();j++){
            cout<<nmatrix[i][j]<<" ";
        }
        cout<<endl;
    }
    save_matrix("a.txt",nmatrix,mp);
    cout<<"-----restore------"<<endl;
    restore_matrix("a.txt",rmatrix,rmp);
    for(auto g=rmp.begin();g!=rmp.end();g++){
        cout<<(g->second)<<" ";
    }
    cout<<endl<<"========"<<endl;
    for(size_t i=0;i<rmatrix.size();i++){
        for(size_t j=0;j<rmatrix.size();j++){
            cout<<rmatrix[i][j]<<" ";
        }
        cout<<endl;
    }

    //most similar
    cout<<"--------------"<<endl;
    auto v=most_similar("The",rmatrix,rmp);
    for(auto z=v.begin();z!=v.end();z++){
        cout<<*z<<endl;
    }
    return  0;
}