#ifndef SKIP_LIST
#define SKIP_LIST
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <mutex>
#include <fstream>
#include <optional>

/****************************************跳表节点****************************************/
template<typename K, typename V> 
class Node
{
public:    
    Node() {} 
    Node(K k, V v, int); 
    ~Node();
    K get_key() const;
    V get_value() const;
    void set_value(V);        
    Node<K, V> **next_;//指向与该节点连接的所有节点，表大小为[level+1]
    int level_;//节点的层数
private:
    K key_;//键
    V value_;//值
};

template<typename K, typename V> 
Node<K, V>::Node(const K k, const V v, int level) : key_(k), value_(v), level_(level)
{    
    this->next_ = new Node<K, V>*[level + 1];//0-level
    memset(this->next_, 0, sizeof(Node<K, V>*) * (level+1));//初始化为nullptr
};

template<typename K, typename V> 
Node<K, V>::~Node()
{
    delete []next_;
};

template<typename K, typename V> 
K Node<K, V>::get_key() const
{
    return key_;
};

template<typename K, typename V> 
V Node<K, V>::get_value() const
{
    return value_;
};

template<typename K, typename V> 
void Node<K, V>::set_value(V value_)
{
    this->value_ = value_;
};

/****************************************跳表实现****************************************/ 
template <typename K, typename V> 
class SkipList
{
public: 
    SkipList(int max_level);
    ~SkipList();
    int get_random_level();
    Node<K, V>* create_node(K, V, int);
    bool insert(K, V);//增    
    std::optional<V> search(K);//查
    bool remove(K);//删
    bool change(K, V);//改
    void display();
    void display_lists();
    void flush_to_file(std::ofstream&);
    int size();
private:        
    int max_level_;             // 跳表最大层数    
    int current_level_;         // 目前的层数     
    Node<K, V> *header_;        // 表头   
    int current_count_;         //跳表节点数 
};

template<typename K, typename V>
Node<K, V>* SkipList<K, V>::create_node(const K key, const V value, int level)
{
    Node<K, V> *node = new Node<K, V>(key, value, level);
    return node;
}

template<typename K, typename V>
bool SkipList<K, V>::insert(const K key, const V value)
{    
    Node<K, V> *current = this->header_;

    Node<K, V> *update[max_level_ + 1];//找到将插入节点之前的节点
    memset(update, 0, sizeof(Node<K, V>*) * (max_level_ + 1));  

    for(int i = current_level_; i >= 0; i--)
    {
        while(current->next_[i] != NULL && current->next_[i]->get_key() < key)
        {
            current = current->next_[i]; 
        }
        update[i] = current;
    }

    current = current->next_[0];//待插入节点的下一个节点

    if (current != NULL && current->get_key() == key)//节点已存在
    {
        return false;
    }

    if (current == NULL || current->get_key() != key )//插入操作
    {       
        int random_level = get_random_level();//新节点层数
        if (random_level > current_level_)
        {
            for (int i = current_level_ + 1; i <= random_level; i++)
            {
                update[i] = header_;
            }
            current_level_ = random_level;
        }
        Node<K, V>* inserted_node = create_node(key, value, random_level);
        for (int i = 0; i <= random_level; i++)
        {
            inserted_node->next_[i] = update[i]->next_[i];
            update[i]->next_[i] = inserted_node;
        }
        ++current_count_;
    }
    return true;
}

template<typename K, typename V> 
void SkipList<K, V>::display()
{
    std::cout << "\n---------all data in database-----------"<<"\n"; 

    Node<K, V> *node = this->header_->next_[0]; 

    while (node != NULL)
    {
        std::cout << node->get_key() << " : " << node->get_value() << " ;\n";
        node = node->next_[0];
    }
    std::cout << std::endl;
    
}

template<typename K, typename V> 
void SkipList<K, V>::display_lists()
{
    std::cout << "\n*****Skip List*****"<<"\n"; 
    for (int i = 0; i <= current_level_; i++)
    {
        Node<K, V> *node = this->header_->next_[i]; 
        std::cout << "Level " << i << ": ";
        while (node != NULL)
        {
            std::cout << node->get_key() << " : " << node->get_value() << " ;";
            node = node->next_[i];
        }
        std::cout << std::endl;
    }
}

// 跳表持久化
template<typename K, typename V> 
void SkipList<K, V>::flush_to_file(std::ofstream& fstrm)
{
    Node<K, V> *node = this->header_->next_[0];
    while (node != NULL)
    {
        fstrm << node->get_key() << ":" << node->get_value() << "\n";
        node = node->next_[0];
    }
    return ;
}

template<typename K, typename V> 
int SkipList<K, V>::size()
{ 
    return current_count_;
}

template<typename K, typename V> 
bool SkipList<K, V>::remove(K key)
{
    Node<K, V> *current = this->header_; 
    Node<K, V> *update[max_level_ + 1];
    memset(update, 0, sizeof(Node<K, V>*)*(max_level_ + 1));

    for (int i = current_level_; i >= 0; i--)
    {
        while (current->next_[i] !=NULL && current->next_[i]->get_key() < key)
        {
            current = current->next_[i];
        }
        update[i] = current;
    }

    current = current->next_[0];
    if (current != NULL && current->get_key() == key)
    {       
        for (int i = 0; i <= current_level_; i++)
        {
            if (update[i]->next_[i] != current) 
                break;
            update[i]->next_[i] = current->next_[i];
        }
        current->~Node(); 
        while (current_level_ > 0 && header_->next_[current_level_] == 0)
        {
            --current_level_; 
        }
        --current_count_;
        return true;
    }
    else
        return false;
}

template<typename K, typename V> 
std::optional<V> SkipList<K, V>::search(K key)
{
    Node<K, V> *current = header_;

    for (int i = current_level_; i >= 0; i--)
    {
        while (current->next_[i] && current->next_[i]->get_key() < key)
        {
            current = current->next_[i];
        }
    }

    current = current->next_[0];

    if (current && current->get_key() == key)
    {
        return current->get_value();
    }
    return std::nullopt;
}

template<typename K, typename V> 
bool SkipList<K, V>::change(K key, V value)
{
    std::cout << "search_element-----------------" << std::endl;
    Node<K, V> *current = header_;

    for (int i = current_level_; i >= 0; i--)
    {
        while (current->next_[i] && current->next_[i]->get_key() < key)
        {
            current = current->next_[i];
        }
    }

    current = current->next_[0];

    if (current && current->get_key() == key)
    {
        current->set_value(value);
        return true;
    }
    std::cout << "Not Found Key:" << key << std::endl;
    return false;
}

template<typename K, typename V> 
SkipList<K, V>::SkipList(int max_level) : max_level_(max_level), current_level_(0), current_count_(0)
{
//跳表的头节点
    K k;
    V v;
    this->header_ = new Node<K, V>(k, v, max_level_);
};

template<typename K, typename V> 
SkipList<K, V>::~SkipList()
{
    delete header_;
}

template<typename K, typename V>
int SkipList<K, V>::get_random_level()
{
    int k = 1;
    while (rand() % 2)
    {
        k++;
    }
    k = (k < max_level_) ? k : max_level_;
    return k;
};
#endif
