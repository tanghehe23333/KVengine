#ifndef KV_ENGINE
#define KV_ENGINE
#include <string>
#include <mutex>
#include <fstream>
#include "../skip_list/skip_list.h"

class KVengine
{
public:
    KVengine(int level = 10) : skipListLevel_(level)
    {
        this->data_ = new SkipList<std::string, std::string>(10);
        this->load();
    }
    ~KVengine()
    {
        this->flush();
    }
    bool insert(const std::string& key, const std::string& value);//增
    bool remove(const std::string& key);//删
    std::optional<std::string> search(const std::string& key);//查
    bool change(const std::string& key, const std::string& value);//改
    void display();//显示
private:
    void load();
    void flush();
    bool get_element_from_line(std::string& line, std::string& key, std::string& value);
private:
    int skipListLevel_;
    SkipList<std::string, std::string>* data_;
    std::mutex mtx_;
    std::ifstream reader_;
    std::ofstream writer_;
    std::string delimiter_ = ":";
    std::string file_ = "./store/file";
};
#endif