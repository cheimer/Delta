// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DeltaBaseWidget.h"
#include "PlayWidget.generated.h"

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

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleHealthChanged(float CurrentHealth, float MaxHealth, bool bIsDamaged);
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ManaBar;

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

private:
	UPROPERTY()
	TArray<USkillInfoWidget*> SkillInfoArray;

	UPROPERTY()
	TMap<int, FChangeSkillSet> SkillChangeAnims;


};
