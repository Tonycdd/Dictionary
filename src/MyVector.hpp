#include <exception>
#pragma once
#include <cassert>
#include<iostream>

template<class T>
class MyVector
{
private:
    T* data;
    size_t size, capacity;

    void clear(){
        delete[] this->data;
        this->data = nullptr;
        this->size = this->capacity = 0;
    }

    void copy(const MyVector<T>& other){
        this->reserve(other.size);
        for (size_t i = 0; i < other.size; i++)
        {
            this->data[i] = other.data[i];
        }
        this->size = other.size;
        this->capacity = other.capacity;
    }

    void reserve(int capacity){
        int newSize = capacity > 0 ? capacity : 1;
        if(this->capacity >= newSize){
            return;
        }
        //inache nqma mqsto
        T* newData = new T[newSize];
        this->capacity = newSize;
        for (size_t i = 0; i < this->size; i++)
        {
            newData[i] = this->data[i];
        }
        delete[] this->data;
        this->data = newData;
    }

    void resize(int size){
        if(this->capacity > size){
            return;
        }
        else{
            this->reserve(size);
        }
    }

    void append(const T& data){
        // v kraq dobavqme - za push_back amortizirana const O(1);
        if(this->size + 1 > this->capacity){
            this->resize(this->capacity*2);
        }
        //inache ima mqsto
        this->data[this->size++] = data;
    }

    void insertBeg(const T& data){
        //insert v nachaloto push_front - O(N);
        if(this->size + 1 > this->capacity){
            this->reserve(this->capacity*2);
        }
        //ima mqsto
        for (size_t i = this->size; i > 0; i--)
        {
            this->data[i] = this->data[i - 1];
        }
        //nakraq ostava da opracim nachaloto
        this->data[0] = data;
        this->size++;        
    }

  void erase(int first, int second) {
    
    int _first = std::max(0, first); 
    int _second = second < 0 ? _first + 1 : second; 

  
    if (_first >= this->size || _second > this->size || _first >= _second) {
        return;
    }


    int shiftCount = _second - _first;


    for (int i = _second; i < this->size; ++i) {
        this->data[i - shiftCount] = this->data[i];
    }


    this->size -= shiftCount;
}
    
    int compare(const MyVector<T>& other)const{
        int minSize = this->size < other.size ? this->size : other.size;
        
        for (size_t i = 0; i < minSize; i++)
        {
            if(this->data[i] < other.data[i]){
                return -1;
            }
            else if(this->data[i] > other.data[i]){
                return 1;
            }
        }
        //ako sa ednakvi
        return 0;
        
    }
public:

    MyVector() : data(nullptr), size(0), capacity(0) {};

    MyVector(int size) : data(nullptr), size(0),capacity(0)
    {
        this->reserve(size);
    };

    MyVector(const MyVector<T>& other) : data(nullptr), size(0), capacity(0)
    {
        this->reserve(other.size);
        this->size = other.size;
        this->capacity = other.capacity;
        for (size_t i = 0; i < other.size; i++)
        {
            this->data[i] = other.data[i];
        }        
    };

    MyVector<T>& operator=(const MyVector<T>& other){
        if(this != &other){
            this->clear();
            this->reserve(other.size);
            for (size_t i = 0; i < other.size; i++)
            {
                this->data[i] = other.data[i];
            }
            this->size = other.size;
            this->capacity= other.capacity;
        }
        return *this;
    }

    ~MyVector(){
        this->clear();
    }

    void push_front(const T& data)
    {
        this->insertBeg(data);
    }

    void push_back(const T& data){
        this->append(data);
    }

    void pop_front(){
        this->erase(0,1);
    }
    void pop_back(){
        this->erase(this->size - 1, this->size);
    }

    T& front() const{
        return this->data[0];
    }
    T& back()const{
        return this->data[this->size-1];
    }
    const T& _front()const{
        return this->data[0];
        
    }
    const T& _back()const{
        return this->data[this->size-1];
    }

    T& at(int index) const{
        //assert(index > 0 && index <= this.size);
        return this->data[index];
    }

    T& operator[](int index)const{
        return this->data[index];
    }
    bool operator<(const MyVector<T>& other)const{
        return this->compare(other) < 0;
    }
    bool operator>(const MyVector<T>& other)const{
        return this->compare(other) > 0;
    }
    bool operator<=(const MyVector<T>& other)const{
        return !this->operator>(other);
    }
    bool operator>=(const MyVector<T>& other)const{
        return !this->operator<(other);
    }
    bool operator!=(const MyVector<T>& other)const{
        return this->compare(other) != 0;
    }
    bool operator==(const MyVector<T>& other)const{
        return !this->operator!=(other);
    }
    void print()const{
        for (size_t i = 0; i < this->size; i++)
        {
            std::cout << data[i] << " ";
        }
    }

    //tuka sa taka, zashtoto na insert i remove shte gi promenqme, zatova ne sa i const, tui kato promenqt obekta
    int getSize()const {
        return this->size;
    }
    int getCapacity()const{
        return this->capacity;
    };
};
