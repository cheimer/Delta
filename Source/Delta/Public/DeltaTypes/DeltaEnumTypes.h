// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//Template Code: Copy and edit following lines
/*
UENUM(BlueprintType)
enum class EDeltaTemplateType : uint8
{
	Type1,
	Type2,
	
	Max UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EDeltaTemplateType, EDeltaTemplateType::Max);
*/

UENUM(BlueprintType)
enum class EDeltaSkillState : uint8
{
	//Playable
	Back,

	//Enemy
	DesertDragon_Ground,
	DesertDragon_Fly,
	
	Max UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EDeltaSkillState, EDeltaSkillState::Max);


UENUM(BlueprintType)
enum class EDeltaSkillType : uint8
{
	//Playable Character
	Parrying UMETA(DisplayName = "Parrying"),
	
	//Back Character
	Shield UMETA(DisplayName = "Shield"),
	MeteorShower UMETA(DisplayName = "MeteorShower"),

	//Desert Dragon
	LeftClaws UMETA(DisplayName = "LeftClaws"),
	RightClaws UMETA(DisplayName = "RightClaws"),
	DoubleClaws UMETA(DisplayName = "DoubleClaws"),
	Bite UMETA(DisplayName = "Bite"),
	SpitFire UMETA(DisplayName = "SpitFire"),
	SpreadFire UMETA(DisplayName = "SpreadFire"),
	FlySpitFire UMETA(DisplayName = "FlySpitFire"),
	FlySpreadFire UMETA(DisplayName = "FlySpreadFire"),
	
	Max UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EDeltaSkillType, EDeltaSkillType::Max);
