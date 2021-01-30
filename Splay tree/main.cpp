#include <iostream>
#include <vector>

using namespace std;

struct Node
{
    Node* left_son;
    Node* right_son;
    Node* father;
    string key;
    vector<string> titles_or_authors;

    Node()
    {
        left_son = right_son = father = nullptr;
    }
    Node(string key, string value): key(key)
    {
        titles_or_authors.push_back(value);
        left_son = right_son = father = nullptr;
    }
};

class SplayTree
{
    Node* root;

public:
    SplayTree();
    void insert(string key, string value);
    void display(string key);

private:
    void splay(Node* node);
    void rotate_left(Node* node);
    void rotate_right(Node* node);
};

SplayTree::SplayTree()
{
    root = nullptr;
}

void SplayTree::insert(string key, string value)
{
    if (root == nullptr)
    {
        root = new Node(key, value);
        return;
    }

    Node* tmp = root;

    while (true)
    {
        if (tmp->key == key)
        {
            tmp->titles_or_authors.push_back(value);
            splay(tmp);
            break;
        }
        else if (tmp->key > key)
        {
            if (tmp->left_son != nullptr)
            {
               tmp = tmp->left_son;
            }
            else
            {
            tmp->left_son = new Node(key, value);
            tmp->left_son->father = tmp;
            splay(tmp->left_son);
            break;
            }
        }
        else
        {
            if ( tmp->right_son != nullptr )
            {
                tmp = tmp->right_son;
            }
            else
            {
                tmp->right_son = new Node(key, value);
                tmp->right_son->father = tmp;
                splay(tmp->right_son);
                break;
            }
        }
    }

}

void SplayTree::display(string key)
{
    Node* tmp = root;
    if (tmp == nullptr)
        return;

    while (tmp->key != key)
    {
        if (tmp->key > key)
        {
            if (tmp->left_son != nullptr)
                tmp = tmp->left_son;
            else
                break;
        }
        else
        {
            if (tmp->right_son != nullptr)
                tmp = tmp->right_son;
            else
                break;
        }
    }

    if (tmp->key == key)
    {
        for (auto & x : tmp->titles_or_authors)
            cout << x << '\n';
    }
    splay(tmp);
}

void SplayTree::splay(Node *node)
{
    while (node->father != nullptr)
    {
        Node* father = node->father;
        Node* grand_father = father->father;

        if (grand_father == nullptr)
        {
            if (father->left_son == node)
                rotate_right(node);
            else
                rotate_left(node);
        }
        else
        {
            if (father == grand_father->left_son)
            {
                if (father->left_son == node)
                {
                    rotate_right(father);
                    rotate_right(node);
                }
                else
                {
                    rotate_left(node);
                    rotate_right(node);
                }
            }
            else
            {
                if (father->left_son == node)
                {
                    rotate_right(node);
                    rotate_left(node);
                }
                else
                {
                    rotate_left(father);
                    rotate_left(node);
                }
            }
        }
    }
    root = node;
}

void SplayTree::rotate_left(Node *node)
{
    node->father->right_son = node->left_son;
    if (node->left_son != nullptr)
        node->left_son->father = node->father;
    node->left_son = node->father;
    node->father = node->father->father;
    node->left_son->father = node;

    if (node->father != nullptr)
    {
        if (node->father->left_son == node->left_son)
            node->father->left_son = node;
        else
            node->father->right_son = node;
    }
}

void SplayTree::rotate_right(Node *node)
{
    node->father->left_son = node->right_son;
    if (node->right_son != nullptr)
        node->right_son->father = node->father;
    node->right_son = node->father;
    node->father = node->father->father;
    node->right_son->father = node;

    if (node->father != nullptr)
    {
        if (node->father->left_son == node->right_son)
            node->father->left_son = node;
        else
            node->father->right_son = node;
    }
}

int main()
{
    ios::sync_with_stdio(false);
    int count;
    cin >> count;
    cin.ignore();

    SplayTree authors;
    SplayTree titles;

    string tmp1, tmp2;
    string action;
    for (int i=0 ; i<count ; i++)
    {
        getline(cin, action);

        if (action == "d")
        {
            getline(cin, tmp1);
            getline(cin, tmp2);
            authors.insert(tmp1, tmp2);
            titles.insert(tmp2, tmp1);
        }
        else if (action == "w")
        {
            getline(cin, tmp1);
            authors.display(tmp1);
        }
        else if (action == "z")
        {
            getline(cin, tmp2);
            titles.display(tmp2);
        }
    }
    return 0;
}