#include <string>
#include <unordered_set>
//  CLASSES:

class Args
{
    private:
    static inline const std::unordered_set<std::string> 
        accepted_args{"--help", "--info","--run"};
    static bool valid_arg(std::string arg) 
    {
        return Args::accepted_args.find(arg) != Args::accepted_args.end();
    }
    public:

    bool print_help = false;
    std::string info_filepath ="";
    std::string run_filepath = "";

    
    Args() {};
    //  returns nonzero on fail to parse args
    int parse(int argc, char* argv[])
    {
        for(int i =1; i < argc; ++i)
        {
            std::string arg = std::string(argv[i]);
            if (!Args::valid_arg(arg)) return 2;    //  usage error
            
            //  parse accordingly:
            if (arg == "--help") print_help = true;
            if (arg == "--info")
            {
                if (
                    i+1 >= argc || 
                    (i+1<argc && Args::valid_arg(std::string(argv[i+1])))
                ) 
                    return 2;   //  usage error
                info_filepath = std::string(argv[i+1]);
                i++;
            }
            if (arg == "--run")
            {
                if (
                    i+1 >= argc || 
                    (i+1<argc && Args::valid_arg(std::string(argv[i+1])))
                ) 
                    return 2;   //  usage error
                run_filepath = std::string(argv[i+1]);
                i++;
            }
        }
        return 0;
    }
};

//  FUNCTIONS:

void print_help(bool error = false);    //  prints help, prints to std::cerr if error=true
