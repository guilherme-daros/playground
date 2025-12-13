#pragma once

namespace sb::system {
class Signal {
 public:
  enum class Enum {
    SIGHUP = 1,
    SIGINT = 2,
    SIGQUIT = 3,
    SIGILL = 4,
    SIGTRAP = 5,
    SIGABRT = 6,
    SIGBUS = 7,
    SIGFPE = 8,
    SIGKILL = 9,
    SIGUSR1 = 10,
    SIGSEGV = 11,
    SIGUSR2 = 12,
    SIGPIPE = 13,
    SIGALRM = 14,
    SIGTERM = 15,
    SIGSTKFLT = 16,
    SIGCHLD = 17,
    SIGCONT = 18,
    SIGSTOP = 19,
    SIGTSTP = 20,
    SIGTTIN = 21,
    SIGTTOU = 22,
    SIGURG = 23,
    SIGXCPU = 24,
    SIGXFSZ = 25,
    SIGVTALRM = 26,
    SIGPROF = 27,
    SIGWINCH = 28,
    SIGPOLL = 29,
    SIGPWR = 30,
    SIGSYS = 31,
  };

  struct SIGHUP {
    constexpr static auto I = Enum::SIGHUP;
    virtual ~SIGHUP() = default;
    virtual auto onSIGHUP() -> void = 0;
    void operator()() { onSIGHUP(); }
  };

  struct SIGINT {
    constexpr static auto I = Enum::SIGINT;
    virtual ~SIGINT() = default;
    virtual auto onSIGINT() -> void = 0;
    void operator()() { onSIGINT(); }
  };

  struct SIGQUIT {
    constexpr static auto I = Enum::SIGQUIT;
    virtual ~SIGQUIT() = default;
    virtual auto onSIGQUIT() -> void = 0;
    void operator()() { onSIGQUIT(); }
  };

  struct SIGILL {
    constexpr static auto I = Enum::SIGILL;
    virtual ~SIGILL() = default;
    virtual auto onSIGILL() -> void = 0;
    void operator()() { onSIGILL(); }
  };

  struct SIGTRAP {
    constexpr static auto I = Enum::SIGTRAP;
    virtual ~SIGTRAP() = default;
    virtual auto onSIGTRAP() -> void = 0;
    void operator()() { onSIGTRAP(); }
  };

  struct SIGABRT {
    constexpr static auto I = Enum::SIGABRT;
    virtual ~SIGABRT() = default;
    virtual auto onSIGABRT() -> void = 0;
    void operator()() { onSIGABRT(); }
  };

  struct SIGBUS {
    constexpr static auto I = Enum::SIGBUS;
    virtual ~SIGBUS() = default;
    virtual auto onSIGBUS() -> void = 0;
    void operator()() { onSIGBUS(); }
  };

  struct SIGFPE {
    constexpr static auto I = Enum::SIGFPE;
    virtual ~SIGFPE() = default;
    virtual auto onSIGFPE() -> void = 0;
    void operator()() { onSIGFPE(); }
  };

  struct SIGKILL {
    constexpr static auto I = Enum::SIGKILL;
    virtual ~SIGKILL() = default;
    virtual auto onSIGKILL() -> void = 0;
    void operator()() { onSIGKILL(); }
  };

  struct SIGUSR1 {
    constexpr static auto I = Enum::SIGUSR1;
    virtual ~SIGUSR1() = default;
    virtual auto onSIGUSR1() -> void = 0;
    void operator()() { onSIGUSR1(); }
  };

  struct SIGSEGV {
    constexpr static auto I = Enum::SIGSEGV;
    virtual ~SIGSEGV() = default;
    virtual auto onSIGSEGV() -> void = 0;
    void operator()() { onSIGSEGV(); }
  };

  struct SIGUSR2 {
    constexpr static auto I = Enum::SIGUSR2;
    virtual ~SIGUSR2() = default;
    virtual auto onSIGUSR2() -> void = 0;
    void operator()() { onSIGUSR2(); }
  };

  struct SIGPIPE {
    constexpr static auto I = Enum::SIGPIPE;
    virtual ~SIGPIPE() = default;
    virtual auto onSIGPIPE() -> void = 0;
    void operator()() { onSIGPIPE(); }
  };

  struct SIGALRM {
    constexpr static auto I = Enum::SIGALRM;
    virtual ~SIGALRM() = default;
    virtual auto onSIGALRM() -> void = 0;
    void operator()() { onSIGALRM(); }
  };

  struct SIGTERM {
    constexpr static auto I = Enum::SIGTERM;
    virtual ~SIGTERM() = default;
    virtual auto onSIGTERM() -> void = 0;
    void operator()() { onSIGTERM(); }
  };

  struct SIGSTKFLT {
    constexpr static auto I = Enum::SIGSTKFLT;
    virtual ~SIGSTKFLT() = default;
    virtual auto onSIGSTKFLT() -> void = 0;
    void operator()() { onSIGSTKFLT(); }
  };

  struct SIGCHLD {
    constexpr static auto I = Enum::SIGCHLD;
    virtual ~SIGCHLD() = default;
    virtual auto onSIGCHLD() -> void = 0;
    void operator()() { onSIGCHLD(); }
  };

  struct SIGCONT {
    constexpr static auto I = Enum::SIGCONT;
    virtual ~SIGCONT() = default;
    virtual auto onSIGCONT() -> void = 0;
    void operator()() { onSIGCONT(); }
  };

  struct SIGSTOP {
    constexpr static auto I = Enum::SIGSTOP;
    virtual ~SIGSTOP() = default;
    virtual auto onSIGSTOP() -> void = 0;
    void operator()() { onSIGSTOP(); }
  };

  struct SIGTSTP {
    constexpr static auto I = Enum::SIGTSTP;
    virtual ~SIGTSTP() = default;
    virtual auto onSIGTSTP() -> void = 0;
    void operator()() { onSIGTSTP(); }
  };

  struct SIGTTIN {
    constexpr static auto I = Enum::SIGTTIN;
    virtual ~SIGTTIN() = default;
    virtual auto onSIGTTIN() -> void = 0;
    void operator()() { onSIGTTIN(); }
  };

  struct SIGTTOU {
    constexpr static auto I = Enum::SIGTTOU;
    virtual ~SIGTTOU() = default;
    virtual auto onSIGTTOU() -> void = 0;
    void operator()() { onSIGTTOU(); }
  };

  struct SIGURG {
    constexpr static auto I = Enum::SIGURG;
    virtual ~SIGURG() = default;
    virtual auto onSIGURG() -> void = 0;
    void operator()() { onSIGURG(); }
  };

  struct SIGXCPU {
    constexpr static auto I = Enum::SIGXCPU;
    virtual ~SIGXCPU() = default;
    virtual auto onSIGXCPU() -> void = 0;
    void operator()() { onSIGXCPU(); }
  };

  struct SIGXFSZ {
    constexpr static auto I = Enum::SIGXFSZ;
    virtual ~SIGXFSZ() = default;
    virtual auto onSIGXFSZ() -> void = 0;
    void operator()() { onSIGXFSZ(); }
  };

  struct SIGVTALRM {
    constexpr static auto I = Enum::SIGVTALRM;
    virtual ~SIGVTALRM() = default;
    virtual auto onSIGVTALRM() -> void = 0;
    void operator()() { onSIGVTALRM(); }
  };

  struct SIGPROF {
    constexpr static auto I = Enum::SIGPROF;
    virtual ~SIGPROF() = default;
    virtual auto onSIGPROF() -> void = 0;
    void operator()() { onSIGPROF(); }
  };

  struct SIGWINCH {
    constexpr static auto I = Enum::SIGWINCH;
    virtual ~SIGWINCH() = default;
    virtual auto onSIGWINCH() -> void = 0;
    void operator()() { onSIGWINCH(); }
  };

  struct SIGPOLL {
    constexpr static auto I = Enum::SIGPOLL;
    virtual ~SIGPOLL() = default;
    virtual auto onSIGPOLL() -> void = 0;
    void operator()() { onSIGPOLL(); }
  };

  struct SIGPWR {
    constexpr static auto I = Enum::SIGPWR;
    virtual ~SIGPWR() = default;
    virtual auto onSIGPWR() -> void = 0;
    void operator()() { onSIGPWR(); }
  };

  struct SIGSYS {
    constexpr static auto I = Enum::SIGSYS;
    virtual ~SIGSYS() = default;
    virtual auto onSIGSYS() -> void = 0;
    void operator()() { onSIGSYS(); }
  };
};
}  // namespace sb::system
