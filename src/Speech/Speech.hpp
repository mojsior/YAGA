namespace GOTHIC_NAMESPACE
{
namespace Speech
{
std::unique_ptr<SpeechEngine> engine;

// A problem found while starting speech, said once the player is in the game (in the menu the menu reader would talk
// over it at once).
inline std::wstring PendingWarning;

// All speech goes through Prism: the running screen reader, or a speech engine when none runs.
void Initialize(const std::string &backend, const int rate, const int volume)
{
    YagaLog::Write("[speech] Prism, asked for backend " + backend);
    auto prism = std::make_unique<SpeechEnginePrism>(backend, rate, volume);
    const std::wstring stale = prism->StaleZdsrLibrary();
    engine = std::move(prism);
    if (!stale.empty())
        PendingWarning = Loc::Format(L"An old ZDSRAPI.dll keeps the Zhengdu screen reader from working. Delete {}",
                                     L"Stara biblioteka ZDSRAPI.dll blokuje czytnik Zhengdu. Usuń plik {}", stale);
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

void SayPendingWarning()
{
    if (PendingWarning.empty())
        return;
    Read(PendingWarning);
    PendingWarning.clear();
}

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
