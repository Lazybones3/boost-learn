#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <thread>
#include <sstream>
#include <vector>
#include <numeric> 

void thead_work1(std::string str) {
    std::cout << "str is " << str << std::endl;
}

class background_task {
public:
    void operator()() {
        std::cout << "background_task called" << std::endl;
    }
};

struct func {
    int& _i;
    func(int & i): _i(i){}
    void operator()() {
        for (int i = 0; i < 3; i++) {
            _i = i;
            std::cout << "_i is " << _i << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
};

void oops() {

		int some_local_state = 0;
		func myfunc(some_local_state);
		std::thread functhread(myfunc);
		//隐患，访问局部变量，局部变量可能会随着}结束而回收或随着主线程退出而回收
		functhread.detach();	
}

void use_join() {
    int some_local_state = 0;
    func myfunc(some_local_state);
    std::thread functhread(myfunc);
    functhread.join();
}

void catch_exception() {
    int some_local_state = 0;
    func myfunc(some_local_state);
    std::thread  functhread{ myfunc };
    try {
        //本线程做一些事情
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }catch (std::exception& e) {
        functhread.join();
        throw;
    }

    functhread.join();
}

class thread_guard {
private:
    std::thread& _t;
public:
    explicit thread_guard(std::thread& t):_t(t){}
    ~thread_guard() {
        //join只能调用一次
        if (_t.joinable()) {
            _t.join();
        }
    }

    thread_guard(thread_guard const&) = delete;
    thread_guard& operator=(thread_guard const&) = delete;
};

void auto_guard() {
    int some_local_state = 0;
    func my_func(some_local_state);
    std::thread  t(my_func);
    thread_guard g(t);
    //本线程做一些事情
    std::cout << "auto guard finished " << std::endl;
}


void print_str(int i, std::string const& s) {
    std::cout << "i is " << i << " str is " << s << std::endl;
}

void danger_oops(int som_param) {
    char buffer[1024];
    sprintf(buffer, "%i", som_param);
    //在线程内部将char const* 转化为std::string
    //指针常量  char * const p  指针本身不能变
    //常量指针  const char * p 指向的内容不能变    
    std::thread t(print_str, 3, buffer);
    t.detach();
    std::cout << "danger oops finished " << std::endl;
}

void safe_oops(int some_param) {
    char buffer[1024];
    sprintf(buffer, "%i", some_param);
    std::thread t(print_str, 3, std::string(buffer));
    t.detach();
}

void change_param(int& param) {
    param++;
}

void ref_oops(int some_param) {
    std::cout << "before change , param is " << some_param << std::endl;
	//需使用引用显示转换
	std::thread  t2(change_param, std::ref(some_param));
	t2.join();
    std::cout << "after change , param is " << some_param << std::endl;
}

class X
{
public:
    void do_lengthy_work() {
        std::cout << "do_lengthy_work " << std::endl;
    }
};

void bind_class_oops() {
	X my_x;
	std::thread t(&X::do_lengthy_work, &my_x);
    t.join();
}

void deal_unique(std::unique_ptr<int> p) {
    std::cout << "unique ptr data is " << *p << std::endl;
    (*p)++;

    std::cout << "after unique ptr data is " << *p << std::endl;
}

void move_oops() {
    auto p = std::make_unique<int>(100);
    std::thread  t(deal_unique, std::move(p));
    t.join();
    //不能再使用p了，p已经被move废弃
   // std::cout << "after unique ptr data is " << *p << std::endl;
}

int main()
{
    std::string hellostr = "hello world!";
    //1 通过()初始化并启动一个线程
    std::thread t1(thead_work1, hellostr);
    //2 主线程等待子线程退出
     t1.join();

    //3 t2被当作函数对象的定义，其类型为返回std::thread, 参数为background_task
	 //std::thread t21(background_task());
	 //t21.join();

    //可多加一层()
    std::thread t2((background_task()));
    t2.join();
    
    //可使用{}方式初始化
    std::thread t3{ background_task() };
    t3.join();

    //4 lambda表达式
    std::thread t4([](std::string  str) {
        std::cout << "str is " << str << std::endl;
        },  hellostr);

    t4.join();

    //5 detach 注意事项
    oops();
    //防止主线程退出过快，需要停顿一下，让子线程跑起来detach
    std::this_thread::sleep_for(std::chrono::seconds(1));

    //6  join 用法
    use_join();

    //7 捕获异常
    catch_exception();

    //8 自动守卫
    auto_guard();

    //危险可能存在崩溃
	danger_oops(100);
	std::this_thread::sleep_for(std::chrono::seconds(2));

	//安全，提前转化
	safe_oops(100);
	std::this_thread::sleep_for(std::chrono::seconds(2));

 //   //绑定引用
 //   ref_oops(100);

 //   //绑定类的成员函数
 //  bind_class_oops();

 //  //通过move传递参数

 //  move_oops();
    return 0;
}
