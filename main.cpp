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
     if (!root)
    {
        fmt::print("{}", "Tree is empty. Nothing to delete.\n");
        return;
    }

    node* parent_node = nullptr;
    node* current_node = root;
    char previous_direction = ' ';
    fmt::print("{}", "Traverse: root");
    for (char left_or_right_direction : traversal_path)
    {
        if (left_or_right_direction == 'l')
        {
            if (current_node->left)
            {
                parent_node = current_node;
                current_node = current_node->left;
                fmt::print("{}", ", l");
                previous_direction = 'l';
            }
            else
            {
                std::cout << ", <l>.\nInvalid path. Can not traverse left from here.\n";
                return;
            }
        }
        else if (left_or_right_direction == 'r')
        {
            if (current_node->right)
            {
                parent_node = current_node;
                current_node = current_node->right;
                fmt::print("{}", ", r");
                previous_direction = 'r';
            }
            else
            {
                std::cout << ", <r>.\nInvalid path. Can not traverse right from here.\n";
                return;
            }
        }
        else
        {
            std::cout << "\nInvalid path. Can only use 'l' or 'r' for traversal.\n";
            return;
        }
    }

    // Case 1: Node to delete has no children.
    if (!current_node->left && !current_node->right)
    {
        if (previous_direction == 'l')
        {
            parent_node->left = nullptr;
        }
        // previous_direction == 'r'
        else
        {
            parent_node->right = nullptr;
        }
    }

    // Case 2: Node to delete has only one child.
    else if (current_node->left && !current_node->right ||
            !current_node->left &&  current_node->right)
    {
        if (previous_direction == 'l')
        {
            if (current_node->left)
            {
                parent_node->left = current_node->left;
                current_node->left = nullptr;
            }

            // current_node->right
            else
            {
                parent_node->left = current_node->right;
                current_node->right = nullptr;
            }
        }

        // previous_direction == 'r'
        else
        {
            if (current_node->left)
            {
                parent_node->right = current_node->left;
                current_node->left = nullptr;
            }

            // current_node->right
            else
            {
                parent_node->right = current_node->right;
                current_node->right = nullptr;
            }
        }
    }

    // Case 3: Node to delete has two children.
    else
    {
        if (previous_direction == 'l')
        {
            if (!current_node->left->left && !current_node->left->right ||
                 current_node->left->left && !current_node->left->right)
            {
                parent_node->left = current_node->left;
                current_node->left->right = current_node->right;
                current_node->left = nullptr;
                current_node->right = nullptr;
            }
            else
            {
                node* inorder_successor_parent = current_node->left;
                node* inorder_successor = current_node->left->right;
                while (inorder_successor->right)
                {
                    inorder_successor_parent = inorder_successor;
                    inorder_successor = inorder_successor->right;
                }
                
                if (inorder_successor->left)
                {
                    inorder_successor_parent->right = inorder_successor->left;
                }
                else
                {
                    inorder_successor_parent->right = nullptr;
                }
                inorder_successor->left = current_node->left;
                inorder_successor->right = current_node->right;
                current_node->left = nullptr;
                current_node->right = nullptr;

                parent_node->left = inorder_successor;
            }
        }

        // previous_direction == 'r'
        else
        {
            if (!current_node->left->left && !current_node->left->right ||
                 current_node->left->left && !current_node->left->right)
            {
                parent_node->right = current_node->left;
                current_node->left->right = current_node->right;
                current_node->left = nullptr;
                current_node->right = nullptr;
            }
            else
            {
                node* inorder_successor_parent = current_node->left;
                node* inorder_successor = current_node->left->right;
                while (inorder_successor->right)
                {
                    inorder_successor_parent = inorder_successor;
                    inorder_successor = inorder_successor->right;
                }

                if (inorder_successor->left)
                {
                    inorder_successor_parent->right = inorder_successor->left;
                }
                else
                {
                    inorder_successor_parent->right = nullptr;
                }
                inorder_successor->left = current_node->left;
                inorder_successor->right = current_node->right;
                current_node->left = nullptr;
                current_node->right = nullptr;

                parent_node->right = inorder_successor;
            }
        }
    }

    fmt::print("{}{}{}", "\nDeleting ", current_node->value, ".\n");
    delete(current_node);
}

// [dd]
void delete_root(node*& root)
{
    if (!root)
    {
        fmt::print("{}", "Tree is empty. Nothing to delete.\n");
        fmt::print("{}", "\n");
        return;
    }
    
    // Case 1: Root node has no children.
    if (!root->left && !root->right)
    {
        delete(root);
        root = nullptr;
    }

    // Case 2: Root node has only one child at left direction.
    else if (root->left && !root->right)
    {
        node* node_to_delete = root;
        root = root->left;
        node_to_delete->left = nullptr;
        delete(node_to_delete);
    }

    // Case 3: Root node has only one child at right direction.
    else if (!root->left &&  root->right)
    {
        node* node_to_delete = root;
        root = root->right;
        node_to_delete->right = nullptr;
        delete(node_to_delete);
    }

    // Case 4: Root node has two children.
    else
    {
        if (!root->left->left && !root->left->right ||
             root->left->left && !root->left->right)
        {
            node* node_to_delete = root;
            root = root->left;
            root->right = node_to_delete->right;
            node_to_delete->left = nullptr;
            node_to_delete->right = nullptr;
            delete(node_to_delete);
        }
        else
        {
            node* inorder_successor_current = root->left;
            node* inorder_successor_next = root->left->right;
            while (inorder_successor_next->right)
            {
                inorder_successor_current = inorder_successor_next;
                inorder_successor_next = inorder_successor_next->right;
            }
            
            if (inorder_successor_next->left)
            {
                inorder_successor_current->right = inorder_successor_next->left;
            }
            else
            {
                inorder_successor_current->right = nullptr;
            }

            node* node_to_delete = root;
            inorder_successor_next->left = root->left;
            inorder_successor_next->right = root->right;
            root = inorder_successor_next;

            node_to_delete->left = nullptr;
            node_to_delete->right = nullptr;
            delete(node_to_delete);
        }
    }
}

// [purge]
void purge(node*& root)
{
    fmt::print("{}", "\n");
    bool purging = true;
    while (purging)
    {
        fmt::print("{}", "\n");
        purging = delete_highest_value_node(root);
    }
    fmt::print("{}", "\n");
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

// height of the left and right subtree of any node differ by not more than 1
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

// [f | full]
void full(node*& root)
{
    // TODO
    fmt::print("{}", "Stub. Come back later. No check for full tree.\n");
}

// [e | complete]
void complete(node*& root)
{
    // TODO
    fmt::print("{}", "Stub. Come back later. No check for complete tree.\n");
}

// [p | perfect]
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

// [s | symmetric]
void symmetric(node*& root)
{
    // TODO
    fmt::print("{}", "Stub. Come back later. No check for symmetric tree.\n");
}

// [p | properties]
void properties(node*& root)
{
    fmt::print("{}{}{}", "Tree height is ", height(root), ".\n");
    balanced(root);
    full(root);
    complete(root);
    perfect(root);
    symmetric(root);
}

/////
///// Rotate

// [rl]
void rotate_left(node*& root)
{
    // TODO
    fmt::print("{}", "Stub. Come back later.\n");
}

// [rr]
void rotate_right(node*& root)
{
    // TODO
    fmt::print("{}", "Stub. Come back later.\n");
}

// [trl]
void traverse_and_rotate_left(node*& root, std::string traversal_path)
{
    // TODO
    fmt::print("{}", "Stub. Come back later.\n");
}

// [trr]
void traverse_and_rotate_right(node*& root, std::string traversal_path)
{
    // TODO
    fmt::print("{}", "Stub. Come back later.\n");
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
///// Save / Load

// [save]
void save_current_tree(node*& root)
{
    // TODO
    fmt::print("{}", "Stub. Come back later.\n");
}

// [load]
void load_saved_tree(node*& root)
{
    // TODO
    fmt::print("{}", "Stub. Come back later.\n");
}

// [ex]
void load_default_example_tree(node*& root)
{
    if (root)
    {
        fmt::print("{}", "Tree is not empty.\n");
        fmt::print("{}", "Purging tree:");
        purge(root);
    }
    fmt::print("{}", "Loading default example tree:\n");
    insert(root, 15);
    insert(root, 7);
    insert(root, 19);
    insert(root, 3);
    insert(root, 11);
    insert(root, 17);
    insert(root, 23);
    insert(root, 2);
    insert(root, 4);
    insert(root, 9);
    insert(root, 13);
    insert(root, 16);
    insert(root, 18);
    insert(root, 21);
    insert(root, 27);
}

/////
///// Help

void print_command_list()
{
    fmt::print("{}", "COMMANDS\n");
    fmt::print("{}", "\tAdd\n");
    fmt::print("{}", "\t[a | add] [<value>]\t Add <value> into tree.\n");
    fmt::print("{}", "\t[r | rdm] [<count>]\t Add <count> random values into tree, using mt19937.\n");
    fmt::print("{}", "\n");

    fmt::print("{}", "\tDelete\n");
    fmt::print("{}", "\t[d | del]\t\t Delete highest value node, using deterministic breadth first search.\n");
    fmt::print("{}", "\t[t] [<traversal_path>]\t Traverse the <traversal_path> and delete node.\n");
    fmt::print("{}", "\t\t\t\t Example: Command> 't rlr'. Traverse path <rlr> and delete node at r. <rl'r'>.\n");
    fmt::print("{}", "\t[dd]\t\t\t Delete root node.\n");
    fmt::print("{}", "\t[purge]\t\t\t Delete complete tree.\n");
    fmt::print("{}", "\n");

    fmt::print("{}", "\tProperties\n");
    fmt::print("{}", "\t[h | height]\t\t Check for tree height.\n");
    fmt::print("{}", "\t[b | balanced]\t\t Check for balanced tree.\n");
    fmt::print("{}", "\t[f | full]\t\t Check for full tree.\n");
    fmt::print("{}", "\t[e | complete]\t\t Check for complete tree.\n");
    fmt::print("{}", "\t[p | perfect]\t\t Check for perfect tree.\n");
    fmt::print("{}", "\t[s | symmetric]\t\t Check for symmetric tree.\n");
    fmt::print("{}", "\t[o | properties]\t Print all tree properties: h, b, f, e, p, s.\n");
    fmt::print("{}", "\n");

    fmt::print("{}", "\tRotation\n");
    fmt::print("{}", "\t[rl]\t\t\t Rotate left at root.\n");
    fmt::print("{}", "\t[rr]\t\t\t Rotate right at root.\n");
    fmt::print("{}", "\t[trl] [<traversal_path>] Traverse the <traversal_path> and rotate left.\n");
    fmt::print("{}", "\t\t\t\t Example: Command> 'trl rlr'. Traverse path <rlr> and rotate left at r, <rl'r'>.\n");
    fmt::print("{}", "\t[trr] [<traversal_path>] Traverse the <traversal_path> and rotate right.\n");
    fmt::print("{}", "\t\t\t\t Example: Command> 'trr rlr'. Traverse path <rlr> and rotate right at r, <rl'r'>.\n");
    fmt::print("{}", "\n");

    fmt::print("{}", "\tPrint\n");
    fmt::print("{}", "\t[g | graph]\t\t Print graphically visualized tree.\n");
    fmt::print("{}", "\t[l | leafes]\t\t Print depth of leafes and nodes that have single child.\n");
    fmt::print("{}", "\t[m | mode] < >\t\t Print tree in ...\n");
    fmt::print("{}", "\t\t  [pre]\t\t ... preorder traversal mode.  Print node on first visit.  Root, left, right.\n");
    fmt::print("{}", "\t\t  [in]\t\t ... inorder traversal mode.   Print node on second visit. Left, root, right.\n");
    fmt::print("{}", "\t\t  [post]\t ... postorder traversal mode. Print node on third visit.  Left, right, root.\n");
    fmt::print("{}", "\n");

    fmt::print("{}", "\tSave / Load\n");
    fmt::print("{}", "\t[save]\t\t\t Save current tree.\n");
    fmt::print("{}", "\t[load]\t\t\t Load saved tree.\n");
    fmt::print("{}", "\t[ex]\t\t\t Load default example tree.\n");
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

        /////
        ///// Add

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

        /////
        ///// Delete

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
                fmt::print("{}{}{}", "Traversing path <", traversal_path, ">:\n");
                fmt::print("{}", "\n");
                traverse_and_delete_last_node(root, traversal_path);
            }
            fmt::print("{}", "\n");
        }

        // [dd]
        else if (input == "dd")
        {
            fmt::print("{}", "Deleting root.\n");
            fmt::print("{}", "\n");
            delete_root(root);
        }

        // [purge]
        else if (input == "purge")
        {
            fmt::print("{}", "Purging tree:");
            purge(root);
        }

        /////
        ///// Properties

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

        // [c | complete]
        else if (input == "e" || input == "complete")
        {
            perfect(root);
            fmt::print("{}", "\n");
        }

        // [f | full]
        else if (input == "f" || input == "full")
        {
            perfect(root);
            fmt::print("{}", "\n");
        }

        // [p | perf]
        else if (input == "p" || input == "perfect")
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

        // [o | properties]
        else if (input == "o" || input == "properties")
        {
            properties(root);
            fmt::print("{}", "\n");
        }

        /////
        ///// Rotate

        // [rl]
        else if (input == "rl")
        {
            rotate_left(root);
            fmt::print("{}", "\n");
        }

        // [rr]
        else if (input == "rr")
        {
            rotate_right(root);
            fmt::print("{}", "\n");
        }

        // [trl]
        else if (input == "trl")
        {
            std::string traversal_path;
            fmt::print("{}", "Enter traversal path to the node to rotate left > ");
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
                fmt::print("{}{}{}", "Traversing path <", traversal_path, ">:\n");
                fmt::print("{}", "\n");
                traverse_and_rotate_left(root, traversal_path);
            }
            fmt::print("{}", "\n");
        }

        // [trr]
        else if (input == "trr")
        {
            std::string traversal_path;
            fmt::print("{}", "Enter traversal path to the node to rotate right > ");
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
                fmt::print("{}{}{}", "Traversing path <", traversal_path, ">:\n");
                fmt::print("{}", "\n");
                traverse_and_rotate_right(root, traversal_path);
            }
            fmt::print("{}", "\n");
        }

        /////
        ///// Print

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

        /////
        ///// Save / Load

        // [save]
        else if (input == "save")
        {
            save_current_tree(root);
            fmt::print("{}", "\n");
        }

        // [load]
        else if (input == "load")
        {
            load_saved_tree(root);
            fmt::print("{}", "\n");
        }

        // [ex]
        else if (input == "ex")
        {
            load_default_example_tree(root);
        }

        /////
        ///// Help

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
