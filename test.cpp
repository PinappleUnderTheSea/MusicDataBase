#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <map>
#include <iomanip>
using namespace std;

void set_coocur_matrix(const vector<string>& words, const map<string, int>& word_index, vector<vector<int>>& coocur_matrix, int n) {
   
    int size = words.size();
    coocur_matrix = vector<vector<int>>(word_index.size(), vector<int>(word_index.size(), 0));

    for (int left = 0, right = n, center = (n + 1) / 2 - 1; right <= size; left++, right++, center++) {
        cout << setprecision(5) << (double)center/size << '\r';     // This is to show the progress of building the co-occurrence matrix.
        // If you want, you can do the similar thing to print the progress of other parts.
        // TODO: impelement this function. Store the co-occurrence matrix in the `coocur_matrix` vector.
        /* Your code here */
        int index_x;
        for(auto itr_word=words.begin()+left;itr_word!=words.begin()+right;++itr_word){
            if(itr_word==words.begin()+center){
                for(auto itr_index=word_index.begin();itr_index!=word_index.end();++itr_index){
                    if(*itr_word==itr_index->first)
                        index_x=itr_index->second;
                        break;
                }
            }
            else{
                int index_y;
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
int main(){

}