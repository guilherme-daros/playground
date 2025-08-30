#include <database/tables/Tag.hpp>
#include <string>

// TagDaoImpl Implementation
namespace sb::database {
TagDAO::TagDAO(Handler& db_handler, std::string table_name)
    : db_handler_{db_handler}, table_name_{std::move(table_name)} {
  CreateTable();
}

void TagDAO::CreateTable() {
  std::lock_guard<std::mutex> lock(db_handler_.get_mutex());
  std::string sql = "CREATE TABLE IF NOT EXISTS " + table_name_ + " (tag TEXT PRIMARY KEY, tag_type INTEGER);";
  char* err_msg = nullptr;
  int rc = sqlite3_exec(db_handler_.get(), sql.c_str(), 0, 0, &err_msg);
  if (rc != SQLITE_OK) {
    Database::Error() << "Failed to create table: " << err_msg << std::endl;
    sqlite3_free(err_msg);
  }
}

auto TagDAO::Create(const TagDTO& obj) -> bool {
  std::lock_guard<std::mutex> lock(db_handler_.get_mutex());
  std::string sql = "INSERT INTO " + table_name_ + " (tag, tag_type) VALUES (?, ?);";
  sqlite3_stmt* stmt = nullptr;
  int rc = sqlite3_prepare_v2(db_handler_.get(), sql.c_str(), -1, &stmt, 0);
  if (rc != SQLITE_OK) {
    Database::Error() << "Failed to prepare statement: " << sqlite3_errmsg(db_handler_.get()) << std::endl;
    return false;
  }

  sqlite3_bind_text(stmt, 1, obj.tag().c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_int(stmt, 2, obj.type());

  rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);

  if (rc != SQLITE_DONE) {
    Database::Error() << "Failed to execute statement: " << sqlite3_errmsg(db_handler_.get()) << std::endl;
    return false;
  }
  return true;
}

auto TagDAO::Read(const std::string& tag) -> std::optional<TagDTO> {
  std::lock_guard<std::mutex> lock(db_handler_.get_mutex());
  std::string sql = "SELECT tag, tag_type FROM " + table_name_ + " WHERE tag = ?;";
  sqlite3_stmt* stmt = nullptr;
  int rc = sqlite3_prepare_v2(db_handler_.get(), sql.c_str(), -1, &stmt, 0);
  if (rc != SQLITE_OK) {
    Database::Error() << "Failed to prepare statement: " << sqlite3_errmsg(db_handler_.get()) << std::endl;
    return std::nullopt;
  }

  sqlite3_bind_text(stmt, 1, tag.c_str(), -1, SQLITE_STATIC);

  rc = sqlite3_step(stmt);
  if (rc == SQLITE_ROW) {
    std::string read_tag = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    int read_type = sqlite3_column_int(stmt, 1);
    sqlite3_finalize(stmt);
    return TagDTO(read_tag, read_type);
  }

  sqlite3_finalize(stmt);
  return std::nullopt;
}

auto TagDAO::ReadAll() -> std::list<TagDTO> {
  std::lock_guard<std::mutex> lock(db_handler_.get_mutex());
  std::list<TagDTO> results;
  std::string sql = "SELECT tag, tag_type FROM " + table_name_ + ";";
  sqlite3_stmt* stmt = nullptr;
  int rc = sqlite3_prepare_v2(db_handler_.get(), sql.c_str(), -1, &stmt, 0);
  if (rc != SQLITE_OK) {
    Database::Error() << "Failed to prepare statement: " << sqlite3_errmsg(db_handler_.get()) << std::endl;
    return results;
  }

  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    std::string read_tag = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    int read_type = sqlite3_column_int(stmt, 1);
    results.emplace_back(read_tag, read_type);
  }

  sqlite3_finalize(stmt);
  return results;
}

auto TagDAO::Update(const TagDTO& obj) -> bool {
  std::lock_guard<std::mutex> lock(db_handler_.get_mutex());
  std::string sql = "UPDATE " + table_name_ + " SET tag_type = ? WHERE tag = ?;";
  sqlite3_stmt* stmt = nullptr;
  int rc = sqlite3_prepare_v2(db_handler_.get(), sql.c_str(), -1, &stmt, 0);
  if (rc != SQLITE_OK) {
    Database::Error() << "Failed to prepare statement: " << sqlite3_errmsg(db_handler_.get()) << std::endl;
    return false;
  }

  sqlite3_bind_int(stmt, 1, obj.type());
  sqlite3_bind_text(stmt, 2, obj.tag().c_str(), -1, SQLITE_STATIC);

  rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);

  if (rc != SQLITE_DONE) {
    Database::Error() << "Failed to execute statement: " << sqlite3_errmsg(db_handler_.get()) << std::endl;
    return false;
  }
  return sqlite3_changes(db_handler_.get()) > 0;
}

auto TagDAO::Delete(const std::string& tag) -> bool {
  std::lock_guard<std::mutex> lock(db_handler_.get_mutex());
  std::string sql = "DELETE FROM " + table_name_ + " WHERE tag = ?;";
  sqlite3_stmt* stmt = nullptr;
  int rc = sqlite3_prepare_v2(db_handler_.get(), sql.c_str(), -1, &stmt, 0);
  if (rc != SQLITE_OK) {
    Database::Error() << "Failed to prepare statement: " << sqlite3_errmsg(db_handler_.get()) << std::endl;
    return false;
  }

  sqlite3_bind_text(stmt, 1, tag.c_str(), -1, SQLITE_STATIC);

  rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);

  if (rc != SQLITE_DONE) {
    Database::Error() << "Failed to execute statement: " << sqlite3_errmsg(db_handler_.get()) << std::endl;
    return false;
  }
  return sqlite3_changes(db_handler_.get()) > 0;
}

auto TagDAO::DeleteAll() -> bool {
  std::lock_guard<std::mutex> lock(db_handler_.get_mutex());
  std::string sql = "DELETE FROM " + table_name_ + ";";
  char* err_msg = nullptr;
  int rc = sqlite3_exec(db_handler_.get(), sql.c_str(), 0, 0, &err_msg);
  if (rc != SQLITE_OK) {
    Database::Error() << "Failed to execute statement: " << err_msg << std::endl;
    sqlite3_free(err_msg);
    return false;
  }
  return true;
}
}  // namespace sb::database
