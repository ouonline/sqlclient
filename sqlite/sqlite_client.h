#ifndef __SQLITE_SQLITE_CLIENT_H__
#define __SQLITE_SQLITE_CLIENT_H__

#include "../sqlclient.h"
#include "sqlite3.h"

namespace sqlclient {

class SqliteClient final : public SqlClient {
public:
    SqliteClient() : m_db(nullptr) {}
    ~SqliteClient() {
        Close();
    }

    bool Open(const char* fname, std::string* errmsg = nullptr);
    void Close();
    std::unique_ptr<SqlResult> Execute(const char* sqlstr, uint32_t len,
                                       std::string* errmsg = nullptr) override;

private:
    sqlite3* m_db;

private:
    SqliteClient(SqliteClient&&) = delete;
    SqliteClient(const SqliteClient&) = delete;
    SqliteClient& operator=(SqliteClient&&) = delete;
    SqliteClient& operator=(const SqliteClient&) = delete;
};

}

#endif
