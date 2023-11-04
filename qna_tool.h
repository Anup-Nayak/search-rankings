#include <iostream>
#include <fstream>
#include "Node.h"
#include "dict.h"


using namespace std;

class dll{
public:
    Node* head;
    dll();
    ~dll();
    void insert(int bcode,int pageno,int parano);
};

struct para
{   //removed left,right
    int b_code;
    int page_no;
    int para_no;
    Dict d;
    double score;
};

class QNA_tool {

private:
    // You are free to change the implementation of this function
    trie csv;
    void csv_process(string& word,int freq);
    void query_llm(string filename, Node* root, int k, string API_KEY);
    vector<para> corpus;
    bool is_separator(char x);
    int cor_size;
    string separators = " .,-:!\"\'()?[];@";
    // You can add attributes/helper functions here

public:

    /* Please do not touch the attributes and
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    
    QNA_tool(); // Constructor
    ~QNA_tool(); // Destructor
    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    Node* get_top_k_para(string question, int k);
    // This function takes in a question, preprocess it
    // And returns a list of paragraphs which contain the question
    // In each Node, you must set: book_code, page, paragraph (other parameters won't be checked)

    void query(string question, string filename);
    // This function takes in a question and a filename.
    // It should write the final answer to the specified filename.

    /* -----------------------------------------*/
    /* Please do not touch the code above this line */

    // You can add attributes/helper functions here
};