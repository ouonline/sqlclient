#include "sqlite_client.h"
#include <iostream>
using namespace std;

namespace outils {

class SqliteRow final : public SqlRow {
public:
    SqliteRow(sqlite3_stmt* stmt) : m_stmt(stmt) {}
    void Get(uint32_t col, int32_t* res) const override {
        *res = sqlite3_column_int(m_stmt, col);
    }
    void Get(uint32_t col, uint32_t* res) const override {
        *res = sqlite3_column_int(m_stmt, col);
    }
    void Get(uint32_t col, int64_t* res) const override {
        *res = sqlite3_column_int64(m_stmt, col);
    }
    void Get(uint32_t col, uint64_t* res) const override {
        *res = sqlite3_column_int64(m_stmt, col);
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
    SqliteRow(const SqliteRow&);
    SqliteRow& operator=(const SqliteRow&);
};

/* -------------------------------------------------------------------------- */

class SqliteColumnMeta final : public SqlColumnMeta {
public:
    SqliteColumnMeta(sqlite3_stmt* stmt) : m_stmt(stmt) {}
    uint32_t GetColumnCount() const override {
        return sqlite3_column_count(m_stmt);
    }
    const char* GetColumnName(uint32_t i) const override {
        return sqlite3_column_name(m_stmt, i);
    }

private:
    sqlite3_stmt* m_stmt;

private:
    SqliteColumnMeta(const SqliteColumnMeta&);
    SqliteColumnMeta& operator=(const SqliteColumnMeta&);
};

/* -------------------------------------------------------------------------- */

class SqliteResult final : public SqlResult {
public:
    SqliteResult(sqlite3* db, sqlite3_stmt* stmt)
        : m_db(db), m_stmt(stmt), m_meta(stmt) {}

    ~SqliteResult() {
        if (m_stmt) {
            sqlite3_finalize(m_stmt);
        }
    }

    const SqlColumnMeta* GetColumnMeta() const override {
        return &m_meta;
    }

    bool ForEachRow(const function<bool (const SqlRow*)>& cb,
                    string* errmsg = nullptr) const override {
        int rc;
        const SqliteRow row(m_stmt);
        while (true) {
            if (!cb(&row)) {
                return true;
            }

            rc = sqlite3_step(m_stmt);
            if (rc != SQLITE_ROW) {
                break;
            }
        }

        if (rc != SQLITE_DONE) {
            if (errmsg) {
                *errmsg = sqlite3_errmsg(m_db);
            }
            return false;
        }

        return true;
    }

private:
    sqlite3* m_db;
    sqlite3_stmt* m_stmt;
    const SqliteColumnMeta m_meta;

private:
    SqliteResult(const SqliteResult&);
    SqliteResult& operator=(const SqliteResult&);
};

/* -------------------------------------------------------------------------- */

SqliteClient::SqliteClient()
    : m_db(nullptr) {}

SqliteClient::~SqliteClient() {
    Close();
}

bool SqliteClient::Open(const string& fname, string* errmsg) {
    int rc = sqlite3_open(fname.c_str(), &m_db);
    if (rc != SQLITE_OK) {
        if (errmsg) {
            *errmsg = sqlite3_errmsg(m_db);
        }
        sqlite3_close(m_db);
        m_db = nullptr;
        return false;
    }
    return true;
}

void SqliteClient::Close() {
    if (m_db) {
        sqlite3_close(m_db);
        m_db = nullptr;
    }
}

bool SqliteClient::Execute(const string& sqlstr, string* errmsg,
                           const std::function<void (const SqlResult*)>& cb) {
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(m_db, sqlstr.data(), sqlstr.size(),
                                &stmt, nullptr);
    const SqliteResult res(m_db, stmt);

    if (rc != SQLITE_OK) {
        if (errmsg) {
            *errmsg = sqlite3_errmsg(m_db);
        }
        return false;
    }

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        return true;
    }

    if (rc != SQLITE_ROW) {
        if (errmsg) {
            *errmsg = sqlite3_errmsg(m_db);
        }
        return false;
    }

    if (cb) {
        cb(&res);
    }

    return true;
}

}
