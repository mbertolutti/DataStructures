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

void insert_helper(node*& root, int value)
{
    if (!root)
    {
        node* new_node = new node;
        new_node->value = value;
        root = new_node;

        fmt::print("{}{}{}", "insert value ", value, ".\n");
        return;
    }
    else if (root->value > value)
    {
        fmt::print("{}", "traverse left,\n");
        insert_helper(root->left, value);
    }
    else
    {
        fmt::print("{}", "traverse right,\n");
        insert_helper(root->right, value);
    }
}

void insert(node*& root, unsigned value)
{
    fmt::print("{}", "Start at root,\n");
    insert_helper(root, value);
    fmt::print("{}", "\n");
}

void random(node*& root, unsigned count)
{
    std::mt19937 gen32;
    for (size_t i = 0; i < count; ++i)
    {
        insert(root, gen32() % 100);
    }
}

void height(node*& root)
{
    unsigned tree_height = 0;
    std::stack<std::pair<node*, unsigned>> todo_stack;
    if (root)
    {
        todo_stack.push(std::make_pair(root, 0));
    }
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
    fmt::print("{}{}{}", "Bst height is ", tree_height, ".\n\n");
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
    
    if (root)
    {
        balanced_helper(root, 0, leafes);

        fmt::print("{}", "Leafes depth: ");
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
    }
    fmt::print("{}{}{}", "bst ", (balanced ? "is " : "is not "), "balanced.\n\n");
}

void command_loop(node*& root)
{
    bool running = true;
    bool print_commands = 1;
    while (running)
    {
        if (print_commands)
        {
            fmt::print("{}", "COMMANDS\n");
            fmt::print("{}", "\t[a | add] [value]\t Inserts value into bst.\n");
            fmt::print("{}", "\t[r | rdm] [count]\t inserts count random values into bst.\n");
            fmt::print("{}", "\t[h | height]\t\t Returns bst height.\n");
            fmt::print("{}", "\t[b | balanced]\t\t Returns whether the bst is balanced or not.\n");
            fmt::print("{}", "\t[q | quit]\t\t Quit program.\n\n");
            fmt::print("{}", "Command> ");
        }

        std::string input;
        std::cin >> input;

        if (print_commands)
        {
            fmt::print("{}", "\n");
        }
        print_commands = 1;

        if (input == "a" || input == "add")
        {
            int value;
            fmt::print("{}", "Enter add argument [value] > ");
            std::cin >> value;
            if(std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore();
                fmt::print("{}", "\n");
                std::cout << "Invalid input.\n\n";
                print_commands = 0;
            }
            else
            {
                fmt::print("{}", "\n");
                fmt::print("{}{}{}", "Inserting value ", value, " into the bst:\n");
                fmt::print("{}", "\n");
                insert(root, value);
            }
        }
        else if (input == "r" || input == "rdm")
        {
            unsigned count;
            fmt::print("{}", "Enter rdm argument [count] > ");
            std::cin >> count;
            if(std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore();
                fmt::print("{}", "\n");
                std::cout << "Invalid input.\n\n";
                print_commands = 0;
            }
            else
            {
                fmt::print("{}", "\n");
                fmt::print("{}{}{}", "Inserting ", count, " random values into the bst:\n");
                fmt::print("{}", "\n");
                random(root, count);
            }
        }
        else if (input == "h" || input == "height")
        {
            height(root);
        }
        else if (input == "b" || input == "balanced")
        {
            balanced(root);
        }
        else if (input == "q" || input == "quit")
        {
            running = false;
        }
        else
        {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "Invalid input.\n\n";
        }
    }
}

int main()
{
    node* root = nullptr;
    command_loop(root);
}
