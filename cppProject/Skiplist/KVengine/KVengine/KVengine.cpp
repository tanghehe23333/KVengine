#include "KVengine.h"

bool KVengine::insert(const std::string& key, const std::string& value)//增
{
    std::lock_guard<std::mutex> guard(mtx_);
    return data_->insert(key, value);
}
bool KVengine::remove(const std::string& key)
{
    std::lock_guard<std::mutex> guard(mtx_);
    return data_->remove(key);
}
std::optional<std::string> KVengine::search(const std::string& key)
{
    std::lock_guard<std::mutex> guard(mtx_);
    return data_->search(key);
}
bool KVengine::change(const std::string& key, const std::string& value)
{
    std::lock_guard<std::mutex> guard(mtx_);
    return data_->change(key, value);
}
void KVengine::display()
{
    std::lock_guard<std::mutex> guard(mtx_);
    data_->display();
}
void KVengine::load()
{
    reader_.open(file_, std::ifstream::in);
    std::cout << "  loading file..." << std::endl;
    std::string line;
    std::string key;
    std::string value;
    while (getline(reader_, line))
    {
        if (!get_element_from_line(line, key, value))
            continue;
        data_->insert(key, value);
        std::cout << "loading key: " << key << " value: " << value << std::endl;
    }
    reader_.close();
}
bool KVengine::get_element_from_line(std::string& line, std::string& key, std::string& value)
{
    if (line.empty())
        return false;        
    auto pos = line.find(delimiter_);
    if (pos == std::string::npos)
        return false;
    key = line.substr(0, pos);
    value = line.substr(pos + 1, line.size());
    return true;
}
void KVengine::flush()
{
    std::cout << "  flush to file..." << std::endl;
    writer_.open(file_, std::fstream::out);
    data_->flush_to_file(writer_);
    writer_.flush();
    writer_.close();
    return;
}