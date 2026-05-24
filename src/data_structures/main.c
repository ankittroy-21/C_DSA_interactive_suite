#include "advanced_sorting.h"
#include "data_structures.h"
/*
data_structures.h includes the following,

Array related: swap_adjacent_pairs(), max_array(), min_array(), print_array(), reverse_array(), array_demo(),
BST related:  bstNode structure, binary_search_tree_Demo(), bst_insert(), bst_inorder(), bst_preorder(), bst_postorder(), countNodes(), tree_height(), destroy_bst(),
Circular Queue related: circular_queue structure, init_circ_queue(), destroy_circ_queue(), enqueue(), dequeue(), display_circ_queue(), circular_queue_Demo(),
Double Linked List related:  doubly_ll_node structure, dll_insertAtBeginning(), dll_insertAtEnd(), dll_printlist(), dll_search(), dll_deleteAtBeginning(), dll_deleteAtEnd(), dll_deleteByValue(), delete_dll(), dll_demo(), dll_reverselist(),
Single Linked List related:  Node structure, sll_printlist(), sll_insertAtEnd(), sll_deleteAtBeginning(), sll_deleteAtEnd(), sll_deleteByValue(), sll_insertAtBeginning(), sll_Demo(), sll_search(), sll_reverseList(), delete_sll()
*/
#include "expression.h"
#include "graph_traversals.h"
#include "hash.h"
#include "safe_input.h"
#include "searching_algorithms.h"
#include "sorting_algorithms_n2.h"
#include "stack.h"
#include <stdio.h>

void data_structures_demo(void);

// only give input as integer value as prompted through the console by programmer. dont attempt to
// put any other type of value neglecting this warning can result in undefined behaviour

int main()
{

    while (1)
    {
        int choice;
        int status = safe_input_int(
            &choice, // variable in which value is to be inserted
            "\nWelcome to DSA library built by Darshan Mukul Parekh"
            "\n(at any point enter '-1' to exit that particular demo)\n\n"
            "click 1 for data structures demo\n"
            "click 2 for expression evaluation (infix to postfix and postfix evaluation) demo\n"
            "click 3 for sorting algorithms (the n^2 family) demo\n"
            "click 4 for advanced sorting algorithms demo\n"
            "click 5 for searching algorithms demo\n"
            "click 6 for graph traversals (bfs/dfs) demo\n"
            "click 7 for hashing algorithms demo\n"
            "enter choice : ",
            1, 7 // limits
        );

        if (status == -111)
        {
            break;
        }

        if (status == 0)
        {
            continue;
        }

        switch (choice)
        {
            case 1:
                data_structures_demo();
                break;
            case 2:
                expression_evaluation_demo();
                break;
            case 3:
                sorting_algorithms_n2_demo();
                break;
            case 4:
                advanced_sorting_demo();
                break;
            case 5:
                searching_algorithms_demo();
                break;
            case 6:
                graph_traversals_demo();
                break;
            case 7:
                hashing_algorithms_demo();
                break;
        }
    }

    return 0;
}