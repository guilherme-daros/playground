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
  void CreateTable() override;
  auto Create(const TagDTO& obj) -> bool override;
  auto Read(const std::string& tag) -> std::optional<TagDTO> override;
  auto ReadAll() -> std::list<TagDTO> override;
  auto Update(const TagDTO& obj) -> bool override;
  auto Delete(const std::string& tag) -> bool override;

 private:
  // Column names could be static const or passed in constructor
  static const std::string TAG_COLUMN;
  static const std::string TYPE_COLUMN;
};

}  // namespace sb::database
