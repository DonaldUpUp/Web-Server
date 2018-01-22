#ifndef TASK_H_INCLUDED
#define TASK_H_INCLUDED

#include <string>

using std::string;

class CTask
{
protected:
    string m_strTaskName;
    void* m_ptrData;
public:
    CTask() {}
    CTask(string taskName)
    {
        m_strTaskName=taskName;
        m_ptrData=NULL;
    }
    virtual int Run()=0;
    //设置任务数据
    void SetData(void* data);
public:
    virtual ~CTask() {}
};



#endif // TASK_H_INCLUDED
