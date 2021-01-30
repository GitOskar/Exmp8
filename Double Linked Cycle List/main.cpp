#include <iostream>
using namespace std;
int n;

struct Waiting_pair
{
    string word;
    int value;
    int replacing_value;
};

struct Pair
{
    Pair* prev;
    Pair* next;
    string word;
    int value;
    Pair() {}
    Pair(const string & word, const int& value):word(word), value(value) {}
    ~Pair(){ cout << word << " "; }
};
class List
{
    Pair* head;
    Waiting_pair* arr;
    unsigned int index;
public:
    List();
    ~List();
    bool empty();
    void push_back(const string & word, const int& value);
    void erase();
    void move_if_element_removed(int to_move);
    void set(Waiting_pair* ptr);
    int this_way_positive(int to_move);
    int this_way_negative(int to_move);
    void set_position(int s);
    void move_if_element_added(int to_move);
};
List::List()
{
    head = nullptr;
    arr = nullptr;
    index = 0;
}
List::~List()
{
    delete [] arr;
    arr = nullptr;
    if (empty())
        return;
    Pair* tmp = head->next;
    Pair* last = head->prev;
    while ( head != last )
    {
        delete head;
        head = tmp;
        tmp = tmp->next;
    }
    delete head;
    head = nullptr;
}

bool List::empty()
{
    return head == nullptr;
}

void List::push_back(const string& word, const int& value)
{
    if (head == nullptr)
    {
        head = new Pair(word, value);
        head->prev = head->next = head;
        return;
    }
    Pair* new_pair = new Pair(word, value);
    new_pair->prev = head->prev;
    head->prev->next = new_pair;
    head->prev = new_pair;
    new_pair->next = head;
}
void List::move_if_element_removed(int to_move)
{
    if (to_move == 0)
        return;
    if (to_move > 0)
    {
        to_move %= (n-1);
        if (to_move==0)
            {
                head = head->prev;
                return;
             }
        to_move = this_way_positive(to_move);
        if (to_move > 0)
            for (int i=0 ; i<to_move ; i++)
                head = head->next;
        else
            for (int i=0 ; i<-to_move ; i++)
                head = head->prev;
    }
    else
    {
        to_move = -to_move;
        to_move %= (n-1);
        if (to_move==0)
            {
                head = head->next;
                return;
            }
        to_move=this_way_negative(-to_move);
        if(to_move < 0)
            for (int i=0 ; i<-to_move ; i++)
                head = head->prev;
        else
            for (int i=0 ; i<to_move ; i++)
                head = head->next;
    }
}
void List::erase()
{
    if (head->prev == head && head->value != 0)
    {
        delete head;
        head = nullptr;
        return;
    }
    if (head->value == 0)
    {
        head->value = arr[index].replacing_value;
        push_back(arr[index].word, arr[index].value);
        index++;
        n++;
        if (head->value == 0 || head->value % n == 0)
            return;
        move_if_element_added(head->value);
        return;
    }
    head->prev->next = head->next;
    head->next->prev = head->prev;
    Pair* tmp = head;
    move_if_element_removed(tmp->value);
    n--;
    delete tmp;
}
void List::set(Waiting_pair *ptr)
{
    arr = ptr;
}
int List::this_way_positive(int to_move)
{
        if(2*to_move < n)
            return to_move;
        else
            return to_move-n;
}
int List::this_way_negative(int to_move)
{
        if(2*(-to_move) < n)
            return to_move;
        else
            return to_move+n;
}
void List::set_position(int s)
{
    for (int i=0 ; i<s ; i++)
        head=head->next;
}
void List::move_if_element_added(int to_move)
{
    if (to_move == 0)
        return;
    if (to_move > 0)
    {
        to_move %= n;
        if (to_move==0)
            {
                head = head->prev;
                return;
             }
        to_move = this_way_positive(to_move);
        if (to_move > 0)
            for (int i=0 ; i<to_move ; i++)
                head = head->next;
        else
            for (int i=0 ; i<-to_move ; i++)
                head = head->prev;

    }
    else
    {
        to_move = -to_move;
        to_move %= n;
        if (to_move==0)
            {
                head = head->next;
                return;
            }
        to_move=this_way_negative(-to_move);
        if(to_move < 0)
            for (int i=0 ; i<-to_move ; i++)
                head = head->prev;
        else
            for (int i=0 ; i<to_move ; i++)
                head = head->next;
    }
}
int main()
{
    ios::sync_with_stdio(false);
    int s;
    cin >> n >> s;

    string word;
    int value;

    List list;

    for (int i=0 ; i<n ; i++)
    {
        cin >> word >> value;
        list.push_back(word, value);
    }

    unsigned int p;
    cin >> p;

    Waiting_pair* arr = new Waiting_pair[p];
    for (unsigned int i=0 ; i<p ; i++ )
            cin >> arr[i].word >> arr[i].value >> arr[i].replacing_value;

    list.set(arr);
    list.set_position(s);

    while(!list.empty())
        list.erase();

    return 0;
}