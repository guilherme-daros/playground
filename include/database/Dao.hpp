#pragma once

#include <string>

#include "database/Interface.hpp"

namespace sb::database {

class TagDaoObj {
 public:
  explicit TagDaoObj(const std::string& table_name, const std::string& tag, int type)
      : table_name_{table_name}, tag_{tag}, type_{type} {}

  explicit TagDaoObj(const std::string& table_name) : table_name_{table_name} {}

  ~TagDaoObj() = default;

  // Getters
  auto table_name() -> const std::string& { return table_name_; }
  auto tag() -> const std::string& { return tag_; }
  auto type() -> const int { return type_; }

  // Setters
  auto set_table_name(const std::string& name) -> void { table_name_ = name; }
  auto set_tag(const std::string& tag) -> void { tag_ = tag; }
  auto set_type(const int& type) -> void { type_ = type; }

 private:
  std::string table_name_;

  // Columns
  std::string tag_;
  int type_;
};

class TagDaoImpl : public Interface<TagDaoObj> {
 public:
  TagDaoImpl() = default;
  ~TagDaoImpl() = default;

  // Interface Methods
  auto Create(TagDaoObj& obj) -> bool override;
  auto Read(TagDaoObj& obj) -> std::optional<TagDaoObj> override;
  auto ReadAll(TagDaoObj& obj) -> std::list<TagDaoObj> override;
  auto Update(TagDaoObj& obj) -> bool override;
  auto Delete(TagDaoObj& obj) -> void override;
  auto DeleteAll(TagDaoObj& obj) -> void override;

  // Auxiliary Methods
  auto ReadByType(TagDaoObj& obj) -> std::optional<TagDaoObj>;
  auto ReadByTag(TagDaoObj& obj) -> std::optional<TagDaoObj>;
  auto UpdateOrCreateIfNotExist(TagDaoObj& obj) -> bool;
  auto Size(const std::string& table_name) -> std::optional<uint32_t>;

 private:
  auto IsTheTagColumn(const std::string& str) -> bool;
  auto IsTheTypeColumn(const std::string& str) -> bool;

  auto TableDataToObjectList(const std::string& table_name,
                             std::list<std::pair<std::string, std::string>>& sql_callback_result)
      -> std::list<TagDaoObj>;

  std::string first_column_ = "tag";
  std::string second_column_ = "tag_type";
};

}  // namespace sb::database
