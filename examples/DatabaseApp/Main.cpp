#include <cstdio>
#include <iostream>

#include "database/Handler.hpp"
#include "database/tables/Tag.hpp"

#include "logger/Logger.hpp"

using Main = sb::logger::Logger<"Main">;

void print_all(sb::database::TagDAO& dao) {
  Main::Info() << "Reading all tags...";
  auto res = dao.ReadAll();
  if (!res) {
    Main::Error() << "Failed to read all tags: " << res.error();
    return;
  }
  auto all_tags = res.value();

  if (all_tags.empty()) {
    Main::Info() << "(No tags in database)";
  } else {
    for (const auto& t : all_tags) {
      Main::Info() << "  Tag: " << t.tag() << ", Type: " << t.type();
    }
  }
}

int main(int argc, char* argv[]) {
  Main::Info() << "Database App Example";

  auto h_res = sb::database::Handler::create("Test.db");
  if (!h_res) {
    Main::Error() << "Failed to open database: " << h_res.error();
    return 1;
  }
  auto h = std::move(h_res.value());
  sb::database::TagDAO dao(h, "MyTags");

  Main::Info() << "Deleting all previous tags...";
  dao.DeleteAll();
  print_all(dao);

  Main::Info() << "Creating new tags...";
  dao.Create(sb::database::TagDTO{"C++", 1});
  dao.Create(sb::database::TagDTO{"Python", 2});
  dao.Create(sb::database::TagDTO{"Rust", 3});
  print_all(dao);

  Main::Info() << "Reading 'Python' tag...";
  auto python_tag_res = dao.Read("Python");
  if (python_tag_res && python_tag_res.value()) {
    auto& python_tag = python_tag_res.value();
    Main::Info() << "  Found Tag: " << python_tag->tag() << ", Type: " << python_tag->type();
  } else {
    Main::Error() << "Could not find 'Python' tag";
  }

  Main::Info() << "Updating 'Python' tag type to 20...";
  dao.Update(sb::database::TagDTO{"Python", 20});
  python_tag_res = dao.Read("Python");
  if (python_tag_res && python_tag_res.value()) {
    auto& python_tag = python_tag_res.value();
    Main::Info() << "  Updated Tag: " << python_tag->tag() << ", Type: " << python_tag->type();
  } else {
    Main::Error() << "Could not find 'Python' tag after update";
  }

  Main::Info() << "Deleting 'Rust' tag...";
  dao.Delete("Rust");
  print_all(dao);

  Main::Info() << "Deleting all remaining tags...";
  dao.DeleteAll();
  print_all(dao);

  (void)argc;
  (void)argv;
  return 0;
}
