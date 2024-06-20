

/*
	1：题目要求，使用两个线程交替打印100以内的奇数和偶数
		分析：交替打印，也就是说资源访问需要有顺序，也就是同步问题
				和生产者消费者问题是非常相似的
*/

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable> // 条件变量

// 全局变量供两个线程访问
static int x = 0;
std::mutex mtx;
std::condition_variable cv;
static bool flag = true; // true代表偶数访问，false代表奇数访问

//奇数的回调函数
void jishu_() {
    for (int i = 0; i < 50; i++) {
        std::unique_lock<std::mutex> lock(mtx);
        while (flag) { // 等待偶数线程打印完毕
            cv.wait(lock);
        }
        // 此时可以打印奇数
        std::cout << "打印奇数: " << x << std::endl;
        x++;
        flag = true;//标志位设置为true,代表进行偶数打印
        cv.notify_all(); // 通知打印偶数
    }
}
//偶数的回调函数
void oushu_() {
    for (int i = 0; i < 50; i++) {
        std::unique_lock<std::mutex> lock(mtx);
        while (!flag) { // 等待奇数线程打印完毕
            cv.wait(lock);//解锁，并且进入等待 
        }
        // 此时可以打印偶数
        std::cout << "打印偶数: " << x << std::endl;
        x++;
        flag = false;//标志位设置为false,代表进行奇数打印
        cv.notify_all(); // 通知打印奇数
    }
}

// 主函数
int main_thread20() {
    // 创建两个线程
    std::thread thread1(jishu_);
    std::thread thread2(oushu_);

    thread1.join();
    thread2.join();
    return 0;
}
