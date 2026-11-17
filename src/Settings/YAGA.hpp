#ifndef YAGA_SETTINGS
#define YAGA_SETTINGS

#include <string>

namespace Settings
{
// Prism backend for speech: "auto" (the running screen reader, else a speech engine) or a backend name such as NVDA,
// JAWS, ZDSR, SAPI or OneCore.
inline std::string SpeechBackend;
// Rate and volume of a speech engine (SAPI, OneCore), 0-100 with 50 as the default; screen readers keep their own.
inline int SpeechRate;
inline int SpeechVolume;

inline int CompassAngleOffset;

// 0 = the navigator hands the hero one waypoint at a time (EV_GOTOPOS) and watches every step,
// 1 = the navigator hands the whole route to the engine (EV_GOROUTE) and only watches the result.
inline int NavigatorMode;
// Speak progress every N waypoints while walking (0 = only start, recalculation and arrival).
inline int NavigatorProgressEvery;
// Enables the yaga_cmd.txt command file used for automated testing.
inline int DevCommands;
// Language of spoken messages: "auto" (the game's language), "pl" or "en".
inline std::string Language;
} // namespace Settings

#endif

#ifdef GOTHIC_NAMESPACE

namespace GOTHIC_NAMESPACE
{
void LoadSpeechEngineSettings()
{
    Settings::SpeechBackend = zoptions->ReadString("YAGA", "SpeechBackend", "auto").ToChar();
    Settings::SpeechRate = zoptions->ReadInt("YAGA", "SpeechRate", 50);
    Settings::SpeechVolume = zoptions->ReadInt("YAGA", "SpeechVolume", 50);

    Settings::CompassAngleOffset = zoptions->ReadInt("YAGA", "CompassAngleOffset", 45);

    Settings::NavigatorMode = zoptions->ReadInt("YAGA", "NavigatorMode", 0);
    Settings::NavigatorProgressEvery = zoptions->ReadInt("YAGA", "NavigatorProgressEvery", 5);
    Settings::DevCommands = zoptions->ReadInt("YAGA", "DevCommands", 0);
    Settings::Language = zoptions->ReadString("YAGA", "Language", "auto").ToChar();
}
} // namespace GOTHIC_NAMESPACE

#endif
