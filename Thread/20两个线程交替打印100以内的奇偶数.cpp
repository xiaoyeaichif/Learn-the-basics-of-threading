

/*
	1����ĿҪ��ʹ�������߳̽����ӡ100���ڵ�������ż��
		�����������ӡ��Ҳ����˵��Դ������Ҫ��˳��Ҳ����ͬ������
				�������������������Ƿǳ����Ƶ�
*/

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable> // ��������

// ȫ�ֱ����������̷߳���
static int x = 0;
std::mutex mtx;
std::condition_variable cv;
static bool flag = true; // true����ż�����ʣ�false������������

//�����Ļص�����
void jishu_() {
    for (int i = 0; i < 50; i++) {
        std::unique_lock<std::mutex> lock(mtx);
        while (flag) { // �ȴ�ż���̴߳�ӡ���
            cv.wait(lock);
        }
        // ��ʱ���Դ�ӡ����
        std::cout << "��ӡ����: " << x << std::endl;
        x++;
        flag = true;//��־λ����Ϊtrue,�������ż����ӡ
        cv.notify_all(); // ֪ͨ��ӡż��
    }
}
//ż���Ļص�����
void oushu_() {
    for (int i = 0; i < 50; i++) {
        std::unique_lock<std::mutex> lock(mtx);
        while (!flag) { // �ȴ������̴߳�ӡ���
            cv.wait(lock);//���������ҽ���ȴ� 
        }
        // ��ʱ���Դ�ӡż��
        std::cout << "��ӡż��: " << x << std::endl;
        x++;
        flag = false;//��־λ����Ϊfalse,�������������ӡ
        cv.notify_all(); // ֪ͨ��ӡ����
    }
}

// ������
int main_thread20() {
    // ���������߳�
    std::thread thread1(jishu_);
    std::thread thread2(oushu_);

    thread1.join();
    thread2.join();
    return 0;
}
