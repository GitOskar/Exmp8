#include <iostream>
#include <vector>

using namespace std;

struct Node
{
    bool happy;
    vector< int > connections;

    Node() :happy( false ) {}
};

struct Dish_node
{
    int count;
    vector< int > connections;
};

class Culinary
{
    int happy_points;
    Node *arr_persons;
    Dish_node *arr_dishes;

public:

    Culinary();
    void insert_and_build();
    bool try_to_free_dish( int dish );
    int get_happy();

};

Culinary::Culinary()
{
    happy_points = 0;
    arr_dishes = nullptr;
    arr_persons = nullptr;
}

void Culinary::insert_and_build()
{
    int persons, dishes, dishes_count;

    cin >> persons >> dishes >> dishes_count;

    arr_persons = new Node[ persons ];
    arr_dishes = new Dish_node[ dishes ];

    for ( int i=0 ; i<dishes ; i++ )
        arr_dishes[ i ].count = dishes_count;

    char dish;

    for ( int i=0; i<persons ; i++ )
    {
        for ( int j=0 ; j<dishes ; j++ )
        {
            cin >> dish;

            if ( dish == 'o' )
                continue;

            arr_persons[ i ].connections.push_back( j );

            if ( arr_persons[ i ].happy )
                continue;

            if ( arr_dishes[ j ].count > 0 )
            {
                arr_persons[ i ].happy = true;
                happy_points++;
                arr_dishes[ j ].count--;
                arr_dishes[ j ].connections.push_back( i );
            }

        }

        if ( !arr_persons[ i ].happy )
        {
            for ( int x : arr_persons[ i ].connections )
            {
                if ( try_to_free_dish( x ) )
                {
                    arr_persons[ i ].happy = true;
                    happy_points++;
                    arr_dishes[ x ].connections.push_back( i );
                    break;
                }
            }
        }
    }
}

bool Culinary::try_to_free_dish( int dish )
{
    for ( auto it = arr_dishes[ dish ].connections.begin() ; it != arr_dishes[ dish ].connections.end() ; it++ )
    {
        for ( int x : arr_persons[ *it ].connections )
        {
            if ( arr_dishes[ x ].count > 0 )
            {
                arr_dishes[ x ].count--;
                arr_dishes[ x ].connections.push_back( x );
                arr_dishes[ dish ].connections.erase( it );
                return true;
            }
        }
    }
    return false;
}

int Culinary::get_happy()
{
    return happy_points;
}

int main()
{
    ios::sync_with_stdio( false );

    Culinary culinary;

    culinary.insert_and_build();

    cout << culinary.get_happy();

    return 0;
}