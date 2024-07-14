#include <assert.h>
#include <sstream>
#include "qna_tool.h"

using namespace std;

QNA_tool::QNA_tool(){
    insert_sentence(0,0,0,0,"");
}

QNA_tool::~QNA_tool(){
    // Implement your function here  
}

void QNA_tool::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    while (book_code >= bookcount) {
        scores.push_back({});
        bookcount++;
        pagecount = 0;
    }
    while (page >= pagecount) {
        scores[book_code].push_back({});
        pagecount++;
        paracount = 0;
    }
    while (paragraph >= paracount) {
        scores[book_code][page].push_back({0});
        paracount++;
    }
    lines.push_back(sentence);
    addresses.push_back({book_code,page,paragraph});
    d.insert_sentence(sentence);    
    return;
}

Node* QNA_tool::get_top_k_para(string question, int k) {
    AhoNode* head = new AhoNode();
    AhoNode* x = head;
    string word = "";
    for (char y : question) {
        const int n = encode[y];
        if (n==-1) {
            x->score+=scorekeeper(word);
            x = head;
            word.clear();
        }
        else {
            word.push_back(y);
            x = x->getchild(n);
        }
    }
    x->score+=scorekeeper(word);
    int index = 0;
    bool go = true;
    x = head;
    for (string l : lines) {
        for (char y : l) {
            const int n = encode[y];
            if (!go && n!=-1) {
                continue;
            }
            else if (!go) {
                go = true;
                continue;
            }
            if (n==-1) {
                scores[addresses[index][0]][addresses[index][1]][addresses[index][2]]+=x->score;
                x = head;
            }
            else if (!x->next[n]) {
                go = false;
                x = head;                                
            }
            else {
                x = x->next[n];
            }
        }
        scores[addresses[index][0]][addresses[index][1]][addresses[index][2]]+=x->score;
        x = head;
        go = true;
        index++;
    }
    vector<vector<int>> res (k,{0,0,0});
    for (int i = 0; i < scores.size(); i++) {
        for (int j = 0; j < scores[i].size(); j++) {
            for (int a = 0; a < scores[i][j].size(); a++) {
                int f = k - 1;
                while (f != -1 && scores[res[f][0]][res[f][1]][res[f][2]] < scores[i][j][a]) {
                    f--;
                }
                if (f != k-1) {
                    res.insert(res.begin()+f+1,{i,j,a});
                    res.pop_back();
                }                
            }
        }
    }
    Node* ans = new Node();
    Node* r = ans;
    for (auto m : res) {
        r->right = new Node(m[0],m[1],m[2],0,0);
        r->right->left = r;
        r = r->right;
        r->right = nullptr;
    }
    ans = ans->right;
    delete ans->left;
    ans->left = nullptr;
    for (auto x : scores) {
        for (auto y : x) {
            for (auto z : y) {
                z = 0;
            }
        }
    }
    return ans;
}

float QNA_tool::scorekeeper(string word) {
    if (word == "") {
        return 0;
    }
    fstream meow;
    int s = word.size();
    meow.open("unigram_freq.csv",ios::in);
    string line, count = "";
    getline(meow,line);
    while (getline(meow,line)) {
        bool go = false;
        for (int i = 0; i < s; i++) {
            if (line[i]!=word[i] && line[i]!=word[i]+32) {
                go = true;
                break;
            }
        }
        if (go || line[s] != ',') {
            continue;
        }
        else {
            for (int j = s + 1; j < line.size(); j++) {
                count.push_back(line[j]);
            }
            return (1+d.get_word_count(word))/(1+stof(count));
        }
    }
    return 1+d.get_word_count(word);
}

void QNA_tool::query(string question, string filename){
    cout<<"Q: "<<question;
    vector<string> words = {};
    string z = "";
    for (char x : question) {
        if (encode[x]!=-1) {
            z.push_back(x);
        }
        else {
            if (z.size()) {
                words.push_back(z);
            }
            z = "";
        }                
    }
    words.push_back(z);
    z = "";
    for (string f : words) {
        int n = d.get_word_count(f);
        n = 50 - n/1000;
        if (n < 1) {
            n = 1;
        }
        for (int i = 0; i < n; i++) {
            z.append(f);
            z.push_back(' ');
        }        
    }    
    query_llm("api_call.py",get_top_k_para(z,7),7,"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",question);
    return;
}

std::string QNA_tool::get_paragraph(int book_code, int page, int paragraph){
    string res = "";
    for (int i = 0; i < lines.size(); i++) {
        if (addresses[i][0]==book_code && addresses[i][1]==page && addresses[i][2]==paragraph) {
            res.append(lines[i]);
        }
    }
    return res;
}

void QNA_tool::query_llm(string filename, Node* root, int k, string API_KEY, string question){

    // first write the k paragraphs into different files

    Node* traverse = root;
    int num_paragraph = 0;

    while(num_paragraph < k){
        assert(traverse != nullptr);
        string p_file = "paragraph_";
        p_file += to_string(num_paragraph);
        p_file += ".txt";
        // delete the file if it exists
        remove(p_file.c_str());
        ofstream outfile(p_file);
        string paragraph = get_paragraph(traverse->book_code, traverse->page, traverse->paragraph);
        assert(paragraph != "$I$N$V$A$L$I$D$");
        outfile << paragraph;
        outfile.close();
        traverse = traverse->right;
        num_paragraph++;
    }

    // write the query to query.txt
    ofstream outfile("query.txt");
    outfile << "These are the excerpts from Mahatma Gandhi's books.\nOn the basis of this, ";
    outfile << question;
    outfile << "\nReply using bullets.";
    outfile.close();
 
    // you do not need to necessarily provide k paragraphs - can configure yourself

    // python3 <filename> API_KEY num_paragraphs query.txt
    string command = "python3 ";
    command += filename;
    command += " ";
    command += API_KEY;
    command += " ";
    command += to_string(k);
    command += " ";
    command += "query.txt";

    system(command.c_str());
    return;
}