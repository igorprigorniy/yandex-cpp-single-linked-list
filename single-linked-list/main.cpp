#include <iostream>
#include "single-linked-list.h"
#include "unit_tests.h"
using namespace std;

// Подопытная структура для создания листа из таких структур
struct Point {
    int x = 0;
    int y = 0;
};

// Перегрузка оператора << для вывода структуры Point
ostream& operator << (ostream& os, const Point& p) {
    os << "("s << p.x << ", "s << p.y << ")"s;
    return os;
}

// Тестирование и демонстрация работы списка
int main() {
    unit_tests::RunTestSingleLinkedList();

    {
        SingleLinkedList<Point> mylist = { {1, 2}, {3, 4}, {5, 6}, {7, 8} };
        cout << mylist << endl;

        mylist.PopFront();
        mylist.PopBack();
        cout << mylist << endl;

        mylist.PushFront({ -1, -2 });
        mylist.PushBack({ -7, -8 });
        cout << mylist << endl;

        const auto it = mylist.InsertAfter(++mylist.begin(), { 10, 20 });
        cout << mylist << endl;

        mylist.EraseAfter(it);
        cout << mylist << endl;
    }

    return 0;
}