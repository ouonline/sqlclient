#include "../mysql_client.h"
#include <iostream>
using namespace std;
using namespace sqlclient;

#undef NDEBUG
#include <assert.h>

int main(void) {
    string errmsg;
    MysqlClient client;
    assert(client.Open("127.0.0.1", 3306, "ouonline", "ouonline", "test", &errmsg));

    string sqlstr = "create table if not exists abc_007 (`id` bigint unsigned primary key);";
    auto res = client.Execute(sqlstr.data(), sqlstr.size(), &errmsg);
    assert(!res && errmsg.empty());

    sqlstr = "insert into abc_007 values (1), (2), (3), (4), (5)";
    res = client.Execute(sqlstr.data(), sqlstr.size(), &errmsg);
    assert(!res && errmsg.empty());

    sqlstr = "select * from abc_007";
    res = client.Execute(sqlstr.data(), sqlstr.size(), &errmsg);
    assert(res);

    auto meta = res->GetColumnInfo();
    assert(meta->GetColumnCount() == 1);

    uint32_t counter = 0;
    const SqlRowRef* row;
    while ((row = res->GetNextRow())) {
        uint64_t id;
        row->Get(0, (int64_t*)(&id));
        cout << "get [" << meta->GetName(0) << "] -> " << id << endl;
        ++counter;
    }
    assert(counter == 5);

    sqlstr = "drop table abc_007";
    assert(client.Execute(sqlstr.data(), sqlstr.size()));
    return 0;
}
