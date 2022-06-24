// bst_command_loop

#include <fmt/core.h>
#include <iostream>
#include <string>
#include <random>
#include <stack>
#include <utility>
#include <vector>

struct node
{
    int value = 0;
    node* left = nullptr;
    node* right = nullptr;
};

void insert(node*& root, int value)
{
    if (!root)
    {
        node* new_node = new node;
        new_node->value = value;
        root = new_node;

        fmt::print("{}{}{}", "insert value ", value, "\n");
        return;
    }
    else if (root->value > value)
    {
        fmt::print("{}", "traverse left\n");
        insert(root->left, value);
    }
    else
    {
        fmt::print("{}", "traverse right\n");
        insert(root->right, value);
    }
}

void random(node*& root, unsigned count)
{
    std::mt19937 gen32;
    for (size_t i = 0; i < count; ++i)
    {
        fmt::print("{}", "start at root\n");
        insert(root, gen32() % 100);
        fmt::print("{}", "\n");
    }
}

void height(node*& root)
{
    unsigned tree_height = 0;
    std::stack<std::pair<node*, unsigned>> todo_stack;
    todo_stack.push(std::make_pair(root, 0));
    while (!todo_stack.empty())
    {
        std::pair<node*, unsigned> current_todo = todo_stack.top();
        todo_stack.pop();
        node* current_node = current_todo.first;
        unsigned current_height = current_todo.second;
        if (current_node->left)
        {
            todo_stack.push(std::make_pair(current_node->left, current_height + 1));
            if (current_height + 1 > tree_height)
            {
                tree_height = current_height + 1;
            }
        }
        if (current_node->right)
        {
            todo_stack.push(std::make_pair(current_node->right, current_height + 1));
            if (current_height + 1 > tree_height)
            {
                tree_height = current_height + 1;
            }
        }
    }
    fmt::print("{}{}{}", "bst height is ", tree_height, "\n\n");
}

void balanced_helper(node*& root, int depth, std::vector<int>& leafes)
{
    if (root->left)
    {
        balanced_helper(root->left, depth + 1, leafes);
    }
    else
    {
        leafes.push_back(depth);
    }
    if (root->right)
    {
        balanced_helper(root->right, depth + 1, leafes);
    }
    else
    {
        leafes.push_back(depth);
    }
}

// height of the left and right subtree of any node differ by not more than 1
void balanced(node*& root)
{
    unsigned balanced = true;
    std::vector<int> leafes = {};
    
    balanced_helper(root, 0, leafes);

    fmt::print("{}", "leafes depth ");
    fmt::print("{}{}", leafes[0], " ");
    for (size_t i = 1; i < leafes.size(); ++i)
    {
        fmt::print("{}{}", leafes[i], " ");
        if (abs(leafes[i - 1] - leafes[i]) > 1)
        {
            balanced = false;
        }
    }
    fmt::print("{}", "\n");
    fmt::print("{}{}{}{}", "bst ", (balanced ? "is " : "is not "), "balanced", "\n\n");
}

void command_loop(node*& root)
{
    bool running = true;
    while (running)
    {
        fmt::print("{}{}", "COMMANDS", "\n");
        fmt::print("{}{}", "\tadd [value]\t inserts value into bst", "\n");
        fmt::print("{}{}", "\trdm [count]\t inserts count random values into bst", "\n");
        fmt::print("{}{}", "\theight\t\t returns bst height", "\n");
        fmt::print("{}{}", "\tbalanced\t returns whether the bst is balanced or not", "\n");
        fmt::print("{}{}", "\tquit\t\t quit program", "\n\n");
        fmt::print("{}", "Command> ");

        std::string input;
        std::cin >> input;
        fmt::print("{}", "\n");

        if (input == "add")
        {
            int value;
            std::cin >> value;
            if(std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore();
                std::cout << "invalid input\n\n";
            }
            else
            {
                fmt::print("{}", "start at root\n");
                insert(root, value);
                fmt::print("{}", "\n");
            }
        }
        else if (input == "rdm")
        {
            unsigned count;
            std::cin >> count;
            if(std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore();
                std::cout << "invalid input\n\n";
            }
            else
            {
                random(root, count);
            }
        }
        else if (input == "height")
        {
            height(root);
        }
        else if (input == "balanced")
        {
            balanced(root);
        }
        else if (input == "quit")
        {
            running = false;
        }
    }
}

int main()
{
    node* root = nullptr;
    command_loop(root);
}
