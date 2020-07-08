#include "../mysql_client.h"
#include <iostream>
using namespace std;
using namespace outils;

int main(void) {
    string errmsg;
    MysqlClient client;
    if (!client.Open("127.0.0.1", 3306, "ouonline", "ouonline", "test",
                     &errmsg)) {
        cerr << "open file failed: " << errmsg << endl;
        return -1;
    }

    if (!client.Execute("create table if not exists abc_007 (`id` bigint unsigned primary key);",
                        &errmsg))  {
        cerr << "create table failed: " << errmsg << endl;
        return -1;
    }

    cout << "create table ok" << endl;

    if (!client.Execute("insert into abc_007 values (1), (2), (3), (4), (5)", &errmsg)) {
        cerr << "insert data failed: " << errmsg << endl;
    } else {
        cout << "insert test data ok" << endl;
    }

    bool ok = client.Execute("select * from abc_007", &errmsg);
    if (!ok)  {
        cerr << "query table failed: " << errmsg << endl;
    } else {
        cout << "select data ok" << endl;
    }

    ok = client.Execute("select * from abc_007", &errmsg, [] (const SqlResult* res) {
        string errmsg;
        auto meta = res->GetColumnMeta();
        bool ok = res->ForEachRow(&errmsg, [&meta] (const SqlRow* row) -> bool {
            uint64_t id;
            row->Get(0, &id);
            cout << "get [" << meta->GetColumnName(0) << "] -> " << id << endl;
            return true;
        });
        if (!ok) {
            cerr << "visit result failed: " << errmsg << endl;
        }
    });
    if (!ok)  {
        cerr << "select data with result failed: " << errmsg << endl;
    } else {
        cout << "select data ok" << endl;
    }

    ok = client.Execute("drop table abc_007", &errmsg);
    if (!ok) {
        cerr << "drop table failed: " << errmsg << endl;
    } else {
        cerr << "drop table ok" << endl;
    }

    return 0;
}
