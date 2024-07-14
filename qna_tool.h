#pragma once
#include <iostream>
#include <fstream>
#include "Node.h"
#include "dict.h"
#include "search.h"

using namespace std;

class QNA_tool {

private:

    // You are free to change the implementation of this function
    void query_llm(string filename, Node* root, int k, string API_KEY, string question);
    // filename is the python file which will call ChatGPT API
    // root is the head of the linked list of paragraphs
    // k is the number of paragraphs (or the number of nodes in linked list)
    // API_KEY is the API key for ChatGPT
    // question is the question asked by the user

    // You can add attributes/helper functions here

public:

    /* Please do not touch the attributes and
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    
    QNA_tool(); // Constructor
    ~QNA_tool(); // Destructor

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);
    // This function is similar to the functions in dict and search 
    // The corpus will be provided to you via this function
    // It will be called for each sentence in the corpus

    Node* get_top_k_para(string question, int k);
    // This function takes in a question, preprocess it
    // And returns a list of paragraphs which contain the question
    // In each Node, you must set: book_code, page, paragraph (other parameters won't be checked)

    std::string get_paragraph(int book_code, int page, int paragraph);
    // Given the book_code, page number, and the paragraph number, returns the string paragraph.
    // Searches through the corpus.

    void query(string question, string filename);
    // This function takes in a question and a filename.
    // It should write the final answer to the specified filename.

    /* -----------------------------------------*/
    /* Please do not touch the code above this line */

    // You can add attributes/helper functions here
    vector<string> lines;
    vector<vector<int>> addresses;
    vector<vector<vector<float>>> scores;
    int bookcount = 0, pagecount = 0, paracount = 0;

    Dict d;

    float scorekeeper(string word);

    const int encode[128] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,36,37,38,39,-1,-1,-1,40,41,-1,-1,-1,42,26,27,28,29,30,31,32,33,34,35,-1,-1,43,44,45,-1,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,-1,46,-1,47,48,49,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,50,51,52,53,-1};
    const int decode[54] = {97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,48,49,50,51,52,53,54,55,56,57,35,36,37,38,42,43,47,60,61,62,92,94,95,96,123,124,125,126};

    class AhoNode{

        public:
        float score;
        AhoNode* next[54] = {nullptr};

        AhoNode() {
            score = 0;
        }

        ~AhoNode() {
            for (AhoNode* x : next) {
                delete x;
            }
        }

        AhoNode* getchild(int x) {
            if (!next[x]) {
                next[x] = new AhoNode();
            }
            return next[x];                
        }

        void show() {
            cout<<score<<endl;
            for (auto z : next) {
                if (z) {
                    z->show();
                }
            }
        }

    };
};