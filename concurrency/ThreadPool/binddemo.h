#pragma once
#include <iostream>
#include <functional>
#include <queue>
#include <thread>
#include <future>
#include <boost/type_index.hpp>
using boost::typeindex::type_id_with_cvr;

int functionint(int param) {
	std::cout << "param is " << param << std::endl;
	return 0;
}

void bindfunction() {
	std::function<int(void)> functionv = std::bind(functionint, 3);
	functionv();
}

void pushtasktoque() {
	std::function<int(void)> functionv = std::bind(functionint, 3);
	using Task = std::packaged_task<void()>;
	std::queue<Task> taskque;
	taskque.emplace([functionv]() {
		functionv();
		});
}

std::future<int> committask() {
	std::function<int(void)> functionv = std::bind(functionint, 3);
	auto taskf = std::make_shared<std::packaged_task<int(void)>>(functionv);
	auto res = taskf->get_future();
	using Task = std::packaged_task<void()>;
	std::queue<Task> taskque;
	taskque.emplace([taskf]() {
		(*taskf)();
		});

	return res;
}

template <class F, class... Args>
std::future<int> commit(F&& f, Args&&... args) {
	//....
		// 利用Boost库打印模板推导出来的 T 类型
	std::cout << "F type：" << type_id_with_cvr<F>().pretty_name() << std::endl;

	// 利用Boost库打印形参的类型
	std::cout << "f type:" << type_id_with_cvr<decltype(f)>().pretty_name() << std::endl;

	std::cout << "Args type：" << type_id_with_cvr<Args...>().pretty_name() << std::endl;

	std::cout << "args type：" << type_id_with_cvr<decltype(args)...>().pretty_name() << std::endl;

	return std::future<int>();
}

// 只要出现了左值引用最后折叠的结果都是左值引用，只有右值引用和右值引用折叠才能变成右值引用。
void reference_collapsing(){
	int a = 3;
	commit(functionint, a);
}

void reference_collapsing2(){
	commit(std::move(functionint), 3);
}


void use_rightref(int && rparam) {
	//....
}

template<typename T>
void use_tempref(T&& tparam) {
	// tparam被折叠成int&&，但是传递给use_rightref时会被当做左值来使用，所以下面这行代码会报编译错误
	// use_rightref(tparam);
	use_rightref(std::forward<T>(tparam));
}

void test_tempref() {
	use_tempref(3);
}
