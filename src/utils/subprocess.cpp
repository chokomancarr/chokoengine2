#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

void Subprocess::Run(const std::string& program, const std::vector<std::string>& args, Subprocess::_CbFunc callback) {
#ifdef PLATFORM_WIN
	CE_NOT_IMPLEMENTED
#else
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) return;
    auto pid = fork();
    if (pid < 0) return;
    else if (!pid) {
        std::vector<const char*> cargs(1, program.c_str());
        for (auto& a : args) {
            cargs.push_back(a.c_str());
        }
        cargs.push_back(nullptr);
        close(pipe_fd[0]);
        dup2(pipe_fd[1], 1);
        setvbuf(stdout, nullptr, _IOLBF, 0);
        execv(program.c_str(), (char**)cargs.data());
        exit(0);
    }
    else {
        close(pipe_fd[1]);
        char data[100];
        int status;
        while (!waitpid(pid, &status, WNOHANG)) {
            size_t dsz;
            if ((int)(dsz = read(pipe_fd[0], data, 100)) > 0){
                std::cout << "Child says " << std::string(data, dsz) << std::endl;
            }
        }
    }
#endif
}

CE_END_NAMESPACE