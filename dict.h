
// Do NOT add any other includes
#include <string> 
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

class Dict {
private:
    
    class Node{

        public:
        long long int nums;
        Node* next[54] = {nullptr};

        Node() {
            nums = 0;
        }

        ~Node() {
            for (Node* x : next) {
                delete x;
            }
        }

        Node* getchild(int x) {
            if (!next[x]) {
                next[x] = new Node();
            }
            return next[x];                
        }

    };

    Node* head;
    const int encode[128] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,36,37,38,39,-1,-1,-1,40,41,-1,-1,-1,42,26,27,28,29,30,31,32,33,34,35,-1,-1,43,44,45,-1,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,-1,46,-1,47,48,49,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,50,51,52,53,-1};
    const int decode[54] = {97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,48,49,50,51,52,53,54,55,56,57,35,36,37,38,42,43,47,60,61,62,92,94,95,96,123,124,125,126};

    void dumper(Node* x, string &y, ofstream& w);

public: 

    /* Please do not touch the attributes and 
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    Dict();

    ~Dict();

    void insert_sentence(string sentence);

    long long int get_word_count(string word);

    void dump_dictionary(string filename);

    /* -----------------------------------------*/
};