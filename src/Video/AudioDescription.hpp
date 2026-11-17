namespace GOTHIC_NAMESPACE
{
namespace AudioDescription
{
// One line of audio description, said when the video reaches `at` seconds. Lines are placed in the pauses of the
// video's dialogue and kept short enough to end before the next spoken line.
struct Cue
{
    float at;
    const wchar_t *english;
    const wchar_t *polish;
};

// The audio description of one video. `frames` is the length of the video the cues were timed against: a video of
// another length (a mod's own intro, for example) gets no description.
struct Track
{
    const char *file; // upper case, no folder
    unsigned long frames;
    std::vector<Cue> cues;
};

// Defined per game in AudioDescription_G1.hpp.
inline const std::vector<Track> &Tracks();

// The start of Bink's BINK structure (binkw32.dll). The engine reads Frames and FrameNum at these offsets itself, in
// zCBinkPlayer::IsPlaying.
struct BinkHeader
{
    unsigned long width;
    unsigned long height;
    unsigned long frames;
    unsigned long frameNum; // the frame shown next, from 1
    unsigned long lastFrameNum;
    unsigned long frameRate; // frames per second = frameRate / frameRateDiv
    unsigned long frameRateDiv;
};

// A cue more than this late (the video jumped ahead) is skipped rather than said out of place.
constexpr double LateCueSeconds = 2.0;

struct Playback
{
    const BinkHeader *bink = nullptr; // the open video, as binkw32.dll knows it
    const Track *track = nullptr;
    double framesPerSecond = 25.0;
    size_t next = 0;
    bool framesSeen = false;
};
inline Playback Current;

inline const BinkHeader *HeaderOf(const zCBinkPlayer *player)
{
    return static_cast<const BinkHeader *>(player->mVideoHandle);
}

// "C:\...\VIDEO\Intro.bik" -> "INTRO.BIK"
inline std::string FileName(const zSTRING &path)
{
    std::string name = path.ToChar();
    if (const size_t slash = name.find_last_of("\\/"); slash != std::string::npos)
        name.erase(0, slash + 1);
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::toupper(c); });
    return name;
}

// Called once the video is open and about to play.
inline void Start(const zCBinkPlayer *player)
{
    Current = {};
    const BinkHeader *bink = HeaderOf(player);
    if (!bink)
        return;

    const std::string file = FileName(player->mVideoFilename);
    double framesPerSecond = 25.0;
    if (bink->frameRate > 0 && bink->frameRateDiv > 0)
    {
        const double rate = static_cast<double>(bink->frameRate) / bink->frameRateDiv;
        if (rate >= 1.0 && rate <= 240.0)
            framesPerSecond = rate;
    }
    YagaLog::Write(std::format("[video] {}: {} frames, {:.2f} fps", file, bink->frames, framesPerSecond));

    if (!Settings::AudioDescription)
        return;
    for (const Track &track : Tracks())
    {
        if (file != track.file)
            continue;
        if (bink->frames != track.frames)
        {
            YagaLog::Write(std::format("[video] the audio description was made for a video of {} frames, not used",
                                       track.frames));
            return;
        }
        Current.bink = bink;
        Current.track = &track;
        Current.framesPerSecond = framesPerSecond;
        YagaLog::Write(std::format("[video] audio description: {} lines", track.cues.size()));
        return;
    }
}

// Called after every decoded frame (BinkDoFrame): says the cues whose time has come.
inline void Update(const void *handle)
{
    if (!Current.track)
        return;
    if (handle != Current.bink)
    {
        if (!Current.framesSeen)
        {
            Current.framesSeen = true;
            YagaLog::Write("[video] frames come from another Bink handle than the player's; no audio description");
        }
        return;
    }
    const BinkHeader *bink = Current.bink;
    if (!Current.framesSeen)
    {
        Current.framesSeen = true;
        YagaLog::Write(std::format("[video] following frames from frame {}", bink->frameNum));
    }

    const double now = (bink->frameNum > 0 ? bink->frameNum - 1 : 0) / Current.framesPerSecond;
    const std::vector<Cue> &cues = Current.track->cues;
    while (Current.next < cues.size() && cues[Current.next].at <= now)
    {
        const Cue &cue = cues[Current.next++];
        if (now - cue.at > LateCueSeconds)
        {
            YagaLog::Write(std::format("[video] skipped the line at {:.2f} s (video at {:.2f} s)", cue.at, now));
            continue;
        }
        YagaLog::Write(std::format("[video] line at {:.2f} s (video at {:.2f} s)", cue.at, now));
        // Queued, not interrupting: a line still being said finishes first.
        Speech::Read(Loc::T(cue.english, cue.polish), 0);
    }
}

// Called when the video is closed (BinkClose). A video skipped with ESC silences the line still being said.
inline void Stop(const void *handle)
{
    if (!Current.track || handle != Current.bink)
        return;
    const BinkHeader *bink = Current.bink;
    if (bink->frameNum + 1 < bink->frames)
    {
        YagaLog::Write(std::format("[video] skipped at frame {} of {}", bink->frameNum, bink->frames));
        Speech::StopReading();
    }
    Current = {};
}
} // namespace AudioDescription
} // namespace GOTHIC_NAMESPACE
