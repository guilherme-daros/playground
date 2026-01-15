#include <cstdio>
#include <iostream>

#include "database/Handler.hpp"
#include "database/tables/Tag.hpp"

#include "logger/Logger.hpp"

using Main = sb::logger::Logger<"Main">;

void print_all(sb::database::TagDAO& dao) {
  Main::Info() << "Reading all tags..." << std::endl;
  auto res = dao.ReadAll();
  if (!res) {
    Main::Error() << "Failed to read all tags: " << res.error() << std::endl;
    return;
  }
  auto all_tags = res.value();

  if (all_tags.empty()) {
    Main::Info() << "(No tags in database)" << std::endl;
  } else {
    for (const auto& t : all_tags) {
      Main::Info() << "  Tag: " << t.tag() << ", Type: " << t.type() << std::endl;
    }
  }
}

int main(int argc, char* argv[]) {
  Main::Info() << "Database App Example" << std::endl;

  auto h_res = sb::database::Handler::create("Test.db");
  if (!h_res) {
    Main::Error() << "Failed to open database: " << h_res.error() << std::endl;
    return 1;
  }
  auto h = std::move(h_res.value());
  sb::database::TagDAO dao(h, "MyTags");

  Main::Info() << "Deleting all previous tags..." << std::endl;
  dao.DeleteAll();
  print_all(dao);

  Main::Info() << "Creating new tags..." << std::endl;
  dao.Create(sb::database::TagDTO{"C++", 1});
  dao.Create(sb::database::TagDTO{"Python", 2});
  dao.Create(sb::database::TagDTO{"Rust", 3});
  print_all(dao);

  Main::Info() << "Reading 'Python' tag..." << std::endl;
  auto python_tag = dao.Read("Python");
  if (python_tag) {
    Main::Info() << "  Found Tag: " << python_tag->tag() << ", Type: " << python_tag->type() << std::endl;
  } else {
    Main::Error() << "Could not find 'Python' tag" << std::endl;
  }

  Main::Info() << "Updating 'Python' tag type to 20..." << std::endl;
  dao.Update(sb::database::TagDTO{"Python", 20});
  python_tag = dao.Read("Python");
  if (python_tag) {
    Main::Info() << "  Updated Tag: " << python_tag->tag() << ", Type: " << python_tag->type() << std::endl;
  } else {
    Main::Error() << "Could not find 'Python' tag after update" << std::endl;
  }

  Main::Info() << "Deleting 'Rust' tag..." << std::endl;
  dao.Delete("Rust");
  print_all(dao);

  Main::Info() << "Deleting all remaining tags..." << std::endl;
  dao.DeleteAll();
  print_all(dao);

  (void)argc;
  (void)argv;
  return 0;
}
