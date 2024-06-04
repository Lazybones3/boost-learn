#include <iostream>
#include <atomic>
#include <thread>
#include <cassert>
#include <vector>
#include <algorithm>
#include <mutex>
#include <memory>

std::atomic<bool> x, y;
std::atomic<int> z;
// memory_order_seq_cst: 全局一致性顺序，保证所有线程看到的执行顺序是一致的
// 可以用于 store, load 和 read-modify-write 操作
void write_x_then_y() {
	x.store(true, std::memory_order_seq_cst);  // 1
	y.store(true, std::memory_order_seq_cst);  // 2
}

void read_y_then_x() {
	while (!y.load(std::memory_order_seq_cst)) { // 3
		std::cout << "y load false" << std::endl;
	}

	if (x.load(std::memory_order_seq_cst)) { //4
		++z;
	}

}

void TestOrderSeqCst() {
	std::thread t1(write_x_then_y);
	std::thread t2(read_y_then_x);
	t1.join();
	t2.join();
	assert(z.load() != 0); // 5
}

// memory_order_relaxed: 宽松的顺序模型，只能保证操作的原子性和修改顺序(modification order)一致性，
// 无法保证修改后可见的顺序是一致的，也无法实现synchronizes-with的关系。
// 可以用于 store, load 和 read-modify-write 操作
void TestOrderRelaxed() {
	std::atomic<bool> rx, ry;

	// 1 sequence before 2
	std::thread t1([&]() {
		rx.store(true, std::memory_order_relaxed); // 1
		ry.store(true, std::memory_order_relaxed); // 2
		});

	// 不能保证t2看见的顺序和t1修改的顺序是一致的
	std::thread t2([&]() {
		while (!ry.load(std::memory_order_relaxed)); //3
		assert(rx.load(std::memory_order_relaxed)); //4
		});

	t1.join();
	t2.join();
}

/**
在acquire-release模型中, 会使用 memory_order_acquire, memory_order_release和memory_order_acq_rel这三种内存顺序，具体用法如下：
- 对原子变量的load可以使用memory_order_acquire内存顺序，这称为acquire操作。
- 对原子变量的store可以使用memory_order_release内存顺序，这称为release操作。
- read-modify-write操作可以使用memory_order_acquire, memory_order_release和memory_order_acq_rel:
1.如果使用memory_order_acquire, 则作为acquire操作;
2.如果使用memory_order_release, 则作为release操作;
3.如果使用memory_order_acq_rel, 则同时为两者。
如果一个acquire操作在同一个原子变量上读取到了一个release操作写入的值, 则这个release操作“synchronizes-with”这个acquire操作。
指令重排时任何指令都不能排到acquire操作的前面, 且不能重排到release操作的后面，否则会违反acquire-release的语义，即acquire-release具有原子性
*/
void TestReleaseAcquire() {
	std::atomic<bool> rx, ry;

	std::thread t1([&]() {
		// 1 sequence-before 2
		rx.store(true, std::memory_order_relaxed); // 1
		ry.store(true, std::memory_order_release); // 2
		});


	std::thread t2([&]() {
		// 2 synchronizes-with 3
		// 3 sequence-before 4
		// 所以1 happens-before 4
		while (!ry.load(std::memory_order_acquire)); //3
		assert(rx.load(std::memory_order_relaxed)); //4
		});

	t1.join();
	t2.join();
}


// 多个线程对同一个变量release操作，另一个线程对这个变量acquire，那么只有一个线程的release操作和这个acquire线程构成同步关系。
void ReleasAcquireDanger() {
	std::atomic<int> xd{0}, yd{ 0 };
	std::atomic<int> zd;

	std::thread t1([&]() {
		xd.store(1, std::memory_order_release);  // (1)
		yd.store(1, std::memory_order_release); //  (2)
		});

	std::thread t2([&]() {
		yd.store(2, std::memory_order_release);  // (3)
		});


	std::thread t3([&]() {
		// 3和4构成同步关系时断言就被触发
		while (!yd.load(std::memory_order_acquire)); //（4）
		assert(xd.load(std::memory_order_acquire) == 1); // (5)
		});

	t1.join();
	t2.join();
	t3.join();
}

/**
针对一个原子变量M的release操作A完成后, 接下来M上可能还会有一连串的其他操作，如果这一连串操作是由：(1)同一线程上的写操作；(2)或任意线程上的read-modify-write操作构成, 则称这一连串操作为以release操作A为首的release sequence。
如果一个acquire操作在同一个原子变量上读到了一个release操作写入的值, 或者读到了以这个release操作为首的release sequence写入的值, 那么这个release操作“synchronizes-with”这个acquire操作
*/
void ReleaseSequence() {
	std::vector<int> data;
	std::atomic<int> flag{ 0 };
	
	std::thread t1([&]() {
		data.push_back(42);  //(1)
		flag.store(1, std::memory_order_release); //(2)相当于操作A
		});

	std::thread t2([&]() {
		int expected = 1;
		while (!flag.compare_exchange_strong(expected, 2, std::memory_order_relaxed)) // (3)相当于read-modify-write操作
			expected = 1;
		});

	std::thread t3([&]() {
		// 2 synchronizes-with 4
		while (flag.load(std::memory_order_acquire) < 2); // (4)
		assert(data.at(0) == 42); // (5)
		});

	t1.join();
	t2.join();
	t3.join();
}

/**
使用memory_order_consume的load操作称为consume操作。
如果一个consume操作在同一个原子变量上读到了一个release操作写入的值, 或以其为首的release sequence写入的值, 则这个release操作“dependency-ordered before”这个consume操作。
*/
void ConsumeDependency() {
	std::atomic<std::string*> ptr;
	int data;

	std::thread t1([&]() {
		std::string* p = new std::string("Hello World"); // (1)
		data = 42; // (2)
		ptr.store(p, std::memory_order_release); // (3)
		});

	std::thread t2([&]() {
		std::string* p2;
		// 3 dependency-ordered before 4
		while (!(p2 = ptr.load(std::memory_order_consume))); // (4)
		assert(*p2 == "Hello World"); // (5)
		assert(data == 42); // (6)
		});

	t1.join();
	t2.join();
}


/**
new一个对象再赋值给变量时会存在多个指令顺序：
第一种情况：
1.为对象allocate一块内存空间
2.调用construct构造对象
3.将构造到的对象地址返回
第二种情况：
1.为对象allocate一块内存空间
2.先将开辟的空间地址返回
3.调用construct构造对象
如果4处采用第二种情况，线程返回了一个还没构造好的对象，而另一个线程运行到1处返回了这个还没构造好的对象，那么如果调用这个对象的成员函数就可能造成程序崩溃
*/
class SingleAuto
{
private:
	SingleAuto()
	{
	}
	SingleAuto(const SingleAuto&) = delete;
	SingleAuto& operator=(const SingleAuto&) = delete;
public:
	~SingleAuto()
	{
		std::cout << "single auto delete success " << std::endl;
	}
	static std::shared_ptr<SingleAuto> GetInst()
	{
		// 1 处
		if (single != nullptr)
		{
			return single;
		}
		// 2 处
		s_mutex.lock();
		// 3 处
		if (single != nullptr)
		{
			s_mutex.unlock();
			return single;
		}
		// 4处
		single = std::shared_ptr<SingleAuto>(new SingleAuto);
		s_mutex.unlock();
		return single;
	}
private:
	static std::shared_ptr<SingleAuto> single;
	static std::mutex s_mutex;
};


std::shared_ptr<SingleAuto> SingleAuto::single = nullptr;
std::mutex SingleAuto::s_mutex;

void TestSingle() {
	std::thread t1([]() {
		std::cout << "thread t1 singleton address is  0x: " << SingleAuto::GetInst() << std::endl;
		});
		
	std::thread t2([]() {
		std::cout << "thread t2 singleton address is 0x: " << SingleAuto::GetInst() << std::endl;
		});

	t2.join();
	t1.join();
}


// 通过内存模型改进的单例模式
class SingleMemoryModel
{
private:
	SingleMemoryModel()
	{
	}
	SingleMemoryModel(const SingleMemoryModel&) = delete;
	SingleMemoryModel& operator=(const SingleMemoryModel&) = delete;
public:
	~SingleMemoryModel()
	{
		std::cout << "single auto delete success " << std::endl;
	}
	static std::shared_ptr<SingleMemoryModel> GetInst()
	{
		// 1 处
		if (_b_init.load(std::memory_order_acquire))
		{
			return single;
		}
		// 2 处
		s_mutex.lock();
		// 3 处
		// 这里用memory_order_relaxed是因为有锁存在
		if (_b_init.load(std::memory_order_relaxed))
		{
			s_mutex.unlock();
			return single;
		}
		// 4处
		single = std::shared_ptr<SingleMemoryModel>(new SingleMemoryModel);
		_b_init.store(true, std::memory_order_release);
		s_mutex.unlock();
		return single;
	}
private:
	static std::shared_ptr<SingleMemoryModel> single;
	static std::mutex s_mutex;
	static std::atomic<bool> _b_init ;
};

std::shared_ptr<SingleMemoryModel> SingleMemoryModel::single = nullptr;
std::mutex SingleMemoryModel::s_mutex;
std::atomic<bool> SingleMemoryModel::_b_init = false;


void TestSingleMemory() {
	std::thread t1([]() {
		std::cout << "thread t1 singleton address is 0x: " << SingleMemoryModel::GetInst() << std::endl;
		});

	std::thread t2([]() {
		std::cout << "thread t2 singleton address is 0x: " << SingleMemoryModel::GetInst() << std::endl;
		});

	t2.join();
	t1.join();
}

int main()
{
	//TestOrderSeqCst();
	//TestOrderRelaxed();
	//TestReleaseAcquire();
	//ReleasAcquireDanger();
	//ReleaseSequence();
	//TestSingle();
	TestSingleMemory();
    std::cout << "Hello World!\n";
}


