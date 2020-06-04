#ifndef __MYSQL_MYSQL_CLIENT_H__
#define __MYSQL_MYSQL_CLIENT_H__

#include "../sqlclient.h"
#include <mysql/mysql.h>

namespace outils {

class MysqlClient final : public SqlClient {
public:
    MysqlClient();
    ~MysqlClient();
    bool Open(const std::string& host, uint16_t port,
              const std::string& user, const std::string& password,
              const std::string& db = "", std::string* errmsg = nullptr);
    void Close();
    bool Execute(const std::string& sqlstr, std::string* errmsg = nullptr,
                 const std::function<void (const SqlResult*)>& cb = nullptr) override;

private:
    MYSQL* m_conn;

public:
    MysqlClient(MysqlClient&&) = default;
    MysqlClient& operator=(MysqlClient&&) = default;

private:
    MysqlClient(const MysqlClient&) = delete;
    MysqlClient& operator=(const MysqlClient&) = delete;
};

}

#endif
