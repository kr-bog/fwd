#ifndef QUEUE_H
#define QUEUE_H

#include "fwd_container.h"
#include <stdexcept>
#include <utility>

template <typename T>
//контейнер queue - потомок fwd_container
class queue : public fwd_container<T> {
    // узел списка
    struct Node {
        T data;             //данные узла
        Node* next;         //указатель на следущий узел
        Node(const T& v, Node* n = nullptr): data(v), next(n) {}            //конструктор копирование
        Node(T&& v, Node* n = nullptr): data(std::move(v)), next(n) {}      //конструктор перемещение
    };

    Node* front_;           //указатель на 1 элемент
    Node* back_;            //последний
    std::size_t sz_;        //количество элементов в контейнере

public:
    //сокращам имена
    using iterator = typename fwd_container<T>::iterator;                           //итератор принадлежит кнт с типом данных Т
    using const_iterator = typename fwd_container<T>::const_iterator;
    using iterator_base = typename fwd_container<T>::iterator_base;                 //абстрактный итератор
    using const_iterator_base = typename fwd_container<T>::const_iterator_base;

    class queue_const_iterator;

    // итератор очереди
    class queue_iterator : public iterator_base {
        Node* cur;                                  // текущий узел итератора
        friend class queue_const_iterator;          //дружеский класс
    public:
        queue_iterator(Node* n = nullptr);          //конструктор с указанием узла

        typename iterator_base::reference operator*() override;     //доступ к элементу
        typename iterator_base::pointer operator->() override;      //доступ к элементу черещ указатель
        queue_iterator& operator++() override;

        //сравнения
        bool operator==(const iterator_base& o) const override;
        bool operator!=(const iterator_base& o) const override;
        bool operator==(const const_iterator_base& o) const override;
        bool operator!=(const const_iterator_base& o) const override;

    protected:
        iterator_base* clone() const override;              //копия итератора
        const_iterator_base* make_const() const override;   //создания конст итератора
    };

    // константный итератор очереди
    class queue_const_iterator : public const_iterator_base {
        const Node* cur;
        friend class queue_iterator;
    public:
        queue_const_iterator(const Node* n = nullptr);
        queue_const_iterator(const queue_iterator& o);

        typename const_iterator_base::reference operator*() const override;
        typename const_iterator_base::pointer operator->() const override;
        queue_const_iterator& operator++() override;

        //сравнения
        bool operator==(const const_iterator_base& o) const override;
        bool operator!=(const const_iterator_base& o) const override;
        bool operator==(const iterator_base& o) const override;
        bool operator!=(const iterator_base& o) const override;

    protected:
        const_iterator_base* clone() const override;
    };

    // конструкторы
    queue();                                //созданет пустую очередь
    ~queue() override;                      //диструктор
    queue(const queue& o);                  //копирующий конструктор
    queue(queue&& o);                       //перемещающий конструктор
    queue& operator=(const queue& o);       //копирующее присваивание
    queue& operator=(queue&& o);            //перемещающее присваивание

    //присваивание через баз
    fwd_container<T>& operator=(const fwd_container<T>& o) override;

    // добавление в конец
    void push(const T& v) override;         //вставка копированием в конец
    void push(T&& v) override;              //вставка перемещением в конец

    // удаление
    T pop() override;                       //удаление из начала

    //доступ к первому элементу
    T& get_front() override;                //получить ссылку на 1 э
    const T& get_front() const override;    //получить конст ссылку на 1 э

    //пустой
    bool is_empty() const override;
    std::size_t size() const override;

    // итераторы
    iterator begin() override;
    iterator end() override;
    const_iterator begin() const override;
    const_iterator end() const override;
    const_iterator cbegin() const override;
    const_iterator cend() const override;

private:
    void clear();                           //очистка очереди
    void copy_from(const queue& o);         //копирование эл из другой оч
};

#include "queue_impl.h"

#endif
