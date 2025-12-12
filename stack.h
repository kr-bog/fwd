#ifndef STACK_H
#define STACK_H

#include "fwd_container.h"
#include <stdexcept>
#include <utility>

template <typename T>
class stack : public fwd_container<T> {
    // узел списка
    struct Node {
        T data;
        Node* next;
        Node(const T& v, Node* n = nullptr): data(v), next(n) {}
        Node(T&& v, Node* n = nullptr): data(std::move(v)), next(n) {}
    };

    Node* top_;         // указатель на верхний элеме
    std::size_t sz_;    //количество элементов в контейнере

public:
    using iterator = typename fwd_container<T>::iterator;
    using const_iterator = typename fwd_container<T>::const_iterator;
    using iterator_base = typename fwd_container<T>::iterator_base;
    using const_iterator_base = typename fwd_container<T>::const_iterator_base;

    class stack_const_iterator;

    // итератор стека
    class stack_iterator : public iterator_base {
        Node* cur;
        friend class stack_const_iterator;
    public:
        stack_iterator(Node* n = nullptr);

        typename iterator_base::reference operator*() override;
        typename iterator_base::pointer operator->() override;
        stack_iterator& operator++() override;

        //сравнения
        bool operator==(const iterator_base& o) const override;
        bool operator!=(const iterator_base& o) const override;
        bool operator==(const const_iterator_base& o) const override;
        bool operator!=(const const_iterator_base& o) const override;

    protected:
        iterator_base* clone() const override;             //копия итератора
        const_iterator_base* make_const() const override;  //создание константного итератора
    };

    // константный итератор стека
    class stack_const_iterator : public const_iterator_base {
        const Node* cur;
        friend class stack_iterator;
    public:
        stack_const_iterator(const Node* n = nullptr);        //конструктор
        stack_const_iterator(const stack_iterator& o);        //конструирование из обычного итератора

        typename const_iterator_base::reference operator*() const override;
        typename const_iterator_base::pointer operator->() const override;
        stack_const_iterator& operator++() override;   //шаг итератора

        // сравнения
        bool operator==(const const_iterator_base& o) const override;
        bool operator!=(const const_iterator_base& o) const override;
        bool operator==(const iterator_base& o) const override;
        bool operator!=(const iterator_base& o) const override;

    protected:
        const_iterator_base* clone() const override;   //копия итератора
    };

    // конструкторы
    stack();                                //создаём пустой стек
    ~stack() override;                      //деструктор
    stack(const stack& o);                  //копирующий конструктор
    stack(stack&& o);                       //перемещающий конструктор
    stack& operator=(const stack& o);       // копирующее присваивание
    stack& operator=(stack&& o);            //перемещающее присваивание
    fwd_container<T>& operator=(const fwd_container<T>& o) override;  //присваивание через баз

    //добавление элемента
    void push(const T& v) override;         //вставка копированием в вершину стека
    void push(T&& v) override;              //вставка перемещением в вершину стека

    // удаление элемента
    T pop() override;

    // доступ к верхнему элементу
    T& get_front() override;
    const T& get_front() const override;

    //пустой
    bool is_empty() const override;
    std::size_t size() const override;

    //итераторы
    iterator begin() override;
    iterator end() override;
    const_iterator begin() const override;
    const_iterator end() const override;
    const_iterator cbegin() const override;
    const_iterator cend() const override;

private:
    void clear();                           //очистка стека
    void copy_from(const stack& o);         //копирование элементов другого стека за один проход
};

#include "stack_impl.h"

#endif
