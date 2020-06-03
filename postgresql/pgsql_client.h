#ifndef __POSTGRESQL_PGSQL_CLIENT_H__
#define __POSTGRESQL_PGSQL_CLIENT_H__

#include "../sqlclient.h"
#include <postgresql/libpq-fe.h>

namespace outils {

class PgsqlClient final : public SqlClient {
public:
    PgsqlClient();
    ~PgsqlClient();
    bool Open(const std::string& host, uint16_t port,
              const std::string& user, const std::string& password,
              const std::string& db = "", std::string* errmsg = nullptr);
    void Close();
    bool Execute(const std::string& sqlstr, std::string* errmsg = nullptr,
                 const std::function<void (const SqlResult*)>& cb = nullptr) override;

private:
    PGconn* m_conn;

public:
    PgsqlClient(PgsqlClient&&) = default;
    PgsqlClient& operator=(PgsqlClient&&) = default;

private:
    PgsqlClient(const PgsqlClient&) = delete;
    PgsqlClient& operator=(const PgsqlClient&) = delete;
};

}

#endif
