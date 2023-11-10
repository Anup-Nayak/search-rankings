#include <assert.h>
#include <sstream>
#include "qna_tool.h"

using namespace std;

#define FILENAME "mahatma-gandhi-collected-works-volume-1.txt"

//preprocess the corporus: store the paragraphs in dll. :: done in constructor of QnA tool.
//for each paragraph keep a dict object/trieif cant import dict
//the dict will store the no. of times each word comes in the para. easily get freqs.




//saem ne sahi hi likha hoga
void insert(int bcode,int pageno,int parano,Node* head){
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
void QNA_tool::csv_process(string& word,long long& freq){
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
    words.insert_sentence(0,0,0,0,sentence);
    if(cor_size && corpus[cor_size-1]->para_no==paragraph && corpus[cor_size-1]->page_no==page && corpus[cor_size-1]->b_code==book_code){
        corpus[cor_size-1]->d.insert_sentence(0,0,0,0,sentence);
    }
    else{
        para* p=new para;
        corpus.push_back(p);
        corpus[cor_size]->b_code=book_code;
        corpus[cor_size]->page_no=page;
        corpus[cor_size]->para_no=paragraph;
        corpus[cor_size]->d.insert_sentence(0,0,0,0,sentence);
        cor_size++;
    }
    return;
}

QNA_tool::QNA_tool(){
    cor_size=0;
    ifstream csv("unigram_freq.csv");
    string f;
    string word;
    csv.ignore(11);
    while(getline(csv,word,',') && getline(csv,f)){
        istringstream iss(f);

        long long freq;
        iss>>freq;
        csv_process(word,freq);
    }
}

QNA_tool::~QNA_tool(){
    for(auto i:corpus){
        delete i;
    } 
}
//HEAPUTIL BEGINS
int par(int idx){
    return (idx-1)/2;
}
int left(int idx){
    return idx*2+1;
}
int right(int idx){
    return idx*2+2;
}


void swap(para** p1,para** p2){
    para* p3=*p1;
    *p1=*p2;
    *p2=p3;
}

int swapmaxchild(vector<para*> &v,int idx){
    int l=left(idx);
    int r=right(idx);
    int max;
    if(v[l]->score<v[r]->score){
        max=r;
    }
    else{
        max=l;
    }
    if(v[idx]->score<v[max]->score){
        swap(&v[max],&v[idx]);
    }
    else{
        return 0;
    }
    return max;
}

//{0,1,2,3,4,5,6,7}
void QNA_tool::heapifyup(vector<para*> &v,int idx){
    if(!idx) return;
    int p=par(idx);
    if(v[p]->score<v[idx]->score){
        swap(&v[p],&v[idx]);
    }
    heapifyup(v,p);
}

void QNA_tool::heapifydn(vector<para*> &v,int idx,int size){
    int l=left(idx);
    int r=right(idx);
    if(l>=size){
        return;
    }
    if(r>=size){
        if(v[l]->score>v[idx]->score){
            swap(&v[l],&v[idx]);
        }
        return;
    }
    idx=swapmaxchild(v,idx);
    if(idx) heapifydn(v,idx,size);
    return;
}

//HEAPUTIL ENDS

//HEAPSORT
void QNA_tool::Sort(vector<para*> &corpus,int k){
    for (int i = 1; i < cor_size; ++i)
    {
        heapifyup(corpus,i);
    }
    int size=corpus.size();
    for (int i = 0; i < k; ++i)
    {
        swap(&corpus[0],&corpus[size-1]);
        heapifydn(corpus,0,size);
        size--;
    }

}

//not getting used in current implementation
bool QNA_tool::is_separator(char x){
    for(auto i:separators){
        if(i==x) return true;
    }
    return false;
}

Node* QNA_tool::get_top_k_para(string question, int k) {
    Node* head=nullptr;
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
        int occurances = w.node->word_count;

        //j is a para object
        //j.d gives us the dict for that paragraph that saem has made
        for(auto j : corpus){
            double countOfWord = j->d.get_word_count(wrd);
            double score = words.get_word_count(wrd);
            j->score += ((occurances)*(score+1) * (countOfWord))/(f_csv+1);
        }
    }

    
    Sort(corpus,k);
    head=new Node(corpus[cor_size-k]->b_code,corpus[cor_size-k]->page_no,corpus[cor_size-k]->para_no,0,0);
    for(int i=(cor_size-k+1);i<cor_size;i++){
        insert(corpus[i]->b_code,corpus[i]->page_no,corpus[i]->para_no,head);
    }
    return head;


}

void QNA_tool::query(string question, string filename){
    // Implement your function here  
    std::cout << "Q: " << question << std::endl;
    std::cout << "A: Studying COL106 :)" << std::endl;
    return;
}

std::string QNA_tool::get_paragraph(int book_code, int page, int paragraph){

    cout << "Book_code: " << book_code << " Page: " << page << " Paragraph: " << paragraph << endl;
    
    std::string filename = "Corpora/MK Gandhi/mahatma-gandhi-collected-works-volume-";
    filename += to_string(book_code);
    filename += ".txt";

    std::ifstream inputFile(filename);

    std::string tuple;
    std::string sentence;

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the input file " << filename << "." << std::endl;
        exit(1);
    }

    std::string res = "";

    while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
        // Get a line in the sentence
        tuple += ')';

        int metadata[5];
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
            (metadata[2] == paragraph)
        ){
            res += sentence;
        }
    }

    inputFile.close();
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
    // You can add anything here - show all your creativity and skills of using ChatGPT
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