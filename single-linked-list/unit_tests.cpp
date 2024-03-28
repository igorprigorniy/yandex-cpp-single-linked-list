#include <stdexcept>
#include "unit_tests.h"
#include "unit_tests_framework.h"
#include "single-linked-list.h"
using namespace std;

// Пространство имён для Unit-тестов
namespace unit_tests {

// Структура подопытного объекта, который инкрементирует счётчик по указателю
// (если он установлен) при вызове своего деструктора
struct DeletionSpy {
    ~DeletionSpy() {
        if (deletion_counter_ptr) {
            ++(*deletion_counter_ptr);
        }
    }

    // Указатель на счётчик 
    int* deletion_counter_ptr = nullptr;
};

// Структура подопытного объекта, который выдёт исключение после создания N-ой копии
struct ThrowOnCopy {
    ThrowOnCopy() = default;

    explicit ThrowOnCopy(int& copy_counter) noexcept : countdown_ptr(&copy_counter) { }

    ThrowOnCopy(const ThrowOnCopy& other) : countdown_ptr(other.countdown_ptr)
    {
        if (countdown_ptr) {
            if (*countdown_ptr == 0) throw std::bad_alloc();
            else --(*countdown_ptr);
        }
    }

    // Присваивание элементов этого типа не требуется
    ThrowOnCopy& operator=(const ThrowOnCopy& rhs) = delete;

    // Указатель на счётчик обратного отсчёта. Если он не равен nullptr, то уменьшается при каждом копировании.
    // Как только счётчик обнулится, конструктор копирования выбросит исключение
    int* countdown_ptr = nullptr;
};

// UNIT-TEST: Проверка PopFront
void UnitTestPopFront() {
    SingleLinkedList<int> numbers{ 3, 14, 15, 92, 6 };
    numbers.PopFront();
    ASSERT((numbers == SingleLinkedList<int>{14, 15, 92, 6}));

    SingleLinkedList<DeletionSpy> list;
    list.PushFront(DeletionSpy{});
    int deletion_counter = 0;
    list.begin()->deletion_counter_ptr = &deletion_counter;
    ASSERT_EQUAL(deletion_counter, 0);
    list.PopFront();
    ASSERT_EQUAL(deletion_counter, 1);
}

// UNIT-TEST: Проверка доступа к позиции, предшествующей begin
void UnitTestBeforeBegin() {
    SingleLinkedList<int> empty_list;
    const auto& const_empty_list = empty_list;
    ASSERT_EQUAL(empty_list.before_begin(), empty_list.cbefore_begin());
    ASSERT_EQUAL(++empty_list.before_begin(), empty_list.begin());
    ASSERT_EQUAL(++empty_list.cbefore_begin(), const_empty_list.begin());

    SingleLinkedList<int> numbers{ 1, 2, 3, 4 };
    const auto& const_numbers = numbers;
    ASSERT_EQUAL(numbers.before_begin(), numbers.cbefore_begin());
    ASSERT_EQUAL(++numbers.before_begin(), numbers.begin());
    ASSERT_EQUAL(++numbers.cbefore_begin(), const_numbers.begin());
}

// UNIT-TEST: Проверка InsertAfter
void UnitTestInsertAfter() {
    // Вставка в пустой список
    {
        SingleLinkedList<int> lst;
        const auto inserted_item_pos = lst.InsertAfter(lst.before_begin(), 123);
        ASSERT((lst == SingleLinkedList<int>{123}));
        ASSERT_EQUAL(inserted_item_pos, lst.begin());
        ASSERT_EQUAL(*inserted_item_pos, 123);
    }

    // Вставка в непустой список
    {
        SingleLinkedList<int> lst{ 1, 2, 3 };
        auto inserted_item_pos = lst.InsertAfter(lst.before_begin(), 123);

        ASSERT_EQUAL(inserted_item_pos, lst.begin());
        ASSERT_NOT_EQUAL(inserted_item_pos, lst.end());
        ASSERT_EQUAL(*inserted_item_pos, 123);
        ASSERT((lst == SingleLinkedList<int>{123, 1, 2, 3}));

        inserted_item_pos = lst.InsertAfter(lst.begin(), 555);
        ASSERT_EQUAL(++SingleLinkedList<int>::Iterator(lst.begin()), inserted_item_pos);
        ASSERT_EQUAL(*inserted_item_pos, 555);
        ASSERT((lst == SingleLinkedList<int>{123, 555, 1, 2, 3}));
    };
}

// UNIT-TEST: Проверка обеспечения строгой гарантии безопасности исключений
void UnitTestNoExcept() {
    bool exception_was_thrown = false;
    for (int max_copy_counter = 10; max_copy_counter >= 0; --max_copy_counter) {
        SingleLinkedList<ThrowOnCopy> list{ ThrowOnCopy{}, ThrowOnCopy{}, ThrowOnCopy{} };
        try {
            int copy_counter = max_copy_counter;
            list.InsertAfter(list.cbegin(), ThrowOnCopy(copy_counter));
            ASSERT_EQUAL(list.GetSize(), 4u);
        }
        catch (const std::bad_alloc&) {
            exception_was_thrown = true;
            ASSERT_EQUAL(list.GetSize(), 3u);
            break;
        }
    }
    ASSERT(exception_was_thrown);
}

// UNIT-TEST: Проверка EraseAfter
void UnitTestEraseAfter() {
    {
        SingleLinkedList<int> lst{ 1, 2, 3, 4 };
        const auto& const_lst = lst;
        const auto item_after_erased = lst.EraseAfter(const_lst.cbefore_begin());
        ASSERT((lst == SingleLinkedList<int>{2, 3, 4}));
        ASSERT_EQUAL(item_after_erased, lst.begin());
    }

    {
        SingleLinkedList<int> lst{ 1, 2, 3, 4 };
        const auto item_after_erased = lst.EraseAfter(lst.cbegin());
        ASSERT((lst == SingleLinkedList<int>{1, 3, 4}));
        ASSERT_EQUAL(item_after_erased, (++lst.begin()));
    }

    {
        SingleLinkedList<int> lst{ 1, 2, 3, 4 };
        const auto item_after_erased = lst.EraseAfter(++(++lst.cbegin()));
        ASSERT((lst == SingleLinkedList<int>{1, 2, 3}));
        ASSERT_EQUAL(item_after_erased, lst.end());
    }

    {
        SingleLinkedList<DeletionSpy> list{ DeletionSpy{}, DeletionSpy{}, DeletionSpy{} };
        auto after_begin = ++list.begin();
        int deletion_counter = 0;
        after_begin->deletion_counter_ptr = &deletion_counter;
        ASSERT_EQUAL(deletion_counter, 0u);
        list.EraseAfter(list.cbegin());
        ASSERT_EQUAL(deletion_counter, 1u);
    }
}

// Запуск Unit-тестов для односвязного списка
void RunTestSingleLinkedList() {
    RUN_TEST(UnitTestPopFront);
    RUN_TEST(UnitTestBeforeBegin);
    RUN_TEST(UnitTestInsertAfter);
    RUN_TEST(UnitTestNoExcept);
    RUN_TEST(UnitTestEraseAfter);
}

}