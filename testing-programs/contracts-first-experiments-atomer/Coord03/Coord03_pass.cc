/**
 * C++ version of Java's original at
 * https://github.com/trxsys/gluon
 *
 * References
 *
 * [1] https://ntrs.nasa.gov/archive/nasa/casi.ntrs.nasa.gov/20030015242.pdf
 * [2] C. Artho, K. Havelund, and A. Biere, "High-level data races," Software
 *     Testing, Verification and Reliability, vol. 13, no. 4, pp. 207–227, 2003.
 *     https://doi.org/10.1002/stvr.281
 *
 * @Author VeriFIT group at www.fit.vutbr.cz (smrcka@fit.vutbr.cz)
 * @Date 2016-03-08
 *
 */

#include <thread> // thread
#include <string> // string
#include <mutex> // mutex
#include <atomic> // atomic
#include <utility> // pair, make_pair
#include <vector>
#include <iostream> // cout

#define synchronized std::lock_guard<std::mutex> lock

class Vars {
    std::mutex mtx;

protected:

    std::atomic<int> x;
    std::atomic<int> y;

public:
    Vars(int x = 0, int y = 0) : x(x), y(y) {}

    // @Atomic
    int getX() {
        synchronized{mtx};
        return x;
    }

    // @Atomic
    void setX(int x) {
        synchronized{mtx};
        this->x = x;
    }

    // @Atomic
    int getY() {
        synchronized{mtx};
        return y;
    }

    // @Atomic
    void setY(int y) {
        synchronized{mtx};
        this->y = y;
    }

    // @Atomic
    std::pair<int, int> getXY() {
        synchronized{mtx};
        return std::make_pair((int)x, (int)y);
    }

    // @Atomic
    void setXY(int x1, int x2) {
        synchronized{mtx};
        x = x1;
        y = x2;
    }
};

class T1 {

   /*
    * d1 = new Coord(1,2);
    * c.setXY(d1);
    */

public:
    Vars &v;
    T1(Vars &v) : v(v) {}

    void operator()() {
        std::cout << "Running t1 ..." << std::endl;
        v.setXY(1,2);
    }

};

class T2 {
public:
    Vars &v;
    T2(Vars &v) : v(v) {}

private:
    void use(int a){
        // Do something...
    }

public:
    void operator()() {
        std::cout << "Running t2 ..." << std::endl;
        int z = v.getX();
        use(z);
    }
};

std::mutex T3Mtx;

class T3 {

    /*
     * x3 = c.getX();
     * y3 = c.getY();
     * use(x3,y3);
     */
private:
    Vars &v;

public:
    T3(Vars &v) : v(v) {}

    void operator()() {
        std::cout << "Running t3 ..." << std::endl;
        T3Mtx.lock();
        int x3 = v.getX();
        int y3 = v.getY();
        T3Mtx.unlock();
        use(x3,y3);
    }

private:
    void use(int x3, int y3) {
        // Do something...
    }
};

class T4 {

    /*
     * x4 = c.getX();
     * use(x4);
     * d4 = c.getXY();
     * x4 = d4.getX();
     * y4 = d4.getY();
     * use(x4,y4);
     */
private:
    Vars &v;
public:
    T4(Vars &v) : v(v) {}

    void operator()() {
        std::cout << "Running t4 ..." << std::endl;
        int x4 = v.getX();
        use(x4);
        std::pair<int,int> v2 = v.getXY();
        x4 = v2.first;
        int y4 = v2.second;
        use(x4,y4);
    }

private:
    void use(int x4, int y4) {
        // Do something...
    }

    void use(int x4) {
        // Do something...
    }
};

int main() {
    Vars v;
    std::vector<std::thread> threads;

    for(int i = 0;i<10;i++){
        std::thread t1{T1{v}};
        threads.push_back(std::move(t1));

        std::thread t2{T2{v}};
        threads.push_back(std::move(t2));

        std::thread t3{T3{v}};
        threads.push_back(std::move(t3));

        std::thread t4{T4{v}};
        threads.push_back(std::move(t4));
    }

    for (auto &t : threads)
        t.join();

    return 0;
}
