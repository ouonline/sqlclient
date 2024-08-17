#include "mysql_client.h"
using namespace std;

namespace sqlclient {

class MysqlRow final : public SqlRowRef {
public:
    MysqlRow() {}
    void Reset(MYSQL_ROW row, unsigned long* length) {
        m_row = row;
        m_length = length;
    }
    void Get(uint32_t col, int32_t* res) const override {
        *res = atol(m_row[col]);
    }
    void Get(uint32_t col, int64_t* res) const override {
        *res = atoll(m_row[col]);
    }
    void Get(uint32_t col, float* res) const override {
        *res = strtof(m_row[col], nullptr);
    }
    void Get(uint32_t col, double* res) const override {
        *res = strtod(m_row[col], nullptr);
    }
    void Get(uint32_t col, const char** base, uint32_t* len) const override {
        *base = m_row[col];
        *len = m_length[col];
    }

private:
    MYSQL_ROW m_row;
    unsigned long* m_length;

private:
    MysqlRow(MysqlRow&&) = delete;
    MysqlRow(const MysqlRow&) = delete;
    MysqlRow& operator=(MysqlRow&&) = delete;
    MysqlRow& operator=(const MysqlRow&) = delete;
};

/* -------------------------------------------------------------------------- */

class MysqlColumnMeta final : public SqlColumnMeta {
public:
    MysqlColumnMeta(MYSQL_RES* result) {
        m_field_num = mysql_num_fields(result);
        m_field_list = mysql_fetch_fields(result);
    }
    uint32_t GetColumnCount() const override {
        return m_field_num;
    }
    const char* GetColumnName(uint32_t idx) const override {
        return m_field_list[idx].name;
    }

private:
    uint32_t m_field_num;
    const MYSQL_FIELD* m_field_list;

private:
    MysqlColumnMeta(MysqlColumnMeta&&) = delete;
    MysqlColumnMeta(const MysqlColumnMeta&) = delete;
    MysqlColumnMeta& operator=(MysqlColumnMeta&&) = delete;
    MysqlColumnMeta& operator=(const MysqlColumnMeta&) = delete;
};

/* -------------------------------------------------------------------------- */

class MysqlResult final : public SqlResult {
public:
    MysqlResult(MYSQL_RES* result) : m_result(result), m_meta(result) {}
    ~MysqlResult() {
        mysql_free_result(m_result);
    }

    const SqlColumnMeta* GetColumnMeta() const override {
        return &m_meta;
    }

    SqlRowRef* GetNextRow() const override {
        MYSQL_ROW row = mysql_fetch_row(m_result);
        if (row) {
            unsigned long* length = mysql_fetch_lengths(m_result);
            m_row.Reset(row, length);
            return &m_row;
        }
        return nullptr;
    }

private:
    MYSQL_RES* m_result;
    MysqlColumnMeta m_meta;
    mutable MysqlRow m_row;

private:
    MysqlResult(MysqlResult&&) = delete;
    MysqlResult(const MysqlResult&) = delete;
    MysqlResult& operator=(MysqlResult&&) = delete;
    MysqlResult& operator=(const MysqlResult&) = delete;
};

/* -------------------------------------------------------------------------- */

bool MysqlClient::Open(const char* host, uint16_t port, const char* user, const char* password,
                       const char* db, string* errmsg) {
    m_conn = mysql_init(nullptr);
    if (!m_conn) {
        if (errmsg) {
            *errmsg = "mysql init failed.";
        }
        return false;
    }

    if (mysql_real_connect(m_conn, host, user, password, db, port, nullptr, 0) != 0) {
        if (errmsg) {
            *errmsg = mysql_error(m_conn);
        }
        return false;
    }

    return true;
}

void MysqlClient::Close() {
    if (m_conn) {
        mysql_close(m_conn);
        m_conn = nullptr;
    }
}

unique_ptr<SqlResult> MysqlClient::Execute(const char* sqlstr, uint32_t len, string* errmsg) {
    int err = mysql_real_query(m_conn, sqlstr, len);
    if (err) {
        if (errmsg) {
            *errmsg = mysql_error(m_conn);
        }
        return unique_ptr<SqlResult>();
    }

    MYSQL_RES* result = mysql_store_result(m_conn);
    return make_unique<MysqlResult>(result);
}

}
