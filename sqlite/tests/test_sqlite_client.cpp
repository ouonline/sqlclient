#include "../sqlite_client.h"
#include <iostream>
using namespace std;
using namespace sqlclient;

#undef NDEBUG
#include <assert.h>

int main(void) {
    string errmsg;

    SqliteClient client;
    assert(client.Open(":memory:"));

    string sqlstr("create table if not exists abc_007 (`id` bigint unsigned primary key);");
    auto res = client.Execute(sqlstr.data(), sqlstr.size(), &errmsg);
    assert(!res && errmsg.empty());

    sqlstr = "insert into abc_007 values (1), (2), (3), (4), (5)";
    res = client.Execute(sqlstr.data(), sqlstr.size(), &errmsg);
    assert(!res && errmsg.empty());

    sqlstr = "select * from abc_007";
    res = client.Execute(sqlstr.data(), sqlstr.size(), &errmsg);
    assert(res);

    auto meta = res->GetColumnMeta();
    assert(meta->GetColumnCount() == 1);

    uint32_t counter = 0;
    const SqlRowRef* row;
    while ((row = res->GetNextRow())) {
        uint64_t id;
        row->Get(0, (int64_t*)(&id));
        cout << "get [" << meta->GetColumnName(0) << "] -> " << id << endl;
        ++counter;
    }
    assert(counter == 5);

    return 0;
}
