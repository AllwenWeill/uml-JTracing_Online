# class 

所有class名大写开头
ex: class Test{};

## class成员变量

以m_开头
ex: m_data;

优先使用初始化列表，而不是赋值操作
ex: Test(const std::string& name, const int& data)
      :m_name(name),
       m_data(data),
       m_num(0)
    {}
    
# 指针

p+所指向的对象
ex:pT, //pointer to T
