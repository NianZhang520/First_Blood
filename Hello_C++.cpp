#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class worker //---------------------------------------定义一个抽象职工类，从这里可以继承出具体的普通职工类、经理类和老板类，三种子类可以通过调用同名函数打印出他们各自的职责。
{
public:

    int num; //---------------------------------------工号
    string name;//------------------------------------姓名
    int job;//----------------------------------------岗位，1代表工人，2代表经理，3代表老板
    virtual void printjob() = 0;//--------------------根据不同输入 打印不同职责的抽象函数，多态主要在这里体现
    ~worker()
    {}
};

class employee: public worker//-----------------------继承出一个普通职工类
{
public:
    employee(int num,string name, int job)
    {
        this->num = num;
        this->name = name;
        this->job = job;
    }
    virtual void printjob()
    {
        cout << "普通职工：完成各项工作" << endl;
    }
};

class manager: public worker//------------------------继承出一个经理类
{
public:
    manager(int num,string name, int job)
    {
        this->num = num;
        this->name = name;
        this->job = job;
    }
    virtual void printjob()
    {
        cout << "经理，分配各项工作" << endl;
    }
};

class boss: public worker//---------------------------继承出一个老板类
{
public:
    boss(int num,string name, int job)
    {
        this->num = num;
        this->name = name;
        this->job = job;
    }
    virtual void printjob()
    {
        cout << "老板，管理所有事务" << endl;
    }
};


class sys_manage //-----------------------------------定义一个管理系统类，从这里可以调用各种操作函数，如增加、删除、查找等。
{
public:
    sys_manage()
    {
        peopleNum = 0;
        empArray = NULL;
    }
    int peopleNum;//----------------------------------记录文件中记录的人数
    worker ** empArray;//-----------------------------定义一个指向指针数组的指针，数组里的每一项都是一个指向子类的指针，不同子类里面存储的就是不同职员的信息
    void init_mem()//---------------------------------定义一个初始化函数，主要作用是将文件中的每行数据构造成不同的子类，然后将指向这些子类的父类指针存放到堆区的指针数组中，最后用empArray接收指针数组的首地址
    {
        int len = 0, i = 0;
        char buf[1024] = {0};
        ifstream ifs;
        ifs.open("member_list.txt",ios::in);
        if(!ifs.is_open())
        {
            //cout << "文件不存在或文件为空" << endl;
        }
        else
        {
            while(ifs.getline(buf,1024))
            {
                len++;//------------------------------统计文件的行数，即为保存的人数，后续就创建相同长度的指针数组
            }
        }
        ifs.close();

        ifs.open("member_list.txt",ios::in);
        worker ** space;
        space = new worker * [len];//-----------------在堆区创建相同长度的指针数组，命名为space
        int num = 0;
        string name;
        int job;
        while(ifs >> num && ifs >> name && ifs >> job)//读取文件中每行的三个数据
        {
            worker * worker;//------------------------创建父类指针，用来指向子类对象
            switch (job)//----------------------------根据文件中每行的最后一个数据的不同，创建不同的子类
            {
                case 1:
                {
                    worker = new employee(num,name,job);
                    break;
                }
                case 2:
                {
                    worker = new manager(num,name,job);
                    break;
                }
                case 3:
                {
                    worker = new boss(num,name,job);
                    break;
                }
            }
            space[i] = worker;//----------------------将指向子类的指针放到指针数组中
            i++;
        }
        ifs.close();
        empArray = space;//---------------------------将新建的space指针赋值给empArray，从而让empArray指向存有父类指针的指针数组
        this->peopleNum = len;//----------------------将文件中的人数同步到peopleNum中
    }
    void addworker()//--------------------------------增加人员函数
    {
        init_mem();//---------------------------------添加之前，先进行初始化，让empArray指向指针数组，同时更新peopleNum，使其能够记录文件中的人数
        int num;
        string name;
        int job;
        int addlen,i = 0;
        int newlen = this->peopleNum;
        worker ** newspace = NULL;
        cout << "请输入想要添加的职工人数" << endl;
        cin >> addlen;//存储用户想要添加的职工数
        newlen = newlen + addlen;
        newspace = new worker * [newlen];//-----------在堆区构建一个新的数组，数组的长度就是原来的人数加上用户想要加入的人数
        if(this->empArray != NULL)//------------------如果原来的数组不为空，就把原来数组里的数据复制到新建的数组里面
        {
            for (i=0; i<peopleNum; i++)
            {
                newspace[i] = this->empArray[i];
                //cout << newspace[i]->name << endl;
            }
        }
        worker * worker;
        for(i=0; i<addlen; i++)
        {
            cout << "请输入第"<< i+1 << "人的工号" << endl;
            cin >> num;
            cout << "请输入第"<< i+1 << "人的姓名" << endl;
            cin >> name;
            cout << "请输入第"<< i+1 << "人的岗位" << endl;
            cout << "输入1,代表普通职工" << endl; 
            cout << "输入2,代表经理" << endl; 
            cout << "输入3,代表老板" << endl; 
            cin >> job;
            switch (job)
            {
                case 1:
                {
                    worker = new employee(num,name,job);
                    break;
                }
                case 2:
                {
                    worker = new manager(num,name,job);
                    break;
                }
                case 3:
                {
                    worker = new boss(num,name,job);
                    break;
                }
            }
            newspace[i+peopleNum] = worker;
        }
        this->peopleNum = newlen;
        delete[] this->empArray;//--------------------释放掉原来数组中的数据
        this->empArray = newspace;//------------------将指针指向新的数组

        ofstream ofs;//-------------------------------把数组中指针所指向的数据写入文件
        ofs.open("member_list.txt",ios::out);
        for(i=0; i<newlen; i++)
        {
            ofs << empArray[i]->num << '\t' << empArray[i]->name << '\t' << empArray[i]->job << endl;
        }
        ofs.close();
        cout << "已经成功添加" << addlen << "名职工" << endl;
    }


    void showworker() //------------------------------展示人员函数 
    {
        int i;
        int len = 0;
        ifstream ifs;
        char buf[1024] = {0};
        ifs.open("member_list.txt",ios::in);
        while(ifs.getline(buf,1024))
        {
            len++;//----------------------------------统计文件的行数，即为保存的人数6
        }
        ifs.close();
        init_mem();//---------------------------------先进行初始化，创建指针数组并让empArray指向它

        for(i=0; i<len; i++)
        {
            cout << "工号：" <<  empArray[i]->num << '\t';
            cout << "姓名：" <<  empArray[i]->name << '\t';
            empArray[i]->printjob();
            cout << endl;
        }

    }
    void findworker()//-------------------------------查找人员函数
    {
        init_mem();
        cout << "请输入想要查找的人的名字" << endl;
        string finder;
        cin >> finder;
        int i = 0;
        for (i=0; i<peopleNum; i++)
        {
            if(empArray[i]->name == finder)
            {
                cout << "工号：" <<  empArray[i]->num << '\t';
                cout << "姓名：" <<  empArray[i]->name << '\t';
                empArray[i]->printjob();
                cout << endl;
            }
        }
    }
    void deleteworker()//-----------------------------删除人员函数
    {
        init_mem();
        cout << "请输入想要删除的人的名字" << endl;
        string deleter;
        cin >> deleter;
        int i = 0, j = 0;
        for (i=0; i<peopleNum; i++)
        {
            if(empArray[i]->name == deleter)
            {
                for (j=i; j<peopleNum; j++)
                {
                    empArray[j] = empArray[j+1];
                }
                this->peopleNum--;
            }
        }
        ofstream ofs;//-------------------------------把修改后的数据写入文件
        ofs.open("member_list.txt",ios::out);
        for(i=0; i<peopleNum; i++)
        {
            ofs << empArray[i]->num << '\t' << empArray[i]->name << '\t' << empArray[i]->job << endl;
        }
        ofs.close();
        cout << "成功删除该员工" << endl;
    }
};

void showmenu() //------------------------------------展示菜单的函数
{
    cout << "**************************" << endl;
    cout << "***欢迎使用职工管理系统***" << endl;
    cout << "******0.退出管理程序******" << endl;
    cout << "******1.增加职工信息******" << endl;
    cout << "******2.显示职工信息******" << endl;
    cout << "******3.查找职工信息******" << endl;
    cout << "******4.删除离职员工******" << endl;
    cout << "**************************" << endl;
}

void startsystem()
{
    sys_manage sm;
    while(true)
    {
        showmenu();
        int c;
        cin >> c;
        switch (c)
        {
            case 0:
            {
                cout << "欢迎下次使用！" << endl;
                return;
            }
            case 1:
            {
                sm.addworker();
                break;
            }
            case 2:
            {   
                sm.showworker();
                break;
            }
            case 3:
            {
                sm.findworker();
                break;
            }
            case 4:
            {
                sm.deleteworker();
                break;
            }
            default:
            {
                cout << "您输入的数字有误" << endl;
            }
        } 
    }
}
int main(){
    startsystem();
    return 0;
}