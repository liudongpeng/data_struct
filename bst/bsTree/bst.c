#include "bst.h"



bst_t* creat_bst()
{
    bst_t* tree = malloc(sizeof(bst_t));
    if(tree == NULL)
    {
        return NULL;
    }
    memset(tree, 0, sizeof(bst_t));
    
    return tree;
}

int destroy_bst(bst_t* tree)
{
    bst_node_t* stack[10000];
    int top = -1;
    bst_node_t *cur;

    stack[++top] = tree->root;
    cur = tree->root->left_node;

    while(cur != NULL || top != -1 )
    {
        while(cur != NULL)
        {
            stack[++top] = cur;
            cur = cur->left_node;
        }
        
        cur = stack[top]->right_node;

        free(stack[top--]);
        tree->node_count--;
    }

    free(tree);
    puts("destroy tree ok");
    return 0;
}

/**
 * @brief 计算此节点的度
 * 
 * @param node 
 * @return 0： 度为0，即叶子节点
 *         1： 度为1，即只有左子树或者右子树
 *         2： 度为2，既有左子树又有右子树
 */
int bst_calc_du_node(bst_node_t* node)
{
    if(node == NULL)
    {
        return -1;
    }

    if((node->left_node == NULL && node->right_node != NULL) || (node->left_node != NULL && node->right_node == NULL))
    {
        return 1;
    }
    else if(node->left_node == NULL && node->right_node == NULL)
    {
        return 0;
    }
    else
    {
        return 2;
    }
}


/**
 * @brief 计算节点是否是叶子节点
 * 
 * @param node 
 * @return 是返回1 不是返回0
 */
int bst_leaf_node(bst_node_t* node)
{
    if(node == NULL)
    {
        return 0;
    }

    return bst_calc_du_node(node) == 0 ? 1 : 0;
}

/**
 * @brief 创建节点
 * 
 * @param key: 键值
 * @return 新节点的指针
 */
bst_node_t* bst_creat_node(int key)
{
    bst_node_t* new_node = malloc(sizeof(bst_node_t));
    if(new_node == NULL)
    {
        return NULL;
    }
    memset(new_node, 0, sizeof(bst_node_t));
    new_node->key = key;

    return new_node;
}

/*! 寻找目标节点，找到返回节点，没找到返回NULL */
/**
 * @brief   寻找目标节点，找到返回节点，没找到返回NULL
 * 
 * @param   root: 从此节点开始找
 * @param   key: 键值
 * @return  找到了返回目标节点，没找到返回NULL
 */
bst_node_t* bst_search_node(bst_node_t* root, int key)
{
    bst_node_t* target = root;

    if(root == NULL)
    {
        return NULL;
    }

    while(target != NULL)
    {
        if(key < target->key)
        {
            target = target->left_node;
        }
        else if(key > target->key)
        {
            target = target->right_node;
        }
        else
        {
            return target;
        }
    }

    /* 没有目标节点 */
    return NULL;
}

/**
 * @brief 给树上插入一个新节点
 * 
 * @param tree 树
 * @param key 键值
 * @return 返回树本身
 */
bst_t* bst_insert_node(bst_t* tree, int key)
{
    bst_node_t* cur;
    bst_node_t* new_node;


    // 如果是空树，则创建根结点
    if (tree->root == NULL)
    {
        new_node = bst_creat_node(key);
        if (new_node == NULL)
        {
            return tree;
        }

        tree->root = new_node;
        //tree->root->parent_node = tree->root;
        tree->root->flag_left_or_right = -1;
        tree->node_count++;
        return tree;
    }

    new_node = bst_creat_node(key);
    if (new_node == NULL)
    {
        return tree;
    }

    cur = tree->root;
    new_node->parent_node = cur;
    while (cur != NULL)
    {
        if (key < cur->key)
        { 
            if (cur->left_node == NULL)
            {
                //cur = cur->left_node;
                new_node->parent_node = cur;
                new_node->flag_left_or_right = LEFT_NODE;
                cur->left_node = new_node;
                tree->node_count++;
                break;
            }
            else
            {
                new_node->parent_node = cur;
                cur = cur->left_node;
            }
        }
        else if (key > cur->key)
        {
            if (cur->right_node == NULL)
            {
                //cur = cur->right_node;
                new_node->parent_node = cur;
                new_node->flag_left_or_right = RIGHT_NODE;
                cur->right_node = new_node;
                tree->node_count++;
                break;
            }
            else
            {
                new_node->parent_node = cur;
                cur = cur->right_node;
            }
        }
        else
        {
            // 暂时先不做任何动作
            break;
        }
    }

    return tree;
}



/**
 * @brief 删除叶子节点
 * 
 * @param tree 树
 * @param del 要删除的节点
 * @param flag 要删除的节点是其父节点的左节点还是右节点
 * @return int 
 */
static int bst_delete_leaf_node(bst_t* tree, bst_node_t* del, int flag)
{
    bst_node_t* del_parent = del->parent_node;

    /*! del == root，即要删除的叶子节点是根节点 */
    if(del == tree->root)
    {
        tree->root = NULL;
    }
    else
    {
        if(flag == LEFT_NODE)
        {
            del_parent->left_node = NULL;
        }
        if(flag == RIGHT_NODE)
        {
            del_parent->right_node = NULL;
        }
    }

    free(del);
    tree->node_count--;
    return 0;
}


/**
 * @brief Get the rsb min node object
 * 
 * @param root 
 * @param parentNode 
 * @return bst_node_t* 
 */
bst_node_t* get_rsb_min_node(bst_node_t* root, bst_node_t** parentNode)
{
    bst_node_t* cur;

    if(root == NULL)
    {
        *parentNode = NULL;
        return NULL;
    }

    *parentNode = root;
    cur = root->right_node;
    while(cur->left_node != NULL)
    {
        *parentNode = cur;
        cur = cur->left_node;
    }

    return cur;
}

/**
 * @brief 删除键值对应的节点
 * 
 * @param tree 
 * @param key 
 * @return int 
 */
int bst_delete_node(bst_t* tree, int key)
{
    bst_node_t *del = NULL, *del_parent = NULL;
    bst_node_t *rsb_min_node, *rsb_min_node_parent;
    int flag;


    if(tree == NULL || tree->root == NULL)
    {
        return 0;
    }

    del = bst_search_node(tree->root, key);
    if(del == NULL)
    {
        return 0;
    }
    /*! 根结点的父节点是空NULL */
    del_parent = del->parent_node;
    
    if(del->left_node == NULL && del->right_node == NULL) /*!< 叶子节点 */
    {
        if(del == tree->root) /*! 是父节点 */
        {
            bst_delete_leaf_node(tree, del, -1);
            return 0;
        }
        else /*! 不是父节点 */
        {
            /*! 计算节点是其父节点的左节点还是右节点 */
            flag = key < del_parent->key ? LEFT_NODE : RIGHT_NODE;
            bst_delete_leaf_node(tree, del, flag);
            return 0;
        }
    }
    else if(del->left_node == NULL && del->right_node != NULL) /*!< 删除节点的左孩子为空，右孩子不为空 */
    {
        if(del == tree->root) /*! 根结点 */
        {
            tree->root = tree->root->right_node;
            tree->root->parent_node = NULL;
        }
        else
        {
            /*! 计算节点是其父节点的左节点还是右节点 */
            flag = key < del_parent->key ? LEFT_NODE : RIGHT_NODE;
            if(flag == LEFT_NODE)
            {
                del->right_node->parent_node = del->parent_node;
                del_parent->left_node = del->right_node;        
            }
            else
            {
                del->right_node->parent_node = del->parent_node;
                del_parent->right_node = del->right_node;
            }
        }
        free(del);
        tree->node_count--;
    }
    else if(del->left_node != NULL && del->right_node == NULL) /*!< 删除节点的右孩子为空，左孩子不为空 */
    {
        if(del == tree->root) /*! 根结点 */
        {
            tree->root = tree->root->left_node;
            tree->root->parent_node = NULL;
        }
        else
        {
            /*! 计算节点是其父节点的左节点还是右节点 */
            flag = key < del_parent->key ? LEFT_NODE : RIGHT_NODE;
            if(flag == LEFT_NODE)
            {
                del->left_node->parent_node = del->parent_node;
                del_parent->left_node = del->left_node;
            }
            else
            {
                del->left_node->parent_node = del->parent_node;
                del_parent->right_node = del->left_node;
            }
        }
        free(del);
        tree->node_count--;
    }
    else /*!< 删除节点的左右孩子都不为空 */
    {
        rsb_min_node = get_rsb_min_node(del, &rsb_min_node_parent);
        if(del == tree->root) /*! 根结点 */
        {
            if(del->right_node->left_node == NULL)
            {
                rsb_min_node->left_node = tree->root->left_node;
                tree->root->left_node->parent_node = rsb_min_node;

                tree->root = rsb_min_node;
                tree->root->parent_node = NULL;
            }
            else
            {
                rsb_min_node_parent->left_node = rsb_min_node->right_node;
                if(rsb_min_node_parent->left_node == NULL)
                {
                    rsb_min_node->left_node = tree->root->left_node;
                    tree->root->left_node->parent_node = rsb_min_node;

                    rsb_min_node->right_node = tree->root->right_node;
                    tree->root->right_node->parent_node = rsb_min_node;

                    tree->root = rsb_min_node;
                    tree->root->parent_node = NULL;
                }
                else
                {
                    rsb_min_node_parent->left_node->parent_node = rsb_min_node_parent;

                    rsb_min_node->left_node = tree->root->left_node;
                    tree->root->left_node->parent_node = rsb_min_node;

                    rsb_min_node->right_node = tree->root->right_node;
                    tree->root->right_node->parent_node = rsb_min_node;

                    tree->root = rsb_min_node;
                    tree->root->parent_node = NULL;
                }
            }
            free(del);
            tree->node_count--;
            return 0;
        }
        else /*!< 非根节点 */
        {
            flag = key < del_parent->key ? LEFT_NODE : RIGHT_NODE;
            if(flag == LEFT_NODE) /*! 要删除的节点是其父节点的左节点 */
            {
                if(del == rsb_min_node_parent)
                {
                    rsb_min_node->parent_node = del->parent_node;
                    del->parent_node->left_node = rsb_min_node;

                    rsb_min_node->left_node = del->left_node;
                    del->left_node->parent_node = rsb_min_node;
                }
                else
                {
                    rsb_min_node_parent->left_node = rsb_min_node->right_node;
                    if(rsb_min_node_parent->left_node == NULL)
                    {
                        rsb_min_node->left_node = del->left_node;
                        del->left_node->parent_node = rsb_min_node;

                        rsb_min_node->right_node = del->right_node;
                        del->right_node->parent_node = rsb_min_node;

                        del->parent_node->left_node = rsb_min_node;
                        rsb_min_node->parent_node = del->parent_node;
                    }
                    else
                    {
                        rsb_min_node_parent->left_node->parent_node = rsb_min_node_parent;

                        rsb_min_node->left_node = del->left_node;
                        del->left_node->parent_node = rsb_min_node;

                        rsb_min_node->right_node = del->right_node;
                        del->right_node->parent_node = rsb_min_node;

                        del->parent_node->left_node = rsb_min_node;
                        rsb_min_node->parent_node = del->parent_node;
                    }
                }
            }
            else /*! 要删除的节点是其父节点的右节点 */
            {
                if(del == rsb_min_node_parent)
                {
                    rsb_min_node->parent_node = del->parent_node;
                    del->parent_node->right_node = rsb_min_node;

                    rsb_min_node->left_node = del->left_node;
                    del->left_node->parent_node = rsb_min_node;
                }
                else
                {
                    rsb_min_node_parent->left_node = rsb_min_node->right_node;
                    if(rsb_min_node_parent->left_node == NULL)
                    {
                        rsb_min_node->left_node = del->left_node;
                        del->left_node->parent_node = rsb_min_node;

                        rsb_min_node->right_node = del->right_node;
                        del->right_node->parent_node = rsb_min_node;

                        del->parent_node->right_node = rsb_min_node;
                        rsb_min_node->parent_node = del->parent_node;
                    }
                    else
                    {
                        rsb_min_node_parent->left_node->parent_node = rsb_min_node_parent;

                        rsb_min_node->left_node = del->left_node;
                        del->left_node->parent_node = rsb_min_node;

                        rsb_min_node->right_node = del->right_node;
                        del->right_node->parent_node = rsb_min_node;

                        del->parent_node->right_node = rsb_min_node;
                        rsb_min_node->parent_node = del->parent_node;
                    }
                }
            }
            free(del);
            tree->node_count--;
            return 0;
        }
    }

    return 0;
}


void infix(bst_node_t* root)
{
    if(root == NULL)
    {
        return;
    }

    infix(root->left_node);
    printf("%d ", root->key);
    infix(root->right_node);
}


/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
/*
bst_node_t* searchNode(bst_node_t* root, int key, bst_node_t** parentNode)
{
    bst_node_t* cur = root;

    if(root == NULL)
    {
        *parentNode = NULL;
        return NULL;
    }

    while(cur)
    {
        if(cur->key < key)
        {
            *parentNode = cur;
            cur = cur->right_node;
        }
        else if(cur->key > key)
        {
            *parentNode = cur;
            cur = cur->left_node;
        }
        else
        {
            break;
        }
    }

    return cur;
}

bst_node_t* deleteNode(bst_t* tree, int key)
{
    bst_node_t *del, *del_parent;
    bst_node_t *rsb_min_node, *rsb_min_node_parent;
    bst_node_t* root;
    
    if(tree == NULL)
    {
        return NULL;
    }

    root = tree->root;

    if(root == NULL)
    {
        return root;
    }
    
    // 找到要删除的节点及其父节点，根节点的父节点是NULL
    del = searchNode(root, key, &del_parent);

    // 没找到要删除的节点则直接退出
    if(del == NULL)
    {
        return root;
    }

    // 找到要删除的节点并判断是哪种情况
    if(del->left_node == NULL && del->right_node == NULL) // 叶子节点
    {
        if(del == root) // 根节点
        {
            root = NULL;
        }
        else // 非根节点
        {
            if(del->key < del_parent->key) //要删除的节点是其父节点的左节点
            {
                del_parent->left_node = NULL;
            }
            else
            {
                del_parent->right_node = NULL;
            }
        }
        free(del);
    }
    else if(del->left_node == NULL && del->right_node != NULL) // 删除节点的左孩子为空，右孩子不为空
    {
        if(del == root) // 根节点
        {
            free(del);
            root = root->right_node;
        }
        else // 非根节点
        {
            if(del->key < del_parent->key) // 要删除的节点是其父节点的左节点
            {
                del_parent->left_node = del->right_node;
            }
            else
            {
                del_parent->right_node = del->right_node;
            }
            free(del);
        }
    }
    else if(del->left_node != NULL && del->right_node == NULL) // 删除节点的右孩子为空，左孩子不为空
    {
        if(del == root) // 根节点
        {
            free(del);
            root = root->left_node;
        }
        else // 非根节点
        {
            if(del->key < del_parent->key) // 要删除的节点是其父节点的左节点
            {
                del_parent->left_node = del->left_node;
            }
            else
            {
                del_parent->right_node = del->left_node;
            }
            free(del);
        }
    }
    else // 删除节点的左右孩子都不为空
    {
        rsb_min_node = get_rsb_min_node(del, &rsb_min_node_parent);
        if(del == root) // 根节点
        {
            if(rsb_min_node_parent == root) // 根节点右孩子的左节点为NULL
            {
                free(del);
                rsb_min_node->left_node = root->left_node;
                root = rsb_min_node;
            }
            else
            {
                free(del);
                rsb_min_node_parent->left_node = rsb_min_node->right_node;
                rsb_min_node->left_node = root->left_node;
                rsb_min_node->right_node = root->right_node;
                root = rsb_min_node;
            }
        }
        else // 非根节点
        {
            if(del->key < del_parent->key) // 要删除的节点是其父节点的左节点
            {
                if(rsb_min_node_parent == del)
                {
                    rsb_min_node->left_node = del->left_node;
                    del_parent->left_node = rsb_min_node;
                }
                else
                {
                    rsb_min_node_parent->left_node = rsb_min_node->right_node;
                    rsb_min_node->left_node = del->left_node;
                    rsb_min_node->right_node = del->right_node;
                    del_parent->left_node = rsb_min_node;
                }
            }
            else // 要删除的节点是其父节点的右节点
            {
                if(rsb_min_node_parent == del)
                {
                    rsb_min_node->left_node = del->left_node;
                    del_parent->right_node = rsb_min_node;
                }
                else
                {
                    rsb_min_node_parent->left_node = rsb_min_node->right_node;
                    rsb_min_node->left_node = del->left_node;
                    rsb_min_node->right_node = del->right_node;
                    del_parent->right_node = rsb_min_node;
                }
            }
            free(del);
        }
    }

    tree->node_count--;
    return root;
}
*/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/


