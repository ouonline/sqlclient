#ifndef __SQLCLIENT_SQLCLIENT_H__
#define __SQLCLIENT_SQLCLIENT_H__

#include <stdint.h>
#include <memory>
#include <string>

namespace sqlclient {

class SqlRowRef {
public:
    virtual ~SqlRowRef() {}
    virtual void Get(uint32_t col, int32_t* res) const = 0;
    virtual void Get(uint32_t col, int64_t* res) const = 0;
    virtual void Get(uint32_t col, float* res) const = 0;
    virtual void Get(uint32_t col, double* res) const = 0;
    virtual void Get(uint32_t col, const char** base, uint32_t* len) const = 0;

    void Get(uint32_t col, std::string* res) const {
        const char* base = nullptr;
        uint32_t len = 0;
        Get(col, &base, &len);
        res->assign(base, len);
    }
};

class SqlColumnInfo {
public:
    virtual ~SqlColumnInfo() {}
    virtual uint32_t GetColumnCount() const = 0;
    virtual const char* GetName(uint32_t idx) const = 0;
};

class SqlResult {
public:
    virtual ~SqlResult() {}
    virtual const SqlColumnInfo* GetColumnInfo() const = 0;
    virtual const SqlRowRef* GetNextRow() const = 0;
};

class SqlClient {
public:
    virtual ~SqlClient() {}
    virtual std::unique_ptr<SqlResult> Execute(const char* sqlstr, uint32_t len,
                                               std::string* errmsg = nullptr) = 0;
};

}

#endif
