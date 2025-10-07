// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Options/OptionsDataRegistry.h"

#include "EnhancedInputSubsystems.h"
#include "BlueprintFunctionLibrary/FrontFunctionLibrary.h"
#include "GameplayTags/FrontGameplayTags.h"
#include "GameUserSettings/FrontGameUserSettings.h"
#include "Helper/DeltaDebugHelper.h"
#include "Internationalization/StringTableRegistry.h"
#include "UI/Options/OptionsDataInteractionHelper.h"
#include "UI/Options/DataObject/ListDataObjectCollection.h"
#include "UI/Options/DataObject/ListDataObjectKeyRemap.h"
#include "UI/Options/DataObject/ListDataObjectScalar.h"
#include "UI/Options/DataObject/ListDataObjectString.h"
#include "UI/Options/DataObject/ListDataObjectStringResolution.h"
#include "UserSettings/EnhancedInputUserSettings.h"

#define MAKE_OPTIONS_DATA_CONTROL(SetterOrGetterFuncName) \
	MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UFrontGameUserSettings, SetterOrGetterFuncName))

#define GET_DESCRIPTION(InKey) \
	LOCTABLE("/Game/UI/StringTable/ST_OptionsScreenDescription.ST_OptionsScreenDescription", InKey)

void UOptionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
	InitGameplayCollectionTab();
	InitAudioCollectionTab();
	InitVideoCollectionTab();
	InitControlCollectionTab(InOwningLocalPlayer);
}

void UOptionsDataRegistry::InitGameplayCollectionTab()
{
	UListDataObjectCollection* GameplayTabCollection =  NewObject<UListDataObjectCollection>();
	GameplayTabCollection->SetDataID(FName("GameplayTabCollection"));
	GameplayTabCollection->SetDataDisplayName(FText::FromString("Gameplay"));

	// Difficulty
	{
		UListDataObjectString* GameDifficulty = NewObject<UListDataObjectString>();
		GameDifficulty->SetDataID(FName("GameDifficulty"));
		GameDifficulty->SetDataDisplayName(FText::FromString(TEXT("Difficulty")));
		GameDifficulty->SetDescriptionRichText(FText::FromString(
			"Adjust game experience.\n\n"
			"<Bold>Easy : </>Default\n"
			"<Bold>Normal : </>Enemy damage + 50%\n"
			"<Bold>Hard : </>Enemy damage + 100%\n"
			));
		
		GameDifficulty->AddDynamicOption(TEXT("Easy"), FText::FromString(TEXT("Easy")));
		GameDifficulty->AddDynamicOption(TEXT("Normal"), FText::FromString(TEXT("Normal")));
		GameDifficulty->AddDynamicOption(TEXT("Hard"), FText::FromString(TEXT("Hard")));

		GameDifficulty->SetDefaultValueFromString(TEXT("Normal"));

		GameDifficulty->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentGameDifficulty));
		GameDifficulty->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentGameDifficulty));

		GameDifficulty->SetShouldApplySettingsImmediately(true);

		GameplayTabCollection->AddChildListData(GameDifficulty);
	}

	// Test
	{
		UListDataObjectString* TestItem = NewObject<UListDataObjectString>();
		TestItem->SetDataID(FName("TestItem"));
		TestItem->SetDataDisplayName(FText::FromString(TEXT("Test Image")));
		TestItem->SetSoftDescriptionImage(UFrontFunctionLibrary::GetOptionsSoftImageByTag(FrontGameplayTags::Front_Image_TestImage));
		
		GameplayTabCollection->AddChildListData(TestItem);
	}

	RegisteredOptionsTabCollections.Add(GameplayTabCollection);
	
}

void UOptionsDataRegistry::InitAudioCollectionTab()
{
	UListDataObjectCollection* AudioTabCollection =  NewObject<UListDataObjectCollection>();
	AudioTabCollection->SetDataID(FName("AudioTabCollection"));
	AudioTabCollection->SetDataDisplayName(FText::FromString("Audio"));

	// Volume
	{
		UListDataObjectCollection* VolumeCategoryCollection = NewObject<UListDataObjectCollection>();
		VolumeCategoryCollection->SetDataID(FName("VolumeCategoryCollection"));
		VolumeCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Volume")));
		
		AudioTabCollection->AddChildListData(VolumeCategoryCollection);
		
		// Overall
		{
			UListDataObjectScalar* OverallVolume = NewObject<UListDataObjectScalar>();
			OverallVolume->SetDataID(FName("OverallVolume"));
			OverallVolume->SetDataDisplayName(FText::FromString(TEXT("Overall Volume")));
			OverallVolume->SetDescriptionRichText(FText::FromString(TEXT("Set master volume")));
			
			OverallVolume->SetDisplayValueRange(TRange<float>(0.0f, 1.0f));
			OverallVolume->SetOutputValueRange(TRange<float>(0.0f, 2.0f));
			OverallVolume->SetSliderStepSize(0.01f);
			
			OverallVolume->SetDefaultValueFromString(FString::SanitizeFloat(1.0f));
			OverallVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			OverallVolume->SetNumberFormattingOptions(UListDataObjectScalar::NoDecimal());
			
			OverallVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetOverallVolume));
			OverallVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetOverallVolume));
			OverallVolume->SetShouldApplySettingsImmediately(true);
		
			VolumeCategoryCollection->AddChildListData(OverallVolume);
		}
		
		// BGM
		{
			UListDataObjectScalar* BGMVolume = NewObject<UListDataObjectScalar>();
			BGMVolume->SetDataID(FName("BGMVolume"));
			BGMVolume->SetDataDisplayName(FText::FromString(TEXT("BGM Volume")));
			BGMVolume->SetDescriptionRichText(FText::FromString(TEXT("Set BGM")));
			
			BGMVolume->SetDisplayValueRange(TRange<float>(0.0f, 1.0f));
			BGMVolume->SetOutputValueRange(TRange<float>(0.0f, 2.0f));
			BGMVolume->SetSliderStepSize(0.01f);
			
			BGMVolume->SetDefaultValueFromString(FString::SanitizeFloat(1.0f));
			BGMVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			BGMVolume->SetNumberFormattingOptions(UListDataObjectScalar::NoDecimal());
			
			BGMVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetBGMVolume));
			BGMVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetBGMVolume));
			BGMVolume->SetShouldApplySettingsImmediately(true);
		
			VolumeCategoryCollection->AddChildListData(BGMVolume);
		}
		
		// SFX
		{
			UListDataObjectScalar* VFXVolume = NewObject<UListDataObjectScalar>();
			VFXVolume->SetDataID(FName("VFXVolume"));
			VFXVolume->SetDataDisplayName(FText::FromString(TEXT("Sound Effect Volume")));
			VFXVolume->SetDescriptionRichText(FText::FromString(TEXT("Set Sound Effect")));
			
			VFXVolume->SetDisplayValueRange(TRange<float>(0.0f, 1.0f));
			VFXVolume->SetOutputValueRange(TRange<float>(0.0f, 2.0f));
			VFXVolume->SetSliderStepSize(0.01f);
			
			VFXVolume->SetDefaultValueFromString(FString::SanitizeFloat(1.0f));
			VFXVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			VFXVolume->SetNumberFormattingOptions(UListDataObjectScalar::NoDecimal());
			
			VFXVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetSFXVolume));
			VFXVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetSFXVolume));
			VFXVolume->SetShouldApplySettingsImmediately(true);
		
			VolumeCategoryCollection->AddChildListData(VFXVolume);
		}
	}

	// Sound
	{
		UListDataObjectCollection* SoundCategoryCollection = NewObject<UListDataObjectCollection>();
		SoundCategoryCollection->SetDataID(FName("SoundCategoryCollection"));
		SoundCategoryCollection->SetDataDisplayName(FText::FromString("Sound"));

		AudioTabCollection->AddChildListData(SoundCategoryCollection);

		// Allow Background Audio
		{
			UListDataObjectStringBool* AllowBGAudio = NewObject<UListDataObjectStringBool>();
			AllowBGAudio->SetDataID(FName("AllowBGAudio"));
			AllowBGAudio->SetDataDisplayName(FText::FromString("Allow Background Audio"));
			AllowBGAudio->OverrideTrueDisplayText(FText::FromString("Enabled"));
			AllowBGAudio->OverrideFalseDisplayText(FText::FromString("Disabled"));

			AllowBGAudio->SetFalseAsDefaultValue();
			
			AllowBGAudio->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAllowBackgroundAudio));
			AllowBGAudio->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAllowBackgroundAudio));
			AllowBGAudio->SetShouldApplySettingsImmediately(true);

			SoundCategoryCollection->AddChildListData(AllowBGAudio);
		}
		
		// Use HDR Audio
		{
			UListDataObjectStringBool* UseHDRAudio = NewObject<UListDataObjectStringBool>();
			UseHDRAudio->SetDataID(FName("UseHDRAudio"));
			UseHDRAudio->SetDataDisplayName(FText::FromString("Use HDR Audio"));
			UseHDRAudio->OverrideTrueDisplayText(FText::FromString("Enabled"));
			UseHDRAudio->OverrideFalseDisplayText(FText::FromString("Disabled"));

			UseHDRAudio->SetFalseAsDefaultValue();
			
			UseHDRAudio->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetUseHDRAudio));
			UseHDRAudio->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetUseHDRAudio));
			UseHDRAudio->SetShouldApplySettingsImmediately(true);

			SoundCategoryCollection->AddChildListData(UseHDRAudio);
		}
	}
	

	RegisteredOptionsTabCollections.Add(AudioTabCollection);
}

void UOptionsDataRegistry::InitVideoCollectionTab()
{
	UListDataObjectCollection* VideoTabCollection =  NewObject<UListDataObjectCollection>();
	VideoTabCollection->SetDataID(FName("VideoTabCollection"));
	VideoTabCollection->SetDataDisplayName(FText::FromString("Video"));

	UListDataObjectStringEnum* CreatedWindowMode = nullptr;

	// Display
	{
		UListDataObjectCollection* DisplayCategoryCollection = NewObject<UListDataObjectCollection>();
		DisplayCategoryCollection->SetDataID(FName("DisplayCategoryCollection"));
		DisplayCategoryCollection->SetDataDisplayName(FText::FromString("Display"));
		
		VideoTabCollection->AddChildListData(DisplayCategoryCollection);

		FOptionsDataEditCondition PackagedBuildOnlyCondition;
		PackagedBuildOnlyCondition.SetEditConditionFunc(
			[]()->bool
			{
				bool bIsInEditor = GIsEditor || GIsPlayInEditorWorld;

				return !bIsInEditor;
			}
		);
		PackagedBuildOnlyCondition.SetDisabledRichReason(TEXT("\n\n <Disabled>This setting only adjust in package</>"));

		// Window Mode
		{
			UListDataObjectStringEnum* WindowMode = NewObject<UListDataObjectStringEnum>();
			WindowMode->SetDataID(FName("WindowMode"));
			WindowMode->SetDataDisplayName(FText::FromString("Window Mode"));
			WindowMode->SetDescriptionRichText(GET_DESCRIPTION("WindowModeDescKey"));

			WindowMode->AddEnumOption(EWindowMode::Fullscreen, FText::FromString("Fullscreen Mode"));
			WindowMode->AddEnumOption(EWindowMode::WindowedFullscreen, FText::FromString("Borderless Mode"));
			WindowMode->AddEnumOption(EWindowMode::Windowed, FText::FromString("Windowed Mode"));
			WindowMode->SetDefaultValueFromEnumOption(EWindowMode::WindowedFullscreen);

			WindowMode->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFullscreenMode));
			WindowMode->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFullscreenMode));
			WindowMode->SetShouldApplySettingsImmediately(true);

			WindowMode->AddEditCondition(PackagedBuildOnlyCondition);

			DisplayCategoryCollection->AddChildListData(WindowMode);

			CreatedWindowMode = WindowMode;
		}

		// Screen Resolution
		{
			UListDataObjectStringResolution* ScreenResolution = NewObject<UListDataObjectStringResolution>();
			ScreenResolution->SetDataID(FName("ScreenResolution"));
			ScreenResolution->SetDataDisplayName(FText::FromString("Screen Resolution"));
			ScreenResolution->SetDescriptionRichText(FText::FromString("Set Screen Resolution"));

			ScreenResolution->InitResolutionValues();
			
			ScreenResolution->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetScreenResolution));
			ScreenResolution->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetScreenResolution));
			ScreenResolution->SetShouldApplySettingsImmediately(true);
			
			FOptionsDataEditCondition WindowModeEditCondition;
			WindowModeEditCondition.SetEditConditionFunc(
				[CreatedWindowMode]()->bool
				{
					return CreatedWindowMode->GetCurrentValueAsEnum<EWindowMode::Type>() != EWindowMode::WindowedFullscreen;
				}
			);
			WindowModeEditCondition.SetDisabledRichReason(TEXT("\n\n<Disabled>Borderless Screen cannot adjust resolution</>"));
			WindowModeEditCondition.SetForcedStringValue(ScreenResolution->GetMaximumAllowedResolution());

			ScreenResolution->AddEditCondition(PackagedBuildOnlyCondition);
			ScreenResolution->AddEditCondition(WindowModeEditCondition);

			ScreenResolution->AddEditDependencyData(CreatedWindowMode);

			DisplayCategoryCollection->AddChildListData(ScreenResolution);
		}
	}

	// Graphics
	{
		UListDataObjectCollection* GraphicsCategoryCollection = NewObject<UListDataObjectCollection>();
		GraphicsCategoryCollection->SetDataID(FName("GraphicsCategoryCollection"));
		GraphicsCategoryCollection->SetDataDisplayName(FText::FromString("Graphics"));

		VideoTabCollection->AddChildListData(GraphicsCategoryCollection);

		// Display Gamma
		{
			UListDataObjectScalar* DisplayGamma = NewObject<UListDataObjectScalar>();
			DisplayGamma->SetDataID(FName("DisplayGamma"));
			DisplayGamma->SetDataDisplayName(FText::FromString("Brightness"));
			DisplayGamma->SetDescriptionRichText(FText::FromString("Set Display brightness"));
			
			DisplayGamma->SetDisplayValueRange(TRange<float>(0.0f, 1.0f));
			DisplayGamma->SetOutputValueRange(TRange<float>(1.7f, 2.7f)); // Unreal default is 2.2f;
			DisplayGamma->SetSliderStepSize(0.01f);
			DisplayGamma->SetDisplayNumericType(ECommonNumericType::Percentage);
			DisplayGamma->SetNumberFormattingOptions(UListDataObjectScalar::NoDecimal());

			DisplayGamma->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentDisplayGamma));
			DisplayGamma->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentDisplayGamma));
			DisplayGamma->SetShouldApplySettingsImmediately(true);

			DisplayGamma->SetDefaultValueFromString(FString::SanitizeFloat(2.2f));

			GraphicsCategoryCollection->AddChildListData(DisplayGamma);
		}

		UListDataObjectStringInteger* CreatedOverallQuality;
		//Overall Quality
		{
			UListDataObjectStringInteger* OverallQuality = NewObject<UListDataObjectStringInteger>();
			OverallQuality->SetDataID(FName("OverallQuality"));
			OverallQuality->SetDataDisplayName(FText::FromString("Overall Quality"));
			OverallQuality->SetDescriptionRichText(FText::FromString("Set All quality"));

			OverallQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			OverallQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			OverallQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			OverallQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			OverallQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));

			OverallQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetOverallScalabilityLevel));
			OverallQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetOverallScalabilityLevel));
			OverallQuality->SetShouldApplySettingsImmediately(true);
			
			GraphicsCategoryCollection->AddChildListData(OverallQuality);
			
			CreatedOverallQuality = OverallQuality;
		}

		//Resolution Scale
		{
			UListDataObjectScalar* ResolutionScale = NewObject<UListDataObjectScalar>();
			ResolutionScale->SetDataID(FName("ResolutionScale"));
			ResolutionScale->SetDataDisplayName(FText::FromString("3D Resolution"));
			ResolutionScale->SetDescriptionRichText(FText::FromString("Set Resolution Scale"));

			ResolutionScale->SetDisplayValueRange(TRange<float>(0.0f, 1.0f));
			ResolutionScale->SetOutputValueRange(TRange<float>(0.0f, 1.0f));
			ResolutionScale->SetSliderStepSize(0.01f);
			ResolutionScale->SetDisplayNumericType(ECommonNumericType::Percentage);
			ResolutionScale->SetNumberFormattingOptions(UListDataObjectScalar::NoDecimal());
			
			ResolutionScale->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetResolutionScaleNormalized));
			ResolutionScale->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetResolutionScaleNormalized));
			ResolutionScale->SetShouldApplySettingsImmediately(true);
			
			ResolutionScale->AddEditDependencyData(CreatedOverallQuality);
			CreatedOverallQuality->AddEditDependencyData(ResolutionScale);
			
			GraphicsCategoryCollection->AddChildListData(ResolutionScale);
		}

		//Global Illumination Quality
		{
			UListDataObjectStringInteger* GlobalIlluminationQuality = NewObject<UListDataObjectStringInteger>();
			GlobalIlluminationQuality->SetDataID(FName("GlobalIlluminationQuality"));
			GlobalIlluminationQuality->SetDataDisplayName(FText::FromString("Global Illumination Quality"));
			GlobalIlluminationQuality->SetDescriptionRichText(FText::FromString("Set All Illumination"));

			GlobalIlluminationQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			GlobalIlluminationQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			GlobalIlluminationQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			GlobalIlluminationQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			GlobalIlluminationQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));

			GlobalIlluminationQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetGlobalIlluminationQuality));
			GlobalIlluminationQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetGlobalIlluminationQuality));
			GlobalIlluminationQuality->SetShouldApplySettingsImmediately(true);

			GlobalIlluminationQuality->AddEditDependencyData(CreatedOverallQuality);
			CreatedOverallQuality->AddEditDependencyData(GlobalIlluminationQuality);
			
			GraphicsCategoryCollection->AddChildListData(GlobalIlluminationQuality);
		}

		//Shadow Quality
		{
			UListDataObjectStringInteger* ShadowQuality = NewObject<UListDataObjectStringInteger>();
			ShadowQuality->SetDataID(FName("ShadowQuality"));
			ShadowQuality->SetDataDisplayName(FText::FromString("Shadow Quality"));
			ShadowQuality->SetDescriptionRichText(FText::FromString("Set All Shadow"));

			ShadowQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			ShadowQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			ShadowQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			ShadowQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			ShadowQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));

			ShadowQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetShadowQuality));
			ShadowQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetShadowQuality));
			ShadowQuality->SetShouldApplySettingsImmediately(true);

			ShadowQuality->AddEditDependencyData(CreatedOverallQuality);
			CreatedOverallQuality->AddEditDependencyData(ShadowQuality);
			
			GraphicsCategoryCollection->AddChildListData(ShadowQuality);
		}

		//AntiAliasing Quality
		{
			UListDataObjectStringInteger* AntiAliasingQuality = NewObject<UListDataObjectStringInteger>();
			AntiAliasingQuality->SetDataID(FName("AntiAliasingQuality"));
			AntiAliasingQuality->SetDataDisplayName(FText::FromString("Anti-Aliasing Quality"));
			AntiAliasingQuality->SetDescriptionRichText(FText::FromString("Set All Anti-Aliasing"));

			AntiAliasingQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			AntiAliasingQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			AntiAliasingQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			AntiAliasingQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			AntiAliasingQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));

			AntiAliasingQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAntiAliasingQuality));
			AntiAliasingQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAntiAliasingQuality));
			AntiAliasingQuality->SetShouldApplySettingsImmediately(true);

			AntiAliasingQuality->AddEditDependencyData(CreatedOverallQuality);
			CreatedOverallQuality->AddEditDependencyData(AntiAliasingQuality);
			
			GraphicsCategoryCollection->AddChildListData(AntiAliasingQuality);
		}

		//View Distance Quality
		{
			UListDataObjectStringInteger* ViewDistanceQuality = NewObject<UListDataObjectStringInteger>();
			ViewDistanceQuality->SetDataID(FName("ViewDistanceQuality"));
			ViewDistanceQuality->SetDataDisplayName(FText::FromString("View Distance Quality"));
			ViewDistanceQuality->SetDescriptionRichText(FText::FromString("Set All View Distance"));

			ViewDistanceQuality->AddIntegerOption(0, FText::FromString(TEXT("Near")));
			ViewDistanceQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			ViewDistanceQuality->AddIntegerOption(2, FText::FromString(TEXT("Far")));
			ViewDistanceQuality->AddIntegerOption(3, FText::FromString(TEXT("Very Far")));
			ViewDistanceQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));

			ViewDistanceQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetViewDistanceQuality));
			ViewDistanceQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetViewDistanceQuality));
			ViewDistanceQuality->SetShouldApplySettingsImmediately(true);

			ViewDistanceQuality->AddEditDependencyData(CreatedOverallQuality);
			CreatedOverallQuality->AddEditDependencyData(ViewDistanceQuality);
			
			GraphicsCategoryCollection->AddChildListData(ViewDistanceQuality);
		}

		//Texture Quality
		{
			UListDataObjectStringInteger* TextureQuality = NewObject<UListDataObjectStringInteger>();
			TextureQuality->SetDataID(FName("TextureQuality"));
			TextureQuality->SetDataDisplayName(FText::FromString("Texture Quality"));
			TextureQuality->SetDescriptionRichText(FText::FromString("Set All Texture"));

			TextureQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			TextureQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			TextureQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			TextureQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			TextureQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));

			TextureQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetTextureQuality));
			TextureQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetTextureQuality));
			TextureQuality->SetShouldApplySettingsImmediately(true);

			TextureQuality->AddEditDependencyData(CreatedOverallQuality);
			CreatedOverallQuality->AddEditDependencyData(TextureQuality);
			
			GraphicsCategoryCollection->AddChildListData(TextureQuality);
		}
		
		//Visual Effects Quality
		{
			UListDataObjectStringInteger* VisualEffectsQuality = NewObject<UListDataObjectStringInteger>();
			VisualEffectsQuality->SetDataID(FName("VisualEffectsQuality"));
			VisualEffectsQuality->SetDataDisplayName(FText::FromString("Visual Effects Quality"));
			VisualEffectsQuality->SetDescriptionRichText(FText::FromString("Set All Visual Effects"));

			VisualEffectsQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			VisualEffectsQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			VisualEffectsQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			VisualEffectsQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			VisualEffectsQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));

			VisualEffectsQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetVisualEffectQuality));
			VisualEffectsQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVisualEffectQuality));
			VisualEffectsQuality->SetShouldApplySettingsImmediately(true);

			VisualEffectsQuality->AddEditDependencyData(CreatedOverallQuality);
			CreatedOverallQuality->AddEditDependencyData(VisualEffectsQuality);
			
			GraphicsCategoryCollection->AddChildListData(VisualEffectsQuality);
		}
		
		//Reflection Quality
		{
			UListDataObjectStringInteger* ReflectionQuality = NewObject<UListDataObjectStringInteger>();
			ReflectionQuality->SetDataID(FName("ReflectionQuality"));
			ReflectionQuality->SetDataDisplayName(FText::FromString("Reflection Quality"));
			ReflectionQuality->SetDescriptionRichText(FText::FromString("Set All Reflection"));

			ReflectionQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			ReflectionQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			ReflectionQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			ReflectionQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			ReflectionQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));

			ReflectionQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetReflectionQuality));
			ReflectionQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetReflectionQuality));
			ReflectionQuality->SetShouldApplySettingsImmediately(true);

			ReflectionQuality->AddEditDependencyData(CreatedOverallQuality);
			CreatedOverallQuality->AddEditDependencyData(ReflectionQuality);
			
			GraphicsCategoryCollection->AddChildListData(ReflectionQuality);
		}
		
		//Post Processing Quality
		{
			UListDataObjectStringInteger* PostProcessingQuality = NewObject<UListDataObjectStringInteger>();
			PostProcessingQuality->SetDataID(FName("PostProcessingQuality"));
			PostProcessingQuality->SetDataDisplayName(FText::FromString("Post Processing Quality"));
			PostProcessingQuality->SetDescriptionRichText(FText::FromString("Set All Post Processing"));

			PostProcessingQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			PostProcessingQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			PostProcessingQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			PostProcessingQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			PostProcessingQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));

			PostProcessingQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetPostProcessingQuality));
			PostProcessingQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetPostProcessingQuality));
			PostProcessingQuality->SetShouldApplySettingsImmediately(true);

			PostProcessingQuality->AddEditDependencyData(CreatedOverallQuality);
			CreatedOverallQuality->AddEditDependencyData(PostProcessingQuality);
			
			GraphicsCategoryCollection->AddChildListData(PostProcessingQuality);
		}
	}

	// Advanced Graphics Category
	{
		UListDataObjectCollection* AdvancedGraphicsCategory = NewObject<UListDataObjectCollection>();
		AdvancedGraphicsCategory->SetDataID(FName("AdvancedGraphicsCategory"));
		AdvancedGraphicsCategory->SetDataDisplayName(FText::FromString("Advanced Graphics"));

		VideoTabCollection->AddChildListData(AdvancedGraphicsCategory);

		// Vertical Sync
		{
			UListDataObjectStringBool* VerticalSync = NewObject<UListDataObjectStringBool>();
			VerticalSync->SetDataID(FName("VerticalSync"));
			VerticalSync->SetDataDisplayName(FText::FromString("V-Sync"));
			VerticalSync->SetDescriptionRichText(FText::FromString("Set V-Sync"));

			VerticalSync->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(IsVSyncEnabled));
			VerticalSync->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVSyncEnabled));
			VerticalSync->SetShouldApplySettingsImmediately(true);
			
			VerticalSync->SetFalseAsDefaultValue();

			FOptionsDataEditCondition FullscreenOnlyCondition;
			FullscreenOnlyCondition.SetEditConditionFunc(
				[CreatedWindowMode]()->bool
				{
					return CreatedWindowMode->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::Fullscreen;
				}
			);
			FullscreenOnlyCondition.SetDisabledRichReason(TEXT("\n\n<Disabled>This only works in Full screen window</>"));
			FullscreenOnlyCondition.SetForcedStringValue(TEXT("false"));

			VerticalSync->AddEditCondition(FullscreenOnlyCondition);

			AdvancedGraphicsCategory->AddChildListData(VerticalSync);
		}

		// Frame Rate Limit
		{
			UListDataObjectString* FrameRateLimit = NewObject<UListDataObjectString>();
			FrameRateLimit->SetDataID(FName("FrameRateLimit"));
			FrameRateLimit->SetDataDisplayName(FText::FromString("Frame Rate Limit"));
			FrameRateLimit->SetDescriptionRichText(FText::FromString("Set Frame rate limit"));

			FrameRateLimit->AddDynamicOption(FString::SanitizeFloat(30.0f), FText::FromString("30 FPS"));
			FrameRateLimit->AddDynamicOption(FString::SanitizeFloat(60.0f), FText::FromString("60 FPS"));
			FrameRateLimit->AddDynamicOption(FString::SanitizeFloat(90.0f), FText::FromString("90 FPS"));
			FrameRateLimit->AddDynamicOption(FString::SanitizeFloat(120.0f), FText::FromString("120 FPS"));
			FrameRateLimit->AddDynamicOption(FString::SanitizeFloat(0.0f), FText::FromString("No Limit"));
			FrameRateLimit->SetDefaultValueFromString(FString::SanitizeFloat(120.0f));
			
			FrameRateLimit->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFrameRateLimit));
			FrameRateLimit->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFrameRateLimit));
			FrameRateLimit->SetShouldApplySettingsImmediately(true);

			AdvancedGraphicsCategory->AddChildListData(FrameRateLimit);
		}
		
	}

	
	RegisteredOptionsTabCollections.Add(VideoTabCollection);
}

void UOptionsDataRegistry::InitControlCollectionTab(ULocalPlayer* InOwningLocalPlayer)
{
	UListDataObjectCollection* ControlTabCollection =  NewObject<UListDataObjectCollection>();
	ControlTabCollection->SetDataID(FName("ControlTabCollection"));
	ControlTabCollection->SetDataDisplayName(FText::FromString("Control"));

	UEnhancedInputLocalPlayerSubsystem* EISubsystem = InOwningLocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(EISubsystem);

	UEnhancedInputUserSettings* EIUserSettings = EISubsystem->GetUserSettings();
	check(EIUserSettings);
	
	// Keyboard Mouse Category
	{
		UListDataObjectCollection* KeyboardMouseCategoryCollection =  NewObject<UListDataObjectCollection>();
		KeyboardMouseCategoryCollection->SetDataID(FName("KeyboardMouseCategoryCollection"));
		KeyboardMouseCategoryCollection->SetDataDisplayName(FText::FromString("Keyboard & Mouse"));

		ControlTabCollection->AddChildListData(KeyboardMouseCategoryCollection);

		// Keyboard Mouse Inputs
		{
			FPlayerMappableKeyQueryOptions KeyboardMouseOnly;
			KeyboardMouseOnly.KeyToMatch = EKeys::S;
			KeyboardMouseOnly.bMatchBasicKeyTypes = true;
			
			for (const TPair<FGameplayTag, UEnhancedPlayerMappableKeyProfile*>& ProfilePair : EIUserSettings->GetAllSavedKeyProfiles())
			{
				UEnhancedPlayerMappableKeyProfile* MappableKeyProfile = ProfilePair.Value;
				check(MappableKeyProfile);

				for (const TPair<FName, FKeyMappingRow>& MappingRowPair : MappableKeyProfile->GetPlayerMappingRows())
				{
					for (const FPlayerKeyMapping& KeyMapping : MappingRowPair.Value.Mappings)
					{
						if (MappableKeyProfile->DoesMappingPassQueryOptions(KeyMapping, KeyboardMouseOnly))
						{
							if (!UListDataObjectKeyRemap::IsPossibleToEdit(KeyMapping)) continue;
							
							UListDataObjectKeyRemap* KeyRemapDataObject = NewObject<UListDataObjectKeyRemap>();
							KeyRemapDataObject->SetDataID(KeyMapping.GetMappingName());
							KeyRemapDataObject->SetDataDisplayName(KeyMapping.GetDisplayName());

							KeyRemapDataObject->InitKeyRemapData(EIUserSettings, MappableKeyProfile, ECommonInputType::MouseAndKeyboard, KeyMapping);

							KeyboardMouseCategoryCollection->AddChildListData(KeyRemapDataObject);
						}
					}
				}
			}
		}
	}

	// Gamepad Category
	{
		UListDataObjectCollection* GamepadCategoryCollection =  NewObject<UListDataObjectCollection>();
		GamepadCategoryCollection->SetDataID(FName("GamepadCategoryCollection"));
		GamepadCategoryCollection->SetDataDisplayName(FText::FromString("Gamepad"));

		ControlTabCollection->AddChildListData(GamepadCategoryCollection);

		
		// Gamepad Mouse Inputs
		{
			FPlayerMappableKeyQueryOptions GamepadOnly;
			GamepadOnly.KeyToMatch = EKeys::Gamepad_FaceButton_Bottom;
			GamepadOnly.bMatchBasicKeyTypes = true;
			
			for (const TPair<FGameplayTag, UEnhancedPlayerMappableKeyProfile*>& ProfilePair : EIUserSettings->GetAllSavedKeyProfiles())
			{
				UEnhancedPlayerMappableKeyProfile* MappableKeyProfile = ProfilePair.Value;
				check(MappableKeyProfile);

				for (const TPair<FName, FKeyMappingRow>& MappingRowPair : MappableKeyProfile->GetPlayerMappingRows())
				{
					for (const FPlayerKeyMapping& KeyMapping : MappingRowPair.Value.Mappings)
					{
						if (MappableKeyProfile->DoesMappingPassQueryOptions(KeyMapping, GamepadOnly))
						{
							if (!UListDataObjectKeyRemap::IsPossibleToEdit(KeyMapping)) continue;
							
							UListDataObjectKeyRemap* KeyRemapDataObject = NewObject<UListDataObjectKeyRemap>();
							KeyRemapDataObject->SetDataID(KeyMapping.GetMappingName());
							KeyRemapDataObject->SetDataDisplayName(KeyMapping.GetDisplayName());

							KeyRemapDataObject->InitKeyRemapData(EIUserSettings, MappableKeyProfile, ECommonInputType::Gamepad, KeyMapping);

							GamepadCategoryCollection->AddChildListData(KeyRemapDataObject);
						}
					}
				}
			}
		}

	}
	
	RegisteredOptionsTabCollections.Add(ControlTabCollection);
}

TArray<UListDataObjectBase*> UOptionsDataRegistry::GetListSourceItemsBySelectedTabID(const FName& InSelectedTabID) const
{
	UListDataObjectCollection* const* FoundTabCollectionPtr = RegisteredOptionsTabCollections.FindByPredicate(
		[InSelectedTabID](UListDataObjectCollection* AvailableTabCollection)->bool
		{
			return AvailableTabCollection->GetDataID() == InSelectedTabID;
		}
	);

	check(FoundTabCollectionPtr);

	UListDataObjectCollection* FoundTabCollection = *FoundTabCollectionPtr;

	TArray<UListDataObjectBase*> AllChildListItems;

	for (UListDataObjectBase* ChildListData : FoundTabCollection->GetAllChildListData())
	{
		if (!ChildListData) continue;

		AllChildListItems.Add(ChildListData);

		if (ChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursively(ChildListData, AllChildListItems);
		}
	}

	return AllChildListItems;
}

void UOptionsDataRegistry::FindChildListDataRecursively(UListDataObjectBase* InParentData, TArray<UListDataObjectBase*>& OutFoundChildListData) const
{
	if (!InParentData || !InParentData->HasAnyChildListData()) return;

	for (UListDataObjectBase* SubChildListData : InParentData->GetAllChildListData())
	{
		if (!SubChildListData) continue;

		OutFoundChildListData.Add(SubChildListData);

		if (SubChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursively(SubChildListData, OutFoundChildListData);
		}
	}
}
