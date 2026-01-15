#pragma once

#include <string>

#include "database/BaseDAO.hpp"  // New base class
#include "database/Handler.hpp"

namespace sb::database {

class TagDTO {
 public:
  TagDTO() = default;
  explicit TagDTO(std::string tag, int type) : tag_{std::move(tag)}, type_{type} {}
  ~TagDTO() = default;

  // Getters
  auto tag() const -> const std::string& { return tag_; }
  auto type() const -> int { return type_; }

  // Setters
  auto set_tag(std::string tag) -> void { tag_ = std::move(tag); }
  auto set_type(int type) -> void { type_ = type; }

 private:
  // Columns
  std::string tag_;
  int type_;
};

class TagDAO : public BaseDAO<TagDTO, std::string> {
 public:
  explicit TagDAO(Handler& db_handler, std::string table_name);
  ~TagDAO() = default;

  // Interface Methods (implementing virtual methods from BaseDAO)
  auto CreateTable() -> Result<void> override {
    std::string sql = "CREATE TABLE IF NOT EXISTS " + table_name_ + " (" + TAG_COLUMN + " TEXT PRIMARY KEY, " +
                      TYPE_COLUMN + " INTEGER);";
    return execute_statement(sql, [](sqlite3_stmt*) {});
  }

  auto Create(const TagDTO& obj) -> Result<void> override {
    std::string sql = "INSERT INTO " + table_name_ + " (" + TAG_COLUMN + ", " + TYPE_COLUMN + ") VALUES (?, ?);";
    return execute_statement(sql, [&](sqlite3_stmt* stmt) {
      sqlite3_bind_text(stmt, 1, obj.tag().c_str(), -1, SQLITE_STATIC);
      sqlite3_bind_int(stmt, 2, obj.type());
    });
  }

  auto Read(const std::string& tag) -> Result<std::optional<TagDTO>> override {
    std::string sql =
        "SELECT " + TAG_COLUMN + ", " + TYPE_COLUMN + " FROM " + table_name_ + " WHERE " + TAG_COLUMN + " = ?;";
    return query_single(
        sql, [&](sqlite3_stmt* stmt) { sqlite3_bind_text(stmt, 1, tag.c_str(), -1, SQLITE_STATIC); },
        [](sqlite3_stmt* stmt) -> TagDTO {
          std::string tag_str = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
          int type = sqlite3_column_int(stmt, 1);
          return TagDTO{tag_str, type};
        });
  }

  auto ReadAll() -> Result<std::list<TagDTO>> override {
    std::string sql = "SELECT " + TAG_COLUMN + ", " + TYPE_COLUMN + " FROM " + table_name_ + ";";
    return query_all(sql, [](sqlite3_stmt* stmt) -> TagDTO {
      std::string tag_str = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
      int type = sqlite3_column_int(stmt, 1);
      return TagDTO{tag_str, type};
    });
  }

  auto Update(const TagDTO& obj) -> Result<void> override {
    std::string sql = "UPDATE " + table_name_ + " SET " + TYPE_COLUMN + " = ? WHERE " + TAG_COLUMN + " = ?;";
    return execute_statement(sql, [&](sqlite3_stmt* stmt) {
      sqlite3_bind_int(stmt, 1, obj.type());
      sqlite3_bind_text(stmt, 2, obj.tag().c_str(), -1, SQLITE_STATIC);
    });
  }

  auto Delete(const std::string& tag) -> Result<void> override {
    std::string sql = "DELETE FROM " + table_name_ + " WHERE " + TAG_COLUMN + " = ?;";
    return execute_statement(sql,
                             [&](sqlite3_stmt* stmt) { sqlite3_bind_text(stmt, 1, tag.c_str(), -1, SQLITE_STATIC); });
  }

 private:
  // Column names could be static const or passed in constructor
  static const std::string TAG_COLUMN;
  static const std::string TYPE_COLUMN;
};

}  // namespace sb::database
