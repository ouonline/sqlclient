#ifndef __SQLITE_SQLITE_CLIENT_H__
#define __SQLITE_SQLITE_CLIENT_H__

#include "../sqlclient.h"
#include "sqlite3/sqlite3.h"

namespace outils {

class SqliteClient final : public SqlClient {
public:
    SqliteClient();
    ~SqliteClient();
    bool Open(const std::string& fname, std::string* errmsg = nullptr);
    void Close();
    bool Execute(const std::string& sqlstr, std::string* errmsg = nullptr,
                 const std::function<void (const SqlResult*)>& cb = nullptr) override;

private:
    sqlite3* m_db;

public:
    SqliteClient(SqliteClient&&) = default;
    SqliteClient& operator=(SqliteClient&&) = default;

private:
    SqliteClient(const SqliteClient&) = delete;
    SqliteClient& operator=(const SqliteClient&) = delete;
};

}

#endif
