#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "./skip_list/skip_list.h"
#include "./KVengine/KVengine.h"
using std::vector;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::istringstream;

int main()
{
    srand (time(NULL)); 
    KVengine kv;
    string line;
    string mode;
    string word;
    cout << "i: insert  s: search  r: remove  c: change  d: display  q: quit" << endl;
    while (std::cin >> mode)
    {
        if (mode == "q")
            break;
        else if (mode == "i")
        {
            cout << "format like XXX XXX" << endl;
            cin.ignore();
            getline(cin, line);            
            istringstream strm(line);
            vector<string> input;
            while (strm >> word)
                input.push_back(word);
            if (input.size() != 2)
            {
                cout << "format wrong" << endl;
                continue;
            }
            kv.insert(input[0], input[1]);                   
        }
        else if (mode == "s")
        {
            cout << "input key" << endl;
            cin >> word;
            auto ans =  kv.search(word);
            if (ans)
                cout << word << " : " << ans.value() << endl;
            else
                cout << "no exist" << endl;
        }
        else if (mode == "r")
        {
            cout << "input key" << endl;
            cin >> word;
            if (kv.remove(word))
                cout << "remove done" << endl;
            else
                cout << "no exist this key" << endl;

        }
        else if (mode == "c")
        {
            cin.ignore();
            getline(cin, line);
            istringstream strm(line);
            vector<string> input;
            while (strm >> word)
                input.push_back(word);
            if (word.size() != 2)
            {
                cout << "format wrong" << endl;
                continue;
            }
            if (kv.change(input[0], input[1]))
                cout << "change done" << endl;
            else
                cout << "no exist this key" << endl;
        }
        else if (mode == "d")
        {
            kv.display();
        }
        else
            cout << "you can only input i OR s OR r OR c OR d OR q!" << endl;      
    cout << "i: insert  s: search  r: remove  c: change  d: display  q: quit" << endl;
    }
    return 0;
}