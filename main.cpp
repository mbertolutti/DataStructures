// bst_interactive

#include <fmt/core.h>
#include <cstdint>
#include <iostream>
#include <string>
#include <random>
#include <stack>
#include <utility>
#include <vector>
#include <queue>

/////
///// Config

const static std::string version = "v1.3c";

/////
///// Custom Data Structure

struct node
{
    std::int16_t value = 0;
    node* left = nullptr;
    node* right = nullptr;
};

/////
///// Misc

void print_logo()
{
    fmt::print(    "{}", "OOO  OOOO    OOOO  OOOOO  OOOO  \n");
    fmt::print(    "{}", " O   O   O  O        O    O   O \n");
    fmt::print(    "{}", " O   OOOO    OOOO    O    OOOO  \n");
    fmt::print(    "{}", " O   O   O       O   O    O   O \n");
    fmt::print("{}{}{}", "OOO  OOOO    OOOO    O    OOOO   ", version, ".\n");
    fmt::print("    {}", "\n");
}

void print_welcome_message()
{
    fmt::print("{}{}{}", "Welcome to the Interactive Binary Search Tree Builder ", version, ".\n\n");
}

void print_goodbye_message()
{
    fmt::print("{}{}{}", "Thank you for using the Interactive Binary Search Tree Builder ", version, ".\n\n");
}

/////
///// Add

// [a | add] [<value>]
void insert_helper(node*& root, std::int16_t value)
{
    if (!root)
    {
        node* new_node = new node;
        new_node->value = value;
        root = new_node;
        fmt::print("{}{}{}", ".\nInsert value ", value, ".\n");
        return;
    }
    else if (root->value > value)
    {
        fmt::print("{}", ", l");
        insert_helper(root->left, value);
    }
    else
    {
        fmt::print("{}", ", r");
        insert_helper(root->right, value);
    }
}

void insert(node*& root, std::int16_t value)
{
    fmt::print("{}", "Traverse: root");
    insert_helper(root, value);
    fmt::print("{}", "\n");
}

// [r | rdm] [<count>]
void random(node*& root, std::int16_t count)
{
    std::mt19937 gen32 = std::mt19937(time(nullptr));
    for (std::int16_t i = 0; i < count; ++i)
    {
        fmt::print("{}{}{}{}{}", "Insert node ", i + 1, " / ", count,  ":\n");
        insert(root, gen32() % 100);
    }
}

/////
///// Delete

// [d | del]
bool delete_highest_value_node(node*& root)
{
    if (!root)
    {
        fmt::print("{}", "Tree is empty. Nothing to delete.\n");
        return false;
    }

    node* parent_node = nullptr;
    node* current_node = root;

    fmt::print("{}", "Traverse: root");
    // Traverse to last node. (Highest value.)
    while (current_node->right)
    {
        fmt::print("{}", ", r");
        parent_node = current_node;
        current_node = parent_node->right;
    }

    fmt::print("{}", ".\n");
    if (current_node->left)
    {
        if (!parent_node)
        {
            root = current_node->left;
        }
        else
        {
            parent_node->right = current_node->left;
        }
        current_node->left = nullptr;
    }
    else
    {
        if (parent_node)
        {
            parent_node->right = nullptr;
        }
        else
        {
            root = nullptr;
        }
    }
    fmt::print("{}{}{}", "Deleting ", current_node->value, ".\n");
    delete(current_node);
    return true;
}

// [t] [<traverse>]
void traverse_and_delete_last_node(node*& root, std::string traversal_path)
{
    // TODO
    fmt::print("{}", "Stub. Come back later.\n");
}

// [purge]
void purge(node*& root)
{
    while (delete_highest_value_node(root));
}

/////
///// Properties

// [h | height]
std::int16_t height(node*& root)
{
    std::int16_t tree_height = 0;
    std::stack<std::pair<node*, std::int16_t>> todo_stack;
    if (root)
    {
        todo_stack.push(std::make_pair(root, 0));
    }
    while (!todo_stack.empty())
    {
        std::pair<node*, std::int16_t> current_todo = todo_stack.top();
        todo_stack.pop();
        node* current_node = current_todo.first;
        std::int16_t current_height = current_todo.second;
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
    return tree_height;
}

// [b | bal]
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
// [h | height]
void balanced(node*& root)
{
    if (!root)
    {
        fmt::print("{}", "Tree is empty. No check for balanced tree.\n");
        return;
    }

    std::int16_t balanced = true;
    std::vector<int> leafes = {};
    leafes_helper(root, 0, leafes);
    for (std::int16_t i = 1; i < leafes.size(); ++i)
    {
        if (abs(leafes[i - 1] - leafes[i]) > 1)
        {
            balanced = false;
            break;
        }
    }
    fmt::print("{}{}{}", "Tree ", (balanced ? "is " : "is not "), "balanced.\n");
}

void perfect(node*& root)
{
    if(!root)
    {
        fmt::print("{}", "Tree is empty. No check for perfect tree.");
        fmt::print("{}", "\n");
        return;
    }

    std::int16_t perfect = true;
    std::int16_t level_before_leaf = height(root);
    std::queue<node*> todo_queue;
    todo_queue.push(root);
    uint16_t node_count_for_previous_level = 1;
    uint16_t node_count_for_current_level = 1;
    while (perfect)
    {
        if (level_before_leaf == 1)
        {
            bool last_node_or_not_perfect = false;
            while (node_count_for_current_level--)
            {
                node* current_node = todo_queue.front();
                todo_queue.pop();

                if (current_node->left && current_node->right)
                {
                    if (last_node_or_not_perfect)
                    {
                        perfect = false;
                        break;
                    }
                    todo_queue.push(current_node->left);
                    todo_queue.push(current_node->right);
                }
                else
                {
                    if (!current_node->left && current_node->right)
                    {
                        perfect = false;
                        break;
                    }
                    else if (current_node->left && !current_node->right ||
                            !current_node->left && !current_node->right)
                    {
                        if (last_node_or_not_perfect)
                        {
                            perfect = false;
                            break;
                        }
                        last_node_or_not_perfect = true;
                    }
                }
            }
        }

        // loop until reaching one level before leaf
        while (node_count_for_current_level--)
        {
            node* current_node = todo_queue.front();
            todo_queue.pop();

            if (current_node->left && current_node->right)
            {
                todo_queue.push(current_node->left);
                todo_queue.push(current_node->right);
            }
            else
            {
                perfect = false;
                break;
            }
        }
        node_count_for_current_level = node_count_for_previous_level * 2;
    }
    
    fmt::print("{}{}{}", "Tree ", (perfect ? "is " : "is not "), "perfect.\n");
}

void symmetric(node*& root)
{
    // TODO
    fmt::print("{}", "Stub. Come back later.\n");
}

// [p | properties]
void properties(node*& root)
{
    fmt::print("{}{}{}", "Tree height is ", height(root), ".\n");
    balanced(root);
    perfect(root);
    symmetric(root);
}

/////
///// Print

// [g | graph]
void print_graph(node*& root)
{
    if (!root)
    {
        fmt::print("{}", "Tree is empty. No graphical print.\n");
        return;
    }

    std::vector<node*> clean_me_up_helper_for_parents_with_single_child = {};
    std::int16_t print_level = 3;
    std::queue<node*> todo_queue = {};
    todo_queue.push(root);
    
    std::int16_t stub_count = 0;
    std::int16_t current_node_count = 1;
    bool printing = true;
    while (printing && print_level > -1)
    {
        std::int16_t current_level_node_index = current_node_count;
        while (current_level_node_index)
        {
            switch (print_level)
            {
                case 3:
                {
                    fmt::print("{}", "              ");
                } break;
                case 2:
                {
                    fmt::print("{}", "         ");
                } break;
                case 1:
                {
                    fmt::print("{}", "     ");
                } break;
                case 0:
                {
                    fmt::print("{}", "  ");
                } break;
            }
            node* current_node = todo_queue.front();
            todo_queue.pop();
            fmt::print("{}", current_node->value);

            if (current_node->left)
            {
                if (current_node->value == -1)
                {
                    ++stub_count;
                }
                todo_queue.push(current_node->left);
            }
            else 
            {
                node* stub_node = new node();
                clean_me_up_helper_for_parents_with_single_child.push_back(stub_node);
                todo_queue.push(stub_node);
                stub_node->value = -1;
            }

            if (current_node->right)
            {
                if (current_node->value == -1)
                {
                    ++stub_count;
                }
                todo_queue.push(current_node->right);
            }
            else 
            {
                node* stub_node = new node();
                clean_me_up_helper_for_parents_with_single_child.push_back(stub_node);
                todo_queue.push(stub_node);
                stub_node->value = -1;
            }
            if (current_node_count == stub_count)
            {
                printing = false;
            }
            --current_level_node_index;
        }
        fmt::print("{}", "\n");
        --print_level;
        current_node_count *= 2;
        stub_count = 0;
    }

}

// Print depth of leafes and nodes that have single child.
void print_leafes_depth(node*& root)
{
    std::vector<int> leafes = {};
    
    if (!root)
    {
        fmt::print("{}", "Tree is empty. No print.");
        return;
    }
    leafes_helper(root, 0, leafes);

    fmt::print("{}", "Depth: ");
    for (std::int16_t i = 0; i < leafes.size(); ++i)
    {
        fmt::print("{}{}", leafes[i], " ");
    }
}

// [m | mode] [pre]. Print node at first visit. Print order: root, left, right.
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

// [m | mode] [in]. Print node at second visit. Print order: left, root, right.
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

// [m | mode] [post]. Print node at third visit. Print order: left, right, root.
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

/////
///// Help

void print_command_list()
{
    fmt::print("{}", "COMMANDS\n");
    fmt::print("{}", "\tAdd\n");
    fmt::print("{}", "\t[a | add] [<value>]\t Add <value> into tree.\n");
    fmt::print("{}", "\t[r | rdm] [<count>]\t Add <count> random values into tree. (mt19937.)\n");
    fmt::print("{}", "\n");

    fmt::print("{}", "\tDelete\n");
    fmt::print("{}", "\t[d | del]\t\t Delete highest value node. (deterministic breadth first search.)\n");
    fmt::print("{}", "\t[t] [<traversal_path>]\t Traverse the <traversal_path> and delete last node.\n");
    fmt::print("{}", "\t\t\t\t Example: Command> 't rlr'. Traverse path <rlr> and delete node at last r.\n");
    fmt::print("{}", "\n");
    fmt::print("{}", "\t[purge]\t\t\t Delete complete tree.\n");
    fmt::print("{}", "\n");

    fmt::print("{}", "\tProperties\n");
    fmt::print("{}", "\t[h | height]\t\t Check for tree height.\n");
    fmt::print("{}", "\t[b | bal]\t\t Check for balanced tree.\n");
    fmt::print("{}", "\t[f | perf]\t\t Check for perfect tree.\n");
    fmt::print("{}", "\t[s | symmetric]\t\t Check for symmetric tree.\n");
    fmt::print("{}", "\t[p | properties]\t Print Tree properties: h, b, f, s.\n");
    fmt::print("{}", "\n");

    fmt::print("{}", "\tPrint\n");
    fmt::print("{}", "\t[g | graph]\t\t Print graphically visualized tree.\n");
    fmt::print("{}", "\t[l | leafes]\t\t Print depth of leafes and nodes that have single child.\n");
    fmt::print("{}", "\t[m | mode] < >\t\t Print tree in ...\n");
    fmt::print("{}", "\t\t  [pre]\t\t ... preorder traversal mode.\n");
    fmt::print("{}", "\t\t  [in]\t\t ... inorder traversal mode.\n");
    fmt::print("{}", "\t\t  [post]\t ... postorder traversal mode.\n");
    fmt::print("{}", "\n");

    fmt::print("{}", "\tHelp\n");
    fmt::print("{}", "\t[c | commands]\t\t Overview of all commands.\n");
    fmt::print("{}", "\t[q | quit]\t\t Quit program.\n");
    fmt::print("{}", "\n");
}

/////
///// Command loop / Control loop

void command_loop(node*& root)
{
    fmt::print("{}", "Enter 'c' or 'commands' for an overview of all possible commands.\n\n");
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

        // [a | add]
        if (input == "a" || input == "add")
        {
            std::int16_t value;
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

        // [r | rdm]
        else if (input == "r" || input == "rdm")
        {
            std::int16_t count;
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

        // [d | del]
        else if (input == "d" || input == "del")
        {
            delete_highest_value_node(root);
            fmt::print("{}", "\n");
        }

        // [t]
        else if (input == "t")
        {
            std::string traversal_path;
            fmt::print("{}", "Enter traversal path to the node to delete > ");
            std::cin >> traversal_path;
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
                fmt::print("{}{}{}", "Traversing ", traversal_path, " to the node to delete.\n");
                fmt::print("{}", "\n");
                traverse_and_delete_last_node(root, traversal_path);
            }
            fmt::print("{}", "\n");
        }

        // [purge]
        else if (input == "purge")
        {
            purge(root);
            fmt::print("{}", "\n");
        }

        // [p | properties]
        else if (input == "p" || input == "properties")
        {
            properties(root);
            fmt::print("{}", "\n");
        }

        // [h | height]
        else if (input == "h" || input == "height")
        {
            fmt::print("{}{}{}", "Tree height is ", height(root), ".\n");
            fmt::print("{}", "\n");
        }

        // [b | bal]
        else if (input == "b" || input == "bal")
        {
            balanced(root);
            fmt::print("{}", "\n");
        }

        // [f | perf]
        else if (input == "f" || input == "perf")
        {
            perfect(root);
            fmt::print("{}", "\n");
        }

        // [s | symmetric]
        else if (input == "s" || input == "symmetric")
        {
            symmetric(root);
            fmt::print("{}", "\n");
        }

        // [g | graph]
        else if (input == "g" || input == "graph")
        {
            print_graph(root);
            fmt::print("{}", "\n");
        }

        // [l | leafes]
        else if (input == "l" || input == "leafes")
        {
            fmt::print("{}", "Printing depth of leafes and nodes that have a single child:\n");
            fmt::print("{}", "\n");
            print_leafes_depth(root);
            fmt::print("{}", "\n\n");
        }

        // [m | mode]
        else if (input == "m" || input == "mode")
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
                // [pre]
                if (print_mode == "pre")
                {
                    fmt::print("{}", "\n");
                    fmt::print("{}", "Printing tree in preorder traversal mode:\n");
                    fmt::print("{}", "\n");
                    if (!root)
                    {
                        fmt::print("{}", "Tree is empty. No print.");
                    }
                    else
                    {
                        print_preorder(root);
                    }
                    fmt::print("{}", "\n\n");
                }

                // [in]
                else if (print_mode == "in")
                {
                    fmt::print("{}", "\n");
                    fmt::print("{}", "Printing tree in inorder traversal mode:\n");
                    fmt::print("{}", "\n");
                    if (!root)
                    {
                        fmt::print("{}", "Tree is empty. No print.");
                    }
                    else
                    {
                        print_inorder(root);
                    }
                    fmt::print("{}", "\n\n");
                }

                // [post]
                else if (print_mode == "post")
                {
                    fmt::print("{}", "\n");
                    fmt::print("{}", "Printing tree in postorder traversal mode:\n");
                    fmt::print("{}", "\n");
                    if (!root)
                    {
                        fmt::print("{}", "Tree is empty. No print.");
                    }
                    else
                    {
                        print_postorder(root);
                    }
                    fmt::print("{}", "\n\n");
                }
            }
        }

        // [c | commands]
        else if (input == "c" || input == "commands")
        {
            fmt::print("{}", "\n");
            print_command_list();
        }

        // [q | quit]
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

/////
///// Entry point

int main()
{
    node* root = nullptr;

    print_welcome_message();
    print_logo();

    command_loop(root);

    print_goodbye_message();
}
