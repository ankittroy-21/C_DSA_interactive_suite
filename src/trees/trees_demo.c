#include "trees.h"
#include "safe_input.h"
#include <stdio.h>

void trees_demo(void)
{
    int tree_status, tree_choice;

    while (1)
    {
        tree_status = safe_input_int(
            &tree_choice,
            "\nenter 1 for Binary Search Tree demo"
            "\nenter 2 for AVL Tree demo"
            "\nenter 3 for Threaded Binary Tree demo"
            "\nenter 4 for Trie demo"
            "\nenter choice : ",
            1, 4
        );

        if (tree_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting trees_demo....\n");
            return;
        }

        if (tree_status == 0)
            continue;

        switch (tree_choice)
        {
            case 1:
                binary_search_tree_Demo();
                break;

            case 2:
                avl_demo();
                break;

            case 3:
                TBT_demo();
                break;

            case 4:
                trie_demo();
                break;
        }
    }
}