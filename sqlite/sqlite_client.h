#include "../sqlclient.h"
#include "sqlite3/sqlite3.h"

namespace outils {

class SqliteClient final : public SqlClient {
public:
    SqliteClient();
    ~SqliteClient();
    bool Open(const std::string& fname, std::string* errmsg = nullptr);
    void Close();
    bool Execute(const std::string& sqlstr, std::string* errmsg = nullptr,
                 const std::function<void (const SqlResult*)>& cb = nullptr) override;

private:
    sqlite3* m_db;

private:
    SqliteClient(const SqliteClient&);
    SqliteClient& operator=(const SqliteClient&);
};

}
