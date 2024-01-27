//
// Created by André Ribeiro on 27/01/2024.
//

#include <iostream>

struct Node
{
    int key;
    Node *left, *right, *parent;
    bool color;
};

class RedBlackTree
{
public:
    Node* root;

    void left_rotate(Node* node)
    {
        Node* y = node->right;
        node->right = y->left;

        if (y->left)
        {
            y->left->parent = node;
        }

        y->parent = node->parent;
        if (!node->parent)
        {
            root = y;
        }
        else if (node == node->parent->left)
        {
            node->parent->left = y;
        }
        else
        {
            node->parent->right = y;
        }

        y->left = node;
        node->parent = y;
    }

    void right_rotate(Node* node)
    {
        Node* y = node->left;
        node->left = y->right;

        if (y->right)
        {
            y->right->parent = node;
        }

        y->parent = node->parent;
        if (!node->parent)
        {
            root = y;
        }
        else if (node == node->parent->right)
        {
            node->parent->right = y;
        }
        else
        {
            node->parent->left = y;
        }

        y->right = node;
        node->parent = y;
    }

    void insert(int k)
    {
        Node* x = root;
        Node* y = nullptr;
        Node* z = new Node{k};
        while (x)
        {
            y = x;
            if (z->key < x->key)
            {
                x = x->left;
            }
            else
            {
                x = x->right;
            }
        }
        z->parent = y;
        if (!y)
        {
            root = z;
        }
        else if (z->key < y->key)
        {
            y->left = z;
        }
        else
        {
            y->right = z;
        }
        z->left = nullptr;
        z->right = nullptr;
        z->color = true;
        rb_insert_fixup(z);
    }

    void rb_insert_fixup(Node* z)
    {
        while (z->parent && z->parent->color)
        {
            if (z->parent == z->parent->parent->left)
            {
                Node* y = z->parent->parent->right;
                if (y->color)
                {
                    z->parent->color = false;
                    y->color = false;
                    z->parent->parent->color = true;
                    z = z->parent->parent;
                }
                else if (z == z->parent->right)
                {
                    z = z->parent;
                    left_rotate(z);
                }
                z->parent->color = false;
                z->parent->parent->color = true;
                right_rotate(z->parent->parent);
            }
            else
            {
                Node* y = z->parent->parent->left;
                if (y && y->color)
                {
                    z->parent->color = false;
                    y->color = false;
                    z->parent->parent->color = true;
                    z = z->parent->parent;
                }
                else if (z == z->parent->left)
                {
                    z = z->parent;
                    right_rotate(z);
                }
                if (z->parent)
                {
                    z->parent->color = false;
                    z->parent->parent->color = true;
                    left_rotate(z->parent->parent);
                }
            }
        }
        root->color = false;
    }
};

void print_tree(const Node* node)
{
    // print the tree with the left node on the left and hte right node on the right
    if (node)
    {
        std::cout << node->key << " ";
        print_tree(node->left);
        print_tree(node->right);
    }
}

int main()
{
    // create a tree
    RedBlackTree tree{};
    tree.root = new Node{10};

    tree.insert(11);
    tree.insert(20);
    tree.insert(30);
    tree.insert(40);

    print_tree(tree.root);
}
