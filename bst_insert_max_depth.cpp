// bst_insert_max_depth

#include <fmt/core.h>
#include <random>

struct node
{
    int val = 0;
    int tree_height = 0;
    node* left = nullptr;
    node* right = nullptr;
};

int insert(int val, node*& root, int tree_height)
{
    if (!root)
    {
        fmt::print("{}{}{}{}{}", "insert value ", val, " at depth ", tree_height, "\n");
        node* new_node = new node();
        new_node->val = val;
        new_node->tree_height = tree_height;
        root = new_node;
        return 1;
    }
    if (val < root->val)
    {
        fmt::print("{}{}{}", "traverse left, current depth ", tree_height, "\n");
        return 1 + insert(val, root->left, tree_height + 1);
    }
    else
    {
        fmt::print("{}{}{}", "traverse right, current depth ", tree_height, "\n");
        return 1 + insert(val, root->right, tree_height + 1);
    }
}

int main()
{
    std::mt19937 gen32;
    node* bst_root = nullptr;
    int max_depth = 0;
    for (size_t i = 0; i < 100; ++i)
    {
        int current_depth = insert(gen32() % 100, bst_root, 1);
        if (current_depth > max_depth)
        {
            max_depth = current_depth;
        }
    }

    fmt::print("{}{}{}", "max depth is ", max_depth, "\n\n");
}
