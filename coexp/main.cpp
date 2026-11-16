#include "iostream"
#include "filesystem"
#include "string"
#include<fstream>
#include "chrono"
#include "cstdlib"

namespace fs = std::filesystem;

static inline void process_metadata(const fs::path& source){
  std::string filename = source.filename().string();
  fs::path abspath = fs::absolute(source);
  std::string extension = fs::path(source).extension().string();
  fs::path parent_directory = fs::absolute(source).parent_path();

  if (!fs::is_regular_file(source)) {
    std::cerr<<"this is not a regular file\n";
    return;
  }

  std::uintmax_t file_size = fs::file_size(source);
  bool empty = fs::is_empty(source);
  fs::file_time_type last_modification_time = fs::last_write_time(source);
  fs::perms permissions = fs::status(source).permissions();
   
  //display the process_metadata

  std::cout<<"metadata for the file :\n";
  std::cout<<"\t File name: "<<filename<<"\n";
  std::cout<<"\t Absolute path: "<<abspath<<"\n";
  std::cout<<"\t Extension :"<<extension<<"\n";
  std::cout<<"\t File size: "<<file_size<<"\n";
  std::cout<<"\t Empty status : "<<std::boolalpha <<empty<<"\n";
  std::cout<<"\t Regular file :true\n";
  std::cout<<"\t Parent directory: "<<parent_directory<<"\n";
  std::cout << "\t Last modification time: "
          << last_modification_time.time_since_epoch().count() << "\n";
  std::cout<<"\t permissions: "<<static_cast<unsigned>(permissions)<<"\n";
}
int main (int argc, char *argv[]) {
  
  if (argc != 2) {
    std::cerr<<"Usage : coexp <include source file\n>";
    return EXIT_FAILURE;
  }

  std::string source = argv[1];
  
  try {
    if (!fs::exists(source)) {
      std::cerr<<"file doesn't exists\n";
      return EXIT_FAILURE;
    }

    if (!fs::is_regular_file(source)) {
      std::cerr<<"input is not a regulare file\n";
      return EXIT_FAILURE;
    }
    
    std::string extension = fs::path(source).extension().string();

    if (extension != ".c" && 
        extension != ".cpp" && 
        extension != ".cc" && 
        extension != ".cxx") 
    {
      std::cerr<<"unsupported type file\n";
      return EXIT_FAILURE;
    }

    std::ifstream file(source);
    if (!file) {
      std::cerr<<"failed to open the file\n";
      return EXIT_FAILURE;
    }

    process_metadata(source);

    if (extension == ".c") {
      std::cout<<"\nc source file\n";
    } else if(extension == ".cpp"){
      std::cout<<"\n cpp source file\n";
    }

  } catch (const fs::filesystem_error& e) {
    std::cerr<<"filesystem error :\t"<<e.what()<<"\n";
    return EXIT_FAILURE;
  }
   return EXIT_SUCCESS;
}
