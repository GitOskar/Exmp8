#include <iostream>
using namespace std;

struct Tree
{
    int value;
    Tree* father;
    Tree* left_son;
    Tree* brother;

public:
    Tree();
    Tree( int value );
    void insert_and_build( const int & count );
    void display();
    void display_preorder( Tree* Tree );
};
Tree::Tree( int value ):value( value )
{
    father = nullptr;
    left_son = nullptr;
    brother = nullptr;
}
void Tree::insert_and_build( const int & count )
{
    int counter, new_value;
    Tree* tmp = this;

    for ( int i=0 ; i<count ; i++ )
    {
        cin >> counter;
        tmp = this;

        for ( int j=0 ; j<counter ; j++ )
        {
            cin >> new_value;

            if ( tmp->left_son == nullptr )
            {
                tmp->left_son = new Tree( new_value );
                tmp->left_son->father = tmp;
                tmp = tmp->left_son;
                continue;
            }

            tmp = tmp->left_son;

            while ( tmp->value != new_value && tmp->brother != nullptr )
                tmp = tmp->brother;

            if ( tmp->value != new_value )
            {
                tmp->brother = new Tree( new_value );
                tmp->brother->father = tmp->father;
                tmp = tmp->brother;
            }
        }
    }
}
void Tree::display_preorder( Tree* Tree )
{
    cout << Tree->value << ' ';
    if ( Tree -> left_son != nullptr )
        display_preorder( Tree -> left_son );
    if ( Tree -> brother != nullptr )
        display_preorder( Tree -> brother );
}

int main()
{
    ios::sync_with_stdio( false );
    int n;
    cin >> n;
    Tree Tree(0);

    Tree.insert_and_build( n );

    Tree.display_preorder( Tree.left_son );

    return 0;
}