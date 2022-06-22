// bst_command_loop

#include <fmt/core.h>
#include <iostream>
#include <string>
#include <random>

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

void command_loop(node*& root)
{
    bool running = true;
    while (running)
    {
        fmt::print("{}{}", "COMMANDS", "\n");
        fmt::print("{}{}", "\tadd [value]\t inserts value into bst", "\n");
        fmt::print("{}{}", "\trdm [count]\t inserts count random values into bst", "\n");
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
        if (input == "rdm")
        {
            unsigned count;
            std::cin >> count;
            random(root, count);
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
