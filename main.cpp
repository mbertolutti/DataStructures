// bst_interactive

#include <fmt/core.h>
#include <cstdint>
#include <iostream>
#include <string>
#include <random>
#include <stack>
#include <utility>
#include <vector>

struct node
{
    std::uint16_t value = 0;
    node* left = nullptr;
    node* right = nullptr;
};

void insert_helper(node*& root, std::uint16_t value)
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

void insert(node*& root, std::uint16_t value)
{
    fmt::print("{}", "Start at root,\n");
    insert_helper(root, value);
    fmt::print("{}", "\n");
}

void random(node*& root, std::uint16_t count)
{
    std::mt19937 gen32 = std::mt19937(time(nullptr));
    for (size_t i = 0; i < count; ++i)
    {
        insert(root, gen32() % 100);
    }
}

void height(node*& root)
{
    std::uint16_t tree_height = 0;
    std::stack<std::pair<node*, std::uint16_t>> todo_stack;
    if (root)
    {
        todo_stack.push(std::make_pair(root, 0));
    }
    while (!todo_stack.empty())
    {
        std::pair<node*, std::uint16_t> current_todo = todo_stack.top();
        todo_stack.pop();
        node* current_node = current_todo.first;
        std::uint16_t current_height = current_todo.second;
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
    fmt::print("{}{}{}", "tree height is ", tree_height, ".\n\n");
}

void leafes_helper(node*& root, int depth, std::vector<int>& leafes)
{
    if (root->left)
    {
        leafes_helper(root->left, depth + 1, leafes);
    }
    else
    {
        leafes.push_back(depth);
    }
    if (root->right)
    {
        leafes_helper(root->right, depth + 1, leafes);
    }
    else
    {
        leafes.push_back(depth);
    }
}

// height of the left and right subtree of any node differ by not more than 1
void balanced(node*& root)
{
    std::uint16_t balanced = true;
    std::vector<int> leafes = {};
    
    if (root)
    {
        leafes_helper(root, 0, leafes);

        for (size_t i = 1; i < leafes.size(); ++i)
        {
            if (abs(leafes[i - 1] - leafes[i]) > 1)
            {
                balanced = false;
                break;
            }
        }
        fmt::print("{}", "\n");
    }
    fmt::print("{}{}{}", "tree ", (balanced ? "is " : "is not "), "balanced.\n\n");
}

void print_preorder(node*& root)
{
    if (!root)
    {
        return;
    }
    fmt::print("{}{}", root->value, " ");
    print_preorder(root->left);
    print_preorder(root->right);
}

void print_inorder(node*& root)
{
    if (!root)
    {
        return;
    }
    print_preorder(root->left);
    fmt::print("{}{}", root->value, " ");
    print_preorder(root->right);
}

void print_postorder(node*& root)
{
    if (!root)
    {
        return;
    }
    print_preorder(root->left);
    print_preorder(root->right);
    fmt::print("{}{}", root->value, " ");
}

void print_leafes_depth(node*& root)
{
    std::vector<int> leafes = {};
    
    if (root)
    {
        leafes_helper(root, 0, leafes);

        fmt::print("{}", "Depth: ");
        for (size_t i = 0; i < leafes.size(); ++i)
        {
            fmt::print("{}{}", leafes[i], " ");
        }
    }
    else
    {
        fmt::print("{}", "Tree is empty.");
    }
}

void print_command_list()
{
    fmt::print("{}", "COMMANDS\n");
    fmt::print("{}", "\t[a | add] [<value>]\t Insert <value> into tree.\n");
    fmt::print("{}", "\t[r | rdm] [<count>]\t Insert <count> random values into tree.\n");
    fmt::print("{}", "\t[d | del] < >\t\t Delete last node. (deterministic breadth first search)\n");
    fmt::print("{}", "\t[i | deli] < >\t\t Delete position (deterministic breadth first search) ...\n");
    fmt::print("{}", "\t\t  [r]\t\t ... root node.\n");
    fmt::print("{}", "\t\t  [<i>]\t\t ... <i>th node.\n");
    fmt::print("{}", "\t\t  [p | purge]\t ... all nodes.\n");

    fmt::print("{}", "\t[b | bal]\t\t Tree balanced?\n");
    fmt::print("{}", "\t[f | perf]\t\t Tree perfect?\n");
    fmt::print("{}", "\t[h | height]\t\t Tree height.\n");
    fmt::print("{}", "\t[l | leafes]\t\t Print leafes and single parents missing children depth.\n");
    fmt::print("{}", "\t[g | graph]\t\t Print graphicly visualized tree.\n");

    fmt::print("{}", "\t[p | print] < >\t\t Print tree in ...\n");
    fmt::print("{}", "\t\t  [pre]\t\t ... preorder traversal mode.\n");
    fmt::print("{}", "\t\t  [in]\t\t ... inorder traversal mode.\n");
    fmt::print("{}", "\t\t  [post]\t ... postorder traversal mode.\n");

    fmt::print("{}", "\t[c | com]\t\t Print command list.\n");
    fmt::print("{}", "\t[q | quit]\t\t Quit program.\n\n");
}

void command_loop(node*& root)
{
    fmt::print("{}", "Welcome to interactive binary search tree builder v1.3c.\n\n");
    fmt::print("{}", "Enter 'c' or 'com' for a list of all commands.\n\n");
    bool running = true;
    bool print_command_line = 0;
    fmt::print("{}", "Command> ");
    while (running)
    {
        if (print_command_line)
        {
            fmt::print("{}", "Command> ");
        }
        print_command_line = 1;

        std::string input;
        std::cin >> input;

        if (input == "a" || input == "add")
        {
            std::uint16_t value;
            fmt::print("{}", "Enter add argument [value] > ");
            std::cin >> value;
            if(std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore();
                fmt::print("{}", "\n");
                std::cout << "Invalid input.\n\n";
                print_command_line = 0;
            }
            else
            {
                fmt::print("{}", "\n");
                fmt::print("{}{}{}", "Inserting value ", value, " into the tree:\n");
                fmt::print("{}", "\n");
                insert(root, value);
            }
        }

        else if (input == "r" || input == "rdm")
        {
            std::uint16_t count;
            fmt::print("{}", "Enter rdm argument [count] > ");
            std::cin >> count;
            if(std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore();
                fmt::print("{}", "\n");
                std::cout << "Invalid input.\n\n";
                print_command_line = 0;
            }
            else
            {
                fmt::print("{}", "\n");
                fmt::print("{}{}{}", "Inserting ", count, " random values into the tree:\n");
                fmt::print("{}", "\n");
                random(root, count);
            }
        }

        else if (input == "d" || input == "del")
        {
            // TODO
            fmt::print("{}", "Stub. Come back later.\n\n");
        }

        else if (input == "i" || input == "deli")
        {
            // TODO
            fmt::print("{}", "Stub. Come back later.\n\n");
        }

        else if (input == "b" || input == "bal")
        {
            balanced(root);
        }

        else if (input == "f" || input == "perf")
        {
            // TODO
            fmt::print("{}", "Stub. Come back later.\n\n");
        }

        else if (input == "h" || input == "height")
        {
            height(root);
        }

        else if (input == "p" || input == "print")
        {
            std::string print_mode;
            fmt::print("{}", "Enter print mode [pre, in, post] > ");
            std::cin >> print_mode;
            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore();
                fmt::print("{}", "\n");
                std::cout << "Invalid input.\n\n";
                print_command_line = 0;
            }
            else
            {
                if (print_mode == "pre")
                {
                    fmt::print("{}", "\n");
                    fmt::print("{}", "Printing tree in preorder traversal mode:\n");
                    fmt::print("{}", "\n");
                    if (!root)
                    {
                        fmt::print("{}", "Tree is empty.");
                    }
                    else
                    {
                        print_preorder(root);
                    }
                    fmt::print("{}", "\n\n");
                }

                else if (print_mode == "in")
                {
                    fmt::print("{}", "\n");
                    fmt::print("{}", "Printing tree in inorder traversal mode:\n");
                    fmt::print("{}", "\n");
                    if (!root)
                    {
                        fmt::print("{}", "Tree is empty.");
                    }
                    else
                    {
                        print_inorder(root);
                    }
                    fmt::print("{}", "\n\n");
                }

                else if (print_mode == "post")
                {
                    fmt::print("{}", "\n");
                    fmt::print("{}", "Printing tree in postorder traversal mode:\n");
                    fmt::print("{}", "\n");
                    if (!root)
                    {
                        fmt::print("{}", "Tree is empty.");
                    }
                    else
                    {
                        print_postorder(root);
                    }
                    fmt::print("{}", "\n\n");
                }
            }
        }

        else if (input == "l" || input == "leafes")
        {
            fmt::print("{}", "\n");
            fmt::print("{}", "Printing leafes and single parents missing children depth:\n");
            fmt::print("{}", "\n");
            print_leafes_depth(root);
            fmt::print("{}", "\n\n");
        }

        else if (input == "g" || input == "graph")
        {
            // TODO
            fmt::print("{}", "Stub. Come back later.\n\n");
        }

        else if (input == "c" || input == "com")
        {
            fmt::print("{}", "\n");
            print_command_list();
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
