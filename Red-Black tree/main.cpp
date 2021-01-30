#include <iostream>

#define RED false
#define BLACK true

using namespace std;

struct List_node
{
    List_node* prev;
    List_node* next;
    string name;

    List_node(string name) :name(name) {}
};

class List
{
    List_node* head;
    List_node* tail;

public:
    List();
    List_node* push_back(string name);
    void swap_pos(List_node* n1, List_node* n2);
    void display();
    void move(List_node* node, int distance);
};

struct Node
{
    string key;
    List_node* value;
    bool colour;
    Node* smaller_son;
    Node* bigger_son;
    Node* father;

    Node(string key, List_node* value) : key(key), value(value), colour(RED), smaller_son(nullptr), bigger_son(nullptr), father(nullptr) {}
};

class Dictionary
{
    Node* root;

public:
    Dictionary();
    void insert(string key, List_node* value);
    List_node* search(string key);
    void display(Node* node, string father, string son);

private:
    void restore_red_and_black(Node* node);
    bool uncle(Node * node);
    void rotate_left_with_colour(Node* node);
    void rotate_right_with_colour(Node* node);
    void rotate_left_without_colour(Node* node);
    void rotate_right_without_colour(Node* node);
};

int main()
{
    ios::sync_with_stdio(false);

    Dictionary dictionary;
    List list;

    int name_count, action_count, distance;

    cin >> name_count;

    string name, tmp;

    for (int i=0 ; i<name_count ; i++)
    {
        cin >> name;

        dictionary.insert(name, list.push_back(name));
    }

    cin >> action_count;
    char action;

    for (int i=0 ; i<action_count ; i++)
    {
        cin >> action >> name;

        switch (action)
        {
        case 'a':
            dictionary.insert(name, list.push_back(name));
            break;

        case 'r':
            cin >> tmp;
            list.swap_pos(dictionary.search(name), dictionary.search(tmp));
            break;

        case 'm':
            cin >> distance;
            list.move(dictionary.search(name), distance);
            break;
        }
    }

    list.display();

    return 0;
}

Dictionary::Dictionary()
{
    root = nullptr;
}

void Dictionary::insert(string key, List_node* value)
{
    if (root == nullptr)
    {
        root = new Node(key, value);
        root->colour = BLACK;
        return;
    }

    Node* tmp = root;

    while (true)
    {
        if (tmp->key > key)
        {
            if (tmp->smaller_son != nullptr)
            {
                tmp = tmp->smaller_son;
            }
            else
            {
                tmp->smaller_son = new Node(key, value);
                tmp->smaller_son->father = tmp;
                tmp = tmp->smaller_son;
                break;
            }
        }
        else
        {
            if (tmp->bigger_son != nullptr)
            {
                tmp = tmp->bigger_son;
            }
            else
            {
                tmp->bigger_son = new Node(key, value);
                tmp->bigger_son->father = tmp;
                tmp = tmp->bigger_son;
                break;
            }
        }
    }

    restore_red_and_black(tmp);
}

List_node* Dictionary::search(string key)
{
    Node* tmp = root;

    while (tmp->key != key)
    {
        if (tmp->key > key)
            tmp = tmp->smaller_son;
        else
            tmp = tmp->bigger_son;
    }

    return tmp->value;
}

void Dictionary::restore_red_and_black(Node *node)
{
    if (node == root)
    {
        root->colour = BLACK;
        return;
    }

    if (node->father->colour == BLACK)
        return;

    bool uncle_colour = uncle(node);

    if (uncle_colour == RED)
    {
        node->colour = RED;
        node->father->colour = BLACK;
        node->father->father->colour = RED;
        restore_red_and_black(node->father->father);
    }
    else
    {
        if (node->father->father->smaller_son == node->father)
        {
            if (node->father->smaller_son == node)
            {
                node = node->father;
                rotate_left_with_colour(node);
            }
            else
            {
                rotate_right_without_colour(node);
                rotate_left_with_colour(node);
            }
        }
        else
        {
            if (node->father->bigger_son == node)
            {
                node = node->father;
                rotate_right_with_colour(node);
            }
            else
            {
                rotate_left_without_colour(node);
                rotate_right_with_colour(node);
            }
        }
    }
}

bool Dictionary::uncle(Node *node)
{
    if (node->father->father->smaller_son == node->father)
    {
        if(node->father->father->bigger_son == nullptr || node->father->father->bigger_son->colour == BLACK)
            return BLACK;
        else
        {
            node->father->father->bigger_son->colour = BLACK;
            return RED;
        }
    }
    else
    {
        if(node->father->father->smaller_son == nullptr || node->father->father->smaller_son->colour == BLACK)
            return BLACK;
        else
        {
            node->father->father->smaller_son->colour = BLACK;
            return RED;
        }
    }
}

void Dictionary::rotate_right_with_colour(Node *node)
{
    rotate_right_without_colour(node);

    if (node->smaller_son->colour == RED)
        node->smaller_son->colour = BLACK;
    else
        node->smaller_son->colour = RED;

    if (node->colour == RED)
        node->colour = BLACK;
    else
        node->colour = RED;

    if (node->smaller_son == root)
        root = node;
}

void Dictionary::rotate_left_with_colour(Node *node)
{
    rotate_left_without_colour(node);

    if (node->bigger_son->colour == RED)
        node->bigger_son->colour = BLACK;
    else
        node->bigger_son->colour = RED;

    if (node->colour == RED)
        node->colour = BLACK;
    else
        node->colour = RED;

    if (node->bigger_son == root)
        root = node;
}

void Dictionary::rotate_left_without_colour(Node *node)
{
    if (node->father->father != nullptr)
    {
        if (node->father->father->bigger_son == node->father)
            node->father->father->bigger_son = node;
        else
            node->father->father->smaller_son = node;
    }
    node->father->smaller_son = node->bigger_son;
    if (node->bigger_son != nullptr)
        node->bigger_son->father = node->father;
    node->bigger_son = node->father;
    node->father = node->bigger_son->father;
    node->bigger_son->father = node;
}

void Dictionary::rotate_right_without_colour(Node *node)
{
    if (node->father->father != nullptr)
    {
        if (node->father->father->bigger_son == node->father)
            node->father->father->bigger_son = node;
        else
            node->father->father->smaller_son = node;
    }
    node->father->bigger_son = node->smaller_son;
    if (node->smaller_son != nullptr)
        node->smaller_son->father = node->father;
    node->smaller_son = node->father;
    node->father = node->smaller_son->father;
    node->smaller_son->father = node;
}

List::List()
{
    head = new List_node("");
    tail = new List_node("");

    head->prev = nullptr;
    head->next = tail;
    tail->prev = head;
    tail->next = nullptr;
}

List_node* List::push_back(string name)
{
    List_node* new_node = new List_node(name);
    new_node->prev = tail->prev;
    new_node->next = tail;
    tail->prev = new_node;
    new_node->prev->next = new_node;
    return new_node;
}

void List::swap_pos(List_node *n1, List_node *n2)
{
    if (n1->prev != n2 && n1->next != n2)
    {
        n1->prev->next = n2;
        n1->next->prev = n2;

        n2->prev->next = n1;
        n2->next->prev = n1;

        swap(n1->prev, n2->prev);
        swap(n1->next, n2->next);
    }
    else
    {
        if (n1->prev == n2)
        {
            n1->prev = n2->prev;
            n2->next = n1->next;
            n2->next->prev = n2;
            n1->prev->next = n1;

            n1->next = n2;
            n2->prev = n1;
        }
        else
        {
            n2->prev = n1->prev;
            n1->next = n2->next;
            n2->prev->next = n2;
            n1->next->prev = n1;

            n2->next = n1;
            n1->prev = n2;
        }
    }
}

void List::display()
{
    List_node* tmp = head->next;

    while (tmp != tail)
    {
        cout << tmp->name << "\n";
        tmp = tmp->next;
    }
}

void List::move(List_node *node, int distance)
{
    List_node* tmp = node;
    if (distance > 0)
    {
        for (int i=0 ; i<distance ; i++)
        {
            tmp = tmp->prev;
        }

        node->prev->next = node->next;
        node->next->prev = node->prev;

        tmp->prev->next = node;
        node->prev = tmp->prev;
        node->next = tmp;
        tmp->prev = node;
    }
    else
    {
        for (int i=distance ; i<0 ; i++)
            tmp = tmp->next;

        node->prev->next = node->next;
        node->next->prev = node->prev;

        tmp->next->prev = node;
        node->next = tmp->next;
        node->prev = tmp;
        tmp->next = node;
    }
}