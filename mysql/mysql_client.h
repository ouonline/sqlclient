#ifndef __MYSQL_MYSQL_CLIENT_H__
#define __MYSQL_MYSQL_CLIENT_H__

#include "../sqlclient.h"
#include <mysql/mysql.h>

namespace sqlclient {

class MysqlClient final : public SqlClient {
public:
    MysqlClient() : m_conn(nullptr) {}
    ~MysqlClient() {
        Close();
    }

    bool Open(const char* host, uint16_t port, const char* user, const char* password,
              const char* db = nullptr, std::string* errmsg = nullptr);
    void Close();
    std::unique_ptr<SqlResult> Execute(const char* sqlstr, uint32_t len,
                                       std::string* errmsg = nullptr) override;

private:
    MYSQL* m_conn;

private:
    MysqlClient(MysqlClient&&) = delete;
    MysqlClient(const MysqlClient&) = delete;
    MysqlClient& operator=(MysqlClient&&) = delete;
    MysqlClient& operator=(const MysqlClient&) = delete;
};

}

#endif
