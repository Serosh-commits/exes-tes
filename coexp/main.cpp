#include "iostream"
#include "filesystem"
#include "string"
#include<fstream>
#include "chrono"
#include "cstdlib"
#include "vector"
#include<sstream>
#include<unistd.h>
#include<sys/wait.h>
#include<unistd.h>

namespace fs = std::filesystem;
#define max_buf 
static inline void process_source_content(std::ifstream& file) {
    std::string line;
    std::size_t line_count = 0;
    std::size_t blank_line = 0;
    std::size_t preprocessor_count = 0;
    std::size_t include_count = 0;
    std::size_t define_count = 0;
    while (std::getline(file, line)) {
        line_count++;
        std::size_t first = line.find_first_not_of(" \t\r\n");

        if (first == std::string::npos) {
            blank_line++;
            continue;
        }

        if (line[first] == '#') {
            preprocessor_count++;
            std::string directive = line.substr(first);

            if (directive.starts_with("#include")) {
                include_count++;
            }

            if (directive.starts_with("#define")) {
                define_count++;
            }
        }
    }
    std::cout << "\nSource Content Analysis:\n";
    std::cout << "\tTotal lines: " << line_count << "\n";
    std::cout << "\tblank lines: " << blank_line << "\n";
    std::cout << "\tpreprocessor directives: " << preprocessor_count << "\n";
    std::cout << "\tInclude directives: " << include_count << "\n";
    std::cout << "\tdefine directives: " << define_count << "\n";
}
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

static bool  is_compiler_available(const std::string& compiler){
    
    const char* path_env = std::getenv("PATH");

    if(path_env == nullptr){
        return false;
    }

    std::stringstream paths(path_env);
    std::string directory;

    while(std::getline(paths, directory, ':')){
        if(directory.empty()){
            directory = ".";
        }

        fs::path executable = fs::path(directory) / compiler;
       
        if(fs::exists(executable) && fs::is_regular_file(executable)){
            return true;
        }
    }

    return false;
}

static void get_version(const std::string& comp){
    
    pid_t pid = fork();
    
    if(pid < 0){
        std::cerr<<"failed to create the child process\n";
        return;
    }

    if(pid == 0){
        std::string compiler = comp;
        char version_arg[] = "--version";
        
        char *args[] = {
            compiler.data(),
            version_arg,
            nullptr
        };

        execvp(args[0], args);
        std::cerr<<"failed to execute :"<<args[0]<<"\n";
        _exit(127);
    }
    

    int status = 0;
    if(waitpid(pid, &status, 0) == -1){
        std::cerr<<"waitpid failed\n";
        return;
    }

    if(WIFEXITED(status)){
        int exit_code = WEXITSTATUS(status);

        if(exit_code == 0){
            
        } else {
            std::cout<<comp<<"failed with exit code: "<<exit_code<<"\n";
        }
    } else if(WIFSIGNALED(status)){
        std::cout<<comp<<"terminated by signal: "
                        << WTERMSIG(status)<<"\n";
    }
}
static void detect_compiler(){
    bool gcc_available = is_compiler_available("gcc");
    bool clang_available = is_compiler_available("clang");
    
    std::cout<<"compilers available:\n";
    if(gcc_available){
        std::cout<<"\tGCC :\n";
        get_version("gcc");
    } else {
        std::cout<<"\t GCC unvailaible\n";
    }

    if(clang_available){
        std::cout<<"\tClang :\n";
        get_version("clang");
    } else {
        std::cout<<"\tClang not presant\n";
    }
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
    
    detect_compiler();
    process_metadata(source);
    process_source_content(file);

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
