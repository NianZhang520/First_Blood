#include<iostream>
#include<string>
using namespace std;

template <class T>
class MyArray
{
private:
    int m_capacity;
    int m_size = 0;
    T* ptr;
public:
    MyArray(int cap)//有参构造
    {
        this->m_capacity = cap;
        this->ptr = new T[m_capacity];
        //cout << "有参构造函数调用" << endl;
    }
    MyArray(const MyArray & arr)//拷贝构造
    {
        this->m_capacity = arr.m_capacity;
        this->m_size = arr.m_size;
        this->ptr = new T[arr.m_capacity];
        int i = 0;
        for(i=0; i<arr.m_size; i++)
        {
            this->ptr[i] = arr.ptr[i];
        }
        //cout << "拷贝构造函数调用" << endl;
    }
    MyArray& operator= (MyArray & arr)
    {
        if(this->ptr != NULL)
        {
            delete[] this->ptr;
            this->ptr = NULL;
        }
        this->m_capacity = arr.m_capacity;
        this->m_size = arr.m_size;
        this->ptr = new T[arr.m_capacity];
        int i = 0;
        for(i=0; i<arr.m_size; i++)
        {
            this->ptr[i] = arr.ptr[i];
        }
        //cout << "等号重载调用函数调用" << endl;
        return *this;
    }
    void push_back(T data)
    {
        this->ptr[this->m_size] = data;
        this->m_size++;
    }
    void pop_back()
    {
        this->m_size--;
    }
    T operator[] (int i)
    {
        //cout << this->ptr[i] <<endl;
        return this->ptr[i];
    }
    // T& operator[] (int i)
    // {
    //     //cout << this->ptr[i] <<endl;
    //     return this->ptr[i];
    // }
    ~MyArray()//析构函数
    {
        if (this->ptr != NULL)
        {
            delete[] this->ptr;
            this->ptr = NULL;
        }
        //cout << "析构函数调用" << endl;
    }
    int getCapacity()
    {
        //cout << this->m_capacity << endl;
        return this->m_capacity;
    }
    int getSize()
    {
        //cout << this->m_size << endl;
        return this->m_size;
    }
};

class person
{
public:
    friend ostream& operator<<(ostream& cout, person& p);
    string name;
    int age;

    person()
    {}
    person(string namee, int agee)
    {
        this->name = namee;
        this->age = agee;
    }
};

ostream& operator<<(ostream &cout, person &p)
{
    cout << "姓名：" << p.name << "\t" << "年龄" << p.age << endl;
    return cout;
}

void test01()
{
    int i = 0;
    MyArray<int>lst(10);
    for(i=0; i<5; i++)
    {
        lst.push_back(i);
    }
    MyArray<int>lst1(lst);
    MyArray<int>lst2(100);
    lst2 = lst1;
    lst2.pop_back();
    for(i=0; i<lst2.getSize(); i++)
    {
        cout << lst2[i] << endl;
    }
}
void test02()
{
    person p1("张念", 20);
    person p2("张秉驰", 10);
    person p3("张娟", 50);
    person p4("赵雪娥", 76);
    int i = 0;
    MyArray<person>lst(10);
    lst.push_back(p1);
    lst.push_back(p2);
    lst.push_back(p3);
    lst.push_back(p4);
    MyArray<person>lst1(lst);
    MyArray<person>lst2(100);
    lst2 = lst1;
    for(i=0; i<lst2.getSize(); i++)
    {
        person p;
        p = lst2[i];
        cout << p << endl;
        //cout << lst2[i] << endl;
    }
    cout << lst2.getCapacity() << endl;
    cout << lst2.getSize() << endl;
}

int main()
{
    //test01();
    test02();
}