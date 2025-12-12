#include <gtest/gtest.h>
#include <sstream>
#include <algorithm>
#include <string>
#include "fwd_container.h"
#include "stack.h"
#include "queue.h"

//тесты стека
//проверка итераторов
TEST(StackTest, Stack_Iterator)
{
    stack<int> s;
    s.push(10);
    s.push(20);
    s.push(30);                     //стек 30 -> 20 -> 10

    stack<int>::const_iterator cit = s.cbegin(), ocit;      // конст итератор
    EXPECT_EQ(*cit, 30);            //разыменование - верх стека
    ocit = ++cit;                   //префиксный инкремент
    EXPECT_EQ(*cit, 20);
    EXPECT_EQ(*ocit, 20);
    ocit = cit++;
    EXPECT_EQ(*cit, 10);
    EXPECT_EQ(*ocit, 20);
    ++cit;
    EXPECT_EQ(cit, s.cend());

    const stack<int>& r = s;        //конст ссылка
    cit = r.begin();                //begin() от конст возвращает const_iterator
    EXPECT_EQ(*cit, 30);
    ++cit;
    EXPECT_EQ(*cit, 20);
    cit++;
    EXPECT_EQ(*cit, 10);
    ++cit;
    EXPECT_EQ(cit, r.end());

    stack<int>::iterator it = s.begin(), oit;       //неконст итератор
    EXPECT_EQ(*it, 30);
    oit = ++it;
    EXPECT_EQ(*it, 20);
    EXPECT_EQ(*oit, 20);
    oit = it++;
    EXPECT_EQ(*it, 10);
    EXPECT_EQ(*oit, 20);
    *oit = 5;
    EXPECT_EQ(*it, 10);
    EXPECT_EQ(*oit, 5);
    ++it;
    EXPECT_EQ(it, s.end());

    std::stringstream sout;
    sout << s;
    EXPECT_EQ(sout.str(), "30 5 10");
}

// проверка range-based for
TEST(StackTest, Stack_For)
{
    stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);

    const stack<int>& r = s;
    int expected1[] = {3, 2, 1};
    int idx = 0;
    for (auto& v : r) EXPECT_EQ(v, expected1[idx++]);

    int expected2[] = {6, 4, 2};
    idx = 0;
    for (auto& v : s) v *= 2;       // изменение через for

    idx = 0;
    for (const auto& v : s) EXPECT_EQ(v, expected2[idx++]);

    int expected3[] = {7, 5, 3};

    for (stack<int>::iterator it = s.begin(); it != s.end(); ++it) {
        *it += 1;
    }
    idx = 0;
    for (stack<int>::const_iterator it = s.begin(); it != s.end(); ++it)
        EXPECT_EQ(*it, expected3[idx++]);
    idx = 0;
    for (stack<int>::const_iterator it = r.begin(); it != r.end(); ++it)
        EXPECT_EQ(*it, expected3[idx++]);
    idx = 0;
    for (stack<int>::const_iterator it = s.cbegin(); it != s.cend(); ++it)
        EXPECT_EQ(*it, expected3[idx++]);
}

// проверка push, pop, копирования, перемещения
TEST(StackTest, Stack_PushPopCopy)
{
    stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);

    s.pop();
    s.push(10);
    s.push(20);

    stack<int> copy_s(s);           //конструктор копирования

    int expected_orig[] = {20, 10, 2, 1};
    int idx = 0;
    for (auto v : copy_s) EXPECT_EQ(v, expected_orig[idx++]);

    copy_s.pop();
    copy_s.pop();
    copy_s.push(99);
    copy_s.push(98);
    copy_s.push(97);

    idx = 0;
    for (auto v : s) EXPECT_EQ(v, expected_orig[idx++]);

    int expected_copy[] = {97, 98, 99, 2, 1};
    idx = 0;
    for (auto v : copy_s) EXPECT_EQ(v, expected_copy[idx++]);

    stack<int> moved_s(std::move(copy_s));      // конструктор перемещения

    int expected_moved[] = {97, 98, 99, 2, 1};
    idx = 0;
    for (auto v : moved_s) EXPECT_EQ(v, expected_moved[idx++]);

    EXPECT_TRUE(copy_s.empty());    //после move пустой

    stack<int> s2;
    s2 = s;
    idx = 0;
    for (auto v : s2) EXPECT_EQ(v, expected_orig[idx++]);

    s2.pop();
    s2.push(42);
    s2.push(99);

    idx = 0;
    for (auto v : s) EXPECT_EQ(v, expected_orig[idx++]);    //не изменился

    int expected_copy_asgn[] = {99, 42, 10, 2, 1};
    idx = 0;
    for (auto v : s2) EXPECT_EQ(v, expected_copy_asgn[idx++]);

    stack<int> s3;
    s3 = std::move(s2);             //присваивание перемещением
    idx = 0;
    for (auto v : s3) EXPECT_EQ(v, expected_copy_asgn[idx++]);

    EXPECT_TRUE(s2.empty());

    s3.pop();
    s3.push(77);
    int expected_final[] = {77, 42, 10, 2, 1};
    idx = 0;
    for (auto v : s3) EXPECT_EQ(v, expected_final[idx++]);
}

//проверка потокового ввода/вывода
TEST(StackTest, Stack_IO)
{
    stack<int> s;
    s.push(0);

    std::stringstream sin("1 2 3 4 5");
    sin >> s;                       //читаем через push

    EXPECT_EQ(s.size(), 6);

    int expected[] = {5, 4, 3, 2, 1, 0};
    int idx = 0;
    for (auto v : s) EXPECT_EQ(v, expected[idx++]);

    std::stringstream sout;
    sout << s;                      // выводим в поток

    std::string expected_str = "5 4 3 2 1 0";
    EXPECT_EQ(sout.str(), expected_str);

    for (auto& v : s) v += 10;

    std::stringstream sout2;
    sout2 << s;
    std::string expected_str2 = "15 14 13 12 11 10";
    EXPECT_EQ(sout2.str(), expected_str2);
}

//проверка работы со стандартными алгоритмами
TEST(StackTest, Stack_Algs)
{
    stack<int> s;

    for (int i = 1; i <= 5; ++i) s.push(i);

    auto it = std::find_if(s.begin(), s.end(), [](int v){ return v % 2 == 0; });
    EXPECT_EQ(*it, 4);              //нашли первое четное
    *it = 3;                        //изменили

    const stack<int>& r = s;
    auto cit = std::find_if(r.begin(), r.end(), [](int v){ return v % 2 == 0; });
    EXPECT_EQ(*cit, 2);             //теперь первое четное - 2

    it = std::find_if(s.begin(), s.end(), [](int v){ return v == 0; });
    EXPECT_EQ(it, s.end());         //не нашли - вернул end

    auto count_even = std::count_if(s.begin(), s.end(), [](int v){ return v % 2 == 0; });
    EXPECT_EQ(count_even, 1);       //посчитали четные

    std::replace_if(s.begin(), s.end(), [](int v){ return v % 2 != 0; }, 99);   // заменили нечетные
    int expected_replace[] = {99, 99, 99, 2, 99};
    int idx = 0;
    for (auto v : s) EXPECT_EQ(v, expected_replace[idx++]);

    std::for_each(s.begin(), s.end(), [](int& v){ v += 1; });   //прибавили 1 к каждому
    int expected_for_each[] = {100, 100, 100, 3, 100};
    idx = 0;
    for (auto v : s) EXPECT_EQ(v, expected_for_each[idx++]);
}

// проверка полиморфизма через базовый класс
TEST(ContainerTest, BaseContainer)
{
    stack<int> s;
    queue<int> q;

    s.push(1);
    s.push(2);
    s.push(3);                      // стек: 3 -> 2 -> 1
    q.push(10);
    q.push(20);
    q.push(30);                     // очередь: 10 -> 20 -> 30

    fwd_container<int>& bs = s;     //ссылка на базовый класс
    fwd_container<int>& bq = q;
    const fwd_container<int>& cs = s;
    const fwd_container<int>& cq = q;
    (void)cq;

    bs = q;                         //присваивание через базовый класс

    int expected_s_after[] = {10, 20, 30};
    int idx = 0;
    for (auto v : s) EXPECT_EQ(v, expected_s_after[idx++]);

    bq = s;
    int expected_q_after[] = {30, 20, 10};
    idx = 0;
    for (auto v : q) EXPECT_EQ(v, expected_q_after[idx++]);

    idx = 0;
    for (auto& it : bs) EXPECT_EQ(it, expected_s_after[idx++]);     //итерация через базовый

    idx = 0;
    for (const auto& it : bs) EXPECT_EQ(it, expected_s_after[idx++]);

    idx = 0;
    for (auto& it : cs) EXPECT_EQ(it, expected_s_after[idx++]);

    idx = 0;
    for (auto& it : bq) EXPECT_EQ(it, expected_q_after[idx++]);

    idx = 0;
    for (const auto& it : bq) EXPECT_EQ(it, expected_q_after[idx++]);

    idx = 0;
    for (auto& it : bq) EXPECT_EQ(it, expected_q_after[idx++]);
}

//тесты очереди

TEST(QueueTest, Queue_Iterator)
{
    queue<int> q;
    q.push(10); q.push(20); q.push(30);

    queue<int>::const_iterator cit = q.cbegin();
    EXPECT_EQ(*cit, 10);            // первый элемент
    ++cit; EXPECT_EQ(*cit, 20);
    cit++; EXPECT_EQ(*cit, 30);
    ++cit; EXPECT_EQ(cit, q.cend());

    queue<int>::iterator it = q.begin();
    EXPECT_EQ(*it, 10);
    ++it; *it = 5;                  //меняем 20 на 5
    std::stringstream sout;
    sout << q;
    EXPECT_EQ(sout.str(), "10 5 30");
}

TEST(QueueTest, Queue_For)
{
    queue<int> q;
    q.push(1); q.push(2); q.push(3);

    int expected1[] = {1, 2, 3};
    int idx = 0;
    for (auto& v : q) { EXPECT_EQ(v, expected1[idx++]); v *= 2; }

    int expected2[] = {2, 4, 6};
    idx = 0;
    for (const auto& v : q) EXPECT_EQ(v, expected2[idx++]);

    for (queue<int>::iterator it = q.begin(); it != q.end(); ++it) *it += 1;
    int expected3[] = {3, 5, 7};
    idx = 0;
    for (queue<int>::const_iterator it = q.cbegin(); it != q.cend(); ++it)
        EXPECT_EQ(*it, expected3[idx++]);
}

TEST(QueueTest, Queue_PushPopCopy)
{
    queue<int> q;
    q.push(1); q.push(2); q.push(3);
    q.pop();                        //убрали 1
    q.push(10);                     //очередь: 2 -> 3 -> 10

    queue<int> copy(q);             //копирование
    EXPECT_EQ(copy.size(), 3);

    int expected[] = {2, 3, 10};
    int idx = 0;
    for (auto v : copy) EXPECT_EQ(v, expected[idx++]);

    queue<int> moved(std::move(copy));      //перемещение
    EXPECT_TRUE(copy.empty());
    EXPECT_EQ(moved.size(), 3);

    queue<int> q2;
    q2 = q;
    q2.push(99);
    EXPECT_EQ(q.size(), 3);         //оригинал не изменился
    EXPECT_EQ(q2.size(), 4);
}

TEST(QueueTest, Queue_IO)
{
    queue<int> q;
    q.push(0);
    std::stringstream sin("1 2 3");
    sin >> q;

    EXPECT_EQ(q.size(), 4);
    std::stringstream sout;
    sout << q;
    EXPECT_EQ(sout.str(), "0 1 2 3");
}

TEST(QueueTest, Queue_Algs)
{
    queue<int> q;
    for (int i = 1; i <= 5; ++i) q.push(i);

    auto it = std::find_if(q.begin(), q.end(), [](int v){ return v % 2 == 0; });
    EXPECT_EQ(*it, 2);              // первое четное

    auto count = std::count_if(q.begin(), q.end(), [](int v){ return v > 3; });
    EXPECT_EQ(count, 2);            // 4 и 5

    std::for_each(q.begin(), q.end(), [](int& v){ v *= 2; });
    int expected[] = {2, 4, 6, 8, 10};
    int idx = 0;
    for (auto v : q) EXPECT_EQ(v, expected[idx++]);
}

// тесты для string (проверка шаблона)

TEST(StackStringTest, Stack_Iterator)
{
    stack<std::string> s;
    s.push("a"); s.push("b"); s.push("c");

    stack<std::string>::const_iterator cit = s.cbegin();
    EXPECT_EQ(*cit, "c");
    ++cit; EXPECT_EQ(*cit, "b");
    ++cit; EXPECT_EQ(*cit, "a");

    stack<std::string>::iterator it = s.begin();
    *it = "x";                      // меняем c на x
    std::stringstream sout;
    sout << s;
    EXPECT_EQ(sout.str(), "x b a");
}

TEST(StackStringTest, Stack_For)
{
    stack<std::string> s;
    s.push("a"); s.push("b"); s.push("c");

    std::string expected[] = {"c", "b", "a"};
    int idx = 0;
    for (auto& v : s) { EXPECT_EQ(v, expected[idx++]); v += "!"; }

    std::string expected2[] = {"c!", "b!", "a!"};
    idx = 0;
    for (const auto& v : s) EXPECT_EQ(v, expected2[idx++]);
}

TEST(StackStringTest, Stack_PushPopCopy)
{
    stack<std::string> s;
    s.push("one"); s.push("two"); s.push("three");
    s.pop();                        // убрали three

    stack<std::string> copy(s);
    EXPECT_EQ(copy.size(), 2);

    stack<std::string> moved(std::move(copy));
    EXPECT_TRUE(copy.empty());

    stack<std::string> s2;
    s2 = s;
    s2.push("new");
    EXPECT_EQ(s.size(), 2);         // оригинал не изменился
    EXPECT_EQ(s2.size(), 3);
}

TEST(StackStringTest, Stack_IO)
{
    stack<std::string> s;
    std::stringstream sin("hello world");
    sin >> s;

    EXPECT_EQ(s.size(), 2);
    std::stringstream sout;
    sout << s;
    EXPECT_EQ(sout.str(), "world hello");
}

TEST(StackStringTest, Stack_Algs)
{
    stack<std::string> s;
    s.push("aa"); s.push("bbb"); s.push("c");   // стек: c -> bbb -> aa

    auto it = std::find_if(s.begin(), s.end(), [](const std::string& v){ return v.length() > 2; });
    EXPECT_EQ(*it, "bbb");          //нашли строку длиннее 2

    auto count = std::count_if(s.begin(), s.end(), [](const std::string& v){ return v.length() < 3; });
    EXPECT_EQ(count, 2);            // c и aa
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();}
