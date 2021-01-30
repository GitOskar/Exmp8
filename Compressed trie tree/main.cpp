#include <iostream>
#include <vector>
#include <map>

using namespace std;

struct MapNode;

struct Node
{
    string key;
    map <char, MapNode> sons;

    vector<string> titles_or_authors;

    Node() {}
    Node(string key) :key(key) {}
};

struct MapNode
{
    Node* node;

    MapNode() :node(nullptr) {}
};

class TrieTree
{
    Node* root;

public:
    TrieTree();
    bool find(string key);
    void insert(string key, string value);
    void display(string key);
};

TrieTree::TrieTree()
{
    root = new Node();
    root->key = "";
}

void TrieTree::insert(string key, string value)
{
    Node* father = root;
    int index = 1;
    char mapIndex = key[0];

    if (father->sons[ key[0] ].node == nullptr)
    {
        father->sons[ key[0] ].node = new Node(key);
    }
    Node* tmp = father->sons[ key[0] ].node;

    while (tmp->key != key)
    {
        int min_length = min(key.length(), tmp->key.length());

        while(tmp->key[index] == key[index] && index<min_length)
                index++;

        if (index == min_length)
        {
            if (key.length() > tmp->key.length())
            {
                father = tmp;
                if (tmp->sons[ key[index] ].node == nullptr)
                    tmp->sons[ key[index] ].node = new Node(key);
                tmp = tmp->sons[ key[index] ].node;
                mapIndex = key[index];
            }
            else
            {
                Node* newNode = new Node(key);

                newNode->sons[ tmp->key[index] ].node = tmp;
                father->sons[mapIndex].node = newNode;
                tmp = newNode;
                break;
            }
        }
        else
        {
            Node* newNode = new Node(key.substr(0, index));
            newNode->sons[ tmp->key[index] ].node = tmp;
            newNode->sons[ key[index] ].node = new Node(key);
            father->sons[mapIndex].node = newNode;
            tmp = newNode->sons[ key[index] ].node;
            break;
        }
    }
    tmp->titles_or_authors.push_back(value);
}

void TrieTree::display(string key)
{
    Node* tmp = root;
    size_t index=0;

    while(tmp->key != key)
    {
        if (index >= key.length())
            return;

        tmp = tmp->sons[ key[index] ].node;

        if (tmp == nullptr)
            return;

        index = tmp->key.length();
    }

    for (auto & x : tmp->titles_or_authors)
        cout << x << '\n';
}

int main()
{
    ios::sync_with_stdio(false);
    int count;
    cin >> count;
    cin.ignore();

    TrieTree authors;
    TrieTree titles;


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