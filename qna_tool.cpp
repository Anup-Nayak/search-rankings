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

//saem ne sahi hi likha hoga
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

int get_indx(char x){
    int asc=int(x);
    if(asc>47 && asc<58) return (asc-22);
    if(asc>64 && asc<91) return (asc-65);
    if(asc>96 && asc<123) return (asc-97);
    if(x=='/') return 36;
    if(x=='_') return 37;
    if(asc==35) return 38;
    if(asc==36) return 39;
    if(asc==37) return 40;
    if(asc==38) return 41;
    if(asc==42) return 42;
    if(asc==43) return 43;
    if(asc==60) return 44;
    if(asc==62) return 45;
    if(asc==61) return 46;
    if(asc==94) return 47;
    if(asc==96) return 48;
    if(asc==92) return 49;
    if(asc==123) return 50;
    if(asc==124) return 51;
    if(asc==125) return 52;
    if(asc==126) return 53;
    //add other possible symbols
    return -1;
}

//anup ne likha hai galat nahi ho sakta
void QNA_tool::csv_process(string& word,int freq){
    //insert the word in trie, set word_count to freq 
    trie_node* csvroot=csv.root;
    for(auto i : word){
        int asc = get_indx(i);
        if(csvroot->child[asc]){
            csvroot = csvroot->child[asc];
        }
        else{
            csvroot->child[asc] = new trie_node();
            csvroot = csvroot->child[asc];
        }
    }
    csvroot->word_count=freq;
}

//saem ne samjhaya hai galat nahi hai
void QNA_tool::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    if(cor_size && corpus[cor_size-1].para_no==paragraph && corpus[cor_size-1].page_no==page && corpus[cor_size-1].b_code==book_code){
        corpus[cor_size-1].d.insert_sentence(0,0,0,0,sentence);
    }
    else{
        para p;
        corpus.push_back(p);
        corpus[cor_size].b_code=book_code;
        corpus[cor_size].page_no=page;
        corpus[cor_size].para_no=paragraph;
        corpus[cor_size].d.insert_sentence(0,0,0,0,sentence);
        cor_size++;
    }
    return;
}

QNA_tool::QNA_tool(){
    cor_size=0;
    ifstream csv("unigram_freq.csv");
    string f;
    string word;
    while(getline(csv,word,',') && getline(csv,f)){
        int freq=stoi(f);
        csv_process(word,freq);
    }
}

QNA_tool::~QNA_tool(){
    // Implement your function here  
}

//write a mergesort to sort the corpus as per corpus-ka-element.score
void MergeSort(vector<para> &corpus){
    
}

//not getting used in current implementation
bool QNA_tool::is_separator(char x){
    for(auto i:separators){
        if(i==x) return true;
    }
    return false;
}

Node* QNA_tool::get_top_k_para(string question, int k) {
    dll top_k;
    Dict q;
    q.insert_sentence(0,0,0,0,question);
    for(auto w : q.distinct_words){
        w.freq=csv.search(w.word);
        if(w.freq<0) w.freq=0;
    }
    /*w:: w.word - gives the word in lower case
    ,w.freq - frequency in csv file
    ,w.node - w.node->word_count gives the no. of times it is in "question"
    */
    for(auto w : q.distinct_words){
        string wrd = w.word;
        int f_csv = w.freq;
        int count = w.node->word_count;

        //j is a para object
        //j.d gives us the dict for that paragraph that saem has made
        for(auto j : corpus){
            float countOfWord = j.d.get_word_count(wrd);
            j.score += ((countOfWord+1) * (countOfWord))/(f_csv+1);
        }
    }

    
    MergeSort(corpus);

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