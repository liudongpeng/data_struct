#ifndef _BST_H_
#define _BST_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>



// 父子节点
#define PARENT_NODE	1
#define CHILD_NODE	0

// 左右孩子
#define LEFT_NODE	1
#define RIGHT_NODE	0


// 树节点
typedef struct bst_node
{
	/*! 数据 */
	int key;

	/*! 此节点的左右节点 */
	struct bst_node* left_node;
	struct bst_node* right_node;

	/*! 此节的父节 */
	struct bst_node* parent_node;
	
	/*! 此节点是父节点的左节点还是右节点 */
	int flag_left_or_right;

} bst_node_t;


// 树
typedef struct _t
{
	/*! 树根 */
	struct bst_node* root;

	/*! 树中的节点数 */
	int node_count;
} bst_t;





bst_t* creat_bst();

int destroy_bst();

int bst_calc_du_node(bst_node_t* node);

int bst_leaf_node(bst_node_t* node);

bst_node_t* bst_creat_node(int key);

bst_node_t* bst_search_node(bst_node_t* root, int key);

bst_t* bst_insert_node(bst_t* tree, int key);

bst_node_t* get_rsb_min_node(bst_node_t* root, bst_node_t** parentNode);

int bst_delete_node(bst_t* tree, int key);




void infix(bst_node_t* root);


/*
bst_node_t* deleteNode(bst_t* tree, int key);

bst_node_t* searchNode(bst_node_t* root, int key, bst_node_t** parentNode);
*/












#endif // _BST_H_
