// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/DeltaFrontPlayerController.h"

#include "Camera/CameraActor.h"
#include "GameUserSettings/FrontGameUserSettings.h"
#include "Kismet/GameplayStatics.h"

void ADeltaFrontPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	TArray<AActor*> FoundCameras;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACameraActor::StaticClass(), FName("Default"), FoundCameras);

	if (!FoundCameras.IsEmpty())
	{
		SetViewTarget(FoundCameras[0]);
	}

	UFrontGameUserSettings* UserSettings = UFrontGameUserSettings::Get();
	if (FMath::IsNearlyEqual(UserSettings->GetLastCPUBenchmarkResult(), -1.0f, 0.01f) ||
		FMath::IsNearlyEqual(UserSettings->GetLastGPUBenchmarkResult(), -1.0f, 0.01f))
	{
		UserSettings->RunHardwareBenchmark();
		UserSettings->ApplyHardwareBenchmarkResults();
	}
	
	
}
