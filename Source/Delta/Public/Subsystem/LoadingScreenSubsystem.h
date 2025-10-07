// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LoadingScreenSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadingReasonUpdated, const FString&, CurrentLoadingReason);

/**
 * 
 */
UCLASS()
class DELTA_API ULoadingScreenSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	//~ Begin FTickableGameObject
	virtual UWorld* GetTickableGameObjectWorld() const override;
	virtual void Tick(float DeltaTime) override;

	virtual ETickableTickType GetTickableTickType() const override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;
	//~ End FTickableGameObject

	UPROPERTY(BlueprintAssignable)
	FOnLoadingReasonUpdated OnLoadingReasonUpdated;
	
private:
	void OnMapPreLoaded(const FWorldContext& WorldContext, const FString& MapName);
	void OnMapPostLoaded(UWorld* LoadedWorld);

	void TryUpdateLoadingScreen();

	bool IsPreLoadScreenActive() const;

	bool ShouldShowLoadingScreen();

	bool CheckTheNeedToShodLoadingScreen();

	void TryDisplayLoadingScreenIfNone();

	void TryRemoveLoadingScreen();

	void NotifyLoadingScreenVisibilityCahgend(bool bIsVisible);

	bool bIsCurrentlyLoadingMap = false;

	float HoldLoadingScreenStartUpTime = -1.0f;

	FString CurrentLoadingReason;

	TSharedPtr<SWidget> CachedCreatedWidget;
	
};
