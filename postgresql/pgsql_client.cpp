#include "pgsql_client.h"
#include <string>
using namespace std;

namespace sqlclient {

class PgsqlRow final : public SqlRowRef {
public:
    PgsqlRow() {}
    void Reset(PGresult* result, int cur_row) {
        m_result = result;
        m_cur_row = cur_row;
    }
    void Get(uint32_t col, int32_t* res) const override {
        *res = atoll(PQgetvalue(m_result, m_cur_row, col));
    }
    void Get(uint32_t col, int64_t* res) const override {
        *res = atoll(PQgetvalue(m_result, m_cur_row, col));
    }
    void Get(uint32_t col, float* res) const override {
        *res = strtof(PQgetvalue(m_result, m_cur_row, col), nullptr);
    }
    void Get(uint32_t col, double* res) const override {
        *res = strtod(PQgetvalue(m_result, m_cur_row, col), nullptr);
    }
    void Get(uint32_t col, const char** base, uint32_t* len) const override {
        *base = PQgetvalue(m_result, m_cur_row, col);
        *len = PQgetlength(m_result, m_cur_row, col);
    }

private:
    PGresult* m_result;
    int m_cur_row;

private:
    PgsqlRow(PgsqlRow&&) = delete;
    PgsqlRow(const PgsqlRow&) = delete;
    PgsqlRow& operator=(PgsqlRow&&) = delete;
    PgsqlRow& operator=(const PgsqlRow&) = delete;
};

/* -------------------------------------------------------------------------- */

class PgsqlColumnMeta final : public SqlColumnInfo {
public:
    PgsqlColumnMeta(PGresult* result) : m_result(result) {}
    uint32_t GetColumnCount() const override {
        return PQnfields(m_result);
    }
    const char* GetName(uint32_t idx) const override {
        return PQfname(m_result, idx);
    }

private:
    PGresult* m_result;

private:
    PgsqlColumnMeta(PgsqlColumnMeta&&) = delete;
    PgsqlColumnMeta(const PgsqlColumnMeta&) = delete;
    PgsqlColumnMeta& operator=(PgsqlColumnMeta&&) = delete;
    PgsqlColumnMeta& operator=(const PgsqlColumnMeta&) = delete;
};

/* -------------------------------------------------------------------------- */

class PgsqlResult final : public SqlResult {
public:
    PgsqlResult(PGresult* result) : m_counter(0), m_result(result), m_meta(result) {}

    ~PgsqlResult() {
        if (m_result) {
            PQclear(m_result);
        }
    }

    const SqlColumnInfo* GetColumnInfo() const override {
        return &m_meta;
    }

    SqlRowRef* GetNextRow() const override {
        if (m_counter == PQntuples(m_result)) {
            return nullptr;
        }

        m_row.Reset(m_result, m_counter);
        ++m_counter;
        return &m_row;
    }

private:
    mutable int m_counter;
    PGresult* m_result;
    PgsqlColumnMeta m_meta;
    mutable PgsqlRow m_row;

private:
    PgsqlResult(PgsqlResult&&) = delete;
    PgsqlResult(const PgsqlResult&) = delete;
    PgsqlResult& operator=(PgsqlResult&&) = delete;
    PgsqlResult& operator=(const PgsqlResult&) = delete;
};

/* -------------------------------------------------------------------------- */

bool PgsqlClient::Open(const char* host, uint16_t port, const char* user, const char* password,
                       const char* db, string* errmsg) {
    string conn_info = string("host='") + host + "' port='" + std::to_string(port) +
        "' user='" + user + "' password='" + password + "'";
    if (db) {
        conn_info += string(" dbname='") + db + "'";
    }

    m_conn = PQconnectdb(conn_info.c_str());
    if (!m_conn) {
        if (errmsg) {
            *errmsg = PQerrorMessage(m_conn);
        }
        return false;
    }

    return (PQstatus(m_conn) == CONNECTION_OK);
}

void PgsqlClient::Close() {
    if (m_conn) {
        PQfinish(m_conn);
        m_conn = nullptr;
    }
}

unique_ptr<SqlResult> PgsqlClient::Execute(const char* sqlstr, uint32_t, string* errmsg) {
    PGresult* result = PQexec(m_conn, sqlstr);
    unique_ptr<SqlResult> res = make_unique<PgsqlResult>(result);

    int status = PQresultStatus(result);
    if (status != PGRES_COMMAND_OK && status != PGRES_TUPLES_OK) {
        // TODO signal error
        if (errmsg) {
            *errmsg = PQresultErrorMessage(result);
        }
        return unique_ptr<SqlResult>();
    }

    return res;
}

}
