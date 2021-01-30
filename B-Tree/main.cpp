#include <iostream>
#define T 20
#define SONS_COUNT 2*T
#define VALUES_COUNT 2*T-1
#define NAN -1
using namespace std;

struct Value
{
    double key;
    string name;

    Value(){}
    Value(double key, string name) :key(key), name(name) {}
    bool operator<(Value v);
    bool operator>(Value v);
};

bool Value::operator<(Value v)
{
    return key < v.key;
}

bool Value::operator>(Value v)
{
    return key > v.key;
}

struct Node
{
    Node* father;
    Node** sons;
    Value* values;
    bool leaf;
    int count;

    Node();
};

Node::Node()
{
    father = nullptr;
    sons = new Node*[SONS_COUNT];
    for (int i=0 ; i<SONS_COUNT ; i++)
        sons[i] = nullptr;
    values = new Value[VALUES_COUNT];
    leaf = true;
    count = 0;
}

class BTree
{
    Node* root;

    void split(Node* node);
    void display_inorder(Node* node, double from, double to);
    void steal_or_merge(Node* node);
    void steal_from_left(Node* father, int left_index, int right_index);
    void steal_from_right(Node* father, int left_index, int right_index);
    void merge(Node *father, int left_son, int right_son);

public:
    BTree();
    bool empty();
    Node* find_leaf(Value value);
    void insert(Value value);
    string delet(double key);
    void display(double from, double to);
};

void BTree::split(Node *node)
{
    Value value_to_insert = node->values[T-1];

    if (node->father == nullptr) //if node is root
    {
        node->father = new Node;
        root = node->father;
        root->sons[0] = node;
    }

    Node* tmp = node->father;

    int pos = 0;
    //finding pos
    while (pos < SONS_COUNT && tmp->sons[pos] != node)
        pos++;

    for (int i = tmp->count ; i != pos ; i--)
    {
        //relocating keys
        tmp->values[i] = tmp->values[i-1];
        //relocating sons
        tmp->sons[i+1] = tmp->sons[i];
    }

    tmp->values[pos] = value_to_insert;
    tmp->sons[pos+1] = new Node;
    tmp->count++;

    Node* second_son = tmp->sons[pos+1];
    second_son->leaf = node->leaf;
    int it = 0;
    for (int i = T ; i<VALUES_COUNT ; i++)
    {
        second_son->values[it] = node->values[i];
        second_son->sons[it] = node->sons[i];
        if (!second_son->leaf)
            second_son->sons[it]->father = second_son;
        node->sons[i] = nullptr;
        it++;
    }
    second_son->sons[T-1] = node->sons[VALUES_COUNT];
    if (!second_son->leaf)
        second_son->sons[T-1]->father = second_son;
    node->sons[VALUES_COUNT] = nullptr;

    second_son->father = tmp;

    second_son->count = T-1;
    node->count = T-1;

    tmp->leaf = false;

    if (tmp->count == VALUES_COUNT) //if max size reached
        split(tmp);
}

void BTree::display_inorder(Node* node, double from, double to)
{
    if (to < node->values[0].key && node->sons[0] != nullptr)
    {
        display_inorder(node->sons[0], from, to);
    }
    else if (from > node->values[node->count-1].key && node->sons[node->count-1] != nullptr)
    {
        display_inorder(node->sons[node->count], from, to);
    }
    else if (!node->leaf)
    {
        int from_index = NAN, to_index = NAN;

        for (int i=0 ; i<node->count ; i++)
            if (node->values[i].key >= from)
            {
                from_index = i;
                break;
            }
        for (int i=node->count-1 ; i>=0 ; i--)
            if (node->values[i].key <= to)
            {
                to_index = i;
                break;
            }

        if (from_index != NAN && to_index != NAN)
        {
            if (node->sons[to_index+1] != nullptr)
                display_inorder(node->sons[to_index+1], from, to);

            while (to_index >= from_index)
            {
                cout << node->values[to_index].name << endl;
                if (node->sons[to_index] != nullptr)
                    display_inorder(node->sons[to_index], from, to);
                to_index--;
            }
        }
    }
    else
    {
        int from_index = NAN, to_index = NAN;

        for (int i=0 ; i<node->count ; i++)
            if (node->values[i].key >= from)
            {
                from_index = i;
                break;
            }

        for (int i=node->count-1 ; i>=0 ; i--)
            if (node->values[i].key <= to)
            {
                to_index = i;
                break;
            }

        if (from_index != NAN && to_index != NAN)
        {
            while (to_index >= from_index)
                cout << node->values[to_index--].name << endl;
        }
    }
}

void BTree::steal_or_merge(Node *node)
{
    Node* tmp = node->father;
    int it = 0;

    while (tmp->sons[it] != node)
        it++;

    if (it > 0) //if node is not first
    {
        if (tmp->sons[it-1]->count >= T) //if we can steal from left
        {
            steal_from_left(tmp, it-1, it);
        }
        else if (it < tmp->count-1) //if right exists
        {
            if (tmp->sons[it+1]->count >= T) //if we can steal from right
               steal_from_right(tmp, it, it+1);

            else //cannot steal form neighbours merge with someone
                merge(tmp, it-1, it);
        }
        else //cannot steal form left and right does not exists
        {
            merge(tmp, it-1, it);
        }
    }
    else //if was first
    {
        if (tmp->sons[it+1]->count >= T) //if we can steal from right
            steal_from_right(tmp, it, it+1);

        else //cannot steal from right and node is first
            merge(tmp, it, it+1);
    }
}

void BTree::steal_from_left(Node *father, int left_index, int right_index)
{
    Node* left_brother = father->sons[left_index];
    Node* right_brother = father->sons[right_index];

    for (int i=right_brother->count ; i>0 ; i--)
    {
        //relocating keys and sons
        right_brother->values[i] = right_brother->values[i-1];
        right_brother->sons[i+1] = right_brother->sons[i];
    }
    right_brother->sons[1] = right_brother->sons[0];
    //setting first value and first son
    right_brother->values[0] = father->values[left_index];
    right_brother->sons[0] = left_brother->sons[left_brother->count];
    if (right_brother->sons[0] != nullptr)
        right_brother->sons[0]->father = right_brother;
    //setting null on stolen son pos
    left_brother->sons[left_brother->count] = nullptr;
    right_brother->count++;
    father->values[left_index] = father->sons[left_index]->values[--left_brother->count];
}

void BTree::steal_from_right(Node *father, int left_index, int right_index)
{
    Node* left_brother = father->sons[left_index];
    Node* right_brother = father->sons[right_index];

    left_brother->values[left_brother->count++] = father->values[left_index];
    father->values[left_index] = right_brother->values[0];
    left_brother->sons[left_brother->count] = right_brother->sons[0];
    if (left_brother->sons[left_brother->count] != nullptr)
        left_brother->sons[left_brother->count]->father = left_brother;
    for (int i=1 ; i<right_brother->count ; i++)
    {
        right_brother->sons[i-1] = right_brother->sons[i];
        right_brother->values[i-1] = right_brother->values[i];
    }
    right_brother->sons[right_brother->count-1] = right_brother->sons[right_brother->count];
    right_brother->sons[right_brother->count--] = nullptr;
}

void BTree::merge(Node *father, int left_son, int right_son)
{
    Node* left_brother = father->sons[left_son];
    Node* right_brother = father->sons[right_son];
    left_brother->values[left_brother->count++] = father->values[left_son];

    for (int i = left_son ; i<father->count ; i++)
    {
        father->values[i] = father->values[i+1];
        father->sons[i+1] = father->sons[i+2];
    }
    father->sons[father->count--] = nullptr;
    for (int i=0 ; i<right_brother->count ; i++)
    {
        left_brother->values[left_brother->count] = right_brother->values[i];
        left_brother->sons[left_brother->count] = right_brother->sons[i];
        if (left_brother->sons[left_brother->count] != nullptr)
            left_brother->sons[left_brother->count]->father = left_brother;
        left_brother->count++;
    }
    left_brother->sons[left_brother->count] = right_brother->sons[right_brother->count];
    if (left_brother->sons[left_brother->count] != nullptr)
        left_brother->sons[left_brother->count]->father = left_brother;
    delete right_brother;

    if (father == root)
    {
        if (father->count == 0)
        {
            root = left_brother;
            delete father;
            left_brother->father = nullptr;
        }
        else if (father->count < T-1)
        {
            steal_or_merge(father);
        }
    }
}

BTree::BTree()
{
    root = nullptr;
}

bool BTree::empty()
{
    return root == nullptr;
}

Node *BTree::find_leaf(Value value)
{
    Node* tmp = root;

    while(!tmp->leaf)
    {
        if (tmp->values[0] > value)
        {
            tmp = tmp->sons[0];
        }
        else
        {
            int it=1;

            while(tmp->values[it] < value && it<tmp->count)
                it++;

            tmp = tmp->sons[it];
        }
    }

    return tmp;
}

void BTree::insert(Value value)
{
    if (empty())
    {
        root = new Node;
        root->values[0] = value;
        root->count++;
        return;
    }

    Node *tmp = find_leaf(value);
    int pos = 0;
    while (pos < tmp->count && tmp->values[pos] < value)
        pos++;

    for (int i = tmp->count ; i != pos ; i--)
        tmp->values[i] = tmp->values[i-1];

    tmp->values[pos] = value;

    if (++tmp->count == VALUES_COUNT)
        split(tmp);
}

string BTree::delet(double key)
{
    Node* tmp = root;
    int it;
    while (true)
    {
        it = 0;

        while(tmp->values[it].key < key && it<tmp->count)
            it++;
        if (tmp->values[it].key == key)
            break;
        tmp = tmp->sons[it];
    }

    string name = tmp->values[it].name;

    if (tmp->leaf)
    {
        for ( ; it<tmp->count-1 ; it++)
        {
            tmp->values[it] = tmp->values[it+1];
        }
        tmp->count--;
        if (tmp == root)
        {
            if (root->count == 0)
            {
                delete root;
                root = nullptr;
            }
        }
        else if (tmp->count < T-1)
            steal_or_merge(tmp);

        return name;
    }
    else
    {
        Node* tmp2 = tmp->sons[it];
        while (!tmp2->leaf)
            tmp2 = tmp2->sons[tmp2->count];

        tmp->values[it] = tmp2->values[--tmp2->count];

        if (tmp2->count < T-1)
            steal_or_merge(tmp2);

        return name;
    }
}

void BTree::display(double from, double to)
{
    display_inorder(root, from, to);
}

int main()
{
    ios::sync_with_stdio(false);

    BTree bTree;

    int name_count, action_count;
    double danger_points;

    cin >> name_count;

    string name;

    for (int i=0 ; i<name_count ; i++)
    {
        cin >> name >> danger_points;

        bTree.insert(Value(danger_points, name));
    }
    cin >> action_count;
    char action;
    double tmp;
    for (int i=0 ; i<action_count ; i++)
    {
        cin >> action;

        switch (action)
        {
            case 'a':
                cin >> name >> danger_points;
                bTree.insert(Value(danger_points, name));
                break;

            case 'm':
                cin >> danger_points >> tmp;
                bTree.insert(Value(tmp, bTree.delet(danger_points)));
                break;

            case 's':
                cin >> danger_points >> tmp;
                bTree.display(danger_points, tmp);
                break;
        }
    }
    return 0;
}