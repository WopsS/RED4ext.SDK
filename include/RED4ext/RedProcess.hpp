#include <RED4ext/RED4ext.hpp>

namespace RED4ext::red
{

struct Process
{
    struct FixedWString
    {
        uint32_t length;
        uint32_t maxLength;
        const wchar_t* str;
    };

    enum class ExecutionFlags : unsigned char
    {
        // unsets CREATE_NO_WINDOW
        ShouldCreateWindow = 0x1,
        // sets CREATE_BREAKAWAY_FROM_JOB | CREATE_SUSPENDED in Process Creation Flags
        BreakawayAndSuspend = 0x2,
        // unsets bInheritHandles
        NoInheritHandles = 0x4
    };

    DEFINE_ENUM_FLAG_OPERATORS(ExecutionFlags);

    enum class ReadFlags : unsigned char {
        Unk1 = 0x1,
        WriteToFile = 0x2,
        GetCString = 0x4
    };

    DEFINE_ENUM_FLAG_OPERATORS(ReadFlags);

    static constexpr uint32_t defaultTimeout = 60000;
    
    // creates the read & write pipes, and handle information
    /// @pattern 48 89 5C 24 08 57 48 83 EC 40 33 C0 C7 44 24 20 18 00 00 00 48 8D 91 08 20 00 00 48 89 81 00 20
    Process();

    /// @pattern 40 53 48 83 EC 20 48 8B D9 48 8B 89 08 20 00 00 FF 15 42 70 4B 00 48 8B 8B 00 20 00 00 48 83 C4
    bool __fastcall CloseHandles();

    // assigns to this->errorCode, but also returns
    /// @pattern 40 53 48 83 EC 20 48 8D 99 28 20 00 00 48 8B 89 10 20 00 00 48 8B D3 FF 15 C3 6F 4B 00 8B 03 48
    int32_t __fastcall GetExitCode();

    /// @pattern 40 55 56 57 41 56 B8 68 20 00 00 E8 00 52 E7 FF 48 2B E0 49 8B E9 49 8B F0 8B FA 4C 8B F1 F6 C2
    void __fastcall ReadFromPipe(ReadFlags aFlags, CString* aFilename, CString* aStdOut);

    // converts aWorkingDirectory to a FixedWString
    /// @pattern 48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 40 48 8B FA 48 8B F1 48 8D 54 24 30 49 8B C9 49 8B D8
    bool __fastcall Execute(CString* aCommand, FixedWString* aArgs, CString* aWorkingDirectory, ExecutionFlags aFlags);

    /// @pattern 48 89 5C 24 10 48 89 74 24 18 55 57 41 54 41 56 41 57 48 8D 6C 24 B0 48 81 EC 50 01 00 00 0F 57
    bool __fastcall Execute(CString* aCommand, FixedWString* aArgs, FixedWString* aWorkingDirectory, ExecutionFlags aFlags);

    /// @pattern 48 89 5C 24 08 57 48 83 EC 20 48 8B D9 41 0F B6 F8 48 8B 89 10 20 00 00 FF 15 52 6D 4B 00 85 C0
    bool __fastcall Terminate(uint32_t aExitCode, bool closeHandles);

    /// @pattern 48 83 EC 48 48 8B 89 10 20 00 00 FF 15 F7 6B 4B 00 85 C0 74 4A 3D 02 01 00 00 74 3C FF 15 BE 6A
    bool __fastcall WaitUntilCompleted(uint32_t aTimeoutMS);

    wchar_t command[0x1000]; // 0000
    PHANDLE readPipe;        // 2000
    PHANDLE writePipe;       // 2008
    HANDLE handle;           // 2010
    HANDLE hThread;          // 2018
    uint64_t unk2;           // 2020
    int32_t errorCode;       // 2028
};

} // RED4ext namespace