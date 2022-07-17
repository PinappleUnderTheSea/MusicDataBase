#include "grams.h"
#include "util.h"
#include <iostream>
#include <regex>
#include <set>
#include <numeric>
#include <iomanip>
#include <cmath>

using namespace std;

// define some global variables
vector<string> words;                       // all words, sort by appearance order
vector<pair<string, int>> freq;             // frequency of coocurrence of each word
// for simplicity, each row of the co-occurrence matrix represents the word by lexicographical order
// thus, we'd use a map to store the index of the word which is `word_index`
map<string, int> word_index;                // mapping the word to the index in the co-occurrence matrix
map<int, string> index_word;                // mapping the index to the word in the co-occurrence matrix
vector<vector<int>> coocur_matrix;          // co-ocurrence martix, each row is a n-gram
vector<vector<double>> normalized_matrix;   // normalized co-ocurrence martix
// You can add more global variables here if you need.
// Notice that if you add more global variables, you should declare them in the `grams.h` using `extern`

/**
 * @brief read the input file and record all words in the file. 
 *  Store the words in the vector `words` according to the appearance order.
 * 
 * @param filename  file name of the input file
 * @param words     vector to store the words
 * @return size_t   the size of the `words` vector
 */
size_t record_words(const string& filename, vector<string>& words) {
    ifstream file(filename);
    if (!file.good()) {
        cerr << "Error: cannot open file " << filename << endl;
        exit(1);
    }
    string line;
    while (getline(file, line)) {
        transform(line.begin(), line.end(), line.begin(), ::tolower);
        split_string(line, words);
    }
    file.close();
    return words.size();
}

/**
 * @brief go through the vector `words` and record the frequency of each word.
 * e.g.:
 *  words = ["a", "a", "b", "c", "d", "e", "e"]
 *  freq = [("a", 2), ("b", 1), ("c", 1), ("d", 1), ("e", 2)]
 * 
 * @param words     vector of words
 * @param freq      vector to store the frequency of each word
 * @return size_t   the size of the `freq` vector
 */
size_t record_freq(const vector<string>& words, vector<pair<string, int>>& freq) {
    // hint: you may find the `map` data structure useful
    // TODO: impelement this function.
    /* Your code here */
//     auto num=words.begin();
//     map<string,int> map1;
//     for(size_t i=0;i<words.size();i++){
//         auto vec_num=freq.begin();
//         for(size_t j=0;j<freq.size();j++){
//             if(*num==vec_num->first){ 
//                 ++vec_num->second;
//                 break;
//             }
//             ++vec_num;
//         }    
 
//         if (vec_num==freq.end()){
//             pair<string,int> pair1;
//             pair1.first=*num;
//             pair1.second=1;
//             freq.push_back(pair1);

//         }
//         ++num;
//     }
//     return freq.size();
// }
    map<string,int> map1;
    pair<string,int> pair1;
    for(size_t i = 0; i<words.size();++i){
        map1[words[i]] ++;
    }
    for(map<string,int>::iterator i = map1.begin();i!=map1.end();++i){
        pair1.first = i->first;
        pair1.second = i->second;
        freq.push_back(pair1);
    }
    return freq.size();
}
/**
 * @brief Filter out the 3000 most frequent words.
 * !!Notice that if a word is not in the 3000 most frequent words, it should be replaced by `<unk>`.
 * 
 * @param freq      vector of words and their frequency
 * @param words     original vector of words. You should change the vector `words` to keep the 3000 most frequent words.
 */
bool compare(const pair<string,int>&a, const pair<string,int>&b){
        return a.second>b.second||(a.second==b.second&&a.first<b.first);
}
void filter_words(vector<pair<string, int>>& freq, vector<string>& words) {
    // hint: you can use function `sort` to sort the vector `freq` by modify the compare function.
    // if you want to write a compare function more gracefully, you can try to use `lambda expression`(It's a new feature in C++11.).
    // TODO: implement this function.
    /* Your code here */
    //bool compare(pair<string,int>&a, pair<string,int>&b);
    sort(freq.begin(),freq.end(),compare);
    auto itr=freq.begin();
    auto word=words.begin();
    set<string> s;
    for(int i =0;i<3000;i++){
        s.insert(itr->first);
        ++itr;
    }
    for(size_t j=0;j<words.size();j++){
        int count=s.count(*word);
        if(!count) *word="<unk>";
        ++word;
    }


    return;
}



/**
 * @brief Use `words` vector to construct a word index map. 
 * The key is the word and the value is the position of the word in the alphabetical order.
 * e.g. :
 * words: ["I", "like", "to", "eat", "apple"]
 * word_index: {"I": 0, "apple": 1, "eat": 2, "like": 3, "to": 4}
 * 
 * @param words         vector of words
 * @param word_index    map to store the word index
 * @return size_t       the size of the `word_index` map
 */

size_t set_word_index(const vector<string>& words, map<string, int>& word_index) {
    // TODO: implement this function
    /* Your code here */
    vector<string> words_store;
    for(auto j=words.begin();j!=words.end();j++){
        words_store.push_back(*j);
    }
    vector<string>::iterator words_end;
    sort(words_store.begin(),words_store.end());
    words_end = unique(words_store.begin(),words_store.end());  
    auto itr=words_store.begin();
    pair<string,int> pair_word;
    for(size_t i=0; ;i++){
        pair_word.first = *itr;
        pair_word.second = i;
        word_index.insert(pair_word);

        ++itr;
        if(itr == words_end){
            break;
        }
    }
    return word_index.size();
}

/**
 * @brief Set up the coocur matrix object. 
 * Each row of the coocur matrix represents the word by lexicographical order. 
 * The value of each element in the co-occurrence matrix is the number of times that two words appear together.
 * e.g. :
 * consider n as 3,
 * words: ["<unk>", "I", "like", "to", "eat", "apple", "<unk>"]
 * coocur_matrix:   (sorted by lexicographical order. Thus, these rows represent the words ["<unk>", "I", "apple", "eat", "like", "to"])
 * coocur_matrix[1][4] == 1 represents that if consider "I" as center word, the number of times that "I" and "like" appear together is 1.
 * [0, 1, 1, 0, 0, 0]
 * [1, 0, 0, 0, 1, 0]
 * [1, 0, 0, 1, 0, 0]
 * [0, 0, 1, 0, 0, 1]
 * [0, 1, 0, 0, 0, 1]
 * [0, 0, 0, 1, 1, 0]
 * 
 * @param words             vector of words
 * @param word_index        map of word index
 * @param coocur_matrix     vector of vector to store the co-occurrence matrix
 * @param n                 hyper-parameter of n-gram
 */
void set_coocur_matrix(const vector<string>& words, const map<string, int>& word_index, vector<vector<int>>& coocur_matrix, int n) {
    int size = words.size();
    coocur_matrix = vector<vector<int>>(word_index.size(), vector<int>(word_index.size(), 0));
    for (int left = 0, right = n, center = (n + 1) / 2; right <= size; left++, right++, center++) {
        // if (center % 100 == 0) {
        //     cout << setprecision(5) << (double)center/size << '\r';     // This is to show the progress of building the co-occurrence matrix.
        // }
        // If you want, you can do the similar thing to print the progress of other parts.
        // TODO: impelement this function. Store the co-occurrence matrix in the `coocur_matrix` vector.
        /* Your code here */

        int index_x=0;
        int index_y=0;
        index_x = word_index.find(*(words.begin()+center))->second ;
        
        for(int i = left ; i <right ; ++i){
            if(i!=center){
                // for(auto itr_index=word_index.begin();itr_index!=word_index.end();++itr_index){
                //     if(*itr_word==itr_index->first)
                //         index_y=itr_index->second;
                // }
                index_y = word_index.find(*(words.begin()+i))->second;
                // cout<<index_x<<' '<<index_y<<'\n';
                ++coocur_matrix[index_x][index_y];
            }
        }    
    }
    return;
}
/**
 * @brief normalize the co-occurrence matrix by column.
 * e.g.:
 * co-occurrence matrix:
 * [0, 2, 1, 0, 0, 0]
 * [2, 0, 0, 0, 1, 0]
 * [1, 0, 0, 1, 0, 0]
 * [0, 0, 1, 0, 0, 1]
 * [0, 1, 0, 0, 0, 1]
 * [0, 0, 0, 1, 1, 0]
 * normalized matrix:
 * [0, 2/3, 0.5, 0, 0, 0]
 * [2/3, 0, 0, 0, 0.5, 0]
 * [1/3, 0, 0, 0.5, 0, 0]
 * [0, 0, 0.5, 0, 0, 0.5]
 * [0, 1/3, 0, 0, 0, 0.5]
 * [0, 0, 0, 0.5, 0.5, 0]
 * 
 * @param coocur_matrix         co-occurrence martix
 * @param normalized_matrix     normalized co-occurrence martix
 */
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
            double nor = (sum==0)?0:1.0*vec[i][j]/sum;
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

/**
 * @brief save the normalized co-occurrence matrix to a file. The first line should be the words which each row represents.
 * each number is separated by a space.
 * e.g.:
 * a b c d
 * 0.2 0 0.6 0.2
 * 0 0.5 0.5 0
 * 0.6666666666666666 0.3333333333333333 0 0
 * 0 0 0 1
 * 
 * @param filename      file name of the output file
 * @param matrix        normalized co-occurrence martix
 * @param word_index    map of word to index
 */
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
    else{
        cout<<"fu"<<endl;
    }  
}

/**
 * @brief restore the normalized co-occurrence matrix from file `filename`.
 * Assume that the file is in the same format as the one in `save_matrix` function.
 * e.g.:
 * 0.2 0 0.6 0.2
 * 0 0.5 0.5 0
 * 0.6666666666666666 0.3333333333333333 0 0
 * 0 0 0 1
 * 
 * @param filename    file name of the input file
 * @param matrix      normalized co-occurrence martix
 * @param index_word  map of index to word
 */
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

/**
 * @brief calculate the cosine similarity of word i and word j
 * you can calculate the cosine similarity by using the formula:
 * cosine_similarity = sum(matrix[i][k]*matrix[j][k]) / (sqrt(sum(matrix[i][k]^2)) * sqrt(sum(matrix[j][k]^2)))
 * if you can not understand the formula, please refer to the lab notes.
 * 
 * @param matrix    normalized co-occurrence martix
 * @param i         index of word i
 * @param j         index of word j
 * @return double   cosine similarity of word i and word j
 */
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

/**
 * @brief find the 5 most similar words to the given word. If there are less than 5 words, then simply return all of them.
 * You should sort them by the similarity score in descending order.
 * If they have the same similarity score, then sort them by the alphabetical order.
 * 
 * @param word                  given word
 * @param matrix                normalized co-occurrence martix
 * @param index_word            map of index to word
 * @return vector<string>       vector of 5 most similar words
 */
bool rankd(pair<string,double>a,pair<string,double>b){
    if((a.second)!=(b.second)) return a.second>b.second;
    else return a.first <b.first;
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
            //cout<<"sim="<<sim<<endl;
            pair<string,double>p;
            p.first=word_itr->second;
            p.second=sim;
            sim_rank.push_back(p);
        }
        sort(sim_rank.begin(),sim_rank.end(),rankd);
        for(size_t k=0,x=0;k<sim_rank.size()&&x<5;k++){
            if(sim_rank[k].second!=0&&sim_rank[k].first!=word) {
                ans.push_back(sim_rank[k].first);
                //cout<<(sim_rank[k].first)<<"  sim="<<(sim_rank[k].second)<<endl;
                x++;
            }
        }

    }
    else{
    cerr << "Error: cannot find word " << word << endl;
    } 
    return ans;
}