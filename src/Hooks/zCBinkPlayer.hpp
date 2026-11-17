namespace GOTHIC_NAMESPACE
{
// A video starts with zCBinkPlayer::PlayInit (the file is open by then). Frames are followed in binkw32.dll itself:
// SystemPack's Bink fix (FixBinkNew) plays the frames with its own code instead of zCBinkPlayer::PlayFrame, but every
// player decodes each frame with BinkDoFrame and ends with BinkClose. oCBinkPlayer::PlayInit calls this one.
int __fastcall zCBinkPlayer_PlayInit(zCBinkPlayer *self, void *vtable, int arg);
auto Hook_zCBinkPlayer_PlayInit =
    Union::CreateHook(reinterpret_cast<void *>(zSwitch(0x0043B460, 0x00440390, 0x0043EA50, 0x0043F080)),
                      &zCBinkPlayer_PlayInit, Union::HookType::Hook_Detours);
int __fastcall zCBinkPlayer_PlayInit(zCBinkPlayer *self, void *vtable, int arg)
{
    const int result = Hook_zCBinkPlayer_PlayInit(self, vtable, arg);
    AudioDescription::Start(self);
    return result;
}

inline void *BinkFunction(const char *name)
{
    const HMODULE bink = GetModuleHandleA("binkw32.dll");
    return bink ? reinterpret_cast<void *>(GetProcAddress(bink, name)) : nullptr;
}

int __stdcall BinkDoFrame_Union(void *bink);
auto Hook_BinkDoFrame =
    Union::CreateHook(BinkFunction("_BinkDoFrame@4"), &BinkDoFrame_Union, Union::HookType::Hook_Detours);
int __stdcall BinkDoFrame_Union(void *bink)
{
    const int result = Hook_BinkDoFrame(bink);
    AudioDescription::Update(bink);
    return result;
}

void __stdcall BinkClose_Union(void *bink);
auto Hook_BinkClose = Union::CreateHook(BinkFunction("_BinkClose@4"), &BinkClose_Union, Union::HookType::Hook_Detours);
void __stdcall BinkClose_Union(void *bink)
{
    AudioDescription::Stop(bink);
    Hook_BinkClose(bink);
}
} // namespace GOTHIC_NAMESPACE
