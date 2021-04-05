#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

// current path and space
void example1() {
    fs::path cur_p = fs::current_path();
    std::cout << "current path - " << cur_p << std::endl << std::endl;

    fs::path root_p = fs::path("/");
    fs::space_info space = fs::space(root_p);
    /* capacity  - total size of the filesystem, in bytes
       free      - free space on the filesystem, in bytes
       available - free space available to a non-privileged process
                   (may be equal or less than free) */
    std::cout << "Total     : " << space.capacity/1024/1024/1024 << "GB" << std::endl;
    std::cout << "Free      : " << space.free/1024/1024/1024 << "GB" << std::endl;
    std::cout << "Available : " << space.available/1024/1024/1024 << "GB" << std::endl << std::endl;
}


// parse and combo pathes
void example2() {
    fs::path filePath {"./path/to/file.txt"};

    std::cout << "Path to file: " << filePath << std::endl;
    std::cout << "Parent path : " << filePath.parent_path() << std::endl;
    std::cout << "Filename    : " << filePath.filename() << std::endl;
    std::cout << "Extension   : " << filePath.extension() << std::endl << std::endl;

    std::cout << "Separator in my OS "  << fs::path::preferred_separator << std::endl << std::endl;

    fs::path root{"/"};
    fs::path  dir{"dev/db"};
    fs::path   db{"database.db"};

    fs::path pathToDB = root / dir / db;
    std::cout << pathToDB << std::endl;
    std::cout << std::endl;
}


// create dirs
void example3() {
    fs::path cur_p = fs::current_path();
    fs::create_directory(cur_p / "tmp");
    fs::create_directories(cur_p / "tmp/" / "a/");
    fs::create_directories(cur_p / "tmp/b");
    fs::create_directories(cur_p / "tmp/b" / "bb" / "bbb");
}


// remove dirs
void example4() {
    fs::path cur_p = fs::current_path();
    fs::remove(cur_p / "tmp/b" / "bb" / "bbb");
    fs::remove_all(cur_p / "tmp");
}



std::vector<std::string> getDirectoryFiles(const fs::path& dir) {
    std::vector<std::string> files;
    // id dirs and subdirs, implicit creating of iterator
    for(auto& p: fs::recursive_directory_iterator(dir)) {
      if(fs::is_regular_file(p)) {
          files.push_back(p.path().string());
      }
    }
    return files;
}

std::vector<std::string> getDirectoryDirectories(const fs::path& dir) {
    std::vector<std::string> directories;
    // just in dir, not in subdirs, explicit creating of iterator
    fs::directory_iterator it = fs::begin(fs::directory_iterator(dir));
    fs::directory_iterator it_end = fs::end(fs::directory_iterator(dir));
    for(;it != it_end; ++it) {
      if(fs::is_directory(*it)) {
          directories.push_back(it->path().string());
      }
    }
    return directories;
}

// show dirs, files
void example5() {
    fs::path res_dir = fs::current_path();

    res_dir = res_dir / "FileSystemTestDir" / "Files";
    fs::create_directories(res_dir);

    fs::create_directory(res_dir / "TestDir");

    std::vector<std::string> res_files =
    {
      res_dir.string() + "/test1.txt",
      res_dir.string() + "/test2.txt",
      res_dir.string() + "/test3.txt"
    };

    for (auto &f: res_files)
      std::ofstream(f) << "data";

    auto print_data = [](const std::vector<std::string>& data)
    {
      for (auto &f: data)
        std::cout << f << std::endl;
      std::cout << std::endl;
    };

    auto files = getDirectoryFiles(res_dir);
    print_data(files);

    auto dirs = getDirectoryDirectories(res_dir);
    print_data(dirs);
}


// working with permissions
void example6() {
    auto permission = [](fs::perms aPerm)
    {
      std::cout << ((aPerm & fs::perms::owner_read) != fs::perms::none ? "r" : "-")
           << ((aPerm & fs::perms::owner_write) != fs::perms::none ? "w" : "-")
           << ((aPerm & fs::perms::owner_exec) != fs::perms::none ? "x" : "-")
           << ((aPerm & fs::perms::group_read) != fs::perms::none ? "r" : "-")
           << ((aPerm & fs::perms::group_write) != fs::perms::none ? "w" : "-")
           << ((aPerm & fs::perms::group_exec) != fs::perms::none ? "x" : "-")
           << ((aPerm & fs::perms::others_read) != fs::perms::none ? "r" : "-")
           << ((aPerm & fs::perms::others_write) != fs::perms::none ? "w" : "-")
           << ((aPerm & fs::perms::others_exec) != fs::perms::none ? "x" : "-")
           << std::endl;
    };

    std::ofstream("file_test");

    std::cout << "Initial file permissions for a file: ";
    permission(fs::status("file_test").permissions());

    std::cout << "Adding all bits to owner and group:  ";
    fs::permissions("file_test", fs::perms::owner_all | fs::perms::group_all, fs::perm_options::add);
    permission(fs::status("file_test").permissions());

    std::cout << "Removing the write bits for all:     ";
    fs::permissions("file_test", fs::perms::owner_write | fs::perms::group_write | fs::perms::others_write, fs::perm_options::remove);
    permission(fs::status("file_test").permissions());

    fs::remove("file_test");
}

int main() {
    // uncomment to test smth
    // example1();
    // example2();
    // example3();
    // example4();
    // example5();
    // example6();
    return 0;
}
