#ifndef __POSTGRESQL_PGSQL_CLIENT_H__
#define __POSTGRESQL_PGSQL_CLIENT_H__

#include "../sqlclient.h"
#include <postgresql/libpq-fe.h>

namespace sqlclient {

class PgsqlClient final : public SqlClient {
public:
    PgsqlClient() : m_conn(nullptr) {}
    ~PgsqlClient() {
        Close();
    }

    bool Open(const char* host, uint16_t port, const char* user, const char* password,
              const char* db = nullptr, std::string* errmsg = nullptr);
    void Close();
    std::unique_ptr<SqlResult> Execute(const char* sqlstr, uint32_t len,
                                       std::string* errmsg = nullptr) override;

private:
    PGconn* m_conn;

private:
    PgsqlClient(PgsqlClient&&) = delete;
    PgsqlClient(const PgsqlClient&) = delete;
    PgsqlClient& operator=(PgsqlClient&&) = delete;
    PgsqlClient& operator=(const PgsqlClient&) = delete;
};

}

#endif
