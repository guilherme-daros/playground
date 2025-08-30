#include <database/tables/Tag.hpp>
#include <string>

namespace sb::database {

const std::string TagDAO::TAG_COLUMN = "tag";
const std::string TagDAO::TYPE_COLUMN = "tag_type";

TagDAO::TagDAO(Handler& db_handler, std::string table_name)
    : BaseDAO<TagDTO, std::string>(db_handler, std::move(table_name)) {
  CreateTable();
}

void TagDAO::CreateTable() {
  std::lock_guard<std::mutex> lock(db_handler_.get_mutex());
  std::string sql = "CREATE TABLE IF NOT EXISTS " + table_name_ + " (" + TAG_COLUMN + " TEXT PRIMARY KEY, " +
                    TYPE_COLUMN + " INTEGER);";
  char* err_msg = nullptr;
  int rc = sqlite3_exec(db_handler_.get(), sql.c_str(), 0, 0, &err_msg);
  if (rc != SQLITE_OK) {
    Database::Error() << "Failed to create table: " << err_msg << std::endl;
    sqlite3_free(err_msg);
  }
}

auto TagDAO::Create(const TagDTO& obj) -> bool {
  std::string sql = "INSERT INTO " + table_name_ + " (" + TAG_COLUMN + ", " + TYPE_COLUMN + ") VALUES (?, ?);";
  return execute_statement(sql, [&](sqlite3_stmt* stmt) {
    sqlite3_bind_text(stmt, 1, obj.tag().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, obj.type());
  });
}

auto TagDAO::Read(const std::string& tag) -> std::optional<TagDTO> {
  std::string sql =
      "SELECT " + TAG_COLUMN + ", " + TYPE_COLUMN + " FROM " + table_name_ + " WHERE " + TAG_COLUMN + " = ?;";
  return query_single(
      sql, [&](sqlite3_stmt* stmt) { sqlite3_bind_text(stmt, 1, tag.c_str(), -1, SQLITE_STATIC); },
      [&](sqlite3_stmt* stmt) {
        std::string read_tag = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        int read_type = sqlite3_column_int(stmt, 1);
        return TagDTO(read_tag, read_type);
      });
}

auto TagDAO::ReadAll() -> std::list<TagDTO> {
  std::string sql = "SELECT " + TAG_COLUMN + ", " + TYPE_COLUMN + " FROM " + table_name_ + ";";
  return query_all(sql, [&](sqlite3_stmt* stmt) {
    std::string read_tag = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    int read_type = sqlite3_column_int(stmt, 1);
    return TagDTO(read_tag, read_type);
  });
}

auto TagDAO::Update(const TagDTO& obj) -> bool {
  std::string sql = "UPDATE " + table_name_ + " SET " + TYPE_COLUMN + " = ? WHERE " + TAG_COLUMN + " = ?;";
  return execute_statement(sql, [&](sqlite3_stmt* stmt) {
    sqlite3_bind_int(stmt, 1, obj.type());
    sqlite3_bind_text(stmt, 2, obj.tag().c_str(), -1, SQLITE_STATIC);
  });
}

auto TagDAO::Delete(const std::string& tag) -> bool {
  std::string sql = "DELETE FROM " + table_name_ + " WHERE " + TAG_COLUMN + " = ?;";
  return execute_statement(sql,
                           [&](sqlite3_stmt* stmt) { sqlite3_bind_text(stmt, 1, tag.c_str(), -1, SQLITE_STATIC); });
}

}  // namespace sb::database
