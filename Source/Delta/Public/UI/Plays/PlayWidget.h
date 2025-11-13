// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DeltaBaseWidget.h"
#include "PlayWidget.generated.h"

class ADeltaPlayableCharacter;
class UImage;
class UTextBlock;
class USkillInfoWidget;
class UTargetInfoWidget;
class UProgressBar;

USTRUCT()
struct FChangeSkillSet
{
	GENERATED_BODY()

	int CurrentIndex;

	UPROPERTY()
	UWidgetAnimation* LeftAnim;

	UPROPERTY()
	UWidgetAnimation* RightAnim;

	FChangeSkillSet()
	{
		CurrentIndex = -1;
		LeftAnim = nullptr;
		RightAnim = nullptr;
	}
};

USTRUCT()
struct FPlayableInfo
{
	GENERATED_BODY()

	TWeakObjectPtr<AActor> PlayableCharacter;

	UPROPERTY()
	UImage* Icon;
	UPROPERTY()
	UProgressBar* HealthBar;
	UPROPERTY()
	UProgressBar* ManaBar;
};

/**
 * 
 */
UCLASS()
class DELTA_API UPlayWidget : public UDeltaBaseWidget
{
	GENERATED_BODY()

public:
	void ShowTargetInfo(const AActor* Target) const;
	void HideTargetInfo() const;

	void ChangeSkillSet(const int BeforeIndex, const int AfterIndex);
	void SelectSkill(const int SelectSet, const int SelectIndex, const bool bIsSelect);

	void ChangeCharacter(const AActor* CurrentCharacter, const AActor* NewCharacter);

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void HandleHealthChanged(AActor* ChangedActor, float CurrentHealth, float MaxHealth, bool bIsDamaged);
	UFUNCTION()
	void HandleManaChanged(AActor* ChangedActor, float CurrentMana, float MaxMana);

#pragma region Bindings
	UPROPERTY(meta = (BindWidget))
	UImage* PlayerImage;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ManaBar;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentHealthText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxHealthText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentManaText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxManaText;
	
	UPROPERTY(meta = (BindWidget))
	UImage* AllyImage0;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* AllyHealthBar0;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* AllyManaBar0;

	UPROPERTY(meta = (BindWidget))
	UImage* AllyImage1;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* AllyHealthBar1;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* AllyManaBar1;

	UPROPERTY(meta = (BindWidget))
	UTargetInfoWidget* TargetInfoWidget;

	UPROPERTY(meta = (BindWidget))
	USkillInfoWidget* SkillInfoWidget0;
	UPROPERTY(meta = (BindWidget))
	USkillInfoWidget* SkillInfoWidget1;
	UPROPERTY(meta = (BindWidget))
	USkillInfoWidget* SkillInfoWidget2;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* SkillChange0To1Anim;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* SkillChange1To2Anim;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* SkillChange1To0Anim;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* SkillChange2To1Anim;

#pragma endregion Bindings

private:
	void ChangeInfo(FPlayableInfo& BeforeInfo, FPlayableInfo& AfterInfo);
	
	UPROPERTY()
	TArray<USkillInfoWidget*> SkillInfoArray;

	UPROPERTY()
	TMap<int, FChangeSkillSet> SkillChangeAnims;

	UPROPERTY()
	TArray<FPlayableInfo> PlayableInfos;

};
