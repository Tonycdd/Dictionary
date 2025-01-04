#pragma once
#include <exception>
#include<iostream>
#include <string>
#include<fstream>

// Структура за дървото
struct TrieNode{
    TrieNode* children[26];
    int score; // оценка за всяка буква, само за думите ще я задаваме, за останалите букви ще си е 0
    bool isEndOfWord;  // маркер за това, че се е сформирала дума

    TrieNode() :score(0), isEndOfWord(false){ // дефолтен конструктор, главно за задаване на всичките деца
        for (size_t i = 0; i < 26; i++)
        {
            children[i] = nullptr;
        }
    };
};

class TrieDictionary
{
private:
    TrieNode* root;

    // рекурсивна функция за триене на дървото 
    void remove(TrieNode* root){
        if(!root) {
            return;
        }
        for (size_t i = 0; i < 26; i++)
        {
            if (root->children[i])
            {
                remove(root->children[i]);
                root->children[i] = nullptr; // важно е да се направи, за да няма висящи пойнтъри, след триенето
            }
        }
        delete root; // накрая остава да изтрием само корена
    }

    // рекурсивна функция за копиране от валидно вече речниково дърво
    void copy(TrieNode* dest, TrieNode* src){
        if(!src){
            return;
        }
        dest->score = src->score;
        dest->isEndOfWord = src->isEndOfWord;
        for (size_t i = 0; i < 26; i++)
        {
            if(src->children[i]){
                // ако има дете, то създаваме същото дете в нашия оригинал, след което продъжаваме копирането с тях
                dest->children[i] = new TrieNode(); 
                copy(dest->children[i], src->children[i]);
            }
        }
    }

    // функция за показване на самото речниково дърво, тя работи по следния начин
    // Принтира думите, като естествено се подреждат по азбучен ред, тъй като така е и в един обикновен речник, освен това тя ще принтира тогава и само тогава
    // когато се стигне до буква, чийто маркер казва, че е край на дума.

    void _display(TrieNode* root, std::string prefix)const{
        if(root->isEndOfWord){
            std::cout << prefix << " -> score: " << root->score << std::endl;

        }
        for (size_t i = 0; i < 26; i++)
        {
            if(root->children[i] != nullptr){
                char c = intToChar(i); // взимаме аски кода на буквата - 0 е за 'а' и т.н
                _display(root->children[i], prefix + c);
            }
        }
    }
    
    // Базова проверка на думата и нейната оценка от входния файл, естествено проверката може да се задълбочи, 
    // тук единствено подсигурявам някакви базови изисквания

    bool areValid(const std::string& word, int score){
        for (size_t i = 0; i < word.size(); i++)
        {
            if(isspace(word[i]) || word[i] < 'a' || word[i] > 'z' ){
                return false;
            }
        }

        if(score < 0) {
            return false;
        }
        return true;
        
    }

    // Главна функция за класа, тя се грижи да построи речниково дърво от входния файл
    
    void buildFromFile(const std::string& _file){
        std::fstream file(_file);
        if(!file.is_open()){
            std::cerr << "Error occuried while trying opening file named as: " << _file << std::endl;
            throw std::runtime_error("Error occuried while trying opening file: " + _file);
        }
        std::string word;
        int score;
        while(file >> word >> score){
            if(areValid(word,score)){
                insert(word,score);
            }
            else{
                std::cerr << "Skipping invalid entry: " << word << " " << score << std::endl; // тук не хвърлям грешка, а само сигнализирам, че даден ред от файла е с грешна информация
            }
        }
    }

public:

    // Дефолтен конструктор
    TrieDictionary(){
        root = new TrieNode();
    }
    
    //С аргументи - използвам него за построяването на дървото от файл
    TrieDictionary(const std::string& filename) : TrieDictionary() {
        try
        {
            buildFromFile(filename);
            
        }
        catch(const std::exception& e)
        {
            //не сме построили правилно обекта, трябва да го изтрием и да сигнализираме за това
            remove(this->root); // грижим се да не хвърчи памет извън класа
            std::cerr << e.what() << std::endl;
        }
        
    }

    // Копи конструктор
    TrieDictionary(const TrieDictionary& other){
        root = new TrieNode(); // задаваме си първо валиден корен, след което копираме 
        copy(root, other.root);
    }
    
    // Оператор=
    TrieDictionary& operator=(const TrieDictionary& other){
        if(this != &other){
            remove(this->root);
            root = new TrieNode();
            copy(root, other.root);
        }
        return *this;
    }

    // Деструктор
    ~TrieDictionary(){
        remove(this->root);
    }

    // Взимаме съответстващия индекс на буква
    int charToIndex(char c)const{
        return c - 'a';
    }
    
    //Тук обратно, взимаме буква за даден индекс
    char intToChar(int i)const{
        return i + 'a';
    }

    //Главна функция за класа, грижеща се да добави нова дума в дървото
    // Ако вече има такава буква, функцията не създава ново дете, а продължава по пътя
    // Тук има забележка, но в задачата не е казано, ако се даде някаква дума два пъти - какво да правим, аз съм избрал по-скоро да не броя колко пъти се е добавила 
    // Защото така работи и обикновения речник

    void insert(const std::string& word, int score){
        TrieNode* current = root; // Ще го променяме, затова не взимаме корена, да не го замажем
        for (size_t i = 0; i < word.size(); i++)
        {
            int index = charToIndex(word[i]);
            if(current->children[index] == nullptr){
                TrieNode* newNode = new TrieNode();
                current->children[index] = newNode;
            }
            current = current->children[index];
        }

        current->score = score;
        current->isEndOfWord = true;
    }

    // Функция за търесене на дадена дума с дадена оценка в речника, което може и да е безполезно, но в някои случаи има смисъл
    bool search(const std::string& word, int score)const{
        TrieNode* curr = root;
        for (size_t i = 0; i < word.size(); i++)
        {
            int index = charToIndex(word[i]);
            if(curr->children[index] == nullptr){
                return false;
            }
            curr = curr->children[index];
        }
        if(!curr->isEndOfWord){
            return false;
        }
        return curr->score == score;
    }

    // По-полезната функция от горната, тъй като ми връща и самата оценка, което по-напред ми трябва при смятане на оценки
    int _search(const std::string& word){
        TrieNode* curr = root;
        for (size_t i = 0; i < word.size(); i++)
        {
            int index = charToIndex(word[i]);
            if(curr->children[index] == nullptr){
                return -1;
            }
            curr = curr->children[index];
        }
        if(!curr->isEndOfWord){
            return -1;  // Индикатор, че такава дума няма в речника, после ще се ползва 
        }
        return curr->score;
        
    }

    // Външна фунция за принтенето, тъй като не искам външният свят да ми дава корена
    void display()const{
        _display(root,"");
    }

};



