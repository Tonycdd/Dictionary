#include "Trie.hpp"
#include "KHeap.hpp"
#include <unordered_map>
#include <stack>

// Някаква базова проверка, за да взимаме само думи без знаци, естествено не е най-доброто
// Най-вече правим ненужно копие на думата, като за думи,които са реално ОК - това е напълно ненужна операция
std::string containsSpecialSymbol(const std::string& word){
    std::string specialSymbols = "!@#$%^&*()_+={}[]|\\:;\"'<>,.?/~`-";
    std::string newString;
    for (char c : word)
    {
        if(specialSymbols.find(c) != std::string::npos){
            // ако има специален символ, не го добавяме, 
            continue;
        }
        newString+=c; // иначе го добавяме
    }
    return newString;
}

int main(){
    int M,K;
    std::cout << "Please enter M - number for first M words, which are going to displayed! " << std::endl;
    std::cin >> M;

    std::cout << "Now please enter a number K - number of children for our heap!" << std::endl;
    std::cin >> K;

    // създаваме си речниково дърво
    TrieDictionary dict("words.txt");
    dict.display(); // да видим как изглежда
    std::cout << std::endl;

    // Тук идеята е да направим една хеш таблица - от думи и числа, знам че хеширането на думи не е най-доброто, туй като е малко по-особено и често е по-бавно от други подходи, НО
    // предвид факта, че така ще постигнем бързо добавяне и извличане - вярно в средния случай, но пак...
    // Както и хешът е подходящ за такъв тип задача, където трябва да преброим брой срещания.

    std::unordered_map<std::string, int> wordsCount;
    std::ifstream file("file.txt");
    if(!file.is_open()){
        std::cerr << "File error occured!" << std::endl;
        return 1;
    }
    
    std::string word;
    while(file >> word){
        // за всяка дума, първо я валидираме, след което увеличаваме срещанията й
        std::string currentWord = containsSpecialSymbol(word);
        wordsCount[currentWord]++;    
    }
    file.close(); 

    // Тук това е валидният масив, който имах предвид в конструктора за пирамидата, иначе ще имаме проблем
    MyVector<std::pair<int,std::string>> elements;

    // За всяка двойка в таблицата, ако думата е в речника - променяме крайната оценка, иначе е броят срещания в таблицата
    for (const auto& pair : wordsCount)
    {
        int score = dict._search(pair.first); // тук това имах предвид, че тази функция е по-полезна в изчисляването на оценките
        int finalScore = score > 0 ? score * pair.second : pair.second;
        elements.push_back({finalScore,pair.first}); // добавяме крайно получената двойка от оценка и дума
    }
    
    KHeap heap(K,elements); // правим си пирамида от вектора

    std::cout << "Top " << M << " words:" << std::endl;
    for (int i = 0; i < M && heap.getSize() > 0; i++) {
        auto top = heap.extract(); // ауто е за да не пиша std::pair....
        std::cout << top.second << " -> " << top.first << std::endl;
    }

    return 0;
}