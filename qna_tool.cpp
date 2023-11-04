#include <assert.h>
#include <sstream>
#include "qna_tool.h"

using namespace std;

#define FILENAME "mahatma-gandhi-collected-works-volume-1.txt"

//preprocess the corporus: store the paragraphs in dll. :: done in constructor of QnA tool.
//for each paragraph keep a dict object/trieif cant import dict
//the dict will store the no. of times each word comes in the para. easily get freqs.

dll::dll(){
    head=nullptr;
}

dll::~dll(){
    if(!head) return;
    Node* t=head->right;
    while(head->right){
        t=head->right;
        delete head;
        head=t;
    }
    delete head;
}

void dll::insert(int bcode,int pageno,int parano){
    if(!head){
        head = new Node(bcode,pageno,parano,0,0);
        head->left = nullptr;
        head->right = nullptr;
        return;
    }
    Node* t = new Node(bcode,pageno,parano,0,0);
    if(head->right) head->right->left = t;
    t->right = head->right;
    head->right = t;
    t->left = head;
}

void QNA_tool::csv_process(string word,int freq){
    //insert the word in trie, set word_count to freq 
}
void QNA_tool::corpus_process(int bcode,int pageno,int parano,string sentence){
    //check if the para exists on the top of the vector<para> corpus.
}

QNA_tool::QNA_tool(){
    ifstream csv("unigram_freq.csv");
    
}

QNA_tool::~QNA_tool(){
    // Implement your function here  
}

Node* QNA_tool::get_top_k_para(string question, int k) {
    dll top_k;
    vector<string> words;//F vector not allowed. maybe dll
/*
    break question into into words
    get words' freq. from csv's trie
    for all paragraphs in corpus, look for each words (in question) store 

*/

    // for (int i = 0; i < sentence.size(); ++i)
    // {
    //     if(is_separator(sentence[i])) continue;
    //     int start=i;
    //     trie_node* n=t.root;
    //     while(i<sentence.size() && !(is_separator(sentence[i]))){
    //         i++;
    //     }
    //     words.push_back(sentence.substr(start,i-start));
    // } 
return nullptr;

}

void QNA_tool::query(string question, string filename){
    // Implement your function here  
    std::cout << "Q: " << question << std::endl;
    std::cout << "A: Studying COL106 :)" << std::endl;
    return;
}

void get_paragraph(int book_code, int page, int paragraph, int sentence_no, string &res){

    std::ifstream inputFile(FILENAME);

    std::string tuple;
    std::string sentence;

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the input file." << std::endl;
        exit(1);
    }

    while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
        // Get a line in the sentence
        tuple += ')';

        int metadata[4];
        std::istringstream iss(tuple);

        // Temporary variables for parsing
        std::string token;

        // Ignore the first character (the opening parenthesis)
        iss.ignore(1);

        // Parse and convert the elements to integers
        int idx = 0;
        while (std::getline(iss, token, ',')) {
            // Trim leading and trailing white spaces
            size_t start = token.find_first_not_of(" ");
            size_t end = token.find_last_not_of(" ");
            if (start != std::string::npos && end != std::string::npos) {
                token = token.substr(start, end - start + 1);
            }
            
            // Check if the element is a number or a string
            if (token[0] == '\'') {
                // Remove the single quotes and convert to integer
                int num = std::stoi(token.substr(1, token.length() - 2));
                metadata[idx] = num;
            } else {
                // Convert the element to integer
                int num = std::stoi(token);
                metadata[idx] = num;
            }
            idx++;
        }

        if(
            (metadata[0] == book_code) &&
            (metadata[1] == page) &&
            (metadata[2] == paragraph) &&
            (metadata[3] == sentence_no)
        ){
            res = sentence;
            return;
        }

        res = "$I$N$V$A$L$I$D$";
        return;
    }

}

void QNA_tool::query_llm(string filename, Node* root, int k, string API_KEY){

    // first write the 3 paragraphs into different files
    Node* traverse = root;
    int num_paragraph = 0;
    while(num_paragraph < k){
        assert(traverse != nullptr);
        string p_file = "paragraph_";
        p_file += to_string(num_paragraph);
        p_file += ".txt";
        ofstream outfile(p_file);
        string paragraph;
        get_paragraph(traverse->book_code, traverse->page, traverse->paragraph, traverse->sentence_no, paragraph);
        assert(paragraph != "$I$N$V$A$L$I$D$");
        outfile << paragraph;
        outfile.close();
        traverse = traverse->right;
        num_paragraph++;
    }

    // write the query to query.txt
    ofstream outfile("query.txt");
    outfile << "Hey GPT, What is the purpose of life?";
    // You can add anything here - show all your creativity and skills of using ChatGPT
    outfile.close();
 
    // python3 <filename> API_KEY paragraph_0.txt paragraph_1.txt paragraph_2.txt query.txt
    string command = "python3 ";
    command += filename;
    command += " ";
    command += API_KEY;
    command += " ";
    command += "paragraph_0.txt";
    command += " ";
    command += "paragraph_1.txt";
    command += " ";
    command += "paragraph_2.txt";
    command += " ";
    command += "query.txt";

    system(command.c_str());
    return;
}