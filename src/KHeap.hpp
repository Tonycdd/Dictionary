#pragma once
#include "MyVector.hpp"
#include<string>

class KHeap
{
private:
    MyVector<std::pair<int, std::string>> heap;  // Използвам представяне чрез децата - наредени двойки от оценка и дума
    int k; // броят деца - трябва да са поне 2 или повече
    
    // Ясно е, взимаме индекс на родител
    int getIndexParent(int index)const{
        return (index - 1) / k; 
    }

    // Индекс на дете, малко е объркващо това ж, но ако си го разиграем, ще забележим, че всъщност е така, например,ако к е равно на 2, то тогава при смятането на индекс на дете на такава
    // пирамида, ще забележим, че фор-цикълът се вика за 2i + 1, 2i + 2, което знаем, че са правилните формули за двоична пирамида
    int getChildrenIndex(int i, int j)const{
        return (k*i) + j;
    }

    // Изплуване нагоре
    void sizeUp(int index){
        if(index < 0 || index > heap.getSize()){
            return;
        }
        while(index >= 0){
            int parent = getIndexParent(index);
            
            // Максимална пирамида искаме, сравняваме по оценка
            if(heap.at(index).first > heap.at(parent).first){
                std::swap(heap.at(index), heap.at(parent)); 
                index = parent;
            }
            
            else
            {
                break; // Ще спести работа
            }
        }
    }
    
    void sizeDown(int index) {
        
    int size = heap.getSize(); 
    while (true) {
        int largest = index; 
        for (int i = 1; i <= k; ++i) {
            int childIndex = getChildrenIndex(index,i);
            if (childIndex < size && heap[childIndex].first > heap[largest].first) {
                largest = childIndex; 
            }
        }

        if (largest == index) {
            break; // Няма нужда от размяна; текущият възел е на правилното място
        }

        std::swap(heap[index], heap[largest]); // Разменяме с най-малкото дете
        index = largest; // Продължаваме надолу от новата позиция
    }
}

    //Главна функция, която започва от не-листата на пирамидата, а знаем, че в двоична пирамида те са точно половината от елементите в нея, тоест за к-ична 
    // формулата отдолу е вярна

    void heapify(){
        for (int i = (heap.getSize() / k) - 1; i >= 0; --i)
        {
            sizeDown(i); // Не добавяме нови, а обработваме вече съществуваща колекция 
        }
        
    }
public:
    // ВАЖНО - тук сме сигурни, че подаденият ни вектор е валиден, иначе бихме имали проблем!
    explicit KHeap(int _k, const MyVector<std::pair<int,std::string>>& elements) : k(_k)
    {
        if(this->k < 2){
            throw std::invalid_argument("K must be bigger or equal to 2");
        }
        this->heap = elements;
        heapify();
    } 

    // Копи 
    KHeap(const KHeap& other) : heap(other.heap),k(other.k){};

    //Оператор=
    KHeap& operator=(const KHeap& other){
        if(this!=&other){
            heap = other.heap;
            k = other.k;
        }
        return *this;
    }

    // Деструктор - дефолтен е 
    ~KHeap() = default;

    // Добавяне на нов елемент, като просто добавяме във вектора и пускаме да изплува нагоре
    void insert(int score,const std::string& word){
        heap.push_back({score,word});
        int index = heap.getSize() - 1;
        sizeUp(index);
    }

    //Взимаме на минимум, ако сме конструирали правилно, не би трябвало да гърми, НО
    // При празна пирамида ще има проблем, тъй като това обаче е проблем на потребителя, тъй като даваме възможност за проверка с метод empty(), приемаме че 
    // няма да викаме никога върху празна пирамида

    std::pair<int,std::string> getMin()const{
        return heap.front();
    }

    // Това е функцията
    bool empty()const{
        return heap.getSize() == 0;
    }

    // Махане на върха на пирамидата, връща ни стойността на него, което в случая е наредена двойка от стойност и дума
    std::pair<int,std::string> extract(){
        if(heap.getSize() == 0){
            throw std::logic_error("Our heap is empty!");
        } // Отново, би било проблем на потребителя, тъй като има метод, гарантиращ, че не празна пирамидата
        std::pair<int,std::string> root = heap.front(); // ако е празна, бихме гръмнали тук 
        heap.front() = heap.back(); // както и тук
      
        heap.pop_back();
        
        sizeDown(0);
        return root;
    }

    // Принтиране на пирамидата
    void print() const{
        std::cout << "Size: " << heap.getSize() << std::endl;
        for (size_t i = 0; i < heap.getSize(); i++)
        {
            std::cout << "{"<< heap.at(i).first << ", " << heap.at(i).second << "}" << std::endl;
        }
        
    }
    
    // Гетър за размера, използва метод от класа МойВекор за да вземе сайза
    int getSize()const{
        return heap.getSize();
    }
};


