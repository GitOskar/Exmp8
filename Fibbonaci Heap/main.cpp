#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

#define INF 2100000000

using namespace std;

struct Neighbour
{
    int v, metric;
};

struct Node
{
    int metric;
    vector < Neighbour > neighbours;

    Node(): metric(INF) {}
};

struct queue_node
{
    int index;
    Node value;
    queue_node *father, *left_son, *left_brother, *right_brother;
    bool looser;
    int level;

    queue_node()
    {
        father = left_son = left_brother = right_brother = nullptr;
        looser = false;
        level = 0;
    }

    queue_node( Node & value, int index )
    {
        father = left_son = left_brother = right_brother = nullptr;
        this->value = value;
        this->index = index;
        looser = false;
        level = 0;
    }

};

class Priority_queue
{
    queue_node *head;
    queue_node **arr_of_levels;
    queue_node *min;
    queue_node **ptr_for_element;
    int data_count, possible_lvl_count;

public:

    Priority_queue( int data_count );
    Node top();
    void pop();
    void push( Node & value, int index );
    bool empty();

private:

    void consolidate();
    void decrease_key( int index, Node & new_value );
    void push_to_roots( queue_node *ptr );
    void kill_sons( queue_node *father );
    queue_node *join_elements( queue_node *element_to_join1, queue_node *element_to_join2 );
};

Priority_queue::Priority_queue( int data_count )
{
    this->data_count = data_count;
    possible_lvl_count = ( int ) ceil ( log2 ( ( double ) data_count ) );

    head = new queue_node();
    ptr_for_element = new queue_node*[ data_count ];
    arr_of_levels = new queue_node*[ possible_lvl_count ];
    min = nullptr;

    head->father = nullptr;
    head->left_son = nullptr;
    head->left_brother = nullptr;
    head->right_brother = nullptr;

    int i;
    for ( i=0; i<possible_lvl_count ; i++ )
    {
        ptr_for_element[ i ] = nullptr;
        arr_of_levels[ i ] = nullptr;
    }

    for ( ; i<data_count ; i++ )
        ptr_for_element[ i ] = nullptr;
}

void Priority_queue::consolidate()
{
    if ( empty() )
        return;

    queue_node *tmp = head->right_brother, *element_to_join1, *element_to_join2;

    while ( tmp != nullptr )
    {
        if ( arr_of_levels[ tmp->level ] == nullptr )
        {
            arr_of_levels[ tmp->level ] = tmp;
            tmp = tmp->right_brother;
            continue;
        }
        element_to_join1 = tmp;
        element_to_join2 = arr_of_levels[ tmp->level ];

        tmp = tmp->right_brother;

        while ( arr_of_levels[ element_to_join1->level ] != nullptr )
        {
            element_to_join2 = arr_of_levels[ element_to_join1->level ];
            arr_of_levels[ element_to_join1->level ] = nullptr;
            element_to_join1 = join_elements( element_to_join1, element_to_join2 );
        }

        arr_of_levels[ element_to_join1->level ] = element_to_join1;

    }

    for ( int i=0 ; i<possible_lvl_count ; i++ )
    {
        if ( arr_of_levels[ i ] != nullptr )
        {
            if ( min == nullptr )
                min = arr_of_levels[ i ];
            else if ( arr_of_levels[ i ]->value.metric < min->value.metric )
                min = arr_of_levels[ i ];

            arr_of_levels[ i ] = nullptr;
        }
    }
}

Node Priority_queue::top()
{
    return min->value;
}

void Priority_queue::pop()
{
    kill_sons( min );

    min->left_brother->right_brother = min->right_brother;

    if ( min->right_brother != nullptr )
        min->right_brother->left_brother = min->left_brother;

    ptr_for_element[ min->index ] = nullptr;
    delete min;
    min = nullptr;

    consolidate();
}

void Priority_queue::decrease_key( int index, Node & new_value )
{
    queue_node *tmp = ptr_for_element[ index ];

    tmp->value.metric = new_value.metric;

    if ( tmp->father == nullptr )
    {
        if ( min->value.metric > tmp->value.metric )
            min = tmp;
        return;
    }

    if ( tmp->father->value.metric <= tmp->value.metric )
        return;

    queue_node *father = tmp->father;
    queue_node *it = father->left_son;
    if ( father->left_son == tmp )
        father->left_son = tmp->right_brother;
    while ( it != nullptr )
    {
        if ( it == tmp )
        {
            if ( it->left_brother != nullptr )
                it->left_brother->right_brother = it->right_brother;

            if ( it->right_brother != nullptr )
                it->right_brother->left_brother = it->left_brother;
        }

        it = it->right_brother;
    }

    push_to_roots( tmp );

    while ( true )
    {
        if ( !father->looser )
        {
            father->looser = true;
            break;
        }

        if ( father->father == nullptr )
        {
            kill_sons( father );
            break;
        }

        tmp = father;
        father = father->father;

        it = father->left_son;

        if ( father->left_son == tmp )
            father->left_son = tmp->right_brother;
        while ( it != nullptr )
        {
            if ( it == tmp )
            {
                if ( it->left_brother != nullptr )
                    it->left_brother->right_brother = it->right_brother;

                if ( it->right_brother != nullptr )
                    it->right_brother->left_brother = it->left_brother;
            }

            it = it->right_brother;
        }

        kill_sons( tmp );
        push_to_roots( tmp );
    }


}

void Priority_queue::push( Node & value, int index )
{
    if ( ptr_for_element[ index ] != nullptr )
    {
        decrease_key( index, value );
        return;
    }

    queue_node* new_element = new queue_node( value, index );
    ptr_for_element[ index ] = new_element;

    if ( min == nullptr)
    {
        min = new_element;
        head->right_brother = new_element;
        new_element->left_brother = head;
    }
    else
    {
        min->left_brother->right_brother = new_element;
        new_element->left_brother = min->left_brother;
        new_element->right_brother = min;
        min->left_brother = new_element;

        if ( new_element->value.metric < min->value.metric )
            min = new_element;
    }
}

void Priority_queue::push_to_roots( queue_node *ptr )
{
    min->left_brother->right_brother = ptr;
    ptr->left_brother = min->left_brother;
    ptr->right_brother = min;
    min->left_brother = ptr;

    if ( ptr->value.metric < min->value.metric )
        min = ptr;
    ptr->father = nullptr;
}

void Priority_queue::kill_sons( queue_node *father )
{
    queue_node *son = father->left_son, *tmp;

    while( son != nullptr )
    {
        tmp = son->right_brother;
        push_to_roots( son );
        son = tmp;
    }

    father->left_son = nullptr;
    father->looser = false;
    father->level = 0;
}

bool Priority_queue::empty()
{
    return head->right_brother == nullptr;
}

queue_node *Priority_queue::join_elements(queue_node *element_to_join1, queue_node *element_to_join2)
{
    if ( element_to_join1->value.metric < element_to_join2->value.metric )
    {
        element_to_join2->left_brother->right_brother = element_to_join2->right_brother;
        if ( element_to_join2->right_brother != nullptr )
            element_to_join2->right_brother->left_brother = element_to_join2->left_brother;

        if ( element_to_join1->left_son == nullptr )
        {
            element_to_join1->left_son = element_to_join2;
            element_to_join2->father = element_to_join1;
            element_to_join2->right_brother = nullptr;
            element_to_join2->left_brother = nullptr;
        }
        else
        {
            element_to_join1->left_son->left_brother = element_to_join2;
            element_to_join2->right_brother = element_to_join1->left_son;
            element_to_join2->father = element_to_join1;
            element_to_join2->left_brother = nullptr;
            element_to_join1->left_son = element_to_join2;
        }

        element_to_join1->level++;

        return element_to_join1;
    }
    else
    {
        element_to_join1->left_brother->right_brother = element_to_join1->right_brother;
        if ( element_to_join1->right_brother != nullptr )
            element_to_join1->right_brother->left_brother = element_to_join1->left_brother;

        if ( element_to_join2->left_son == nullptr )
        {
            element_to_join2->left_son = element_to_join1;
            element_to_join1->father = element_to_join2;
            element_to_join1->right_brother = nullptr;
            element_to_join1->left_brother = nullptr;
        }
        else
        {
            element_to_join2->left_son->left_brother = element_to_join1;
            element_to_join1->right_brother = element_to_join2->left_son;
            element_to_join1->father = element_to_join2;
            element_to_join1->left_brother = nullptr;
            element_to_join2->left_son = element_to_join1;
        }

        element_to_join2->level++;
        return element_to_join2;
    }
}