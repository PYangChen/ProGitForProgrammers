#include <stdio.h>
#include <stdlib.h>

#define RED        0    //Red node
#define BLACK    1    // Block node

#define rb_parent(r)   ((r)->parent)
#define rb_color(r) ((r)->color)
#define rb_is_red(r)   ((r)->color==RED)
#define rb_is_black(r)  ((r)->color==BLACK)
#define rb_set_black(r)  do { (r)->color = BLACK; } while (0)
#define rb_set_red(r)  do { (r)->color = RED; } while (0)
#define rb_set_parent(r,p)  do { (r)->parent = (p); } while (0)
#define rb_set_color(r,c)  do { (r)->color = (c); } while (0)

typedef int Type;

typedef struct RBTreeNode{
    unsigned char color;        
    Type   key;                    
    struct RBTreeNode *left;
    struct RBTreeNode *right;
	struct RBTreeNode *parent;    
}Node, *RBTree;

typedef struct rb_root{
    Node *node;
}RBRoot;


RBRoot* create_rbtree();
static Node* create_rbtree_node(Type key, Node *parent, Node *left, Node* right);
static Node* search(RBTree x, Type key);
static void rbtree_left_rotate(RBRoot *root, Node *x);
static void rbtree_right_rotate(RBRoot *root, Node *y);
static void rbtree_insert_fixup(RBRoot *root, Node *node);
static void rbtree_insert(RBRoot *root, Node *node);
int insert_rbtree(RBRoot *root, Type key);
static void rbtree_delete_fixup(RBRoot *root, Node *node, Node *parent);
void rbtree_delete(RBRoot *root, Node *node);
void delete_rbtree(RBRoot *root, Type key);
static void rbtree_destroy(RBTree tree);
void destroy_rbtree(RBRoot *root);


int main(int argc, char *argv[]) {
	FILE *fp;
	char action[20];
	//char stringValue1[20];
	int intValue1;
    Node *node_t;
	RBRoot *root=NULL;
/*	
	fp = fopen("input_07.txt", "r");
	if (fp == NULL)
	{
		printf("Cannot open input.txt\n");
		return 1;
	}
*/
	
	root = create_rbtree(); 
	
	while (scanf("%s %d", action, &intValue1) != EOF)
	{
//		fscanf(fp, "%s", stringValue1);
//		intValue1 = atoi(stringValue1);
		switch(action[0])
		{
			case 'i': //action is insert 
			insert_rbtree(root, intValue1);
			break;
			
			case 's': //action is search
			node_t=search(root->node, intValue1);
			if (node_t==NULL)
			printf("Not found\n");
			else if(node_t->color==1)
			printf("black\n");
			else
			printf("red\n");
			break;
			
			case 'd': //action is delete
			delete_rbtree(root, intValue1);
			break;
			
			case 'q': //action is delete
			destroy_rbtree(root);
//			printf("quit");
			return 0;
			exit(0);
			break;						
		}
		
	}

	node_t=search(root->node, 50);
	if (node_t==NULL)
	printf("Not found \n");
	else
	printf("color=%d \n" , node_t->color);
	if (node_t==NULL)
	printf("Not found\n");
	else if(node_t->color==1)
	printf("black");
	else
	printf("red");

	
	insert_rbtree(root, 50);
	insert_rbtree(root, 50);
	insert_rbtree(root, 50);
	
	node_t=search(root->node, 50);
	if (node_t==NULL)
	printf("Not found \n");
	else if(node_t->color==1)
	printf("black");
	else
	printf("red");
	
	node_t=search(root->node, 0);
	if (node_t==NULL)
	printf("Not found \n");
	else
	printf("color=%d \n" , node_t->color);
	
	insert_rbtree(root, 10);
	
	node_t=search(root->node, 50);
	if (node_t==NULL)
	printf("Not found \n");
	else
	printf("color=%d \n" , node_t->color);
	
	node_t=search(root->node, 10);
	if (node_t==NULL)
	printf("Not found \n");
	else
	printf("color=%d \n" , node_t->color);
	

	
	//delete_rbtree(root, 5);
	
	return 0;
}


RBRoot* create_rbtree()
{
    RBRoot *root = (RBRoot *)malloc(sizeof(RBRoot));
    root->node = NULL;

    return root;
}

static Node* create_rbtree_node(Type key, Node *parent, Node *left, Node* right)
{
    Node* p;

    if ((p = (Node *)malloc(sizeof(Node))) == NULL)
        return NULL;
    p->key = key;
    p->left = left;
    p->right = right;
    p->parent = parent;
    p->color = BLACK;

    return p;
}

static Node* search(RBTree x, Type key)
{
    if (x==NULL || x->key==key)
        return x;

    if (key < x->key)
        return search(x->left, key);
    else
        return search(x->right, key);
}

static void rbtree_left_rotate(RBRoot *root, Node *x)
{
    Node *y = x->right;
    x->right = y->left;
    if (y->left != NULL)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == NULL)
    {

        root->node = y;
    }
    else
    {
        if (x->parent->left == x)
            x->parent->left = y;
        else
            x->parent->right = y;
    }


    y->left = x;

    x->parent = y;
}

static void rbtree_right_rotate(RBRoot *root, Node *y)
{

    Node *x = y->left;

    y->left = x->right;
    if (x->right != NULL)
        x->right->parent = y;


    x->parent = y->parent;

    if (y->parent == NULL)
    {
        root->node = x;
    }
    else
    {
        if (y == y->parent->right)
            y->parent->right = x;
        else
            y->parent->left = x;
    }


    x->right = y;

    y->parent = x;
}


static void rbtree_insert_fixup(RBRoot *root, Node *node)
{
    Node *parent, *gparent;

    while ((parent = rb_parent(node)) && rb_is_red(parent))
    {
        gparent = rb_parent(parent);

        if (parent == gparent->left)
        {
            {
                Node *uncle = gparent->right;
                if (uncle && rb_is_red(uncle))
                {
                    rb_set_black(uncle);
                    rb_set_black(parent);
                    rb_set_red(gparent);
                    node = gparent;
                    continue;
                }
            }

            if (parent->right == node)
            {
                Node *tmp;
                rbtree_left_rotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }

            rb_set_black(parent);
            rb_set_red(gparent);
            rbtree_right_rotate(root, gparent);
        }
        else
        {
            {
                Node *uncle = gparent->left;
                if (uncle && rb_is_red(uncle))
                {
                    rb_set_black(uncle);
                    rb_set_black(parent);
                    rb_set_red(gparent);
                    node = gparent;
                    continue;
                }
            }

            if (parent->left == node)
            {
                Node *tmp;
                rbtree_right_rotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }

            rb_set_black(parent);
            rb_set_red(gparent);
            rbtree_left_rotate(root, gparent);
        }
    }


    rb_set_black(root->node);
}


static void rbtree_insert(RBRoot *root, Node *node)
{
    Node *y = NULL;
    Node *x = root->node;
    while (x != NULL)
    {
        y = x;
        if (node->key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    rb_parent(node) = y;

    if (y != NULL)
    {
        if (node->key < y->key)
            y->left = node;
        else
            y->right = node;
    }
    else
    {
        root->node = node;
    }

    node->color = RED;

    rbtree_insert_fixup(root, node);
}


int insert_rbtree(RBRoot *root, Type key)
{
    Node *node;
    if (search(root->node, key) != NULL)
        return -1;
        
    if ((node=create_rbtree_node(key, NULL, NULL, NULL)) == NULL)
        return -1;
    rbtree_insert(root, node);

    return 0;
}

static void rbtree_delete_fixup(RBRoot *root, Node *node, Node *parent)
{
    Node *other;

    while ((!node || rb_is_black(node)) && node != root->node)
    {
        if (parent->left == node)
        {
            other = parent->right;
            if (rb_is_red(other))
            {
                rb_set_black(other);
                rb_set_red(parent);
                rbtree_left_rotate(root, parent);
                other = parent->right;
            }
            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right)))
            {
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            }
            else
            {
                if (!other->right || rb_is_black(other->right))
                {
                    rb_set_black(other->left);
                    rb_set_red(other);
                    rbtree_right_rotate(root, other);
                    other = parent->right;
                }
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->right);
                rbtree_left_rotate(root, parent);
                node = root->node;
                break;
            }
        }
        else
        {
            other = parent->left;
            if (rb_is_red(other))
            {
                rb_set_black(other);
                rb_set_red(parent);
                rbtree_right_rotate(root, parent);
                other = parent->left;
            }
            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right)))
            {
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            }
            else
            {
                if (!other->left || rb_is_black(other->left))
                {
                    rb_set_black(other->right);
                    rb_set_red(other);
                    rbtree_left_rotate(root, other);
                    other = parent->left;
                }
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->left);
                rbtree_right_rotate(root, parent);
                node = root->node;
                break;
            }
        }
    }
    if (node)
        rb_set_black(node);
}

void rbtree_delete(RBRoot *root, Node *node)
{
    Node *child, *parent;
    int color;

    if ( (node->left!=NULL) && (node->right!=NULL) )
    {
        Node *replace = node;
        replace = replace->right;
        while (replace->left != NULL)
            replace = replace->left;

        if (rb_parent(node))
        {
            if (rb_parent(node)->left == node)
                rb_parent(node)->left = replace;
            else
                rb_parent(node)->right = replace;
        }
        else
            root->node = replace;

        child = replace->right;
        parent = rb_parent(replace);

        color = rb_color(replace);

        if (parent == node)
        {
            parent = replace;
        }
        else
        {
            if (child)
                rb_set_parent(child, parent);
            parent->left = child;

            replace->right = node->right;
            rb_set_parent(node->right, replace);
        }

        replace->parent = node->parent;
        replace->color = node->color;
        replace->left = node->left;
        node->left->parent = replace;

        if (color == BLACK)
            rbtree_delete_fixup(root, child, parent);
        free(node);

        return ;
    }

    if (node->left !=NULL)
        child = node->left;
    else
        child = node->right;

    parent = node->parent;
    color = node->color;

    if (child)
        child->parent = parent;

    if (parent)
    {
        if (parent->left == node)
            parent->left = child;
        else
            parent->right = child;
    }
    else
        root->node = child;

    if (color == BLACK)
        rbtree_delete_fixup(root, child, parent);
    free(node);
}


void delete_rbtree(RBRoot *root, Type key)
{
    Node *z, *node;

    if ((z = search(root->node, key)) != NULL)
        rbtree_delete(root, z);
}

static void rbtree_destroy(RBTree tree)
{
    if (tree==NULL)
        return ;

    if (tree->left != NULL)
        rbtree_destroy(tree->left);
    if (tree->right != NULL)
        rbtree_destroy(tree->right);

    free(tree);
}

void destroy_rbtree(RBRoot *root)
{
    if (root != NULL)
        rbtree_destroy(root->node);

    free(root);
}



