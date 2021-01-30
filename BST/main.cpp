#include <iostream>
#include <set>

using namespace std;

struct Node
{
    Node* father;
    Node* smaller_son;
    Node* bigger_son;
    int value;

    Node( int value ) : father( nullptr ), smaller_son( nullptr ), bigger_son( nullptr ), value( value ) {}
};

class Bst
{
    Node *root;
    set < int > s;
    int min, max;
    int sum;

public:

    Bst();
    void build();
    void result();

private:

    void pruning( Node* current );
    void _delete( Node* to_delete );
    bool find( Node* current );
    void insert( int value );
};

Bst::Bst()
{
    root = nullptr;
}

void Bst::build()
{
    int count, tmp;

    cin >> count;

    for ( int i=0 ; i<count ; i++ )
    {
        cin >> tmp;
        insert( tmp );
    }

    cin >> min >> max;

    cin >> sum;

    pruning( root );
}

void Bst::result()
{
    if ( !find( root ) )
        cout << "NIE ZNALEZIONO";
}

void Bst::pruning( Node* current )
{
    if ( current == nullptr )
        return;
    pruning( current->bigger_son );
    pruning( current->smaller_son );
    if ( current->value < min || current->value > max )
        _delete( current );
}

void Bst::_delete( Node* to_delete )
{
    if ( to_delete->smaller_son == nullptr && to_delete->bigger_son == nullptr )
    {
        if ( to_delete == root )
        {
            delete to_delete;
            root = nullptr;
            return;
        }

        if ( to_delete->father->smaller_son == to_delete )
        {
            to_delete->father->smaller_son = nullptr;
            delete to_delete;
            return;
        }
        else
        {
            to_delete->father->bigger_son = nullptr;
            delete to_delete;
            return;
        }
    }

    if ( to_delete->smaller_son != nullptr && to_delete->bigger_son != nullptr )
    {
        Node* the_biggest_son_of_smaller = to_delete->smaller_son;

        while( the_biggest_son_of_smaller->bigger_son != nullptr )
            the_biggest_son_of_smaller = the_biggest_son_of_smaller->bigger_son;

        swap( the_biggest_son_of_smaller->value, to_delete->value );

        _delete( the_biggest_son_of_smaller );

        return;

    }

    if ( to_delete->smaller_son != nullptr )
    {
        if ( to_delete != root )
        {
            to_delete->smaller_son->father = to_delete->father;

            if ( to_delete->father->smaller_son == to_delete )
            {
                to_delete->father->smaller_son = to_delete->smaller_son;
                delete to_delete;
                return;
            }
            else
            {
                to_delete->father->bigger_son = to_delete->smaller_son;
                delete to_delete;
                return;
            }
        }
        else
        {
            to_delete->smaller_son->father = nullptr;
            root = to_delete->smaller_son;
            delete to_delete;
            return;
        }
    }
    else
    {
        if ( to_delete != root )
        {
            to_delete->bigger_son->father = to_delete->father;

            if ( to_delete->father->smaller_son == to_delete )
            {
                to_delete->father->smaller_son = to_delete->bigger_son;
                delete to_delete;
                return;
            }
            else
            {
                to_delete->father->bigger_son = to_delete->bigger_son;
                delete to_delete;
                return;
            }
        }
        else
        {
            to_delete->bigger_son->father = nullptr;
            root = to_delete->bigger_son;
            delete to_delete;
            return;
        }
    }
}

bool Bst::find( Node* current )
{
    if ( current == nullptr )
        return false;
    if ( find( current->smaller_son ) || find( current->bigger_son ) )
        return true;
    if ( current->value < min || current->value > max )
        return false;
    if ( s.find( sum - current->value ) != s.end() )
    {
        if ( sum - current->value > current->value )
            cout << current->value << ' ' << sum - current->value;

        else
            cout << sum - current->value << ' ' << current->value;

        return true;
    }
    s.insert( current->value );
    return false;
}

void Bst::insert( int value )
{

    if ( root == nullptr )
    {
        root = new Node( value );
        return;
    }

    Node* tmp = root;

    while ( true )
    {
        if ( tmp->value > value )
        {
            if ( tmp->smaller_son != nullptr )
            {
                tmp = tmp->smaller_son;
            }
            else
            {
                tmp->smaller_son = new Node( value );
                tmp->smaller_son->father = tmp;
                break;
            }
        }
        else
        {
            if ( tmp->bigger_son != nullptr )
            {
                tmp = tmp->bigger_son;
            }
            else
            {
                tmp->bigger_son = new Node( value );
                tmp->bigger_son->father = tmp;
                break;
            }
        }
    }
}

int main()
{
    ios::sync_with_stdio( false );

    Bst bst;

    bst.build();

    bst.result();

    return 0;
}