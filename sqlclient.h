#ifndef __SQLCLIENT_SQLCLIENT_H__
#define __SQLCLIENT_SQLCLIENT_H__

#include <string>
#include <functional>
#include <stdint.h>

namespace outils {

class SqlRow {
public:
    virtual ~SqlRow() {}
    virtual void Get(uint32_t col, int32_t* res) const = 0;
    virtual void Get(uint32_t col, uint32_t* res) const = 0;
    virtual void Get(uint32_t col, int64_t* res) const = 0;
    virtual void Get(uint32_t col, uint64_t* res) const = 0;
    virtual void Get(uint32_t col, double* res) const = 0;
    virtual void Get(uint32_t col, const char** base, uint32_t* len) const = 0;

    void Get(uint32_t col, std::string* res) const {
        const char* base = nullptr;
        uint32_t len = 0;
        Get(col, &base, &len);
        if (base && len > 0) {
            res->assign(base, len);
        }
    }
};

class SqlColumnMeta {
public:
    virtual ~SqlColumnMeta() {}
    virtual uint32_t GetColumnCount() const = 0;
    virtual const char* GetColumnName(uint32_t i) const = 0;
};

class SqlResult {
public:
    virtual ~SqlResult() {}
    virtual const SqlColumnMeta* GetColumnMeta() const = 0;
    virtual bool ForEachRow(std::string* errmsg,
                            const std::function<bool (const SqlRow*)>& cb) const = 0;
};

class SqlClient {
public:
    virtual ~SqlClient() {}
    virtual bool Execute(const std::string& sqlstr, std::string* errmsg = nullptr,
                         const std::function<void (const SqlResult*)>& cb = nullptr) = 0;
};

}

#endif
