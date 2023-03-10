#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <json/json.h>

using namespace std;

int main() {
    // 从文件中读取JSON字符串
    ifstream ifs("config.json");
    string content((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
    ifs.close();

    // 解析JSON字符串
    Json::Value root;
    Json::CharReaderBuilder builder;
    Json::CharReader *reader = builder.newCharReader();
    string errors;
    reader->parse(content.c_str(), content.c_str() + content.size(), &root, &errors);
    delete reader;

    // 将JSON对象转换为unordered_map
    unordered_map<string, string> configMap;
    for (auto const& key : root.getMemberNames()) {
        configMap.emplace(key, root[key].asString());
    }

    // 打印unordered_map中的配置项
    for (auto const& [key, value] : configMap) {
        cout << key << " = " << value << endl;
    }

    return 0;
}

