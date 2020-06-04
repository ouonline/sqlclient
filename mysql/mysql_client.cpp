#include "mysql_client.h"
using namespace std;

namespace outils {

class MysqlRow final : public SqlRow {
public:
    MysqlRow(MYSQL_ROW row, unsigned long* length)
        : m_row(row), m_length(length) {}
    void Get(uint32_t col, int32_t* res) const override {
        *res = atoll(m_row[col]);
    }
    void Get(uint32_t col, uint32_t* res) const override {
        *res = atoll(m_row[col]);
    }
    void Get(uint32_t col, int64_t* res) const override {
        *res = atoll(m_row[col]);
    }
    void Get(uint32_t col, uint64_t* res) const override {
        *res = atoll(m_row[col]);
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
    const char* GetColumnName(uint32_t i) const override {
        return m_field_list[i].name;
    }

private:
    uint32_t m_field_num;
    const MYSQL_FIELD* m_field_list;
};

/* -------------------------------------------------------------------------- */

class MysqlResult final : public SqlResult {
public:
    MysqlResult(MYSQL_RES* result) : m_result(result), m_meta(result) {}
    ~MysqlResult() { mysql_free_result(m_result); }

    const SqlColumnMeta* GetColumnMeta() const override {
        return &m_meta;
    }

    bool ForEachRow(const function<bool (const SqlRow*)>& cb, string*) const override {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(m_result))) {
            unsigned long* length = mysql_fetch_lengths(m_result);
            MysqlRow r(row, length);
            if (!cb(&r)) {
                return true;
            }
        }

        return true;
    }

private:
    MYSQL_RES* m_result;
    MysqlColumnMeta m_meta;
};

/* -------------------------------------------------------------------------- */

MysqlClient::MysqlClient() : m_conn(nullptr) {}

MysqlClient::~MysqlClient() {
    Close();
}

bool MysqlClient::Open(const string& host, uint16_t port,
                       const string& user, const string& password,
                       const string& db, string* errmsg) {
    m_conn = mysql_init(nullptr);
    if (!m_conn) {
        if (errmsg) {
            *errmsg = "mysql init failed.";
        }
        return false;
    }

    if (!mysql_real_connect(m_conn, host.c_str(), user.c_str(),
                            password.c_str(), db.c_str(), port, nullptr, 0)) {
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

bool MysqlClient::Execute(const string& sqlstr, string* errmsg,
                          const function<void (const SqlResult*)>& cb) {
    int err = mysql_real_query(m_conn, sqlstr.data(), sqlstr.size());
    if (err) {
        if (errmsg) {
            *errmsg = mysql_error(m_conn);
        }
        return false;
    }

    MYSQL_RES* result = mysql_store_result(m_conn);
    if (result) {
        MysqlResult res(result);
        if (cb) {
            cb(&res);
        }
    }

    return true;
}

}
