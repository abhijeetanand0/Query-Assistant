// Do NOT add any other includes
#include "dict.h"

Dict::Dict(){
    head = new Node();
}

Dict::~Dict(){
    delete head;    
}

void Dict::insert_sentence(string sentence){
    Node* x = head;
    for (char y : sentence) {
        const int n = encode[y];
        if (n==-1) {
            x->nums++;
            x = head;
        }
        else {
            x = x->getchild(n);
        }
    }
    x->nums++; 
    return;
}

long long int Dict::get_word_count(string word){
    Node* x = head;
    for (char y : word) {
        x = x->getchild(encode[y]);
        if (!x) {
            return 0;
        }
    } 
    return x->nums;
}

void Dict::dumper(Node* x, string &s, ofstream& wr) {
    if (x->nums) {   
        wr<<s<<", "<<x->nums<<"\n";
    }
    for (int i = 0; i < 54; i++) {
        if (x->next[i]) {
            s.push_back(decode[i]);
            dumper(x->next[i],s,wr);
            s.pop_back();
        }
    }
    return;
}

void Dict::dump_dictionary(string filename){
    ofstream scribe;
    scribe.open(filename,ios::trunc);
    string s = "";
    Node* x = head;
    for (int i = 0; i < 54; i++) {
        if (x->next[i]) {
            s.push_back(decode[i]);
            dumper(x->next[i],s,scribe);
            s.pop_back();
        }
    }
    return;
}