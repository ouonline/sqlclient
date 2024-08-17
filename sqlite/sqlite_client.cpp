#include "sqlite_client.h"
using namespace std;

namespace sqlclient {

class SqliteRow final : public SqlRowRef {
public:
    SqliteRow(sqlite3_stmt* stmt) : m_stmt(stmt) {}
    void Get(uint32_t col, int32_t* res) const override {
        *res = sqlite3_column_int(m_stmt, col);
    }
    void Get(uint32_t col, int64_t* res) const override {
        *res = sqlite3_column_int64(m_stmt, col);
    }
    void Get(uint32_t col, float* res) const override {
        *res = sqlite3_column_double(m_stmt, col);
    }
    void Get(uint32_t col, double* res) const override {
        *res = sqlite3_column_double(m_stmt, col);
    }
    void Get(uint32_t col, const char** base, uint32_t* len) const override {
        *base = (const char*)sqlite3_column_text(m_stmt, col);
        *len = sqlite3_column_bytes(m_stmt, col);
    }

private:
    sqlite3_stmt* m_stmt;

private:
    SqliteRow(const SqliteRow&) = delete;
    SqliteRow(SqliteRow&&) = delete;
    SqliteRow& operator=(const SqliteRow&) = delete;
    SqliteRow& operator=(SqliteRow&&) = delete;
};

/* -------------------------------------------------------------------------- */

class SqliteColumnMeta final : public SqlColumnMeta {
public:
    SqliteColumnMeta(sqlite3_stmt* stmt) : m_stmt(stmt) {}
    uint32_t GetColumnCount() const override {
        return sqlite3_column_count(m_stmt);
    }
    const char* GetColumnName(uint32_t idx) const override {
        return sqlite3_column_name(m_stmt, idx);
    }

private:
    sqlite3_stmt* m_stmt;

private:
    SqliteColumnMeta(const SqliteColumnMeta&) = delete;
    SqliteColumnMeta(SqliteColumnMeta&&) = delete;
    SqliteColumnMeta& operator=(const SqliteColumnMeta&) = delete;
    SqliteColumnMeta& operator=(SqliteColumnMeta&&) = delete;
};

/* -------------------------------------------------------------------------- */

class SqliteResult final : public SqlResult {
public:
    SqliteResult(sqlite3_stmt* stmt)
        : m_is_first_row(true), m_stmt(stmt), m_row(stmt), m_meta(stmt) {}

    ~SqliteResult() {
        if (m_stmt) {
            sqlite3_finalize(m_stmt);
        }
    }

    const SqlColumnMeta* GetColumnMeta() const override {
        return &m_meta;
    }

    const SqlRowRef* GetNextRow() const override {
        if (m_is_first_row) {
            m_is_first_row = false;
            return &m_row;
        }

        int rc = sqlite3_step(m_stmt);
        if (rc == SQLITE_ROW) {
            return &m_row;
        }

        // TODO signal error if rc != SQLITE_DONE
        return nullptr;
    }

private:
    mutable bool m_is_first_row;
    sqlite3_stmt* m_stmt;
    const SqliteRow m_row;
    const SqliteColumnMeta m_meta;

private:
    SqliteResult(const SqliteResult&) = delete;
    SqliteResult& operator=(const SqliteResult&) = delete;
};

/* -------------------------------------------------------------------------- */

bool SqliteClient::Open(const char* fname, string* errmsg) {
    if (sqlite3_open(fname, &m_db) != SQLITE_OK) {
        if (errmsg) {
            *errmsg = sqlite3_errmsg(m_db);
            return false;
        }
    }

    return true;
}

void SqliteClient::Close() {
    if (m_db) {
        sqlite3_close(m_db);
        m_db = nullptr;
    }
}

unique_ptr<SqlResult> SqliteClient::Execute(const char* sqlstr, uint32_t len, string* errmsg) {
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(m_db, sqlstr, len, &stmt, nullptr);

    unique_ptr<SqlResult> res = make_unique<SqliteResult>(stmt); // res will handle `stmt`
    if (rc != SQLITE_OK) {
        if (errmsg) {
            *errmsg = sqlite3_errmsg(m_db);
        }
        return unique_ptr<SqlResult>();
    }

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        if (errmsg) {
            errmsg->clear();
        }
        return unique_ptr<SqlResult>();
    }

    if (rc != SQLITE_ROW) {
        if (errmsg) {
            *errmsg = sqlite3_errmsg(m_db);
        }
        return unique_ptr<SqlResult>();
    }

    return res;
}

}
