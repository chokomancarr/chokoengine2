#include "chokoengine.hpp"
#ifdef PLATFORM_LNX
#include <sys/wait.h>
#endif

CE_BEGIN_NAMESPACE

int Subprocess::Run(const std::string& program, const std::vector<std::string>& args, Subprocess::_CbFunc callback) {
    ProcessInfo info;
    info.program = program;
    info.args = args;
    info.callback = callback;
    return Run(info);
}

int Subprocess::Run(const ProcessInfo& info) {
    const auto& program = info.program;
    const auto& wdir = info.workingDir;
    const auto& args = info.args;
    const auto& callback = info.callback;
    if ((program.find('/') != std::string::npos) && !IO::FileExists(program)) {
        Debug::Warning("Subprocess", "program \"" + program + "\" does not exist!");
        return 101;
    }
    Debug::Message("Subprocess", "Executing " + program);
    for (int a = 0; a < args.size(); a++) {
        Debug::Message("Subprocess", " With arg[" + std::to_string(a) + "]=" + args[a]);
    }
#ifdef PLATFORM_WIN
	STARTUPINFO si = {};
	PROCESS_INFORMATION pi;
	si.cb = sizeof(si);
	std::string cmd = "cmd /C \"\"" + program + "\" ";
	for (auto& a : args) {
		if (a[0] == '-' || a[0] == '\\')
			cmd += " " + a;
		else
			cmd += " \"" + a + "\"";
	}
	cmd += "\"";
	auto wdir2 = wdir;
	std::replace(wdir2.begin(), wdir2.end(), '/', '\\');
	if (!CreateProcess("C:\\Windows\\System32\\cmd.exe", &cmd[0], NULL, NULL, FALSE, NULL, 0, 
			 wdir.empty() ? 0 : wdir2.c_str(), &si, &pi)) {
		auto err = GetLastError();
		Debug::Warning("Subprocess::Run", "Could not create cmd process! (winapi error " + std::to_string(err) + ")");
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
    if (callback && pipe(pipe_fd) == -1) return 102;
    auto pid = fork();
    if (pid < 0) return 103;
    else if (!pid) {
        if (!wdir.empty())
            chdir(wdir.c_str());
        std::vector<const char*> cargs(1, program.c_str());
        for (auto& a : args) {
            cargs.push_back(a.c_str());
        }
        cargs.push_back(nullptr);
        if (callback) {
            close(pipe_fd[0]);
            dup2(pipe_fd[1], fileno(stdout));
            dup2(pipe_fd[1], fileno(stderr));
        }
        setvbuf(stdout, nullptr, _IOLBF, 0);
        setvbuf(stderr, nullptr, _IOLBF, 0);
        if (program.find('/') == std::string::npos)
            execvp(program.c_str(), (char**)cargs.data());
        else
            execv(program.c_str(), (char**)cargs.data());
        exit(110);
    }
    else {
        if (callback) {
            close(pipe_fd[1]);
        }
        if (!info.wait) return 0;
        char data[100];
        int status;
        while (!waitpid(pid, &status, WNOHANG)) {
            if (callback) {
                size_t dsz;
                if ((int)(dsz = read(pipe_fd[0], data, 100)) > 0){
                    callback(std::string(data, dsz));
                }
            }
        }
		if (WIFEXITED(status)) {
            Debug::Message("Subprocess::Run", "Child process exited with code " + std::to_string(WEXITSTATUS(status)));
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