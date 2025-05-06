#include <iostream>
#include <fstream>
#include <string>
#include "json.hpp"
using namespace std;
using json = nlohmann::json;
class TS {
public:
    TS() {
        setname();
        setid();
        setpass();
    }
    void setname() {
        string f, l;
        cout << "F: ";
        cin>> f;
        cout << "L: ";
        cin>> l;
        name[0]=f;
        name[1]=l;
    }   
    void setid () {
        string id;
        cout<<"ID: ";
        cin>>id;
        ID=id;
    }
    void setpass() {
        string pass;
        cout<<"pass: ";
        cin>>pass;
        password = pass;
    }
    void to_json(json& a, const TS& ts) {
        json j;
        j["id"]=ts.ID;
        for(auto i : ts.name)
        {
            j["name"].push_back(i);
        }
        j["password"]=ts.password;
        a.push_back(j);
    }
private:
	string ID;
	string name[2];
	string password;
};
int main () {
    json TS_Array;
    ifstream in_file("TS.json");
    if (in_file.good()) {
        in_file >> TS_Array;
        in_file.close();
    }
    TS ts1;
    TS ts2;
    ts1.to_json(TS_Array, ts1);
    ts2.to_json(TS_Array, ts2);
    ofstream fout;
    fout.open("TS.json");
    fout << TS_Array.dump(4);
    fout.close();
} 