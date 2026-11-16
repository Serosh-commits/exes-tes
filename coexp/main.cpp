#include "iostream"
#include "filesystem"
#include "string"
#include<fstream>
namespace fs = std::filesystem;
static inline void process_c(std::ifstream *file){

  return exit(EXIT_SUCCESS);
}

static inline void process_cpp(std::ifstream *file){
  return exit(EXIT_SUCCESS);
}
int main (int argc, char *argv[]) {
  
  if (argc != 2) {
    std::cerr<<"Usage : coexp <include source file\n>";
    return EXIT_FAILURE;
  }

  std::string source = argv[1];
  if (!fs::exists(source)) {
    std::cerr<<"file doesn't exists\n";
    return EXIT_FAILURE;
  }

  std::string ext= fs::path(source).extension();

  if (ext == ".c") {
    std::cout<<"c type";
    std::string abspath = fs::absolute(source);
    std::ifstream file(argv[1]);
    if (!file) {
      std::cerr<<"failed to open the file\t : "<<source;
      return EXIT_FAILURE;
    }
    process_c(&file);
  } else if (ext == ".cpp") {
    std::cout<<"c++ type\n";
    std::string abspath = fs::absolute(source);
    std::cout<<"absolute path :\n"<<abspath;
    std::ifstream file(argv[1]);
    if (!file) {
      std::cerr<<"failed to open the file\t :"<<source;
      return EXIT_FAILURE;
    }
    process_cpp(&file);
  }
  return EXIT_SUCCESS;
}
