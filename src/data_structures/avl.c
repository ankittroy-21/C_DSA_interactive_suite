#include "data_structures.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>

int avl_height(const avlNode* node)
{
    if (node == NULL)
        return 0;
    return node->height;
}

int avl_max(int a, int b)
{
    return (a > b) ? a : b;
}

int avl_balance_factor(const avlNode* node)
{
    if (node == NULL)
        return 0;
    return avl_height(node->left) - avl_height(node->right);
}

static avlNode* right_rotate(avlNode* y)
{
    avlNode* x = y->left;
    avlNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = avl_max(avl_height(y->left), avl_height(y->right)) + 1;
    x->height = avl_max(avl_height(x->left), avl_height(x->right)) + 1;

    return x;
}

static avlNode* left_rotate(avlNode* x)
{
    avlNode* y = x->right;
    avlNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = avl_max(avl_height(x->left), avl_height(x->right)) + 1;
    y->height = avl_max(avl_height(y->left), avl_height(y->right)) + 1;

    return y;
}

static avlNode* avl_insert_helper(avlNode* node, int value, int* status)
{
    if (node == NULL)
    {
        avlNode* new_node = malloc(sizeof(avlNode));
        if (new_node == NULL)
        {
            *status = -1;
            return NULL;
        }
        new_node->data = value;
        new_node->height = 1;
        new_node->left = NULL;
        new_node->right = NULL;
        *status = 1;
        return new_node;
    }

    if (value == node->data)
    {
        *status = 0;
        return node;
    }

    if (value < node->data)
    {
        node->left = avl_insert_helper(node->left, value, status);
    }
    else
    {
        node->right = avl_insert_helper(node->right, value, status);
    }

    if (*status != 1)
        return node;

    node->height = avl_max(avl_height(node->left), avl_height(node->right)) + 1;

    int balance = avl_balance_factor(node);

    if (balance > 1 && value < node->left->data)
        return right_rotate(node);

    if (balance < -1 && value > node->right->data)
        return left_rotate(node);

    if (balance > 1 && value > node->left->data)
    {
        node->left = left_rotate(node->left);
        return right_rotate(node);
    }

    if (balance < -1 && value < node->right->data)
    {
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }

    return node;
}

int avl_insert(avlNode** root_ref, int value)
{
    int status = 0;
    *root_ref = avl_insert_helper(*root_ref, value, &status);
    return status;
}

static avlNode* avl_delete_helper(avlNode* root, int value, int* status)
{
    if (root == NULL)
    {
        *status = 0;
        return NULL;
    }

    if (value < root->data)
    {
        root->left = avl_delete_helper(root->left, value, status);
    }
    else if (value > root->data)
    {
        root->right = avl_delete_helper(root->right, value, status);
    }
    else
    {
        *status = 1;
        if ((root->left == NULL) || (root->right == NULL))
        {
            avlNode* temp = root->left ? root->left : root->right;

            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else
            {
                *root = *temp;
            }
            free(temp);
        }
        else
        {
            avlNode* temp = root->right;
            while (temp->left != NULL)
                temp = temp->left;

            root->data = temp->data;

            int dummy_status;
            root->right = avl_delete_helper(root->right, temp->data, &dummy_status);
        }
    }

    if (root == NULL)
        return NULL;

    root->height = avl_max(avl_height(root->left), avl_height(root->right)) + 1;

    int balance = avl_balance_factor(root);

    if (balance > 1 && avl_balance_factor(root->left) >= 0)
        return right_rotate(root);

    if (balance > 1 && avl_balance_factor(root->left) < 0)
    {
        root->left = left_rotate(root->left);
        return right_rotate(root);
    }

    if (balance < -1 && avl_balance_factor(root->right) <= 0)
        return left_rotate(root);

    if (balance < -1 && avl_balance_factor(root->right) > 0)
    {
        root->right = right_rotate(root->right);
        return left_rotate(root);
    }

    return root;
}

int avl_delete(avlNode** root_ref, int value)
{
    int status = 0;
    *root_ref = avl_delete_helper(*root_ref, value, &status);
    return status;
}

void avl_inorder(const avlNode* root)
{
    if (root == NULL)
        return;
    avl_inorder(root->left);
    printf("%d,", root->data);
    avl_inorder(root->right);
}

void avl_preorder(const avlNode* root)
{
    if (root == NULL)
        return;
    printf("%d,", root->data);
    avl_preorder(root->left);
    avl_preorder(root->right);
}

void avl_postorder(const avlNode* root)
{
    if (root == NULL)
        return;
    avl_postorder(root->left);
    avl_postorder(root->right);
    printf("%d,", root->data);
}

void destroy_avl(avlNode* root)
{
    if (root == NULL)
        return;
    destroy_avl(root->left);
    destroy_avl(root->right);
    free(root);
}

void avl_demo(void)
{
    while (1)
    {
        avlNode* root = NULL;
        int total_nodes;
        int total_nodes_status = safe_input_int(&total_nodes,
                                                "\n\nenter total number of nodes you want in the AVL tree, "
                                                "(between 1 and 100), enter '-1' to exit:- ",
                                                1, 100);

        if (total_nodes_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting AVL tree demo\n");
            destroy_avl(root);
            return;
        }
        if (total_nodes_status == 0)
        {
            continue;
        }

        int i = 1;
        while (total_nodes > 0)
        {
            int node_value;
            printf("\nenter value of %d AVL node - ", i);
            int node_value_status = safe_input_int(&node_value, NULL, 1, 100);

            if (node_value_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting AVL tree demo\n");
                destroy_avl(root);
                return;
            }
            if (node_value_status == 0)
            {
                continue;
            }

            int insertion_status = avl_insert(&root, node_value);
            if (insertion_status == 0)
            {
                printf("\nentered same value. only unique values please");
                continue;
            }
            if (insertion_status == -1)
            {
                printf("\ncouldnt insert node due to malloc failure. try again\n");
                continue;
            }
            i++;
            total_nodes--;
        }

        printf("\nheight of the AVL tree is:- %d\n", avl_height(root));

        while (1)
        {
            int traversal_choice;
            int traversal_status = safe_input_int(&traversal_choice,
                                                  "\nenter '1' for inorder, '2' for preorder and "
                                                  "'3' for postorder, '4' to delete a node, '5' to check balance factor, and '-1' to exit:- ",
                                                  1, 5);

            if (traversal_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting AVL tree demo\n");
                destroy_avl(root);
                return;
            }
            if (traversal_status == 0)
            {
                continue;
            }

            if (traversal_choice == 1)
            {
                avl_inorder(root);
                printf("\n");
            }
            else if (traversal_choice == 2)
            {
                avl_preorder(root);
                printf("\n");
            }
            else if (traversal_choice == 3)
            {
                avl_postorder(root);
                printf("\n");
            }
            else if (traversal_choice == 4)
            {
                int delete_value;
                int delete_status;
                while (1)
                {
                    delete_status = safe_input_int(&delete_value,
                                                   "\nenter value to delete (between 1 and 100), enter '-1' to exit:- ",
                                                   1, 100);
                    if (delete_status == INPUT_EXIT_SIGNAL)
                    {
                        printf("\nExiting AVL tree demo\n");
                        destroy_avl(root);
                        return;
                    }
                    if (delete_status == 0)
                        continue;
                    break;
                }
                int status = avl_delete(&root, delete_value);
                if (status == 0)
                {
                    printf("\nvalue not found in the tree\n");
                }
                else
                {
                    printf("\nnode deleted. updated inorder traversal: ");
                    avl_inorder(root);
                    printf("\n");
                }
            }
            else if (traversal_choice == 5)
            {
                printf("\nbalance factor of root node is: %d\n", avl_balance_factor(root));
            }
        }
    }
}
