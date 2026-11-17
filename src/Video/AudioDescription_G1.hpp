namespace GOTHIC_NAMESPACE
{
namespace AudioDescription
{
// Audio description of the Gothic 1 intro (INTRO.BIK, 202 s at 25 fps), played when a new game starts.
//
// The lines fill the pauses of the narration and dialogue, timed against the Polish release (word timings from a
// transcript of its sound track). Names come from the game itself:
// - the narrator is Xardas, one of the mages who made the Barrier ("A ja byłem jednym z nich");
// - the king is Rhobar II, named by the narrator and the judge;
// - the guard who punches the hero at the lake is Bullit, and the man who sends Bullit's men away is Diego (Diego's
//   dialogue Info_Diego_WarumGeholfen: "Gdyby nie ja, Bullit i jego chłopcy mogliby cię wykończyć").
// The judge, the Fire Mage with the letter and the soldiers at the cliff have no names in the game.
inline const std::vector<Track> &Tracks()
{
    static const std::vector<Track> tracks = {
#if ENGINE == Engine_G1
        {"INTRO.BIK",
         5050,
         {
             {1.0f, L"Intro. A dark screen. The narrator is the mage Xardas.",
              L"Intro. Ciemny ekran. Opowiada mag Xardas."},
             {13.2f,
              L"A throne room with pillars and torches. A red carpet leads to the throne, where King Rhobar the "
              L"Second sits in armor.",
              L"Sala tronowa, kolumny i pochodnie. Czerwony dywan wiedzie do tronu. Siedzi na nim król Rhobar Drugi "
              L"w zbroi."},
             {25.6f, L"A wooden barricade at dusk, a plain beyond it.",
              L"Drewniana barykada o zmierzchu, za nią równina."},
             {29.7f, L"Orcs charge with axes.", L"Szarżują orkowie z toporami."},
             {33.2f, L"The orcs clash with the king's soldiers, axes and swords swinging.",
              L"Orkowie ścierają się z żołnierzami króla. Topory i miecze idą w ruch."},
             {38.6f, L"A huge orc breaks through the ranks and cuts a soldier down.",
              L"Potężny ork przedziera się przez szeregi i powala żołnierza."},
             {43.5f, L"An orc raises his axe in triumph. The battlefield burns behind him.",
              L"Ork unosi topór w triumfie. Za nim płonie pole bitwy."},
             {56.4f, L"Prisoners toil in a mine.", L"Więźniowie pracują w kopalni."},
             {69.45f, L"A column of bound prisoners.", L"Kolumna związanych więźniów."},
             {82.9f, L"A mage in a dark robe raises his staff, lightning all around him.",
              L"Mag w ciemnej szacie unosi kostur. Otaczają go błyskawice."},
             {97.45f, L"A blinding flash.", L"Oślepiający błysk."},
             {108.2f, L"A prisoner with a pickaxe stands over a dead guard.",
              L"Więzień z kilofem nad zabitym strażnikiem."},
             {140.85f, L"A judge reads.", L"Czyta sędzia."},
             {163.62f, L"The hero smirks.", L"Bohater się uśmiecha."},
             {176.75f, L"The soldiers throw the hero off the cliff.", L"Żołnierze zrzucają bohatera z urwiska."},
             {178.7f, L"He falls through the shimmering Barrier, straight into a lake.",
              L"Spada przez migoczącą Barierę prosto do jeziora."},
             {183.2f, L"He crawls ashore. Bullit and his men in red armor surround him.",
              L"Wyczołguje się na brzeg. Otaczają go Bullit i jego ludzie w czerwonych zbrojach."},
             {189.65f, L"Bullit punches the hero in the face. Darkness.",
              L"Bullit uderza bohatera pięścią w twarz. Ciemność."},
             {195.95f, L"It's Diego. He leans over the hero lying on the ground.",
              L"To Diego. Pochyla się nad leżącym bohaterem."},
             {200.6f, L"End of the intro.", L"Koniec intra."},
         }},
#endif
    };
    return tracks;
}
} // namespace AudioDescription
} // namespace GOTHIC_NAMESPACE
