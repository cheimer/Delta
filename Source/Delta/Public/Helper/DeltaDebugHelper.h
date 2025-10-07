#pragma once

namespace DeltaDebug
{
	static void Print(const FString& Msg, int32 InKey = -1, const FColor& InColor = FColor::MakeRandomColor())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(InKey, 5.0f, InColor, Msg);

			UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
		}
	}
}