#pragma once

#include <string>

#include "database/Handler.hpp"
#include "database/Interface.hpp"

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

class TagDAO : public Interface<TagDTO> {
 public:
  explicit TagDAO(Handler& db_handler, std::string table_name);
  ~TagDAO() = default;

  // Interface Methods
  auto Create(const TagDTO& obj) -> bool override;
  auto Read(const std::string& tag) -> std::optional<TagDTO> override;
  auto ReadAll() -> std::list<TagDTO> override;
  auto Update(const TagDTO& obj) -> bool override;
  auto Delete(const std::string& tag) -> bool override;
  auto DeleteAll() -> bool override;

 private:
  void CreateTable();
  Handler& db_handler_;
  std::string table_name_;

  std::string first_column_ = "tag";
  std::string second_column_ = "tag_type";
};

}  // namespace sb::database
