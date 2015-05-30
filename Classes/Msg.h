
//用来从resource/message.json读取中文字符串。

//使用方法：
//Msg::get()["key_word"]


#ifndef __simwar__Msg__
#define __simwar__Msg__

#include <string>
#include <fstream>
#include <iostream>

#ifdef __APPLE__
#include "../rapidjson/rapidjson.h"
#include "../rapidjson/document.h"
#include "../rapidjson/writer.h"
#include "../rapidjson/stringbuffer.h"
#endif

#ifdef _WIN32
#include "rapidjson\rapidjson.h"	
#include "rapidjson\document.h"
#include "rapidjson\writer.h"
#include "rapidjson\stringbuffer.h"
#endif

class Msg
{
public:
    static Msg& get(){
        return _instance;
    }

    std::string operator[](const std::string& key){
        if (!_loaded) {
            load();
        }
        return _doc[key.c_str()].GetString();
    }



private:
    Msg(){};
    static Msg _instance;
    bool _loaded = false;
    rapidjson::Document _doc;

    void load(){
        _loaded = true;
        std::ifstream fin;
        fin.open("message.json", std::ios::in);
        assert(fin.is_open());

        // get size
        auto pos_beg = fin.tellg();
        fin.seekg(0, std::ios::end);
        auto size = fin.tellg() - pos_beg;
        std::cout << "size = " << size <<std::endl;
        fin.seekg(0, std::ios::beg);

        std::string json_raw;
        json_raw.reserve(size);

        // read in
        std::string str;
        while (fin.good()) {
            getline(fin, str);
            json_raw.append(str);
        }

        _doc.Parse(json_raw.c_str());
        fin.close();
    }
};

#endif /* defined(__simwar__Msg__) */
