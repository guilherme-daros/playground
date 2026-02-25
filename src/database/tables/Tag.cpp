#include <database/tables/Tag.hpp>
#include <string>

namespace sb::database {

const std::string TagDAO::TAG_COLUMN = "tag";
const std::string TagDAO::TYPE_COLUMN = "tag_type";

TagDAO::TagDAO(Handler& db_handler, std::string table_name)
    : BaseDAO<TagDTO, std::string>(db_handler, std::move(table_name)) {
  (void)CreateTable();
}

}  // namespace sb::database
