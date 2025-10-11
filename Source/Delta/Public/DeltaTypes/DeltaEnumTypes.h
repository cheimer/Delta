// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//////////////////////////////
/// Skills
//////////////////////////////
UENUM(BlueprintType)
enum class EDeltaSkillState : uint8
{
	//Playable
	Back,

	//Enemy
	DesertDragon_Ground,
	DesertDragon_Fly,

	WoodMon,
	
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
	Slash UMETA(DisplayName = "Slash"),
	Lightning UMETA(DisplayName = "Lightning"),
	GroundCrack UMETA(DisplayName = "GroundCrack"),
	Flashes UMETA(DisplayName = "Flashes"),
	ArrowShot UMETA(DisplayName = "ArrowShot"),
	FlyingDisk UMETA(DisplayName = "FlyingDisk"),
	RailGun UMETA(DisplayName = "RailGun"),

	//Desert Dragon
	LeftClaws UMETA(DisplayName = "LeftClaws"),
	RightClaws UMETA(DisplayName = "RightClaws"),
	DoubleClaws UMETA(DisplayName = "DoubleClaws"),
	Bite UMETA(DisplayName = "Bite"),
	SpitFire UMETA(DisplayName = "SpitFire"),
	SpreadFire UMETA(DisplayName = "SpreadFire"),
	FlySpitFire UMETA(DisplayName = "FlySpitFire"),
	FlySpreadFire UMETA(DisplayName = "FlySpreadFire"),

	//Wood Mon
	Swing UMETA(DisplayName = "Swing"),
	BlowAway UMETA(DisplayName = "BlowAway"),
	ThrowRock UMETA(DisplayName = "ThrowRock"),
	TreeBloom UMETA(DisplayName = "TreeBloom"),
	ForestBoom UMETA(DisplayName = "ForestBoom"),
	
	Max UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EDeltaSkillType, EDeltaSkillType::Max);

///////////
/// UI	
///////////

UENUM(BlueprintType)
enum class EDeltaHUDMode : uint8
{
	None,
	Main,
	Play
};

UENUM(BlueprintType)
enum class EDeltaWidgetType : uint8
{
	None,

	// HUDMode : Main
	Main,
	Option,

	// HUDMode : Play
	Play,
	Pause,
	Result
};

UENUM(BlueprintType)
enum class EDeltaConfirmType : uint8
{
	OK,
	YesNo,
	OKCancel,
	
	Unknown UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EDeltaConfirmButtonType : uint8
{
	Confirmed,
	Cancelled,
	Closed,
	
	Unknown UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EDeltaOptionsListDataModifyReasonType : uint8
{
	DirectlyModified,
	DependencyModified,
	ResetToDefault,
	
	Unknown UMETA(Hidden)
};

//////////////
/// Input
//////////////

UENUM(BlueprintType)
enum class EInputPriority : uint8
{
	Character = 0,
	Controller = 1,
	
	Unknown UMETA(Hidden)
};
