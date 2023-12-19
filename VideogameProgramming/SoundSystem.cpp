#include "SoundSystem.h"
#include <conio.h>

SoundSystem::SoundSystem() {
    soundEngine = createIrrKlangDevice();
    if (!soundEngine) {
        cout << "Failed to create Sound Engine" << endl;
        //exit(-1);
    }
}

void SoundSystem::tick(World* world, float deltaTime)
{
    if (!soundEngine) return;
    world->each<SoundComponent>([&](Entity* ent, ComponentHandle<SoundComponent> sound) {

        if (sound->playSound) {
            if ((!soundEngine->isCurrentlyPlaying(sound->filePath) && !sound->stackable) || sound->stackable) {
                //play sound at volume
                soundEngine->setSoundVolume(sound->volume);
                soundEngine->play2D(sound->filePath);
                
                sound->playSound = false;
                cout << "Playing sound: " << sound->filePath << " at volume " <<sound->volume << endl;
            }
        }

        if (sound->cancel) {

            soundEngine->stopAllSoundsOfSoundSource(soundEngine->getSoundSource(sound->filePath));
            sound->cancel = false;
        }
        

    });
}