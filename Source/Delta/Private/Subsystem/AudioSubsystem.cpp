// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/AudioSubsystem.h"

#include "DeveloperSettings/AudioDeveloperSettings.h"
#include "Helper/DeltaDebugHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"

void UAudioSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	CachedAudioSettings = GetDefault<UAudioDeveloperSettings>();
	if (!CachedAudioSettings)
	{
		DeltaDebug::Print(FString(TEXT("Error in load Developer setting")));
		return;
	}

	if (CachedAudioSettings->MasterSoundMix.IsNull())
	{
		DeltaDebug::Print(FString(TEXT("Audio developer settings is not set")));
		return;
	}
	MasterSoundMix = CachedAudioSettings->MasterSoundMix.LoadSynchronous();
	check(MasterSoundMix);

	for (auto SoundClassPair : CachedAudioSettings->SoundClasses)
	{
		if (SoundClassPair.Value.IsNull())
		{
			DeltaDebug::Print(FString(TEXT("Audio SoundClass is not set")));
			return;
		}

		USoundClass* LoadedSoundClass = SoundClassPair.Value.LoadSynchronous();
		SoundClasses.Add(SoundClassPair.Key, LoadedSoundClass);
		check(SoundClasses[SoundClassPair.Key]);
	}
	
}

UAudioSubsystem* UAudioSubsystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert))
		{
			return UGameInstance::GetSubsystem<UAudioSubsystem>(World->GetGameInstance());
		}
	}

	return nullptr;
}

void UAudioSubsystem::SetVolume(ESoundCategory SoundCategory, const float Volume)
{
	if (!SoundClasses.Find(SoundCategory)) return;
	
	UGameplayStatics::SetSoundMixClassOverride(
		GetWorld(),
		MasterSoundMix,
		SoundClasses[SoundCategory],
		Volume,
		1.0f,
		0.0f,
		true
		);
	
	UGameplayStatics::PushSoundMixModifier(GetWorld(), MasterSoundMix);

	CachedAudioSettings = CachedAudioSettings ? CachedAudioSettings : GetDefault<UAudioDeveloperSettings>();
	if (!CachedAudioSettings) return;

	const TSoftObjectPtr<USoundBase>* FoundTestSoundPtr = CachedAudioSettings->TestSounds.Find(SoundCategory);
	if (!FoundTestSoundPtr || FoundTestSoundPtr->IsNull()) return;

	USoundBase* TestSound = FoundTestSoundPtr->LoadSynchronous();
	UGameplayStatics::PlaySound2D(GetWorld(), TestSound);
	
}

void UAudioSubsystem::SetUnfocusedVolume(bool bAllowUnfocusedVolume)
{
	bAllowUnfocusedVolume ? FApp::SetUnfocusedVolumeMultiplier(1.0f) : FApp::SetUnfocusedVolumeMultiplier(0.0f);
}
