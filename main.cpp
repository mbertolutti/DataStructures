// bst_command_loop

#include <fmt/core.h>
#include <iostream>
#include <string>
#include <random>
#include <stack>
#include <utility>

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
    todo_stack.push(std::make_pair(root, 1));
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
                tree_height = current_height;
            }
        }
        if (current_node->right)
        {
            todo_stack.push(std::make_pair(current_node->right, current_height + 1));
            if (current_height + 1 > tree_height)
            {
                tree_height = current_height;
            }
        }
    }
    fmt::print("{}{}{}", "bst height is ", tree_height, "\n\n");
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
        fmt::print("{}{}", "\tquit\t\t quit program", "\n\n");
        fmt::print("{}", "Command> ");

        std::string input;
        std::cin >> input;
        fmt::print("{}", "\n");

        if (input == "add")
        {
            int value;
            std::cin >> value;
            fmt::print("{}", "start at root\n");
            insert(root, value);
            fmt::print("{}", "\n");
        }
        else if (input == "rdm")
        {
            unsigned count;
            std::cin >> count;
            random(root, count);
        }
        else if (input == "height")
        {
            height(root);
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
