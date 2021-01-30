#include <iostream>
#include <vector>
using namespace std;

struct Node
{
    string key;
    vector<string> titles_or_authors;

    Node() {}
    Node(const string & key, const string & value) :key(key) {titles_or_authors.push_back(value);}
};

class HashArray
{
    int size;
    vector<Node>* array;

public:
    HashArray(int count);
    void insert(const string & key, const string & value);
    void display(const string & key);

private:
    int hash(const string & key);
};

HashArray::HashArray(int count)
{

    size = count/4;
    if (size == 0)
        size = 1;
    array = new vector<Node>[size];
}

void HashArray::insert(const string & key, const string & value)
{
    int index = hash(key);

    for (auto & x : array[index])
        if (x.key == key)
        {
            x.titles_or_authors.push_back(value);
            return;
        }

    array[index].push_back(Node(key, value));
}

void HashArray::display(const string & key)
{
    int index = hash(key);

    for (auto & x : array[index])
        if (x.key == key)
        {
            for (auto & y : x.titles_or_authors)
                    cout << y << '\n';

            return;
        }
}

int HashArray::hash(const string & key)
{
    int result = 0;

    for (size_t i=0 ; i<key.length() ; i++)
        result += key[i]*key[i];

    return result % size;
}

int main()
{
    ios::sync_with_stdio(false);
    int count;
    cin >> count;
    cin.ignore();

    HashArray authors(count);
    HashArray titles(count);


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
