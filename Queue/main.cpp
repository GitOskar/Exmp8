#include <iostream>
using namespace std;
int abs(int a)
{
    if (a<0)
        return -a;
    return a;
}
struct Scientist
{
    string first_name, surname;
    int ability;
    Scientist* next;
    Scientist(string first_name, string surname, int ability):first_name(first_name),surname(surname),ability(ability){}
};
struct Node
{
    int value;
    Node* next;
    Node* prev;
    Node(int value):value(value) {}
};
class List
{
private :
    Node* head;
    Node* tail;
public :
    List();
    ~List();
    int max_or_min();
    void pop();
    void push_first_element(int value);
    void push_max(int value);
    void push_min(int value);
};
List::List()
{
    head = new Node(0);
    tail = new Node(0);
    head->next = tail;
    head->prev = nullptr;
    tail->prev = head;
    tail->next = nullptr;
}
List::~List()
{
    Node* tmp1 = head;
    Node* tmp2 = nullptr;
    while (tmp2 != tail)
    {
        tmp2 = tmp1->next;
        delete tmp1;
        tmp1=tmp2;
    }
    delete tmp1;
    head = tail = nullptr;
}
int List::max_or_min()
{
    return tail->prev->value;
}
void List::pop()
{
    Node* tmp = tail->prev;
    tail->prev = tmp->prev;
    tmp->prev->next = tail;
    delete tmp;
}
void List::push_first_element(int value)
{
    head->next = new Node(value);
    head->next->prev = head;
    head->next->next = tail;
    tail->prev = head->next;
}
void List::push_max(int value)
{
    Node* tmp1 = head->next;
    Node* tmp2;
    while (tmp1->next != nullptr && value > tmp1->value)
    {
        tmp2 = tmp1;
        tmp1 = tmp1->next;
        delete tmp2;
    }
    tmp1->prev = new Node(value);
    tmp1->prev->prev = head;
    tmp1->prev->next = tmp1;
    head->next = tmp1->prev;
    head->next->value = value;
}
void List::push_min(int value)
{
    Node* tmp1 = head->next;
    Node* tmp2;
    while (tmp1->next != nullptr && value < tmp1->value)
    {
        tmp2 = tmp1;
        tmp1 = tmp1->next;
        delete tmp2;
    }
    tmp1->prev = new Node(value);
    tmp1->prev->prev = head;
    tmp1->prev->next = tmp1;
    head->next = tmp1->prev;
    head->next->value = value;
}
static Scientist* winner_ptr;
static int size, max_different, winner_sum_different;
class Queue
{
    List max;
    List min;
    Scientist* head;
    Scientist* tail;
    int queue_size;
    int sum;
public:
    Queue(int size);
    ~Queue();
    void insert_and_set_first();
    static void join(Queue & q1, Queue & q2);
    static void check(Queue & q1, Queue & q2);
    static void show_result();
    static void push_and_pop(Queue & q1, Queue & q2);
};
Queue::Queue(int size):queue_size(size/2)
{
    head = nullptr;
    tail = nullptr;
}
Queue::~Queue()
{
    Scientist* tmp1 = head;
    Scientist* tmp2 = nullptr;;
    while (tmp2 != tail)
    {
        tmp2 = tmp1->next;
        delete tmp1;
        tmp1=tmp2;
    }
    delete tmp1;
    head = tail = nullptr;
}
void Queue::insert_and_set_first()
{
    string first_name, surname;
    int ability;
    cin >> first_name >> surname >> ability;
    head = new Scientist(first_name, surname, ability);
    max.push_first_element(head->ability);
    min.push_first_element(head->ability);
    sum = head->ability;
    Scientist* tmp = head;
    for (int i=1 ; i<queue_size ; i++)
    {
        cin >> first_name >> surname >> ability;
        tmp->next = new Scientist(first_name, surname, ability);
        tmp = tmp->next;
        sum += tmp->ability;
        max.push_max(tmp->ability);
        min.push_min(tmp->ability);
    }
    tail = tmp;
    tail->next = nullptr;
}
void Queue::join(Queue & q1, Queue & q2)
{
    q1.tail->next = q2.head;
    q2.tail->next = q1.head;
}
void Queue::check(Queue & q1, Queue & q2)
{
    if (q1.max.max_or_min() - q1.min.max_or_min() <= max_different && q2.max.max_or_min() - q2.min.max_or_min() <= max_different)
        {
            if (winner_ptr==nullptr)
            {
                winner_ptr = q1.head;
                winner_sum_different = abs(q1.sum-q2.sum);
            }
            else if (winner_sum_different > abs(q1.sum-q2.sum))
            {
                winner_ptr = q1.head;
                winner_sum_different = abs(q1.sum-q2.sum);
            }
        }
}
void Queue::show_result()
{
    int i=1;
    if (winner_ptr == nullptr)
    {
        cout << "NIE";
        return;
    }
    Scientist* tmp1 = winner_ptr;
    Scientist* tmp2 = tmp1->next;

    for (; i<size/2+1 ; i++)
    {
        cout << tmp1->first_name << " " << tmp1->surname << "\n";
        tmp1 = tmp2;
        tmp2 = tmp1->next;
    }
    cout << "\n";
    for (; i<size-1 ; i++)
    {
        cout << tmp1->first_name << " " << tmp1->surname << "\n";
        tmp1 = tmp2;
        tmp2 = tmp1->next;
    }
    cout << tmp1->first_name << " " << tmp1->surname << "\n"
         << tmp2->first_name << " " << tmp2->surname;
}
void Queue::push_and_pop(Queue &q1, Queue &q2)
{
    q1.tail->next = q2.head;
    q2.head = q2.head->next;
    q1.tail = q1.tail->next;

    q2.tail->next = q1.head;
    q1.head = q1.head->next;
    q2.tail = q2.tail->next;

    q1.sum = q1.sum - q2.tail->ability + q1.tail->ability;
    q2.sum = q2.sum - q1.tail->ability + q2.tail->ability;

    if ( q2.tail->ability == q1.max.max_or_min() )
        q1.max.pop();
    q1.max.push_max(q1.tail->ability);
    if ( q2.tail->ability == q1.min.max_or_min() )
        q1.min.pop();
    q1.min.push_min(q1.tail->ability);

    if ( q1.tail->ability == q2.max.max_or_min() )
        q2.max.pop();
    q2.max.push_max(q2.tail->ability);
    if ( q1.tail->ability == q2.min.max_or_min() )
        q2.min.pop();
    q2.min.push_min(q2.tail->ability);
}

int main()
{
    ios::sync_with_stdio(false);

    cin >> size >> max_different;
    Queue q1(size);
    Queue q2(size);

    q1.insert_and_set_first();
    q2.insert_and_set_first();
    Queue::check(q1, q2);

    for (int i=1 ; i<size/2 ; i++)
    {
        Queue::push_and_pop(q1, q2);
        Queue::check(q1, q2);
    }

    Queue::join(q1,q2);

    Queue::show_result();

    return 0;
}