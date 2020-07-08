#include "pgsql_client.h"
#include <string>
using namespace std;

namespace outils {

class PgsqlRow final : public SqlRow {
public:
    PgsqlRow(PGresult* result, int cur_row)
        : m_result(result), m_cur_row(cur_row) {}
    void Get(uint32_t col, int32_t* res) const override {
        *res = atoll(PQgetvalue(m_result, m_cur_row, col));
    }
    void Get(uint32_t col, uint32_t* res) const override {
        *res = atoll(PQgetvalue(m_result, m_cur_row, col));
    }
    void Get(uint32_t col, int64_t* res) const override {
        *res = atoll(PQgetvalue(m_result, m_cur_row, col));
    }
    void Get(uint32_t col, uint64_t* res) const override {
        *res = atoll(PQgetvalue(m_result, m_cur_row, col));
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
};

/* -------------------------------------------------------------------------- */

class PgsqlColumnMeta final : public SqlColumnMeta {
public:
    PgsqlColumnMeta(PGresult* result) : m_result(result) {}
    uint32_t GetColumnCount() const override {
        return PQnfields(m_result);
    }
    const char* GetColumnName(uint32_t i) const override {
        return PQfname(m_result, i);
    }

private:
    PGresult* m_result;
};

/* -------------------------------------------------------------------------- */

class PgsqlResult final : public SqlResult {
public:
    PgsqlResult(PGresult* result) : m_result(result), m_meta(result) {}
    ~PgsqlResult() {
        if (m_result) {
            PQclear(m_result);
        }
    }

    const SqlColumnMeta* GetColumnMeta() const override {
        return &m_meta;
    }

    bool ForEachRow(string*, const function<bool (const SqlRow*)>& cb) const override {
        int nr_record = PQntuples(m_result);
        for (int cur_row = 0; cur_row < nr_record; ++cur_row) {
            PgsqlRow row(m_result, cur_row);
            if (!cb(&row)) {
                return true;
            }
        }
        return true;
    }

private:
    PGresult* m_result;
    PgsqlColumnMeta m_meta;
};

/* -------------------------------------------------------------------------- */

PgsqlClient::PgsqlClient() : m_conn(nullptr) {}

PgsqlClient::~PgsqlClient() {
    Close();
}

bool PgsqlClient::Open(const string& host, uint16_t port,
                       const string& user, const string& password,
                       const string& db, string* errmsg) {
    string conn_info = "host='" + host + "' port='" + std::to_string(port) +
        "' user='" + user + "' password='" + password + "'";
    if (!db.empty()) {
        conn_info += " dbname='" + db + "'";
    }

    m_conn = PQconnectdb(conn_info.c_str());
    if (!m_conn) {
        if (errmsg) {
            *errmsg = "connect to db failed.";
        }
        return false;
    }

    if (PQstatus(m_conn) != CONNECTION_OK) {
        if (errmsg) {
            *errmsg = PQerrorMessage(m_conn);
        }
        Close();
        return false;
    }

    return true;
}

void PgsqlClient::Close() {
    if (m_conn) {
        PQfinish(m_conn);
        m_conn = nullptr;
    }
}

bool PgsqlClient::Execute(const string& sqlstr, string* errmsg,
                          const function<void (const SqlResult*)>& cb) {
    PGresult* result = PQexec(m_conn, sqlstr.c_str());
    PgsqlResult pg_res(result);

    int status = PQresultStatus(result);
    if (status != PGRES_COMMAND_OK && status != PGRES_TUPLES_OK) {
        if (errmsg) {
            *errmsg = PQresultErrorMessage(result);
        }
        return false;
    }

    if (cb) {
        cb(&pg_res);
    }

    return true;
}

}
