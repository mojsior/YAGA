namespace GOTHIC_NAMESPACE
{
namespace Speech
{
std::unique_ptr<SpeechEngine> engine;

// All speech goes through Prism: the running screen reader, or a speech engine when none runs.
void Initialize(const std::string &backend, const int rate, const int volume)
{
    YagaLog::Write("[speech] Prism, asked for backend " + backend);
    engine = std::make_unique<SpeechEnginePrism>(backend, rate, volume);
}

void StopReading()
{
    if (engine)
        engine->StopReading();
}

// Every spoken line goes through here, so it is also written to YAGA.log.
int Emit(const std::wstring &text, const int interrupt)
{
    YagaLog::Write(L"[speech] " + text);
    if (!engine)
        return 0;

    return engine->Read(text.c_str(), interrupt);
}

// Read methods

int Read(const char *str, const int interrupt = 1)
{
    return Emit(ToWString(str), interrupt);
}

int Read(const wchar_t *str, const int interrupt = 1)
{
    return Emit(str, interrupt);
}

int Read(const std::string &str, const int interrupt = 1)
{
    return Emit(ToWString(str), interrupt);
}

int Read(const std::wstring &str, const int interrupt = 1)
{
    return Emit(str, interrupt);
}

int Read(const zSTRING &str, const int interrupt = 1)
{
    return Emit(ToWString(str), interrupt);
}

// ReadFormat methods

template <typename... Args> void ReadFormat(std::format_string<Args...> fmt, Args &&...args)
{
    Read(std::format(fmt, std::forward<Args>(args)...));
}
template <typename... Args> void ReadFormatInterrupt(std::format_string<Args...> fmt, Args &&...args)
{
    Read(std::format(fmt, std::forward<Args>(args)...), 1);
}
} // namespace Speech
} // namespace GOTHIC_NAMESPACE
