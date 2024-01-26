#include <iostream>

struct Node
{
    long key;
    Node *left, *right;
    int height;

    explicit Node(const long k) : key(k), left(nullptr), right(nullptr), height(1)
    {
    }


    Node* left_rotation()
    {
        Node* new_root = right;
        Node* nr = new_root->left;

        new_root->left = this;
        right = nr;

        height = 1 + std::max(left ? left->height : 0, right ? right->height : 0);
        new_root->height = 1 + std::max(new_root->left ? new_root->left->height : 0,
                                        new_root->right ? new_root->right->height : 0);

        return new_root;
    }

    Node* right_rotation()
    {
        Node* new_root = left;
        Node* nl = new_root->right;

        new_root->right = this;
        left = nl;

        height = 1 + std::max(left ? left->height : 0, right ? right->height : 0);
        new_root->height = 1 + std::max(new_root->left ? new_root->left->height : 0,
                                        new_root->right ? new_root->right->height : 0);

        return new_root;
    }

    int balance_factor() const
    {
        if (!left && !right)
        {
            return 0;
        }

        if (!left)
        {
            return -right->height;
        }

        if (!right)
        {
            return left->height;
        }


        return left->height - right->height;
    }
};

struct AVLTree
{
    Node* get_min_node(Node* root)
    {
        if (!root || !root->left)
        {
            return root;
        }

        return get_min_node(root->left);
    }

    // O(log n)
    Node* search(Node* root, const long key)
    {
        Node* x = root;
        while (x && key != x->key)
        {
            // search the left side
            if (key < x->key)
            {
                x = x->left;
            }
            // search the right side
            else
            {
                x = x->right;
            }
        }
        return x;
    }

    //O (log n)
    Node* insert(Node* root, const long key)
    {
        if (!root)
        {
            const auto new_node = new Node(key);
            return new_node;
        }

        if (key < root->key)
        {
            root->left = insert(root->left, key);
        }
        else
        {
            root->right = insert(root->right, key);
        }

        root->height = 1 + std::max(root->left ? root->left->height : 0, root->right ? root->right->height : 0);

        // rebalance the tree
        const int bf = root->balance_factor();

        // if we are left heavy and the key is less than value on the root to our left
        // we do a single Right Rotation
        if (bf > 1 && root->left && key < root->left->key)
        {
            return root->right_rotation();
        }
        // if we are right heavy and the key is more than the right key
        // we do a single Left Rotation
        if (bf < -1 && root->right && key > root->right->key)
        {
            return root->left_rotation();
        }

        // if we are left heavy and the key is larger than the left node key
        // we do a left rotate (on the left node) and then a right rotate
        if (bf > 1 && root->left && key > root->left->key)
        {
            root->left = root->left->left_rotation();
            return root->right_rotation();
        }

        // if we are right heavy and the key is less than the right node key
        // we do a right rotate (on the right node) and then a left rotate
        if (bf < -1 && root->right && key < root->right->key)
        {
            root->right = root->right->right_rotation();
            return root->left_rotation();
        }

        return root;
    }

    // O (log n)
    Node* delete_node(Node* root, const long key)
    {
        if (!root)
        {
            return root;
        }

        if (key < root->key)
        {
            root->left = delete_node(root->left, key);
        }

        if (key > root->key)
        {
            root->right = delete_node(root->right, key);
        }

        else
        {
            // if no left node
            if (!root->left)
            {
                Node* temp = root->right;
                delete root;
                return temp;
            }

            // if no right node
            if (!root->right)
            {
                Node* temp = root->left;
                delete root;
                return temp;
            }

            // find inorder successor
            const Node* temp = get_min_node(root->right);
            root->key = temp->key;
            root->right = delete_node(root->right, temp->key);

            root->height = 1 + std::max(root->left ? root->left->height : 0, root->right ? root->right->height : 0);

            // Update the balance factor and balance the tree
            const int bf = root->balance_factor();

            // if tree is left heavy and the balance factor of the left tree is larger than 0
            if (bf > 1 && root->left && root->left->balance_factor() >= 0)
            {
                return root->right_rotation();
            }

            if (bf < -1 && root->right && root->right->balance_factor() <= 0)
            {
                return root->left_rotation();
            }

            if (bf > 1 && root->left && root->left->balance_factor() < 0)
            {
                root->left = root->left->left_rotation();
                return root->right_rotation();
            }

            if (bf < -1 && root->right && root->right->balance_factor() > 0)
            {
                root->right = root->right->right_rotation();
                return root->left_rotation();
            }

            return root;
        }
    }

    void print_tree(const Node* root)
    {
        if (!root)
        {
            return;
        }

        std::cout << root->key << std::endl;
        print_tree(root->left);
        print_tree(root->right);
    }
};


int main()
{
    // create a tree
    AVLTree tree;

    const auto root = new Node(random() % 100);

    // populate tree with random values
    for (int i = 0; i < 10; ++i)
    {
        tree.insert(root, random() % 100);
    }

    tree.insert(root, 65);

    // search for a value
    const Node* vn = tree.search(root, 65);
    std::cout << "Found node with key: " << vn->key << std::endl;

    // delete a value
    const Node* dv = tree.delete_node(root, 65);
    std::cout << "Deleted node with key: " << dv->key << std::endl;

    // print the tree
    std::cout << "Tree" << std::endl;
    tree.print_tree(root);

    free(root);
    return 0;
}


