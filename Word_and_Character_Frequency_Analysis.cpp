#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <string>

/// for char count need space /t and /n in there too!



int IsNumber(int index, std::string & str); // returns end of number from a string
int IsWord(int index , std::string & str); // returns end of word from a string
std::string Make_Lowercase(std::string & str); // makes word lowercase

void Print_Top10_Words(std::unordered_map<std::string, std::pair<int, int>>);
void Print_Top10_Ints(std::unordered_map<std::string, std::pair<int, int>>);
void Print_Top10_Chars(std::unordered_map<char,int>);

bool compare_char(const std::pair<char, int>& a, const std::pair<char, int>& b); // compare char occurrences if tie -> compare ascii values
bool compare(const std::pair<std::string, std::pair<int,int>>& a, const std::pair<std::string, std::pair<int,int>>& b); // compare word and number occurrences if tie -> compare line occurrence

int main() {
    // in pair integers the first is word count second is line occurrence
    std::unordered_map<std::string, std::pair<int , int>> Word_Count;
    std::unordered_map<std::string, std::pair<int, int>> Int_Count;
    std::unordered_map<char,int> Char_Count;
    std::string file;
    std::cout << "Enter the full filename you wish to read, do not include spaces.\n";
    std::cin >> file;
    std::ifstream filename(file); // set file
    if (!filename){                         // check file opens
        std::cout << "Error opening file\n";
        return 1;
    }
    std::string data;
    int line_number=1;
    /// word is consecutive letters
    /// number is consecutive numbers
    while (std::getline(filename, data)){
        for (int i = 0; i < data.size(); i++){
            if (isdigit(data[i])){
                int digit_size = IsNumber(i, data); // find size of number
                std::string number = data.substr(i, digit_size); // substring for number
                Int_Count[number].first++; // add the digit to number table
                bool flag = true;
                for ( auto & digit : Int_Count){
                    if (digit.first == number ) {
                        flag = false;
                    }
                }
                if (flag){
                    Int_Count[number].second = ++line_number;
                }
                i += (digit_size -1);
            }
            if (isalpha(data[i])){
                int word_size = IsWord(i, data); // find size of word
                std::string word = data.substr(i, word_size); // substring for word
                word = Make_Lowercase(word); // word is converted to be all lowercase
                Word_Count[word].first++; // add word to word table with the line number where the word occurred
                bool flag = true;
                for ( auto & str : Word_Count){
                    if (str.first == word ) {
                        flag = false;
                    }
                }
                if (flag){
                    Word_Count[word].second = ++line_number;
                }
                for (char &c : word){
                    Char_Count[c]++; // add character to character table
                }
                i += (word_size -1);
            }
            else { // for newlines, tabs, and spaces
                switch (data[i]) {
                    case ' ':
                        Char_Count[' ']++;
                        break;
                    case '\t':
                        Char_Count['\t']++;
                        break;
                    case '\n':
                        Char_Count['\n']++;
                        break;
                }
            }
        }
        line_number++;
    }
    Print_Top10_Chars(Char_Count);
    Print_Top10_Ints(Int_Count);
    Print_Top10_Words(Word_Count);

    return 0;
}

bool compare_char(const std::pair<char, int>& a, const std::pair<char, int>& b)  {
    if (a.second == b.second) {
        return a.first < b.first;  // Compare by ASCII value if equal
    }
    return a.second > b.second;  // else compare by occurrences
}

bool compare(const std::pair<std::string, std::pair<int,int>>& a, const std::pair<std::string, std::pair<int,int>>& b)  {
    if (a.second.first == b.second.first){ // if occurrences equal
        return a.second.second < b.second.second;  // Compare by line order
    }
    return a.second.first > b.second.first;  // compare by occurrences
}

void Print_Top10_Chars(std::unordered_map<char,int> CharMap){
    std::vector<std::pair<char, int>> charVector(CharMap.begin(), CharMap.end()); // copy to vector
    std::sort(charVector.begin(), charVector.end(), compare_char); // sort vector using compare function
    unsigned long long my_size = 10;
    if (charVector.size()<10){my_size = charVector.size();} // in case less than 10 unique chars
    std::cout << "Top " << my_size << " Chars: \n";
    for (int i = 0 ; i<my_size; i++){
        switch(charVector[i].first){
            case ' ':
                std::cout << "space: " << charVector[i].second << std::endl;
                break;
            case '\t':
                std::cout << "tab: " << charVector[i].second << std::endl;
                break;
            case '\n':
                std::cout << "new line: " << charVector[i].second << std::endl;
                break;
            default :
                std::cout << charVector[i].first << " : " << charVector[i].second << std::endl;
        }
    }
}

void Print_Top10_Words(std::unordered_map<std::string, std::pair<int, int>> WordMap){
    std::vector<std::pair<std::string, std::pair<int,int>>> WordVector(WordMap.begin(), WordMap.end()); // copy to vector
    std::sort(WordVector.begin(), WordVector.end(), compare); // sort vector using compare function
    unsigned long long my_size = 10;
    if (WordVector.size()<10){my_size = WordVector.size();} // in case less than 10 unique chars
    std::cout << "Top " << my_size << " Words: \n";
    for (int i = 0 ; i<my_size; i++){
        std::cout << WordVector[i].first << " : " << WordVector[i].second.first << std::endl;
    }
}
void Print_Top10_Ints(std::unordered_map<std::string, std::pair<int, int>> IntMap){
    std::vector<std::pair<std::string, std::pair<int,int>>> IntVector(IntMap.begin(), IntMap.end()); // copy to vector
    std::sort(IntVector.begin(), IntVector.end(), compare); // sort vector using compare function
    unsigned long long my_size = 10;
    if (IntVector.size()<10){my_size = IntVector.size();} // in case less than 10 unique chars
    std::cout << "Top " << my_size << " Integers: \n";
    for (int i = 0 ; i<my_size; i++){
        std::cout << IntVector[i].first << " : " << IntVector[i].second.first << std::endl;
    }
}

int IsNumber(int index, std::string& str){
    int number_size = 0;
    while (index < str.length() && isdigit(str[index])){
        number_size++;
        index++;
    }
    return (number_size);
}
int IsWord(int index , std::string& str){
    int word_size = 0;
    while (index < str.length() && isalpha(str[index])){
        word_size++;
        index++;
    }
    return (word_size);
}

std::string Make_Lowercase(std::string& str){
    std::string lowercase =str;
    for (int i = 0; i<lowercase.size(); i++){
        lowercase[i]= std::tolower(str[i]);
    }
    return lowercase;
}