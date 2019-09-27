#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

int Subprocess::Run(const std::string& program, const std::vector<std::string>& args, Subprocess::_CbFunc callback) {
    Debug::Message("Subprocess", "Executing " + program);
    for (int a = 0; a < args.size(); a++) {
        Debug::Message("Subprocess", " With arg[" + std::to_string(a) + "]=" + args[a]);
    }
#ifdef PLATFORM_WIN
	STARTUPINFO si = {};
	PROCESS_INFORMATION pi;
	si.cb = sizeof(si);
	std::string cmd = "cmd /C \"" + program + "\"";
	if (!CreateProcess("C:\\Windows\\System32\\cmd.exe", &cmd[0], NULL, NULL, FALSE, CREATE_NO_WINDOW, 0, 0, &si, &pi)) {
		Debug::Warning("Subprocess::Run", "Could not create cmd process!");
		return -1;
	}
	DWORD w;
	do {
		w = WaitForSingleObject(pi.hProcess, 100);
	} while (w == WAIT_TIMEOUT);

	DWORD dret;
	GetExitCodeProcess(pi.hProcess, &dret);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	return (int)dret;
#else
    int pipe_fd[2];
    if (callback && pipe(pipe_fd) == -1) return -2;
    auto pid = fork();
    if (pid < 0) return -3;
    else if (!pid) {
        std::vector<const char*> cargs(1, program.c_str());
        for (auto& a : args) {
            cargs.push_back(a.c_str());
        }
        cargs.push_back(nullptr);
        if (callback) {
            close(pipe_fd[0]);
            dup2(pipe_fd[1], 1);
        }
        setvbuf(stdout, nullptr, _IOLBF, 0);
        execv(program.c_str(), (char**)cargs.data());
        exit(-5);
    }
    else {
        if (callback) {
            close(pipe_fd[1]);
        }
        char data[100];
        int status;
        while (!waitpid(pid, &status, WNOHANG)) {
            if (callback) {
                size_t dsz;
                if ((int)(dsz = read(pipe_fd[0], data, 100)) > 0){
                    std::cout << "Child says " << std::string(data, dsz) << std::endl;
                }
            }
        }
		if (WIFEXITED(status)) {
			return WEXITSTATUS(status);
		}
		else {
			Debug::Warning("Subprocess::Run", "Child process terminated unexpectedly!");
			return -10;
		}
    }
#endif
}

CE_END_NAMESPACE