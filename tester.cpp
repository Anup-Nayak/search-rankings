#include<bits/stdc++.h>
#include<sstream>
#include "Node.h"
#include "qna_tool.h"

using namespace std;

int main(){

    QNA_tool qna_tool;

    int num_books = 98;
    for(int i = 1; i <= num_books; i++){

        std::cout << "Inserting book " << i << std::endl;

        std::string filename = "Corpora/MK Gandhi/mahatma-gandhi-collected-works-volume-";
        filename += to_string(i);
        filename += ".txt";

        std::ifstream inputFile(filename);

        if (!inputFile.is_open()) {
            std::cerr << "Error: Unable to open the input file mahatma-gandhi." << std::endl;
            return 1;
        }

        std::string tuple;
        std::string sentence;


        while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
            // Get a line in the sentence
            tuple += ')';

            std::vector<int> metadata;    
            std::istringstream iss(tuple);

            // Temporary variables for parsing
            std::string token;

            // Ignore the first character (the opening parenthesis)
            iss.ignore(1);

            // Parse and convert the elements to integers
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
                    metadata.push_back(num);
                } else {
                    // Convert the element to integer
                    int num = std::stoi(token);
                    metadata.push_back(num);
                }
            }

            // Now we have the string in sentence
            // And the other info in metadata
            // Add to the dictionary

            // Insert in the qna_tool
            qna_tool.insert_sentence(metadata[0], metadata[1], metadata[2], metadata[3], sentence);
        }

        inputFile.close();
    }

    // string question ="What were the views of Mahatma Gandhi on the Partition of India?";
    // {string question ="What is the purpose of life?";
    //     // {string question ="What were Gandhi's views on the manner of how one should eat?";
    //         /*
    //             Part-1 : Get the top-k paragraphs
    //             Checking Correctness
    //         */
    //         // Let's try to ask a simple question to the qna_tool
    //         Node* head = qna_tool.get_top_k_para(question, 8);
        
    //         vector<string> paras;
    //         while(head != nullptr){
    //             string res = qna_tool.get_paragraph(head->book_code, head->page, head->paragraph);
    //             paras.push_back(res);
    //             if(!head->right) {delete head; break;}
    //             head = head->right;
    //             if(head) delete head->left;
    //         }
    //     }cout<<endl<<endl;
        {string question1 ="What were the views of Mahatma Gandhi on the Partition of India?";
        // {string question1 ="Who was Mahatma Gandhi?";
                 Node* head1 = qna_tool.get_top_k_para(question1, 4);
                
                    vector<string> paras1;
                    while(head1 != nullptr){
                        string res1 = qna_tool.get_paragraph(head1->book_code, head1->page, head1->paragraph);
                        paras1.push_back(res1);
                        if(!head1->right) {delete head1; break;}
                        head1 = head1->right;
                        if(head1) delete head1->left;
                    }}
        // for(int i = 0; i < (int)paras.size(); i++){
        //     cout << paras[i] << endl << endl << endl;


    /*
        Part-2 : Query the LLM
    */
    // qna_tool.query(question, "api_call.py");
    
    return 0;
}