#include "bst.h"
#include <time.h>


#define SIZE    15


// jian cha shuzu zhong you mei you key
int check_arr(int* arr, int key, int len)
{
    if(arr == NULL || len == 0)
    {
        return 0;
    }

    for(int i = 0; i < len; i++)
    {
        if(arr[i] == key)
        {
            return 1;
        }
    }
    return 0;
}

int main()
{
    bst_t* tree = NULL;
    srand((unsigned int)(time(NULL)));

    //int arr[] = { 4, 9, 2, 8, 1, -1, 0, 3, 7, 19 };
    //int arr[] = { 0, 3, 4, 5, 6, 7, 8, 9 };

    
    int arr[SIZE];
    int tmp;
    for(int i = 0; i < SIZE; i++)
    {
        tmp = rand() % 1000;
        if(check_arr(arr, tmp, i) == 1)
        {
            continue;
        }
        arr[i] = tmp;
    }
    

    tree = creat_bst();
    if (tree == NULL)
    {
        return -1;
    }

    int len = sizeof(arr) / sizeof(int);
    for (int i = 0; i < len; i++)
    {
        bst_insert_node(tree, arr[i]);
    }
    
    infix(tree->root);
    printf("\nnode_count: %d\n\n", tree->node_count);

    /*
    for (int i = 0; i < len; i++)
    //for (int i = len - 1; i >= 0; i--)
    {
        bst_delete_node(tree, arr[i]);
        if(tree->root == NULL)
        {
            puts("empty tree");
            return 0;
        }
        infix(tree->root);
        printf("\nnode_count: %d\n\n", tree->node_count);
    }
    */

    destroy_bst(tree);
    
    infix(tree->root);
    printf("\nnode_count: %p %d\n\n", tree->root, tree->node_count);
    

    exit(0);
}