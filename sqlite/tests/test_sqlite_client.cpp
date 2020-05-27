#include "../sqlite_client.h"
#include <iostream>
using namespace std;
using namespace outils;

int main(void) {
    string errmsg;
    SqliteClient client;
    if (!client.Open(":memory:", &errmsg)) { // db in memory
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
        return -1;
    }

    cout << "insert test data ok" << endl;

    bool ok = client.Execute("select * from abc_007", &errmsg);
    if (!ok)  {
        cerr << "query table failed: " << errmsg << endl;
    }
    cout << "ssssssssssssss1" << endl;

    ok = client.Execute("select * from abc_007", &errmsg, [] (const SqlResult* res) {
        string errmsg;
        auto meta = res->GetColumnMeta();
        bool ok = res->ForEachRow([&meta] (const SqlRow* row) -> bool {
            uint64_t id;
            row->Get(0, &id);
            cout << "get [" << meta->GetColumnName(0) << "] -> " << id << endl;
            return true;
        }, &errmsg);
        if (!ok) {
            cerr << "visit result failed: " << errmsg << endl;
        }
    });
    if (!ok)  {
        cerr << "query table failed: " << errmsg << endl;
    }

    cout << "ssssssssssssss2" << endl;

    return 0;
}
