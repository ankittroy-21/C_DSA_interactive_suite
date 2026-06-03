#include "data_structures.h"
#include <assert.h>
#include <stdio.h>

void test_deque_init()
{
    deque dq;
    assert(init_deque(5, &dq) == 1);
    assert(dq.N == 5);
    assert(dq.front == -1);
    assert(dq.rear == -1);
    assert(deque_is_empty(&dq) == true);
    assert(deque_is_full(&dq) == false);
    destroy_deque(&dq);

    printf("Deque init test passed\n");
}

void test_deque_basic_ops()
{
    deque dq;
    init_deque(5, &dq);

    assert(deque_insert_rear(&dq, 10) == 1);
    assert(deque_insert_rear(&dq, 20) == 1);
    assert(deque_insert_front(&dq, 5) == 1);

    assert(deque_get_front(&dq) == 5);
    assert(deque_get_rear(&dq) == 20);

    assert(deque_delete_front(&dq) == 5);
    assert(deque_delete_rear(&dq) == 20);
    assert(deque_delete_front(&dq) == 10);

    assert(deque_is_empty(&dq) == true);
    destroy_deque(&dq);

    printf("Deque basic ops test passed\n");
}

void test_deque_underflow()
{
    deque dq;
    init_deque(5, &dq);

    assert(deque_delete_front(&dq) == -1);
    assert(deque_delete_rear(&dq) == -1);
    assert(deque_get_front(&dq) == -1);
    assert(deque_get_rear(&dq) == -1);

    destroy_deque(&dq);
    printf("Deque underflow test passed\n");
}

void test_deque_overflow()
{
    deque dq;
    init_deque(3, &dq);

    assert(deque_insert_rear(&dq, 1) == 1);
    assert(deque_insert_front(&dq, 2) == 1);
    assert(deque_insert_rear(&dq, 3) == 1);
    assert(deque_is_full(&dq) == true);

    assert(deque_insert_front(&dq, 4) == -1);
    assert(deque_insert_rear(&dq, 5) == -1);

    destroy_deque(&dq);
    printf("Deque overflow test passed\n");
}

void test_deque_wraparound()
{
    deque dq;
    init_deque(3, &dq);

    // Front: -1, Rear: -1
    assert(deque_insert_rear(&dq, 1) == 1); // F: 0, R: 0 [1]
    assert(deque_insert_rear(&dq, 2) == 1); // F: 0, R: 1 [1, 2]
    assert(deque_delete_front(&dq) == 1);   // F: 1, R: 1 [2]

    assert(deque_insert_front(&dq, 3) == 1); // F: 0, R: 1 [3, 2]
    assert(deque_insert_front(&dq, 4) == 1); // F: 2, R: 1 [3, 2] (under the hood F wraps to N-1 which is 2) [4 at idx 2, 3 at idx 0, 2 at idx 1]
    assert(deque_is_full(&dq) == true);

    assert(deque_get_front(&dq) == 4);
    assert(deque_get_rear(&dq) == 2);

    assert(deque_delete_rear(&dq) == 2);   // F: 2, R: 0
    assert(deque_delete_front(&dq) == 4);  // F: 0, R: 0
    assert(deque_delete_front(&dq) == 3);  // F: -1, R: -1

    assert(deque_is_empty(&dq) == true);

    destroy_deque(&dq);
    printf("Deque wraparound test passed\n");
}

int main()
{
    test_deque_init();
    test_deque_basic_ops();
    test_deque_underflow();
    test_deque_overflow();
    test_deque_wraparound();

    printf("All Deque tests passed\n");
    return 0;
}
