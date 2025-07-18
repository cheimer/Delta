// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Flying.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFlying : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DELTA_API IFlying
{
	GENERATED_BODY()

public:
	virtual bool IsFlying() const = 0;
	
};
