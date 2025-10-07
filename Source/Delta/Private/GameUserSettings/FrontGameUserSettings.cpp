// Fill out your copyright notice in the Description page of Project Settings.


#include "GameUserSettings/FrontGameUserSettings.h"

#include "Subsystem/AudioSubsystem.h"

UFrontGameUserSettings::UFrontGameUserSettings()
	: OverallVolume(0.3f), BGMVolume(0.3f), SFXVolume(0.3f), bAllowBackgroundAudio(false), bUseHDRAudio(false)
{
}

UFrontGameUserSettings* UFrontGameUserSettings::Get()
{
	if (GEngine)
	{
		return CastChecked<UFrontGameUserSettings>(GEngine->GetGameUserSettings());
	}

	return nullptr;
}

void UFrontGameUserSettings::SetOverallVolume(const float InOverallVolume)
{
	OverallVolume = InOverallVolume;

	if (GEngine)
	{
		if (UAudioSubsystem* AudioSubsystem = UAudioSubsystem::Get(GEngine->GetCurrentPlayWorld()))
		{
			AudioSubsystem->SetVolume(ESoundCategory::Master, InOverallVolume);
		}
	}
}

void UFrontGameUserSettings::SetBGMVolume(const float InBGMVolume)
{
	BGMVolume = InBGMVolume;

	if (GEngine)
	{
		if (UAudioSubsystem* AudioSubsystem = UAudioSubsystem::Get(GEngine->GetCurrentPlayWorld()))
		{
			AudioSubsystem->SetVolume(ESoundCategory::BGM, InBGMVolume);
		}
	}
}

void UFrontGameUserSettings::SetSFXVolume(const float InSFXVolume)
{
	SFXVolume = InSFXVolume;

	if (GEngine)
	{
		if (UAudioSubsystem* AudioSubsystem = UAudioSubsystem::Get(GEngine->GetCurrentPlayWorld()))
		{
			AudioSubsystem->SetVolume(ESoundCategory::SFX, InSFXVolume);
		}
	}
}

void UFrontGameUserSettings::SetAllowBackgroundAudio(const bool InbAllowBackgroundAudio)
{
	bAllowBackgroundAudio = InbAllowBackgroundAudio;
	
	if (GEngine)
	{
		if (UAudioSubsystem* AudioSubsystem = UAudioSubsystem::Get(GEngine->GetCurrentPlayWorld()))
		{
			AudioSubsystem->SetUnfocusedVolume(InbAllowBackgroundAudio);
		}
	}
}

void UFrontGameUserSettings::SetUseHDRAudio(const bool InbUseHDRAudio)
{
	bUseHDRAudio = InbUseHDRAudio;
	
	if (GEngine)
	{
		if (UAudioSubsystem* AudioSubsystem = UAudioSubsystem::Get(GEngine->GetCurrentPlayWorld()))
		{
			//
		}
	}
}

float UFrontGameUserSettings::GetCurrentDisplayGamma()
{
	if (GEngine)
	{
		return GEngine->GetDisplayGamma();
	}

	return 0.0f;
}

void UFrontGameUserSettings::SetCurrentDisplayGamma(float InNewGamma)
{
	if (GEngine)
	{
		GEngine->DisplayGamma = InNewGamma;
	}
}
